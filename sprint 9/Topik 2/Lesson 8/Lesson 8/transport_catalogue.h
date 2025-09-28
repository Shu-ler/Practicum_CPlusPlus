#pragma once

#include "geo.h"
#include <string>

namespace stop {

	/*
	 * Класс представляет собой модель остановки общественного транспорта.
	 * Содержит информацию о названии остановки и её географических координатах.
	 */
	class Stop {
	public:

		// Конструктор по умолчанию
		Stop() = default;

		// Инициализирует новую остановку с заданными именем и координатами.
		// @param name название остановки
		// @param lat широта
		// @param lon долгота
		Stop(const std::string& name, const double lat, const double lon) :
			name_(name),
			coordinates_{ lat, lon } {
		};

		// Возвращает название остановки.
		// @return название остановки
		std::string GetName() const {
			return name_;
		};

		// Возвращает координаты остановки.
		// @return координаты остановки
		Coordinates GetCoordinates() const {
			return coordinates_;
		}

	private:
		std::string name_{};					// Название остановки.
		Coordinates coordinates_{ 0.0, 0.0 };	// Географические координаты остановки.
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
