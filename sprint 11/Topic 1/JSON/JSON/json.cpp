#include "json.h"

using namespace std;

namespace json {

    namespace {

        Node LoadNode(istream& input);

        Node LoadArray(istream& input) {
            Array result;

            for (char c; input >> c && c != ']';) {
                if (c != ',') {
                    input.putback(c);
                }
                result.push_back(LoadNode(input));
            }

            return Node(move(result));
        }

        Node LoadInt(istream& input) {
            int result = 0;
            while (isdigit(input.peek())) {
                result *= 10;
                result += input.get() - '0';
            }
            return Node(result);
        }

        Node LoadString(istream& input) {
            string line;
            getline(input, line, '"');
            return Node(move(line));
        }

        Node LoadDict(istream& input) {
            Dict result;

            for (char c; input >> c && c != '}';) {
                if (c == ',') {
                    input >> c;
                }

                string key = LoadString(input).AsString();
                input >> c;
                result.insert({ move(key), LoadNode(input) });
            }

            return Node(move(result));
        }

        Node LoadNode(istream& input) {
            char c;
            input >> c;

            if (c == '[') {
                return LoadArray(input);
            }
            else if (c == '{') {
                return LoadDict(input);
            }
            else if (c == '"') {
                return LoadString(input);
            }
            else {
                input.putback(c);
                return LoadInt(input);
            }
        }

    }  // namespace


    // === Реализация Node === Конструкторы ===

    Node::Node(std::nullptr_t) : value_(nullptr) {
    }

    Node::Node(int value) : value_(value) {
    }

    Node::Node(double value) : value_(value) {
    }

    Node::Node(bool value) : value_(value) {
    }

    Node::Node(std::string value) : value_(std::move(value)) {
    }

    // === Реализация Node === Методы проверки типа ===

    bool Node::IsInt() const {
        return std::holds_alternative<int>(value_);
    }

    bool Node::IsDouble() const {
        return IsInt() || IsPureDouble();
    }

    bool Node::IsPureDouble() const {
        return std::holds_alternative<double>(value_);
    }

    bool Node::IsBool() const {
        return std::holds_alternative<bool>(value_);
    }

    bool Node::IsString() const {
        return std::holds_alternative<std::string>(value_);
    }

    bool Node::IsNull() const {
        return std::holds_alternative<std::nullptr_t>(value_);
    }

    bool Node::IsArray() const {
        return std::holds_alternative<Array>(value_);
    }

    bool Node::IsMap() const {
        return std::holds_alternative<Dict>(value_);
    }


    Node::Node(Array array)
        : as_array_(move(array)) {
    }

    Node::Node(Dict map)
        : as_map_(move(map)) {
    }

    Node::Node(int value)
        : as_int_(value) {
    }

    Node::Node(string value)
        : as_string_(move(value)) {
    }

    const Array& Node::AsArray() const {
        return as_array_;
    }

    const Dict& Node::AsMap() const {
        return as_map_;
    }

    int Node::AsInt() const {
        return as_int_;
    }

    const string& Node::AsString() const {
        return as_string_;
    }

    // === Реализация Document ===

    Document::Document(Node root)
        : root_(std::move(root)) {
    }

    const Node& Document::GetRoot() const {
        return root_;
    }

    Document Load(istream& input) {
        return Document{ LoadNode(input) };
    }

    // Основная функция — единственная точка входа для вывода
    void Print(const Document& doc, std::ostream& output) {
        detail::PrintValue(doc.GetRoot().value_, detail::PrintContext{ output });
    }

}  // namespace json
