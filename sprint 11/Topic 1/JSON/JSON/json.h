#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <optional>

namespace json {

    class Node;

    // Основные типы
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;
    using Value = std::variant<std::nullptr_t, int, double, bool, std::string, Array, Dict>;


    // Эта ошибка должна выбрасываться при ошибках парсинга JSON
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
        Node() = default;       // null
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
        Value value_;
    };

    /**
     * @brief Представляет полный JSON-документ.
     */
    class Document {
    public:
        explicit Document(Node root);

        const Node& GetRoot() const;

    private:
        Node root_;
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
