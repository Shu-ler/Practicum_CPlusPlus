#include "json_builder.h"
#include <stdexcept>
#include <variant>

namespace json {

    // Вспомогательные методы Builder

    Node& Builder::Current() {
        return *stack_.back();
    }

    void Builder::CheckBuildReady() {
        if (!stack_.empty()) {
            throw std::logic_error("Builder: cannot Build() — containers are not closed");
        }
        if (std::holds_alternative<std::nullptr_t>(root_.GetValue())) {
            throw std::logic_error("Builder: cannot Build() — no value was set");
        }
    }

    // === Приватные методы "Do" ===

    void Builder::DoKey(std::string key) {
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
    }

    void Builder::DoValue(Node::Value value) {
        Node node = std::visit([](auto&& arg) -> Node {
            return Node{ std::forward<decltype(arg)>(arg) };
            }, std::move(value));

        if (stack_.empty()) {
            if (std::holds_alternative<std::nullptr_t>(root_.GetValue())) {
                root_ = std::move(node);
                return;
            }
            throw std::logic_error("Builder: Value() called on already built object");
        }

        Node& curr = Current();

        if (key_.has_value()) {
            Dict& dict = const_cast<Dict&>(curr.AsDict());
            dict[key_.value()] = std::move(node);
            key_.reset();
        }
        else if (curr.IsArray()) {
            Array& array = const_cast<Array&>(curr.AsArray());
            array.emplace_back(std::move(node));
        }
        else {
            throw std::logic_error("Builder: Value() called in invalid context");
        }
    }

    void Builder::DoStartDict() {
        Node dict_node(Dict{});
        if (stack_.empty()) {
            if (std::holds_alternative<std::nullptr_t>(root_.GetValue())) {
                root_ = std::move(dict_node);
                stack_.push_back(&root_);
                return;
            }
            throw std::logic_error("Builder: StartDict() called on already built object");
        }

        Node& curr = Current();
        if (key_.has_value()) {
            Dict& dict = const_cast<Dict&>(curr.AsDict());
            auto& new_node = dict[key_.value()] = std::move(dict_node);
            stack_.push_back(&new_node);
            key_.reset();
        }
        else if (curr.IsArray()) {
            Array& array = const_cast<Array&>(curr.AsArray());
            array.emplace_back(std::move(dict_node));
            stack_.push_back(&array.back());
        }
        else {
            throw std::logic_error("Builder: StartDict() called in invalid context");
        }
    }

    void Builder::DoStartArray() {
        Node array_node(Array{});
        if (stack_.empty()) {
            if (std::holds_alternative<std::nullptr_t>(root_.GetValue())) {
                root_ = std::move(array_node);
                stack_.push_back(&root_);
                return;
            }
            throw std::logic_error("Builder: StartArray() called on already built object");
        }

        Node& curr = Current();
        if (key_.has_value()) {
            Dict& dict = const_cast<Dict&>(curr.AsDict());
            auto& new_node = dict[key_.value()] = std::move(array_node);
            stack_.push_back(&new_node);
            key_.reset();
        }
        else if (curr.IsArray()) {
            Array& array = const_cast<Array&>(curr.AsArray());
            array.emplace_back(std::move(array_node));
            stack_.push_back(&array.back());
        }
        else {
            throw std::logic_error("Builder: StartArray() called in invalid context");
        }
    }

    void Builder::DoEndDict() {
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
    }

    void Builder::DoEndArray() {
        if (stack_.empty()) {
            throw std::logic_error("Builder: EndArray() called on empty stack");
        }
        if (!Current().IsArray()) {
            throw std::logic_error("Builder: EndArray() called not in array");
        }
        stack_.pop_back();
    }

    // === Builder::Start* — возвращают контекст ===

    DictContext Builder::StartDict() {
        DoStartDict();
        return DictContext(*this);
    }

    ArrayContext Builder::StartArray() {
        DoStartArray();
        return ArrayContext(*this);
    }

    Node Builder::Build() {
        CheckBuildReady();
        return root_;
    }

    // === Реализация контекстов ===

    ValueContext DictContext::Key(std::string key) {
        builder_.DoKey(std::move(key));
        return ValueContext(builder_);
    }

    Builder& DictContext::EndDict() {
        builder_.DoEndDict();
        return builder_;
    }

    Builder& ValueContext::Value(Node::Value value) {
        builder_.DoValue(std::move(value));
        return builder_;
    }

    DictContext ValueContext::StartDict() {
        builder_.DoStartDict();
        return DictContext(builder_);
    }

    ArrayContext ValueContext::StartArray() {
        builder_.DoStartArray();
        return ArrayContext(builder_);
    }

    ArrayContext ArrayContext::Value(Node::Value value) {
        builder_.DoValue(std::move(value));
        return *this;
    }

    DictContext ArrayContext::StartDict() {
        builder_.DoStartDict();
        return DictContext(builder_);
    }

    ArrayContext ArrayContext::StartArray() {
        builder_.DoStartArray();
        return *this;
    }

    Builder& ArrayContext::EndArray() {
        builder_.DoEndArray();
        return builder_;
    }

    // === Реализация шаблонных методов Context — теперь в .cpp ===

    template <typename D>
    DictContext BaseContext<D>::StartDict() {
        builder_.DoStartDict();
        return DictContext(builder_);
    }

    template <typename D>
    ArrayContext BaseContext<D>::StartArray() {
        builder_.DoStartArray();
        return ArrayContext(builder_);
    }

    template <typename D>
    ValueContext BaseContext<D>::Key(std::string key) {
        builder_.DoKey(std::move(key));
        return ValueContext(builder_);
    }

    template <typename D>
    Node BaseContext<D>::Build() {
        return builder_.Build();
    }

    template <typename D>
    Builder& BaseContext<D>::EndDict() {
        builder_.DoEndDict();
        return builder_;
    }

    template <typename D>
    Builder& BaseContext<D>::EndArray() {
        builder_.DoEndArray();
        return builder_;
    }

    // Явная инстанциация шаблонов для всех возможных Derived
    // Это нужно, потому что шаблоны определены в .cpp
    template class BaseContext<DictContext>;
    template class BaseContext<ValueContext>;
    template class BaseContext<ArrayContext>;

} // namespace json
