#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <cctype>
#include <sstream>
#include <limits>
#include <utility>

namespace json {

    // Предварительное объявление
    class Node;
    class Document;

    /**
     * @brief Тип для представления массива JSON — вектор узлов
     */
    using Array = std::vector<Node>;

    /**
     * @brief Тип для представления JSON-объекта (словаря).
     *
     * В JSON называется "object", в C++ реализован как словарь строк в Node.
     */
    using Dict = std::map<std::string, Node>;

    /**
     * @brief Тип значения узла JSON
     *
     * Может быть одним из:
     * - nullptr_t
     * - int
     * - double
     * - bool
     * - std::string
     * - Array
     * - Dict
     */
    using Value = std::variant<
        std::nullptr_t,
        int,
        double,
        bool,
        std::string,
        Array,
        Dict
    >;

    /**
     * @brief Исключение, выбрасываемое при ошибках парсинга
     */
    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    /**
     * @brief Исключение, выбрасываемое при ошибках построения (для Builder)
     */
    class BuildError : public std::logic_error {
    public:
        using logic_error::logic_error;
    };

    /**
     * @brief Узел JSON-документа
     *
     * Хранит одно значение любого типа из Value.
     */
    class Node final 
        : private std::variant<std::nullptr_t, int, double, bool, std::string, Array, Dict> {
    public:

        using variant::variant;
        using Value = variant;

        // Конструкторы (не explicit, чтобы работала инициализация вроде Array{1, 2.5, "str"})
        Node() = default;
        Node(int value);
        Node(double value);
        Node(bool value);
        Node(const std::string& value);
        Node(std::string&& value);
        Node(std::nullptr_t);

        Node(const Array& array);
        Node(Array&& array);

        Node(const Dict& map);
        Node(Dict&& map);

        // Проверки типа
        bool IsInt() const;
        bool IsPureDouble() const;  ///< Возвращает true для double
        bool IsDouble() const;      ///< Возвращает true для int и double
        bool IsBool() const;
        bool IsString() const;
        bool IsNull() const;
        bool IsArray() const;
        bool IsDict() const;         ///< Аналог IsObject

        // Методы получения значения (бросает logic_error при несовпадении типов)
        int AsInt() const;
        bool AsBool() const;
        double AsDouble() const;
        const std::string& AsString() const;
        const Array& AsArray() const;
        const Dict& AsDict() const;

        /**
         * @brief Извлекает значение заданного типа из узла.
         * @tparam T Тип, который ожидается (int, bool, std::string и т.д.)
         * @return Константная ссылка на значение типа T
         * @throws std::logic_error если тип не совпадает
         *
         * Пример:
         * @code
         * node.As<int>();     // проверит, что это int
         * node.As<std::string>(); // проверит, что это строка
         * @endcode
         */
        template<typename T>
        T As() const;

        // Сравнение
        bool operator==(const Node& rhs) const;
        bool operator!=(const Node& rhs) const;

        /**
         * @brief Возвращает ссылку на хранимое значение.
         * @return Константная ссылка на variant
         */
        const Value& GetValue() const;

        /**
         * @brief Возвращает ссылку на хранимое значение (для внутреннего использования).
         * @return Неконстантная ссылка на variant
         */
        Value& GetValueRef();

    private:
        Value value_{};
    };

    template<typename T>
    T Node::As() const {
        if constexpr (std::is_same_v<T, int>) {
            return AsInt();
        }
        else if constexpr (std::is_same_v<T, bool>) {
            return AsBool();
        }
        else if constexpr (std::is_same_v<T, double>) {
            return AsDouble();
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            return AsString();
        }
        else if constexpr (std::is_same_v<T, Array>) {
            return AsArray();
        }
        else if constexpr (std::is_same_v<T, Dict>) {
            return AsDict();
        }
        else {
            static_assert(sizeof(T) == 0, "Node::As<T> not specialized for this type");
        }
    }

    /**
     * @brief Представляет полный JSON-документ.
     *
     * Состоит из одного корневого узла (Node).
     */
    class Document {
    public:

        /**
         * @brief Конструктор по умолчанию.
         * Создаёт документ с пустым корнем (null).
         */
        Document();

        /**
         * @brief Конструктор документа с заданным корневым узлом.
         * @param root Корневой узел JSON-документа.
         */
        explicit Document(Node root);

        /**
         * @brief Возвращает константную ссылку на корневой узел документа.
         * @return const Node& — корневой узел.
         */
        const Node& GetRoot() const;

        /**
         * @brief Сравнивает два JSON-документа на равенство.
         * Документы считаются равными, если их корневые узлы равны.
         * @param other Другой документ для сравнения.
         * @return true, если документы идентичны, иначе false.
         */
        bool operator==(const Document& other) const;

        /**
         * @brief Сравнивает два JSON-документа на неравенство.
         * @param other Другой документ для сравнения.
         * @return true, если документы отличаются, иначе false.
         */
        bool operator!=(const Document& other) const;

    private:
        Node root_; ///< Корневой узел JSON-документа.
    };

    /**
     * @brief Загружает JSON-документ из потока.
     * @param input Входной поток
     * @return Document Объект документа
     * @throw ParsingError При синтаксических ошибках
     */
    Document Load(std::istream& input);

    /**
     * @brief Выводит документ в поток в формате JSON.
     * @param doc Документ
     * @param output Поток вывода
     */
    void Print(const Document& doc, std::ostream& output);

    /**
     * @brief Оператор ввода JSON-документа из потока.
     * @param is Входной поток
     * @param doc Приёмник документа
     * @return Ссылка на is
     *
     * Пример:
     * @code
     * json::Document doc;
     * std::cin >> doc;
     * @endcode
     */
    std::istream& operator>>(std::istream& is, json::Document& doc);

    /**
     * @brief Оператор вывода JSON-документа.
     * @param os Поток вывода
     * @param doc Документ
     * @return Ссылка на os
     *
     * Пример:
     * @code
     * std::cout << doc;
     * @endcode
     */
    std::ostream& operator<<(std::ostream& os, const json::Document& doc);

}  // namespace json
