#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace svg {

    /*
     * ��������������� ���������, �������������� ���������� ����� �� ���������.
     * ������������ ��� ����������� ������� ��������� � SVG-�������
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
     * ��������������� ���������, �������� �������� ��� ������ SVG-��������� � ���������.
     * ������ ������ �� ����� ������, ������� �������� � ��� ������� ��� ������ ��������
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
     * ����������� ������� ����� Object ������ ��� ���������������� ��������
     * ���������� ����� SVG-���������
     * ��������� ������� "��������� �����" ��� ������ ����������� ����
     */
    class Object {
    public:
        void Render(const RenderContext& context) const;

        virtual ~Object() = default;

    private:
        // ����� ����������� ����� ��� �������������� ���������������� ����������
        // ��� ������ SVG-�����, ������������ � ����������� �������
        virtual void RenderObject(const RenderContext& context) const = 0;
    };

    /*
     * ����� Circle ���������� ������� <circle> ��� ����������� �����
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
     */
    class Circle final : public Object {
    public:
        // ��������� ������ ����� (cx, cy)
        Circle& SetCenter(Point center);

        // ��������� ������� ����� (r)
        Circle& SetRadius(double radius);

    private:
        // ����� ��������� ������ ����� (cx, cy) � ������� (r) � ������� SVG
        void RenderObject(const RenderContext& context) const override;

    private:
        Point center_;
        double radius_ = 1.0;
    };

    /*
     * ����� Polyline ���������� ������� <polyline> ��� ����������� ������� �����
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
     */
    class Polyline final : public Object {
    public:
        // ��������� ��������� ������� � ������� �����
        Polyline& AddPoint(Point point);

    private:
        // ����� ���� ����� ������� ����� � ������� SVG
        void RenderObject(const RenderContext& context) const override;

    private:
        std::vector<Point> points_;
    };

    /*
     * ����� Text ���������� ������� <text> ��� ����������� ������
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
     */
    class Text final : public Object {
    public:
        // ����� ���������� ������� ����� (�������� x � y)
        Text& SetPosition(Point pos);

        // ����� �������� ������������ ������� ����� (�������� dx, dy)
        Text& SetOffset(Point offset);

        // ����� ������� ������ (������� font-size)
        Text& SetFontSize(uint32_t size);

        // ����� �������� ������ (������� font-family)
        Text& SetFontFamily(std::string font_family);

        // ����� ������� ������ (������� font-weight)
        Text& SetFontWeight(std::string font_weight);

        // ����� ��������� ���������� ������� (������������ ������ ���� <text>)
        Text& SetData(std::string data);

    private:
        void RenderObject(const RenderContext& context) const override;

    private:
        Point position_{ 0,0 };     // ���������� ������� ����� (�������� x � y)
        Point offset_{ 0,0 };       // C������� ������������ ������� ����� (�������� dx, dy)
        uint32_t size_ = 1;         // ������ ������ (������� font-size)
        std::string font_family_{}; // �������� ������ (������� font-family)
        std::string font_weight_{}; // ������� ������ (������� font-weight)
        std::string data_{};        // ���������� ������� (������������ ������ ���� <text>)
    };

    /*
     * ����������� ������� ����� (����� ���������) ObjectContainer
     * ������������ ��������� ����������� SVG-��������
     */
    class ObjectContainer {
    public:
        // ����� Add ��������� � svg-�������� ����� ������-��������� svg::Object.
        // ������ �������������:
        //  Document doc;
        //  doc.Add(Circle().SetCenter({20, 30}).SetRadius(15));
        template <typename ObjectType>
        void Add(ObjectType object) {
            AddPtr(std::make_unique<ObjectType>(std::move(object)));
        }

        // ��������� � svg-�������� ������-��������� svg::Object
        // ����� ����������� �����, ������ ���� ���������� � ����������� �������
        virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;

    protected:
        // ���������� ������������� ����������
        ~ObjectContainer() = default; 
    };

    /*
     * ����������� ������� ����� (���������) Drawable.
     * ������������ �������, ������� ����� ���� ���������� �� ����� �������,
     * ����������� ��������� ObjectContainer.
     */
    class Drawable {
    public:
        // ����������� ����������.
        // ������� ����� ������ ����� ��������� ����� ��������� �� ������� �����
        virtual ~Drawable() = default;
        
        // ������ ������ �� ��������� ���������� ObjectContainer
        // ����� ����������� �����, ������ ���� ���������� � ����������� �������
        virtual void Draw(ObjectContainer& container) const = 0;
    };

    /*
     * Document - ��������� ObjectContainer, ���������� ���������� ������ ObjectContainer
     */
    class Document : public ObjectContainer {
    public:

        // ��������� � svg-�������� ������-��������� svg::Object
        void AddPtr(std::unique_ptr<Object>&& obj);

        // ������� � ostream svg-������������� ���������
        void Render(std::ostream& out) const;

    private:
        // ������ ��� �������� �������� ���������
        std::vector<std::unique_ptr<Object>> objects_;
    };

}  // namespace svg
