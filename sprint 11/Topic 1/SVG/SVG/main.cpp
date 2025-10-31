#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>


using namespace std::literals;
//using namespace svg;

// TODO В основном приложении — то есть в файле main.cpp — внутри пространства 
// имён shapes создайте классы фигур // Triangle, Star и Snowman, 
// реализующие интерфейс svg::Drawable.
namespace shapes {

    // -------------------- Класс Star --------------------

    // Создает полилинию, окаймляющую звезду. 
    svg::Polyline CreateStar(svg::Point center, double outer_radius, double inner_radius, int num_rays) {
        svg::Polyline polyline;

        for (int i = 0; i <= num_rays; ++i) {
            double angle = 2 * M_PI * (i % num_rays) / num_rays;
            polyline.AddPoint({ center.x + outer_radius * sin(angle), center.y - outer_radius * cos(angle) });
            if (i == num_rays) {
                break;
            }
            angle += M_PI / num_rays;
            polyline.AddPoint({ center.x + inner_radius * sin(angle), center.y - inner_radius * cos(angle) });
        }

        return polyline;
    }

    class Star : public svg::Drawable {
    public:
        Star(svg::Point center, double outer_radius, double inner_radius, int num_rays) 
            : center_(center)
            , outer_radius_(outer_radius)
            , inner_radius_(inner_radius)
            , num_rays_(num_rays) {
        }

        void Draw(svg::ObjectContainer& container) const override {
        
            // Создаём полилинию для звезды
            svg::Polyline star_polyline = CreateStar(center_, outer_radius_, inner_radius_, num_rays_);
            
            // Добавляем полилинию в контейнер
            container.Add(star_polyline);
        };

    private:
        svg::Point center_;
        double outer_radius_;
        double inner_radius_;
        int num_rays_;
    };

    // -------------------- Класс Triangle --------------------

    class Triangle : public svg::Drawable {
    public:
        Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
            : p1_(p1)
            , p2_(p2)
            , p3_(p3) {
        }

        // Реализует метод Draw интерфейса svg::Drawable
        void Draw(svg::ObjectContainer& container) const override {
            container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
        }

    private:
        svg::Point p1_, p2_, p3_;
    };

    // -------------------- Класс Snowman --------------------

    class Snowman : public svg::Drawable {
    public:
        Snowman(svg::Point head_center, double radius) :
            head_center_(head_center), radius_(radius) {
        }

        void Draw(svg::ObjectContainer& container) const override {
            // Определяем координаты и радиусы кругов снеговика
            svg::Point head_center = head_center_;
            double radius = radius_;
            svg::Circle head;
            head.SetCenter(head_center).SetRadius(radius);

            svg::Circle body;
            body.SetCenter({ head_center.x, head_center.y + 2 * radius }).SetRadius(1.5 * radius);

            svg::Circle base;
            base.SetCenter({ head_center.x, head_center.y + 5 * radius }).SetRadius(2 * radius);

            // Добавляем круги в контейнер
            container.Add(base);
            container.Add(body);
            container.Add(head);
        };

    private:
        svg::Point head_center_;
        double radius_;
    };

} // namespace shapes

/*
Пример использования библиотеки. Он будет компилироваться и работать, когда вы реализуете
все классы библиотеки.
*/
/*
namespace {

// Выводит приветствие, круг и звезду
void DrawPicture() {
    Document doc;
    doc.Add(Circle().SetCenter({20, 20}).SetRadius(10));
    doc.Add(Text()
                .SetFontFamily("Verdana"s)
                .SetPosition({35, 20})
                .SetOffset({0, 6})
                .SetFontSize(12)
                .SetFontWeight("bold"s)
                .SetData("Hello C++"s));
    doc.Add(CreateStar({20, 50}, 10, 5, 5));
    doc.Render(std::cout);
}

}  // namespace
*/

template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}

int main() {
    using namespace svg;
    using namespace shapes;
    using namespace std;

    vector<unique_ptr<svg::Drawable>> picture;

    picture.emplace_back(make_unique<Triangle>(Point{ 100, 20 }, Point{ 120, 50 }, Point{ 80, 40 }));
    
    // 5-лучевая звезда с центром {50, 20}, длиной лучей 10 и внутренним радиусом 4
    picture.emplace_back(make_unique<Star>(Point{ 50.0, 20.0 }, 10.0, 4.0, 5));
    
    // Снеговик с "головой" радиусом 10, имеющей центр в точке {30, 20}
    picture.emplace_back(make_unique<Snowman>(Point{ 30, 20 }, 10.0));

    svg::Document doc;
    
    // Так как документ реализует интерфейс ObjectContainer,
    // его можно передать в DrawPicture в качестве цели для рисования
    DrawPicture(picture, doc);

    // Выводим полученный документ в stdout
    doc.Render(cout);
}