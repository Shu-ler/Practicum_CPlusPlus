#pragma once

#include "geo.h"
#include <string>
#include <iostream>

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
		}

		// Возвращает координаты остановки.
		// @return координаты остановки
		Coordinates GetCoordinates() const {
			return coordinates_;
		}

		// Метод установки координат
		// @param lat широта
		// @param lon долгота
		void SetCoordinates(const double lat, const double lon) {
			coordinates_ = { lat, lon };
		}

		// Метод проверки равенства остановок
		// @param other вторая остановка
		bool operator==(const Stop& other) const {
			return name_ == other.name_ && coordinates_ == other.coordinates_;
		}

		// Вывод информации об остановке
		// @param os поток вывода
		// @param stop остановка
		friend std::ostream& operator<<(std::ostream& os, const Stop& stop) {
			using namespace std::literals;
			os << "Stop: "s << stop.name_ 
				<< ", Coordinates: ("s << stop.coordinates_.lat 
				<< ", "s << stop.coordinates_.lng 
				<< ")"s;
			return os;
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
