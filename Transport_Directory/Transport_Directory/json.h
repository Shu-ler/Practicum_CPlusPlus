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
    class Builder;

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
            return AsMap();
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

    /**
     * @class Builder
     * @brief Строитель JSON-структуры.
     *
     * Позволяет постепенно конструировать сложные JSON-объекты.
     *
     * Пример:
     * @code
     * Node root = Builder{}
     *  .StartDict()            // stack: [&root_] (Dict)
     *      .Key("name")        // stack: [&root_["name"]] (null)
     *      .AddString("Bob")   // stack: [&root_] (Dict)
     *      .Key("nums")        // stack: [&root_["nums"]] (null)
     *      .StartArray()       // stack: [&root_["nums"]] (Array)
     *          .AddNumber(1)
     *          .AddNumber(2)
     *      .EndArray()         // stack: [&root_] (Dict)
     *  .EndDict()              // stack: empty
     *  .Build();
     * @endcode
     *
     */
    class Builder {
    public:
        // Добавляет null
        Builder& AddNull();

        // Добавляет логическое значение
        Builder& AddBool(bool value);

        // Добавляет целое число
        Builder& AddNumber(int value);

        // Добавляет вещественное число
        Builder& AddNumber(double value);

        // Добавляет строку
        Builder& AddString(std::string value);

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
         * @brief Начинает установку значения для ключа в JSON-объекте
         * @param key Имя ключа
         * @return Builder& для цепочки вызовов
         * @throws BuildError если вызван вне словаря
         *
         * После вызова Key() ожидается вызов Add...(),
         * который заполнит значение по ключу.
         */
        Builder& Key(std::string key);

        /**
         * @brief Добавляет готовый узел JSON в текущий контекст.
         *
         * Метод работает в двух контекстах:
         * 1. Внутри массива — добавляет узел как новый элемент.
         * 2. После вызова Key() — устанавливает значение поля.
         *
         * После вызова:
         * - В массиве: узел добавлен, стек не меняется.
         * - В словаре: значение установлено, контекст закрывается (pop из стека).
         *
         * @param node Узел для вставки (может быть любым типом: число, строка, массив и т.д.)
         * @return Builder& для цепочки вызовов
         * @throws BuildError если:
         *   - builder пуст (нет активного контекста)
         *   - родитель не является массивом или null-значением
         *
         * @note Не используйте AddNode для вложенных StartArray/StartDict.
         *       Для построения "на лету" используйте цепочку вызовов.
         */
        Builder& AddNode(Node node);

        /**
         * @brief Завершает построение и возвращает результат.
         * @return Узел с построенной JSON-структурой
         * @throw std::logic_error если контейнеры не закрыты
         */
        Node Build();

    private:
        /**
         * @brief Безопасно возвращает ссылку на текущий активный узел
         * @param err_msg Сообщение об ошибке, если стек пуст
         * @return Node& Ссылка на узел, в который можно записывать значение
         * @throws BuildError если стек пуст
         *
         * Используется в Add...() методах, чтобы:
         * - Убедиться, что есть активный контекст
         * - Получить ссылку на узел для присваивания
         *
         * Не удаляет узел из стека — это делает вызывающий.
         */
        Node& GetNodeSafely(const std::string& err_msg);
        std::pair<Node&, bool> GetNodeSafe(const std::string& err_msg);

        // Проверяет, что нет активных контейнеров
        void CheckClosed() const;

    private:
        Node root_{};                         //< Корневой узел
        std::vector<Node*> nodes_stack_{};    //< Стек для отслеживания вложенных контейнеров
    };
}  // namespace json
