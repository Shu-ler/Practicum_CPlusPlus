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
        bool IsPureDouble() const;  ///< Возвращает true для double
        bool IsDouble() const;      ///< Возвращает true для int и double
        bool IsBool() const;
        bool IsString() const;
        bool IsNull() const;
        bool IsArray() const;
        bool IsMap() const;         ///< Аналог IsObject

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

        /**
         * @brief Возвращает ссылку на хранимое значение.
         * @return Константная ссылка на variant
         */
        const Value& GetValue() const;

    private:
        Value value_{};
    };

    /**
     * @brief Представляет полный JSON-документ.
     *
     * Состоит из одного корневого узла (Node).
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

    /**
     * @class Builder
     * @brief Строитель JSON-структуры.
     *
     * Позволяет постепенно конструировать сложные JSON-объекты.
     *
     * Пример:
     * @code
     * Node root = Builder{}
     *     .StartDict()
     *         .Key("name").Value("Alice")
     *         .Key("age").Value(30)
     *     .EndDict()
     *     .Build();
     * @endcode
     *
     */
    class Builder {
    public:
        /**
         * @brief Добавляет произвольное значение в текущий контейнер.
         * @param value Значение для добавления
         * @return Ссылка на *this для цепочки вызовов
         */
        Builder& Value(Value value);

        // Добавляет null
        Builder& Null();

        // Добавляет логическое значение
        Builder& Bool(bool value);

        // Добавляет целое число
        Builder& Number(int value);

        // Добавляет вещественное число
        Builder& Number(double value);

        // Добавляет строку
        Builder& String(std::string value);

        /**
         * @brief Начинает создание массива.
         * @return Ссылка на *this
         * @pre Не может быть вызван внутри массива без завершения предыдущего
         */
        Builder& StartArray();

        /**
         * @brief Завершает текущий массив.
         * @return Ссылка на *this
         * @pre Должен быть вызван после StartArray()
         */
        Builder& EndArray();

        /**
         * @brief Начинает создание словаря (объекта).
         * @return Ссылка на *this
         */
        Builder& StartDict();

        /**
         * @brief Завершает текущий словарь.
         * @return Ссылка на *this
         * @pre Должен быть вызван после StartDict()
         */
        Builder& EndDict();

        /**
         * @brief Добавляет ключ в словарь.
         * @param key Имя ключа
         * @return Ссылка на *this
         * @pre Должен вызываться только внутри словаря
         * @note После Key() нужно вызвать Value(), String() и т.д.
         */
        Builder& Key(std::string key);

        /**
         * @brief Завершает построение и возвращает результат.
         * @return Узел с построенной JSON-структурой
         * @throw std::logic_error если контейнеры не закрыты
         */
        Node Build();

    private:
        // Проверяет, что нет активных контейнеров
        void CheckClosed() const;

        // Добавляет узел в текущий контекст
        void AddNode(Node node);

    private:
        Node root_;                         //< Корневой узел
        std::vector<Node*> nodes_stack_;    //< Стек для отслеживания вложенных контейнеров

    };
}  // namespace json
