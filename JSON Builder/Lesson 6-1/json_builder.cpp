#include "json_builder.h"
#include <stdexcept>
#include <variant>

namespace json {

    // Получить ссылку на текущий узел (верх стека)
    Node& Builder::Current() {
        return *stack_.back();
    }

    // Проверить, можно ли вызвать Build()
    void Builder::CheckBuildReady() {
        if (!stack_.empty()) {
            throw std::logic_error("Builder: cannot Build() — containers are not closed");
        }
        if (std::holds_alternative<std::nullptr_t>(root_.GetValue())) {
            throw std::logic_error("Builder: cannot Build() — no value was set");
        }
    }

    Builder& Builder::Key(std::string key) {
        if (stack_.empty()) {
            throw std::logic_error("Builder: Key() called outside of dictionary");
        }
        if (!Current().IsDict()) {
            throw std::logic_error("Builder: Key() called not in dictionary context");
        }
        if (key_.has_value()) {
            throw std::logic_error("Builder: Key() called, but previous key '" + key_.value() + "' has no value");
        }

        key_ = std::move(key);
        return *this;
    }

    Builder& Builder::Value(Node::Value value) {
        // Используем std::visit, чтобы создать Node из variant
        Node node = std::visit([](auto&& arg) -> Node {
            return Node(std::forward<decltype(arg)>(arg));
            }, std::move(value));

        if (stack_.empty()) {
            // Устанавливаем корень
            if (std::holds_alternative<std::nullptr_t>(root_.GetValue())) {
                root_ = std::move(node);
                return *this;
            }
            else {
                throw std::logic_error("Builder: Value() called on already built object");
            }
        }

        Node& curr = Current();

        if (key_.has_value()) {
            // Устанавливаем значение по ключу
            Dict& dict = const_cast<Dict&>(curr.AsDict());
            dict[key_.value()] = std::move(node);
            key_.reset();
        }
        else if (curr.IsArray()) {
            // Добавляем в массив
            Array& array = const_cast<Array&>(curr.AsArray());
            array.emplace_back(std::move(node));
        }
        else {
            throw std::logic_error("Builder: Value() called in invalid context");
        }

        return *this;
    }

    Builder& Builder::StartDict() {
        Node dict_node(Dict{});

        if (stack_.empty()) {
            if (std::holds_alternative<std::nullptr_t>(root_.GetValue())) {
                root_ = std::move(dict_node);
                stack_.push_back(&root_);
                return *this;
            }
            else {
                throw std::logic_error("Builder: StartDict() called on already built object");
            }
        }

        Node& curr = Current();

        if (key_.has_value()) {
            // Устанавливаем словарь как значение по ключу
            Dict& dict = const_cast<Dict&>(curr.AsDict());
            auto& new_node = dict[key_.value()] = std::move(dict_node);
            stack_.push_back(&new_node);
            key_.reset();
        }
        else if (curr.IsArray()) {
            // Добавляем словарь в массив
            Array& array = const_cast<Array&>(curr.AsArray());
            array.emplace_back(std::move(dict_node));
            stack_.push_back(&array.back());
        }
        else {
            throw std::logic_error("Builder: StartDict() called in invalid context");
        }

        return *this;
    }

    Builder& Builder::EndDict() {
        if (stack_.empty()) {
            throw std::logic_error("Builder: EndDict() called on empty stack");
        }
        if (!Current().IsDict()) {
            throw std::logic_error("Builder: EndDict() called not in dictionary");
        }
        if (key_.has_value()) {
            throw std::logic_error("Builder: EndDict() called, but key '" + key_.value() + "' has no value");
        }

        stack_.pop_back();
        return *this;
    }

    Builder& Builder::StartArray() {
        Node array_node(Array{});

        if (stack_.empty()) {
            if (std::holds_alternative<std::nullptr_t>(root_.GetValue())) {
                root_ = std::move(array_node);
                stack_.push_back(&root_);
                return *this;
            }
            else {
                throw std::logic_error("Builder: StartArray() called on already built object");
            }
        }

        Node& curr = Current();

        if (key_.has_value()) {
            // Устанавливаем массив как значение по ключу
            Dict& dict = const_cast<Dict&>(curr.AsDict());
            auto& new_node = dict[key_.value()] = std::move(array_node);
            stack_.push_back(&new_node);
            key_.reset();
        }
        else if (curr.IsArray()) {
            // Добавляем массив в массив
            Array& array = const_cast<Array&>(curr.AsArray());
            array.emplace_back(std::move(array_node));
            stack_.push_back(&array.back());
        }
        else {
            throw std::logic_error("Builder: StartArray() called in invalid context");
        }

        return *this;
    }

    Builder& Builder::EndArray() {
        if (stack_.empty()) {
            throw std::logic_error("Builder: EndArray() called on empty stack");
        }
        if (!Current().IsArray()) {
            throw std::logic_error("Builder: EndArray() called not in array");
        }

        stack_.pop_back();
        return *this;
    }

    Node Builder::Build() {
        CheckBuildReady();
        return root_;
    }

} // namespace json
