#pragma once
#include "json.h"
#include <optional>
#include <deque>
#include <string>

namespace json {

    class Builder;

    class Builder {
        Node root_;
        std::deque<Node*> stack_;
        std::optional<std::string> key_;

        // Forward declarations
        class BaseContext;
        class DictItemContext;
        class DictValueContext;
        class ArrayItemContext;

        // Друзья
        friend class DictItemContext;
        friend class DictValueContext;
        friend class ArrayItemContext;

        // Вспомогательные
        Node& Current();
        void CheckBuildReady();

        void DoKey(std::string key);
        void DoValue(Node::Value value);
        void DoStartDict();
        void DoStartArray();
        void DoEndDict();
        void DoEndArray();

    public:
        Builder() = default;

        DictItemContext StartDict();
        ArrayItemContext StartArray();
        DictValueContext Key(std::string key);
        Builder& Value(Node::Value value);

        Builder& EndArray();
        Builder& EndDict();
        
        Node Build();

    };

    // =============================================================================
    // BaseContext — общий базовый класс
    // =============================================================================

    class Builder::BaseContext {
    protected:
        Builder& builder_;

    public:
        explicit BaseContext(Builder& b) : builder_(b) {}

        Node Build();
        DictValueContext Key(std::string key);
        DictItemContext Value(Node::Value value);
        DictItemContext StartDict();
        ArrayItemContext StartArray();
        Builder& EndDict();
        Builder& EndArray();
    };

    // =============================================================================
    // DictItemContext — внутри словаря: можно Key, EndDict
    // =============================================================================

    class Builder::DictItemContext : private BaseContext {
        Builder& builder_;
    public:
        explicit DictItemContext(Builder& b);
        
        // Общие методы
        using BaseContext::Key;
        using BaseContext::EndDict;
    };

    // =============================================================================
    // DictValueContext — после Key: можно Value, Start*
    // =============================================================================

    class Builder::DictValueContext : private BaseContext {
        Builder& builder_;
    public:
        explicit DictValueContext(Builder& b);

        // Общие методы
        using BaseContext::Value;
        using BaseContext::StartDict;
        using BaseContext::StartArray;
    };

    // =============================================================================
    // ArrayItemContext — внутри массива: можно Value, Start*, EndArray
    // =============================================================================

    class Builder::ArrayItemContext : private BaseContext {
        Builder& builder_;
    public:
        explicit ArrayItemContext(Builder& b);

        ArrayItemContext Value(Node::Value value);

        // Общие методы
        using BaseContext::StartDict;
        using BaseContext::StartArray;
        using BaseContext::EndArray;
    };

} // namespace json
