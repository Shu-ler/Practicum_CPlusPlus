#pragma once
#include "json.h"
#include <optional>
#include <deque>
#include <string>

/*
        Builder
        │
        ├── StartDict()     → DictContext
        │   ├─ Key(...)     → ValueContext
        │   └─ EndDict()    → Builder&
        │
        ├── StartArray()    → ArrayContext
        │   ├─ Value(...)   → ArrayContext
        │   ├─ StartDict()  → DictContext
        │   ├─ StartArray() → ArrayContext
        │   └─ EndArray()   → Builder&
        │
        ├── Key(...)        → ValueContext
        │   ├─ Value(...)   → Builder&
        │   ├─ StartDict()  → DictContext
        │   └─ StartArray() → ArrayContext
        │
        └── Value(...)      → Builder& (в корне или массиве)
*/

namespace json {

    class Builder;

    // Forward declarations
    class DictContext;
    class ArrayContext;
    class ValueContext;

    // Базовый класс для делегирования
    template <typename Derived>
    class BaseContext {
    protected:
        explicit BaseContext(Builder& b) : builder_(b) {}
        Builder& builder_;

    public:
        DictContext StartDict();
        ArrayContext StartArray();
        ValueContext Key(std::string key);
        Node Build();
        Builder& EndDict();
        Builder& EndArray();
    };

    // Контекст: после StartDict — можно Key или EndDict
    class DictContext : private BaseContext<DictContext> {
    public:
        explicit DictContext(Builder& b);

        ValueContext Key(std::string key);
        Builder& EndDict();
    };

    // Контекст: после StartArray — можно Value, Start*, EndArray
    class ArrayContext : private BaseContext<ArrayContext> {
    public:
        explicit ArrayContext(Builder& b);

        ArrayContext Value(Node::Value value);
        DictContext StartDict();
        ArrayContext StartArray();
        Builder& EndArray();
    };

    // Контекст: после Key — можно Value, StartDict, StartArray
    class ValueContext : private BaseContext<ValueContext> {
    public:
        explicit ValueContext(Builder& b);

        Builder& Value(Node::Value value);
        DictContext StartDict();
        ArrayContext StartArray();
    };

    /**
     * @brief Класс для пошагового построения JSON-объекта.
     * Поддерживает fluent-интерфейс с проверкой контекста на этапе компиляции.
     */
    class Builder {
    public:
        Builder() = default;

        DictContext StartDict();
        ArrayContext StartArray();
        Node Build();

    private:
        // Приватные методы
        Node& Current();
        void CheckBuildReady();

        void DoKey(std::string key);
        void DoValue(Node::Value value);
        void DoStartDict();
        void DoStartArray();
        void DoEndDict();
        void DoEndArray();

        Node root_;
        std::deque<Node*> stack_;
        std::optional<std::string> key_;
    };

} // namespace json
