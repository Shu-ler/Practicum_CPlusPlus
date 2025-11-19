#pragma once
#include "json.h"
#include <optional>
#include <deque>
#include <string>

namespace json {

    /**
     * @brief Класс для построения JSON-объекта с помощью цепочки вызовов (fluent interface).
     *
     * Позволяет пошагово собирать JSON-объект, используя методы:
     *   - StartDict / EndDict — для словарей
     *   - StartArray / EndArray — для массивов
     *   - Key — для задания ключа в словаре
     *   - Value — для добавления значения
     *   - Build — для получения готового Node
     *
     * Пример:
     *   json::Builder{}
     *       .StartDict()
     *           .Key("name"s).Value("Alice"s)
     *           .Key("age"s).Value(30)
     *       .EndDict()
     *       .Build();
     */
    class Builder {
    public:
        Builder() = default;

        /**
         * @brief Задаёт ключ в текущем словаре.
         * @param key строка — ключ.
         * @return Ссылка на *this для цепочки вызовов.
         * @throws std::logic_error если:
         *   - вызван вне словаря;
         *   - предыдущий ключ не был связан со значением.
         */
        Builder& Key(std::string key);

        /**
         * @brief Добавляет значение.
         * Может быть вызван:
         *   - после конструктора — чтобы задать весь объект;
         *   - после Key — чтобы задать значение по ключу;
         *   - в массиве — чтобы добавить элемент.
         * @param value значение любого типа из Node::Value.
         * @return Ссылка на *this.
         * @throws std::logic_error при нарушении контекста.
         */
        Builder& Value(Node::Value value);

        /**
         * @brief Начинает определение словаря.
         * @return Ссылка на *this.
         * @throws std::logic_error если:
         *   - вызван после Key, но до Value;
         *   - вызван в недопустимом контексте.
         */
        Builder& StartDict();

        /**
         * @brief Завершает определение словаря.
         * @return Ссылка на *this.
         * @throws std::logic_error если:
         *   - стек пуст;
         *   - текущий узел — не словарь;
         *   - в словаре есть незавершённый ключ.
         */
        Builder& EndDict();

        /**
         * @brief Начинает определение массива.
         * @return Ссылка на *this.
         * @throws std::logic_error в случае ошибки контекста.
         */
        Builder& StartArray();

        /**
         * @brief Завершает определение массива.
         * @return Ссылка на *this.
         * @throws std::logic_error если:
         *   - стек пуст;
         *   - текущий узел — не массив.
         */
        Builder& EndArray();

        /**
         * @brief Возвращает построенный JSON-объект.
         * @return объект Node.
         * @throws std::logic_error если:
         *   - объект не инициализирован;
         *   - есть незавершённые контейнеры.
         */
        Node Build();

    private:
        /**
         * @brief Корневой узел JSON-объекта.
         *
         * - До первого вызова Value/Start* — содержит std::nullptr_t
         * - После Build() — остаётся неизменным
         */
        Node root_;

        /**
         * @brief Стек указателей на открытые контейнеры.
         *
         * Хранит путь от корня к текущему контексту.
         * Позволяет вернуться к родительскому узлу при EndDict/EndArray.
         *
         * Пуст, если контекст — корень.
         */
        std::deque<Node*> stack_;

        /**
         * @brief Текущий ожидаемый ключ в словаре.
         *
         * Состояния:
         * - nullopt: можно вызвать Key(), Value() (в массиве), Start* или End*.
         * - имеет значение: ожидается вызов Value(), StartDict() или StartArray()
         *   для завершения пары "ключ-значение".
         *
         * Устанавливается в Key(), сбрасывается в Value()/Start*.
         */
        std::optional<std::string> key_;

        // Вспомогательные методы
        Node& Current();  // Получить текущий узел (верх стека)

        /**
         * @brief Проверяет, что Build() может быть вызван.
         * @throws std::logic_error если:
         *   - стек не пуст (есть незавершённые контейнеры);
         *   - root_ не инициализирован.
         */
        void CheckBuildReady();
    };

} // namespace json
