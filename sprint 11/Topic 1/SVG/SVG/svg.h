#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace svg {

    /*
     * Вспомогательная структура, представляющая координаты точки на плоскости.
     * Используется для определения позиций элементов в SVG-графике
     */

    struct Point {
        Point() = default;
        Point(double x, double y)
            : x(x)
            , y(y) {
        }

        double x = 0;
        double y = 0;
    };

    /*
     * Вспомогательная структура, хранящая контекст для вывода SVG-документа с отступами.
     * Хранит ссылку на поток вывода, текущее значение и шаг отступа при выводе элемента
     */
    struct RenderContext {
        RenderContext(std::ostream& out)
            : out(out) {
        }

        RenderContext(std::ostream& out, int indent_step, int indent = 0)
            : out(out)
            , indent_step(indent_step)
            , indent(indent) {
        }

        RenderContext Indented() const {
            return { out, indent_step, indent + indent_step };
        }

        void RenderIndent() const {
            for (int i = 0; i < indent; ++i) {
                out.put(' ');
            }
        }

        std::ostream& out;
        int indent_step = 0;
        int indent = 0;
    };

    /*
     * Абстрактный базовый класс Object служит для унифицированного хранения
     * конкретных тегов SVG-документа
     * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
     */
    class Object {
    public:
        void Render(const RenderContext& context) const;

        virtual ~Object() = default;

    private:
        // Чисто виртуальный метод для предоставления унифицированного интерфейса
        // для вывода SVG-тегов, реализуемого в производных классах
        virtual void RenderObject(const RenderContext& context) const = 0;
    };

    /*
     * Класс Circle моделирует элемент <circle> для отображения круга
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
     */
    class Circle final : public Object {
    public:
        // Установка центра круга (cx, cy)
        Circle& SetCenter(Point center);

        // Установка радиуса круга (r)
        Circle& SetRadius(double radius);

    private:
        // Вывод координат центра круга (cx, cy) и радиуса (r) в формате SVG
        void RenderObject(const RenderContext& context) const override;

    private:
        Point center_;
        double radius_ = 1.0;
    };

    /*
     * Класс Polyline моделирует элемент <polyline> для отображения ломаных линий
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
     */
    class Polyline final : public Object {
    public:
        // Добавляет очередную вершину к ломаной линии
        Polyline& AddPoint(Point point);

    private:
        // Вывод всех точек ломаной линии в формате SVG
        void RenderObject(const RenderContext& context) const override;

    private:
        std::vector<Point> points_;
    };

    /*
     * Класс Text моделирует элемент <text> для отображения текста
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
     */
    class Text final : public Object {
    public:
        // Задаёт координаты опорной точки (атрибуты x и y)
        Text& SetPosition(Point pos);

        // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
        Text& SetOffset(Point offset);

        // Задаёт размеры шрифта (атрибут font-size)
        Text& SetFontSize(uint32_t size);

        // Задаёт название шрифта (атрибут font-family)
        Text& SetFontFamily(std::string font_family);

        // Задаёт толщину шрифта (атрибут font-weight)
        Text& SetFontWeight(std::string font_weight);

        // Задаёт текстовое содержимое объекта (отображается внутри тега <text>)
        Text& SetData(std::string data);

    private:
        void RenderObject(const RenderContext& context) const override;

    private:
        Point position_{ 0,0 };     // Координаты опорной точки (атрибуты x и y)
        Point offset_{ 0,0 };       // Cмещение относительно опорной точки (атрибуты dx, dy)
        uint32_t size_ = 1;         // Размер шрифта (атрибут font-size)
        std::string font_family_{}; // Название шрифта (атрибут font-family)
        std::string font_weight_{}; // Толщина шрифта (атрибут font-weight)
        std::string data_{};        // Содержимое объекта (отображается внутри тега <text>)
    };

    /*
     * Абстрактный базовый класс (почти интерфейс) ObjectContainer
     * Представляет интерфейс контейнеров SVG-объектов
     */
    class ObjectContainer {
    public:
        // Метод Add добавляет в svg-документ любой объект-наследник svg::Object.
        // Пример использования:
        //  Document doc;
        //  doc.Add(Circle().SetCenter({20, 30}).SetRadius(15));
        template <typename ObjectType>
        void Add(ObjectType object) {
            AddPtr(std::make_unique<ObjectType>(std::move(object)));
        }

        // Добавляет в svg-документ объект-наследник svg::Object
        // Чисто виртуальный метод, должен быть реализован в производных классах
        virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;

    protected:
        // Защищённый невиртуальный деструктор
        ~ObjectContainer() = default; 
    };

    /*
     * Абстрактный базовый класс (интерфейс) Drawable.
     * Представляет объекты, которые могут быть нарисованы на любом объекте,
     * реализующем интерфейс ObjectContainer.
     */
    class Drawable {
    public:
        // Виртуальный деструктор.
        // Объекты этого класса будут удаляться через указатель на базовый класс
        virtual ~Drawable() = default;
        
        // Рисует объект на указанном контейнере ObjectContainer
        // Чисто виртуальный метод, должен быть реализован в производных классах
        virtual void Draw(ObjectContainer& container) const = 0;
    };

    /*
     * Document - наследник ObjectContainer, конкретная реализация класса ObjectContainer
     */
    class Document : public ObjectContainer {
    public:

        // Добавляет в svg-документ объект-наследник svg::Object
        void AddPtr(std::unique_ptr<Object>&& obj);

        // Выводит в ostream svg-представление документа
        void Render(std::ostream& out) const;

    private:
        // Вектор для хранения объектов документа
        std::vector<std::unique_ptr<Object>> objects_;
    };

}  // namespace svg
