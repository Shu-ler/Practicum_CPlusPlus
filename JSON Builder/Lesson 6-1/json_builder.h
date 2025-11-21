#pragma once
#include "json.h"
#include <optional>
#include <deque>
#include <string>

namespace json {

    class Builder;

    // Forward declarations
    class DictContext;
    class ArrayContext;
    class ValueContext;

    // Базовый класс для общих методов
    class BaseContext {
    protected:
        explicit BaseContext(Builder& b) : builder_(b) {}
        Builder& builder_;

    public:
        DictContext StartDict();
        ArrayContext StartArray();
        Builder& EndDict();
        Builder& EndArray();
        Node Build();
    };

    // Контекст: после StartDict — можно Key или EndDict
    class DictContext : private BaseContext {
        Builder& builder_;  // ← нужен для Value() после Key()

    public:
        explicit DictContext(Builder& b);

        ValueContext Key(std::string key);  // ← не из BaseContext
        using BaseContext::EndDict;         // ✅
    };

    // Контекст: после StartArray — можно Value, Start*, EndArray
    class ArrayContext : private BaseContext {
        Builder& builder_;  // ← нужен для Value()

    public:
        explicit ArrayContext(Builder& b);

        ArrayContext Value(Node::Value value);  // ← не из BaseContext
        using BaseContext::StartDict;           // ✅
        using BaseContext::StartArray;          // ✅
        using BaseContext::EndArray;            // ✅
    };

    // Контекст: после Key — можно Value, StartDict, StartArray
    class ValueContext : private BaseContext {
        Builder& builder_;  // ← нужен для Value()

    public:
        explicit ValueContext(Builder& b);

        ValueContext Value(Node::Value value);     // ← не из BaseContext
        ValueContext Key(std::string key);  // ← ДОБАВИТЬ
        using BaseContext::StartDict;          // ✅
        using BaseContext::StartArray;         // ✅
    };

    class Builder {
        Node root_;
        std::deque<Node*> stack_;
        std::optional<std::string> key_;

        // Разрешаем контекстам доступ к Do*
        friend class BaseContext;
        friend class DictContext;
        friend class ArrayContext;
        friend class ValueContext;

        void DoKey(std::string key);
        void DoValue(Node::Value value);
        void DoStartDict();
        void DoStartArray();
        void DoEndDict();
        void DoEndArray();
        Node& Current();
        void CheckBuildReady();

    public:
        Builder() = default;

        DictContext StartDict();
        ArrayContext StartArray();
        Node Build();
        Builder& Value(Node::Value value);
    };

} // namespace json
