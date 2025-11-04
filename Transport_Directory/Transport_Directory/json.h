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
     * @brief Псевдоним для массива JSON — вектор узлов
     */
    using Array = std::vector<Node>;

    /**
     * @brief Псевдоним для словаря JSON — карта "ключ-строка" → узел
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
     * @brief Узел JSON-документа
     *
     * Хранит одно значение любого типа из Value.
     */
    class Node {
    public:
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
        bool IsPureDouble() const;  // только double
        bool IsDouble() const;      // int или double
        bool IsBool() const;
        bool IsString() const;
        bool IsNull() const;
        bool IsArray() const;
        bool IsMap() const;  // соответствует IsObject

        // Методы получения значения (бросает logic_error при несовпадении типов)
        int AsInt() const;
        bool AsBool() const;
        double AsDouble() const;
        const std::string& AsString() const;
        const Array& AsArray() const;
        const Dict& AsMap() const;

        // Сравнение
        bool operator==(const Node& rhs) const;
        bool operator!=(const Node& rhs) const;

        // Для внутреннего доступа к variant (используется в Print)
        const Value& GetValue() const;

    private:
        Value value_{};
    };

    /**
     * @brief Представляет полный JSON-документ.
     */
    class Document {
    public:
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

}  // namespace json
