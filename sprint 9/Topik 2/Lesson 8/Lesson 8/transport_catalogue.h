#pragma once

#include "geo.h"
#include <string>

namespace stop {

	class Stop {
	public:
		Stop() = default;
		Stop(const std::string& name, const double lat, const double lon) :
			name_(name),
			coordinates_{ lat, lon } {
		};

		std::string GetName() const {
			return name_;
		};

		Coordinates GetCoordinates() const {
			return coordinates_;
		}

	private:
		std::string name_{};
		Coordinates coordinates_{ 0.0, 0.0 };
	};

} // namespace stop

class TransportCatalogue {

	// TODO: Метод добавления маршрута в базу
	// TODO: Метод добавления остановки в базу
	// TODO: Метод поиска маршрута по имени
	// TODO: Метод поиска остановки по имени
	// TODO: Метод получения информации о маршруте

private:
	// TODO: Члены класса
};
