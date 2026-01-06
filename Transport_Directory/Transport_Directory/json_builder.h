#pragma once
#include "json.h"
#include <optional>
#include <deque>
#include <string>

namespace json {

    /**
     * @brief Построитель JSON-документа с fluent-интерфейсом
     *
     * Позволяет пошагово создавать JSON-объекты с типобезопасным API.
     */
    class Builder {
        Node root_;
        std::deque<Node*> stack_;
        std::optional<std::string> key_;

        // Forward declarations
        class BaseContext;
        class DictItemContext;
        class DictValueContext;
        class ArrayItemContext;

        // Друзья — для доступа к Do* методам
        friend class DictItemContext;
        friend class DictValueContext;
        friend class ArrayItemContext;

        // Вспомогательные методы
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

        // Начало контейнеров
        DictItemContext StartDict();
        ArrayItemContext StartArray();

        // Работа с ключами и значениями
        DictValueContext Key(std::string key);
        Builder& Value(Node::Value value);

        // Завершение контейнеров
        Builder& EndArray();
        Builder& EndDict();
        
        // Финализация
        Node Build();
    };

    // =============================================================================
    // BaseContext — общий базовый класс для контекстов
    //
    // Содержит все возможные методы, но каждый наследник решает,
    // какие из них разрешены через `using`.
    // =============================================================================
    class Builder::BaseContext {
    protected:
        Builder& builder_;

        explicit BaseContext(Builder& b) : builder_(b) {}

    public:
        Node Build();

        DictValueContext Key(std::string key);
        DictItemContext Value(Node::Value value);

        DictItemContext StartDict();
        ArrayItemContext StartArray();
        
        Builder& EndDict();
        Builder& EndArray();
    };

    // =============================================================================
    // DictItemContext — внутри словаря после StartDict(): можно .Key() или .EndDict() 
    // =============================================================================

    class Builder::DictItemContext final : private BaseContext {
    public:
        explicit DictItemContext(Builder& b);
        
        // Доступные операции
        using BaseContext::Key;
        using BaseContext::EndDict;
    };

    // =============================================================================
    // DictValueContext — после .Key(): можно .Value(), .StartDict(), .StartArray()
    // =============================================================================

    class Builder::DictValueContext final : private BaseContext {
    public:
        explicit DictValueContext(Builder& b);

        // Доступные операции
        using BaseContext::Value;
        using BaseContext::StartDict;
        using BaseContext::StartArray;
    };

    // =============================================================================
    // ArrayItemContext — внутри массива: можно .Value(), .Start*, .EndArray()
    // =============================================================================

    class Builder::ArrayItemContext final : private BaseContext {
    public:
        explicit ArrayItemContext(Builder& b);

        // 'Перегрузка' операции
        ArrayItemContext Value(Node::Value value);

        // Доступные операции
        using BaseContext::StartDict;
        using BaseContext::StartArray;
        using BaseContext::EndArray;
    };

} // namespace json
