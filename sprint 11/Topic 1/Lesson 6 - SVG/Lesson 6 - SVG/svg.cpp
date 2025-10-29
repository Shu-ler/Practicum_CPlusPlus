#include "svg.h"

// Экранирует специальные символы SVG
std::string EscapeSvgText(std::string_view text) {
    std::string result;
    for (char c : text) {
        switch (c) {
            case '"':  result += "&quot;";  break;
            case '<':  result += "&lt;";    break;
            case '>':  result += "&gt;";    break;
            case '&':  result += "&amp;";   break;
            case '\'':  result += "&apos;"; break;
            
            default:   result += c; break;
        }
    }
    return result;
}

namespace svg {

    using namespace std::literals;

    void Object::Render(const RenderContext& context) const {
        context.RenderIndent();

        // Делегируем вывод тега своим подклассам
        RenderObject(context);

        context.out << std::endl;
    }

    // ---------- Circle ------------------

    Circle& Circle::SetCenter(Point center) {
        center_ = center;
        return *this;
    }

    Circle& Circle::SetRadius(double radius) {
        radius_ = radius;
        return *this;
    }

    void Circle::RenderObject(const RenderContext& context) const {
        auto& out = context.out;

        context.RenderIndent();

        out << "<circle cx=\""sv 
            << center_.x 
            << "\" cy=\""sv 
            << center_.y 
            << "\" "sv
            << "r=\""sv 
            << radius_ 
            << "\" "sv
            << "/>"sv;
    }

    // ---------- Polyline ------------------

    Polyline& Polyline::AddPoint(Point point) {
        points_.push_back(point);
        return *this;
    }

    void Polyline::RenderObject(const RenderContext& context) const {
        auto& out = context.out;

        context.RenderIndent();

        out << "<polyline points=\""sv;
        bool first = true;
        for (const Point& point : points_) {
            if (first) {
                first = false;
            }
            else {
                out << ' ';
            }
            out << point.x 
                << ',' 
                << point.y;
        }
        out << "\" "sv
            << "/>"sv;
    }

    // ---------- Polyline ------------------

    Text& Text::SetPosition(Point pos) {
        position_ = pos;
        return *this;
    }

    Text& Text::SetOffset(Point offset) {
        offset_ = offset;
        return *this;
    }

    Text& Text::SetFontSize(uint32_t size) {
        size_ = size;
        return *this;
    }

    Text& Text::SetFontFamily(std::string font_family) {
        font_family_ = std::move(font_family);
        return *this;
    }

    Text& Text::SetFontWeight(std::string font_weight) {
        font_weight_ = std::move(font_weight);
        return *this;
    }

    Text& Text::SetData(std::string data) {
        data_ = std::move(data);
        return *this;
    }

    void Text::RenderObject(const RenderContext& context) const {
        auto& out = context.out;

        context.RenderIndent();

        // Вывод обязательных атрибутов
        out << "<text "
            << "x=\"" << position_.x << "\" "
            << "y=\"" << position_.y << "\" "
            << "dx=\"" << offset_.x << "\" "
            << "dy=\"" << offset_.y << "\" "
            << "font-size=\"" << size_ << "\"";

        // Вывод атрибутов с контролем заполнения
        if (!font_family_.empty()) {
            out << " font-family=\"" << font_family_ << "\"";
        }

        if (!font_weight_.empty()) {
            out << " font-weight=\"" << font_weight_ << "\"";
        }

        // Вывод собственно текста
        out << ">"
            << EscapeSvgText(data_) 
            << "</text>"
            << std::endl;
    }

    // ---------- Document ------------------

    void Document::AddPtr(std::unique_ptr<Object>&& obj) {
        objects_.emplace_back(std::move(obj));
    }

    void Document::Render(std::ostream& out) const {

        // Вывод заголовка файла
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";

        // Вывод содержимого файла
        for (const auto& obj : objects_) {
            obj->Render(RenderContext(out, 2));
        }

        // Вывод концовки файла
        out << "</svg>\n";
    }

}  // namespace svg
