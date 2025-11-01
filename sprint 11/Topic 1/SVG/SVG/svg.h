#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace svg {

	using OptionalString = std::optional<std::string>;

	// Кастомный тип
	using Color = std::string;

	// Константа - цвет не задан
	inline const Color NoneColor{ "none" };

	/*
	 * Вспомогательные функции для внутреннего использования при рендеринге SVG.
	 * Эти функции не предназначены для вызова извне и могут изменяться без предупреждения.
	 */
	namespace detail {

		/*
		 * @brief Экранирует специальные символы XML/HTML в строке для безопасного вывода в SVG.
		 *
		 * Заменяет символы:
		 *   - '<' → "&lt;"
		 *   - '>' → "&gt;"
		 *   - '&' → "&amp;"
		 *   - '"' → "&quot;"
		 *   - '\'' → "&apos;"
		 *
		 * @param text Исходная строка
		 * @return std::string Экранированная строка, безопасная для вставки в SVG
		 */
		std::string EscapeSvgText(std::string_view text);

		/*
		 * @brief Выводит значение в поток вывода.
		 *
		 * Базовая перегрузка для всех типов. Использует оператор `<<`.
		 *
		 * @tparam T Тип значения
		 * @param out Поток вывода
		 * @param value Значение для вывода
		 */
		template <typename T>
		inline void RenderValue(std::ostream& out, const T& value) {
			out << value;
		}

		/*
		 * @brief Специализация RenderValue для std::string — экранирует текст перед выводом.
		 *
		 * Используется для атрибутов и содержимого текстовых элементов SVG.
		 *
		 * @param out Поток вывода
		 * @param s Строка для вывода (будет экранирована)
		 */
		template <>
		inline void RenderValue<std::string>(std::ostream& out, const std::string& s) {
			out << EscapeSvgText(s);
		}

		/*
		 * @brief Выводит атрибут SVG в формате `name="value"`.
		 *
		 * Использует RenderValue для вывода значения.
		 *
		 * @tparam AttrType Тип значения атрибута
		 * @param out Поток вывода
		 * @param name Имя атрибута (например, "fill", "stroke-width")
		 * @param value Значение атрибута
		 */
		template <typename AttrType>
		inline void RenderAttr(std::ostream& out, std::string_view name, const AttrType& value) {
			using namespace std::literals;
			out << " "sv 
				<< name 
				<< "=\""sv;
			RenderValue(out, value);
			out << "\"";
		}

		/*
		 * @brief Выводит атрибут SVG, только если значение задано (не пустое).
		 *
		 * Полезно для необязательных атрибутов (например, font-family, stroke-linecap).
		 *
		 * @tparam AttrType Тип значения атрибута
		 * @param out Поток вывода
		 * @param name Имя атрибута
		 * @param value Опциональное значение атрибута
		 */
		template <typename AttrType>
		inline void RenderOptionalAttr(std::ostream& out,
			std::string_view name,
			const std::optional<AttrType>& value) {
			if (value) {
				RenderAttr(out, name, *value);
			}
		}
	} // namespace detail


	/*
	 * Класс StrokeLineCap - перечисление типов формы конца линий.
	 * По умолчанию свойство не выводится (свойство 'stroke-linecap')
	 */
	enum class StrokeLineCap {
		BUTT,
		ROUND,
		SQUARE,
	};

	// Переопределение оператора вывода для StrokeLineCap
	std::ostream& operator<<(std::ostream& out, StrokeLineCap value);

	/*
	 * Класс StrokeLineJoin - перечисление типов формы соединения линий.
	 * По умолчанию свойство не выводится (свойство 'stroke-linejoin')
	 */
	enum class StrokeLineJoin {
		ARCS,
		BEVEL,
		MITER,
		MITER_CLIP,
		ROUND,
	};

	// Переопределение оператора вывода для StrokeLineJoin
	std::ostream& operator<<(std::ostream& out, StrokeLineJoin value);

	/**
	 * @brief Представляет точку на плоскости с координатами x и y.
	 *
	 * Используется для задания положения элементов в SVG-документе.
	 */
	struct Point {
		Point() = default;
		
		/**
		 * @brief Конструктор с координатами.
		 * @param x Координата по горизонтали
		 * @param y Координата по вертикали
		 */
		Point(double x, double y)
			: x(x)
			, y(y) {
		}

		double x = 0;	///< Координата по оси X
		double y = 0;	///< Координата по оси Y
	};

	/**
	 * @brief Контекст рендеринга SVG-элемента.
	 *
	 * Хранит ссылку на поток вывода и управляет отступами для форматированного вывода.
	 */
	struct RenderContext {
		/**
		 * @brief Конструктор с потоком вывода.
		 * @param out Поток, в который будет осуществляться вывод
		 */
		RenderContext(std::ostream& out)
			: out(out) {
		}

		/**
		 * @brief Конструктор с настройками отступов.
		 * @param out Поток вывода
		 * @param indent_step Шаг увеличения отступа
		 * @param indent Текущий уровень отступа (в пробелах)
		 */
		RenderContext(std::ostream& out, int indent_step, int indent = 0)
			: out(out)
			, indent_step(indent_step)
			, indent(indent) {
		}

		/**
		 * @brief Возвращает новый контекст с увеличенным отступом.
		 * @return Новый объект RenderContext с отступом + indent_step
		 */
		RenderContext Indented() const {
			return { out, indent_step, indent + indent_step };
		}

		/**
		 * @brief Выводит текущий отступ в поток.
		 */
		void RenderIndent() const {
			for (int i = 0; i < indent; ++i) {
				out.put(' ');
			}
		}

		std::ostream& out;		///< Поток вывода
		int indent_step = 0;	///< Шаг увеличения отступа
		int indent = 0;			///< Текущий уровень отступа
	};

	/**
	 * @brief Абстрактный базовый класс для всех SVG-элементов.
	 *
	 * Реализует паттерн "Шаблонный метод": метод Render() вызывает виртуальный RenderObject().
	 */
	class Object {
	public:
		/**
		 * @brief Основной метод рендеринга объекта.
		 *
		 * Вызывает виртуальный RenderObject() — шаблонный метод.
		 *
		 * @param context Контекст вывода
		 */
		void Render(const RenderContext& context) const;

		virtual ~Object() = default;

	private:
		/**
		 * @brief Виртуальный метод для вывода конкретного SVG-элемента.
		 * 
		 * Должен быть реализован в каждом наследнике.
		 * 
		 * @param context Контекст вывода
		 */
		virtual void RenderObject(const RenderContext& context) const = 0;
	};

	/*
	 * @brief Базовый шаблонный класс, добавляющий графические свойства контура и заливки.
	 *
	 * Реализует паттерн CRTP (Curiously Recurring Template Pattern) для предоставления
	 * методов-сеттеров, возвращающих ссылку на производный класс (fluent interface).
	 *
	 * Используется как миксин для классов, представляющих SVG-элементы, которые могут
	 * иметь заливку и обводку (например, Circle, Polyline, Rectangle).
	 *
	 * @tparam Owner Производный класс, наследующий от PathProps (например, Circle, Polyline)
	 *
	 * @example
	 * Circle c;
	 * c.SetFillColor("red")
	 *   .SetStrokeColor("black")
	 *   .SetStrokeWidth(2.0);
	 */
	template <typename Owner>
	class PathProps {
	public:

		/*
		 * @brief Устанавливает цвет заливки фигуры.
		 *
		 * Атрибут `fill` в SVG.
		 *
		 * @param color Цвет в виде строки (например, "red", "#ff0000", "none")
		 * @return Ссылка на производный объект для цепочки вызовов
		 */
		Owner& SetFillColor(Color color) {
			fill_color_ = std::move(color);
			return AsOwner();
		}

		/*
		 * @brief Устанавливает цвет обводки (контура) фигуры.
		 *
		 * Атрибут `stroke` в SVG.
		 *
		 * @param color Цвет в виде строки (например, "blue", "#0000ff")
		 * @return Ссылка на производный объект для цепочки вызовов
		 */
		Owner& SetStrokeColor(Color color) {
			stroke_color_ = std::move(color);
			return AsOwner();
		}

		/*
		 * @brief Устанавливает толщину обводки фигуры.
		 *
		 * Атрибут `stroke-width` в SVG.
		 *
		 * @param width Толщина обводки в пользовательских единицах
		 * @return Ссылка на производный объект для цепочки вызовов
		 */
		Owner& SetStrokeWidth(double width) {
			stroke_width_ = width;
			return AsOwner();
		}

		/*
		 * @brief Устанавливает тип окончания линий обводки.
		 *
		 * Атрибут `stroke-linecap` в SVG.
		 * Возможные значения: Butt, Round, Square.
		 *
		 * @param line_cap Тип окончания линии
		 * @return Ссылка на производный объект для цепочки вызовов
		 */
		Owner& SetStrokeLineCap(StrokeLineCap line_cap) {
			stroke_line_cap_ = line_cap;
			return AsOwner();
		}

		/*
		 * @brief Устанавливает тип соединения линий обводки.
		 *
		 * Атрибут `stroke-linejoin` в SVG.
		 * Возможные значения: Miter, Round, Bevel и др.
		 *
		 * @param line_join Тип соединения линий
		 * @return Ссылка на производный объект для цепочки вызовов
		 */
		Owner& SetStrokeLineJoin(StrokeLineJoin line_join) {
			stroke_line_join_ = line_join;
			return AsOwner();
		}

	protected:
		/*
		 * @brief Защищённый деструктор.
		 *
		 * Запрещает удаление объекта типа PathProps через указатель на базовый класс.
		 * Так как класс не предназначен для полиморфного использования.
		 */
		~PathProps() = default;

		/*
		 * @brief Выводит все заданные атрибуты в поток.
		 *
		 * Используется в RenderObject производных классов для вывода атрибутов
		 * fill, stroke, stroke-width и др.
		 *
		 * @param out Поток вывода
		 */
		void RenderAttrs(std::ostream& out) const {
			using detail::RenderOptionalAttr;
			using namespace std::literals;
			RenderOptionalAttr(out, "fill"sv, fill_color_);
			RenderOptionalAttr(out, "stroke"sv, stroke_color_);
			RenderOptionalAttr(out, "stroke-width"sv, stroke_width_);
			RenderOptionalAttr(out, "stroke-linecap"sv, stroke_line_cap_);
			RenderOptionalAttr(out, "stroke-linejoin"sv, stroke_line_join_);
		}

	private:
		/*
		 * @brief Приводит *this к ссылке на производный класс.
		 *
		 * Позволяет возвращать Owner& в сеттерах. Безопасно, если Owner действительно
		 * наследует от PathProps<Owner> (гарантируется шаблоном).
		 *
		 * @return Ссылка на производный объект
		 */
		Owner& AsOwner() {
			return static_cast<Owner&>(*this);
		}

		std::optional<Color> fill_color_;       ///< Цвет заливки (атрибут fill)
		std::optional<Color> stroke_color_;     ///< Цвет обводки (атрибут stroke)
		std::optional<double> stroke_width_;    ///< Толщина обводки (атрибут stroke-width)
		std::optional<StrokeLineCap> stroke_line_cap_;      ///< Тип окончания линии (stroke-linecap) 
		std::optional<StrokeLineJoin> stroke_line_join_;    ///< Тип соединения линий (stroke-linejoin)
	};

	 /**
	  * @brief Представляет SVG-элемент <circle> — окружность.
	  *
	  * Поддерживает атрибуты заливки и обводки через наследование от PathProps.
	  * 
	  * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
	  *
	  * @example
	  * Circle{}
	  *   .SetCenter({100, 100})
	  *   .SetRadius(50)
	  *   .SetFillColor("blue")
	  *   .SetStrokeColor("black");
	  */
	class Circle final : public Object, public PathProps<Circle> {
	public:
		/**
		 * @brief Устанавливает центр окружности.
		 *
		 * Атрибуты `cx` и `cy`.
		 *
		 * @param center Координаты центра
		 * @return Ссылка на объект для цепочки вызовов
		 */
		Circle& SetCenter(Point center);

		/**
		 * @brief Устанавливает радиус окружности.
		 *
		 * Атрибут `r`.
		 *
		 * @param radius Радиус окружности
		 * @return Ссылка на объект для цепочки вызовов
		 */
		Circle& SetRadius(double radius);

	private:
		void RenderObject(const RenderContext& context) const override;

	private:
		Point center_;			///< Центр окружности
		double radius_ = 1.0;	///< Радиус окружности
	};

	 /**
	  * @brief Представляет SVG-элемент <polyline> — ломаную линию.
	  * 
	  * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
	  *
	  * Состоит из последовательности соединённых отрезков.
	  * Поддерживает атрибуты обводки через PathProps.
	  *
	  * @example
	  * Polyline{}
	  *   .AddPoint({0, 0})
	  *   .AddPoint({100, 50})
	  *   .AddPoint({200, 0})
	  *   .SetStrokeColor("red")
	  *   .SetStrokeWidth(2);
	  */
	class Polyline final : public Object, public PathProps<Polyline> {
	public:
		/**
		 * @brief Добавляет точку к ломаной.
		 *
		 * Точки соединяются в порядке добавления.
		 *
		 * @param point Координаты точки
		 * @return Ссылка на объект для цепочки вызовов
		 */
		Polyline& AddPoint(Point point);

	private:
		void RenderObject(const RenderContext& context) const override;

	private:
		std::vector<Point> points_;
	};

	/*
	 * Класс Text моделирует элемент <text> для отображения текста
	 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
	 */

	 /**
	  * @brief Представляет SVG-элемент <text> — текст.
	  * 
	  * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
	  *
	  * Позволяет задать позицию, смещение, размер, шрифт и содержимое текста.
	  *
	  * @example
	  * Text{}
	  *   .SetPosition({50, 50})
	  *   .SetFontSize(16)
	  *   .SetFontFamily("Verdana")
	  *   .SetFontWeight("bold")
	  *   .SetData("Hello, SVG!");
	  */
	class Text final : public Object, public PathProps<Text> {
	public:
		/**
		 * @brief Устанавливает координаты опорной точки текста.
		 *
		 * Атрибуты `x` и `y`.
		 *
		 * @param pos Координаты
		 * @return Ссылка на объект для цепочки вызовов
		 */
		Text& SetPosition(Point pos);

		/**
		 * @brief Устанавливает смещение относительно опорной точки.
		 *
		 * Атрибуты `dx` и `dy`.
		 *
		 * @param offset Смещение
		 * @return Ссылка на объект для цепочки вызовов
		 */
		Text& SetOffset(Point offset);

		/**
		 * @brief Устанавливает размер шрифта.
		 *
		 * Атрибут `font-size`.
		 *
		 * @param size Размер в пользовательских единицах
		 * @return Ссылка на объект для цепочки вызовов
		 */
		Text& SetFontSize(uint32_t size);

		/**
		 * @brief Устанавливает название шрифта.
		 *
		 * Атрибут `font-family`.
		 *
		 * @param font_family Название шрифта (например, "Arial")
		 * @return Ссылка на объект для цепочки вызовов
		 */
		Text& SetFontFamily(std::string font_family);

		/**
		 * @brief Устанавливает толщину шрифта.
		 *
		 * Атрибут `font-weight`.
		 *
		 * @param font_weight Значение (например, "bold", "normal")
		 * @return Ссылка на объект для цепочки вызовов
		 */
		Text& SetFontWeight(std::string font_weight);

		/**
		 * @brief Устанавливает текстовое содержимое.
		 *
		 * Будет экранировано перед выводом.
		 *
		 * @param data Текст
		 * @return Ссылка на объект для цепочки вызовов
		 */
		Text& SetData(std::string data);

	private:
		void RenderObject(const RenderContext& context) const override;

	private:
		Point position_{ 0,0 };			///< Позиция текста (x, y)
		Point offset_{ 0,0 };			///< Смещение (dx, dy)
		uint32_t size_ = 1;				///< Размер шрифта
		OptionalString font_family_;	///< Название шрифта
		OptionalString font_weight_;	///< Толщина шрифта
		std::string data_{};			///< Текстовое содержимое
	};

	/**
	 * @brief Абстрактный интерфейс контейнера SVG-объектов.
	 *
	 * Позволяет добавлять как простые объекты (Circle, Text), так и составные (реализующие Drawable).
	 */
	class ObjectContainer {
	public:
		/**
		 * @brief Добавляет любой объект, наследующий от Object.
		 *
		 * Использует шаблон для автоматического создания unique_ptr.
		 *
		 * @tparam ObjectType Тип объекта (например, Circle)
		 * @param object Объект для добавления
		 */
		template <typename ObjectType>
		void Add(ObjectType object) {
			AddPtr(std::make_unique<ObjectType>(std::move(object)));
		}

		/**
		 * @brief Добавляет объект, реализующий интерфейс Drawable.
		 *
		 * Вызывает drawable.Draw(*this), что позволяет объекту самому добавить
		 * свои элементы в контейнер.
		 *
		 * @param drawable Объект, реализующий Drawable
		 */
		//void Add(const Drawable& drawable) {
		//	drawable.Draw(*this);
		//}

	protected:
		/**
		 * @brief Защищённый деструктор.
		 *
		 * Запрещает удаление через указатель на базовый класс.
		 * Должен быть реализован в потомках.
		 */
		~ObjectContainer() = default;

		/**
		 * @brief Добавляет уникальный указатель на объект.
		 *
		 * Чисто виртуальный метод, должен быть реализован в производных.
		 *
		 * @param obj Указатель на объект
		 */
		virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;
	};

	/**
	 * @brief Интерфейс для объектов, способных рисовать себя в контейнере.
	 *
	 * Позволяет реализовать композицию: например, снеговик с поздравлением.
	 */
	class Drawable {
	public:
		/**
		 * @brief Виртуальный деструктор.
		 *
		 * Обязателен, так как объекты будут удаляться через указатель на базовый класс.
		 */
		virtual ~Drawable() = default;

		/**
		 * @brief Рисует объект, добавляя свои элементы в контейнер.
		 * 
		 * Чисто виртуальный метод, должен быть реализован в производных классах
		 *
		 * @param container Контейнер, в который добавляются элементы
		 */
		virtual void Draw(ObjectContainer& container) const = 0;
	};

	/**
	 * @brief Конкретная реализация ObjectContainer — полный SVG-документ.
	 *
	 * Хранит список объектов и выводит их в формате SVG с правильным заголовком.
	 */
	class Document : public ObjectContainer {
	public:

		/**
		 * @brief Добавляет объект в документ.
		 *
		 * @param obj Указатель на объект
		 */
		void AddPtr(std::unique_ptr<Object>&& obj);

		/**
		 * @brief Выводит весь документ в поток в формате SVG.
		 *
		 * @param out Поток вывода
		 */
		void Render(std::ostream& out) const;

	private:
		std::vector<std::unique_ptr<Object>> objects_;	///< Список объектов документа
	};

}  // namespace svg
