#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <optional>

namespace json {

    class Node;

    // Сохраните объявления Dict и Array без изменения
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;

    using Value = std::variant<std::nullptr_t, int, double, bool, std::string, Array, Dict>;


    namespace detail {

        struct PrintContext {
            std::ostream& out;
            int indent_step = 4;
            int indent = 0;

            void PrintIndent() const {
                for (int i = 0; i < indent; ++i) {
                    out.put(' ');
                }
            }

            PrintContext Indented() const {
                return { out, indent_step, indent + indent_step };
            }
        };

        void PrintValue(const Value& value, const PrintContext& ctx);

    } // namespace detail

    // Эта ошибка должна выбрасываться при ошибках парсинга JSON
    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class Node {
    public:
        // Конструкторы
        Node() = default;       // null

        explicit Node(Array array);
        explicit Node(Dict map);
        explicit Node(int value);
        explicit Node(double value);
        explicit Node(bool value);
        explicit Node(std::string value);
        explicit Node(std::nullptr_t);

        // Методы проверки типа
        bool IsInt() const;
        bool IsDouble() const;           // int или double
        bool IsPureDouble() const;       // только double
        bool IsBool() const;
        bool IsString() const;
        bool IsNull() const;
        bool IsArray() const;
        bool IsMap() const;

        // Методы получения значения
        const Array& AsArray() const;
        const Dict& AsMap() const;
        int AsInt() const;
        double AsDouble() const;
        bool AsBool() const;
        const std::string& AsString() const;

        // Сравнение
        bool operator==(const Node& rhs) const;
        bool operator!=(const Node& rhs) const;

    private:
//        Array as_array_;
//        Dict as_map_;
//        int as_int_ = 0;
//        std::string as_string_;
        Value value_;
    };

    class Document {
    public:
        explicit Document(Node root);

        const Node& GetRoot() const;

    private:
        Node root_;
    };

    // Загрузка JSON из потока
    Document Load(std::istream& input);

    // Вывод JSON в поток
    void Print(const Document& doc, std::ostream& output);

}  // namespace json
