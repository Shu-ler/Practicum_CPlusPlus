#include "svg.h"

namespace svg {

	// === Реализация вспомогательных функций в namespace detail ===

	std::string detail::EscapeSvgText(std::string_view text) {
		std::string result;
		result.reserve(text.size());

		for (char c : text) {
			switch (c) {
			case '<':  result += "&lt;";	break;
			case '>':  result += "&gt;";	break;
			case '&':  result += "&amp;";	break;
			case '"':  result += "&quot;";	break;
			case '\'': result += "&apos;";	break;
			default:   result += c;			break;
			}
		}
		return result;
	}

	using namespace std::literals;

	// === Реализация Object ===

	void Object::Render(const RenderContext& context) const {
		context.RenderIndent();

		// Делегируем вывод тега своим подклассам
		RenderObject(context);

		context.out << std::endl;
	}

	// === Реализация Circle ===

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
		out << "<circle";
		detail::RenderAttr(out, "cx", center_.x);
		detail::RenderAttr(out, "cy", center_.y);
		detail::RenderAttr(out, "r", radius_);
		PathProps<Circle>::RenderAttrs(out); 
		out << "/>";
	}

	// === Реализация Polyline ===

	Polyline& Polyline::AddPoint(Point point) {
		points_.push_back(point);
		return *this;
	}

	void Polyline::RenderObject(const RenderContext& context) const {
		auto& out = context.out;
		out << "<polyline points=\"";
		bool first = true;
		for (const auto& point : points_) {
			if (!first) out << " ";
			first = false;
			out << point.x << "," << point.y;
		}
		out << "\"";
		PathProps<Polyline>::RenderAttrs(out);
		out << "/>";
	}

	// === Реализация Text ===

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
		out << "<text";
		detail::RenderAttr(out, "x", position_.x);
		detail::RenderAttr(out, "y", position_.y);
		detail::RenderAttr(out, "dx", offset_.x);
		detail::RenderAttr(out, "dy", offset_.y);
		detail::RenderAttr(out, "font-size", size_);

		detail::RenderOptionalAttr(out, "font-family", font_family_);
		detail::RenderOptionalAttr(out, "font-weight", font_weight_);

		PathProps<Text>::RenderAttrs(out);

		out << ">";
		detail::RenderValue(out, data_);
		out << "</text>";
	}

	// === Реализация Document ===

	void Document::AddPtr(std::unique_ptr<Object>&& obj) {
		objects_.emplace_back(std::move(obj));
	}

	void Document::Render(std::ostream& out) const {

		// Вывод заголовка файла
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
		out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";

		// Вывод содержимого файла
		for (const auto& obj : objects_) {
			obj->Render(RenderContext(out, 2).Indented());
			//out << "\n";
		}

		// Вывод концовки файла
		out << "</svg>\n";
	}

	// === Реализация StrokeLineCap ===

	std::ostream& operator<<(std::ostream& out, StrokeLineCap value) {
		std::string_view sv;

		switch (value) {
		case StrokeLineCap::BUTT:	sv = "butt";	break;
		case StrokeLineCap::ROUND:	sv = "round";	break;
		case StrokeLineCap::SQUARE:	sv = "square";	break;
		}
		return out << sv;
	}

	// === Реализация StrokeLineJoin ===

	std::ostream& operator<<(std::ostream& out, StrokeLineJoin value) {
		std::string_view sv;

		switch (value) {
		case StrokeLineJoin::ARCS:		 sv = "arcs";		break;
		case StrokeLineJoin::BEVEL:		 sv = "bevel";		break;
		case StrokeLineJoin::MITER:		 sv = "miter";		break;
		case StrokeLineJoin::MITER_CLIP: sv = "miter-clip";	break;
		case StrokeLineJoin::ROUND:		 sv = "round";		break;
		}
		return out << sv;
	}

	// === Реализация Color ===

	/**
	 * @brief Выводит цвет в поток в формате SVG.
	 *
	 * Использует std::visit и if constexpr для безопасного и эффективного
	 * доступа к значению variant без исключений.
	 * 
	 * Обрабатывает все возможные типы, из которых может состоять svg::Color:
	 * - std::monostate → выводит "none"
	 * - std::string   → выводит как есть (например, "red", "#ff0000")
	 * - svg::Rgb      → выводит в формате rgb(R,G,B)
	 * - svg::Rgba     → выводит в формате rgba(R,G,B,A)
	 */
	std::ostream& operator<<(std::ostream& out, const Color& color) {
		
		// Лямбда для обработки каждого типа цвета
		auto print_color = [&out](const auto& value) {
			using T = std::decay_t<decltype(value)>;

			if constexpr (std::is_same_v<T, std::monostate>) {
				out << "none";
			}
			else if constexpr (std::is_same_v<T, std::string>) {
				out << value;
			}
			else if constexpr (std::is_same_v<T, Rgb>) {
				out << "rgb(" << static_cast<int>(value.red) 
					<< ","
					<< static_cast<int>(value.green) 
					<< ","
					<< static_cast<int>(value.blue) 
					<< ")";
			}
			else if constexpr (std::is_same_v<T, Rgba>) {
				out << "rgba(" << static_cast<int>(value.red) 
					<< ","
					<< static_cast<int>(value.green) 
					<< ","
					<< static_cast<int>(value.blue) 
					<< ","
					<< value.opacity 
					<< ")";
			}
			};

		std::visit(print_color, color);
		return out;
	}

}  // namespace svg
