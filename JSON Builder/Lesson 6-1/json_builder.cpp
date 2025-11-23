#include "json_builder.h"
#include <stdexcept>
#include <variant>

namespace json {

    // =============================================================================
    // Builder - Вспомогательные методы
    // =============================================================================

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

    // =============================================================================
    // Builder - Do* методы
    // =============================================================================

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

        Node& curr_node = Current();
        if (key_.has_value()) {
            Dict& dict = const_cast<Dict&>(curr_node.AsDict());
            dict[key_.value()] = std::move(node);
            key_.reset();
        }
        else if (curr_node.IsArray()) {
            Array& array = const_cast<Array&>(curr_node.AsArray());
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

    // =============================================================================
    // Builder - публичные методы
    // =============================================================================

    Builder::DictItemContext Builder::StartDict() {
        DoStartDict();
        return DictItemContext(*this);
    }

    Builder::ArrayItemContext Builder::StartArray() {
        DoStartArray();
        return ArrayItemContext(*this);
    }

    Builder::DictValueContext Builder::Key(std::string key) {
        DoKey(key);
        return DictValueContext(*this);
    }

    Builder& Builder::Value(Node::Value value) {
        DoValue(std::move(value));
        return *this;
    }

    Builder& Builder::EndArray() {
        DoEndArray();
        return *this;
    }

    Builder& Builder::EndDict() {
        DoEndDict();
        return *this;
    }

    Node Builder::Build() {
        CheckBuildReady();
        return root_;
    }

    // =============================================================================
    // BaseContext — реализация
    // =============================================================================

    Node Builder::BaseContext::Build() {
        return builder_.Build();
    }

    Builder::DictValueContext Builder::BaseContext::Key(std::string key) {
        builder_.DoKey(std::move(key));
        return DictValueContext(builder_);
    }

    Builder::DictItemContext Builder::BaseContext::Value(Node::Value value) {
        builder_.DoValue(std::move(value));
        return DictItemContext(builder_);
    }

    Builder::DictItemContext Builder::BaseContext::StartDict() {
        builder_.DoStartDict();
        return DictItemContext(builder_);
    }

    Builder::ArrayItemContext Builder::BaseContext::StartArray() {
        builder_.DoStartArray();
        return ArrayItemContext(builder_);
    }

    Builder& Builder::BaseContext::EndDict() {
        builder_.DoEndDict();
        return builder_;
    }

    Builder& Builder::BaseContext::EndArray() {
        builder_.DoEndArray();
        return builder_;
    }

    // =============================================================================
    // DictItemContext — реализация
    // =============================================================================

    Builder::DictItemContext::DictItemContext(Builder& b) : BaseContext(b), builder_(b) {}

    // =============================================================================
    // DictValueContext — реализация
    // =============================================================================

    Builder::DictValueContext::DictValueContext(Builder& b) : BaseContext(b), builder_(b) {}

    // =============================================================================
    // ArrayItemContext — реализация
    // =============================================================================

    Builder::ArrayItemContext::ArrayItemContext(Builder& b) : BaseContext(b), builder_(b) {}

    Builder::ArrayItemContext Builder::ArrayItemContext::Value(Node::Value value) {
        builder_.DoValue(std::move(value));
        return *this;
    }

} // namespace json
