#pragma once

#include "domain.h"
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <optional>
#include <deque>

namespace trans_cat {

	/**
	 * @brief Хранилище транспортных данных: остановки, маршруты, расстояния.
	 *
	 * Класс предоставляет методы для:
	 * - Добавления остановок и маршрутов
	 * - Установки дорожных расстояний между остановками
	 * - Получения статистики по маршрутам
	 * - Поиска остановок и маршрутов
	 * - Определения маршрутов, проходящих через остановку
	 *
	 * Все методы потоконебезопасны.
	 */
	class TransportCatalogue {
	public:
		// Конструктор по умолчанию
		TransportCatalogue() = default;

		// Запрещаем копирование
		TransportCatalogue(const TransportCatalogue&) = delete;
		TransportCatalogue& operator=(const TransportCatalogue&) = delete;

		// Разрешаем перемещение
		TransportCatalogue(TransportCatalogue&&) = default;
		TransportCatalogue& operator=(TransportCatalogue&&) = default;

		/**
		 * @brief Добавляет новую остановку в каталог.
		 *
		 * Если остановка с таким именем уже существует, обновляет её координаты.
		 * @param name Название остановки
		 * @param coords Географические координаты остановки
		 */
		void AddStop(std::string name, geo::Coordinates coords);

		/**
		 * @brief Добавляет новый маршрут в каталог.
		 *
		 * Маршрут задаётся списком названий остановок.
		 * Если остановки ещё не добавлены — они должны быть добавлены автоматически (без координат).
		 * @param name Название маршрута
		 * @param stops Список названий остановок в порядке следования
		 * @param is_roundtrip true, если маршрут кольцевой (без обратного пути)
		 */
		void AddRoute(std::string name, std::vector<std::string> stops, bool is_roundtrip);

		/**
		 * @brief Ищет остановку по имени.
		 * @param name Имя остановки
		 * @return Указатель на остановку или nullptr, если не найдена
		 */
		const Stop* FindStop(std::string_view name) const;

		/**
		 * @brief Ищет маршрут по имени.
		 * @param name Имя маршрута
		 * @return Указатель на маршрут или nullptr, если не найден
		 */
		const Route* FindRoute(std::string_view name) const;

		/**
		 * @brief Структура с метриками маршрута.
		 */
		struct RouteStat {
			size_t stop_count = 0;           ///< Общее количество остановок (с учётом повторений)
			size_t unique_stop_count = 0;    ///< Количество уникальных остановок
			double route_length = 0.0;       ///< Длина маршрута по дорогам (в метрах)
			double curvature = 0.0;          ///< Коэффициент извилистости (route_length / direct_length)
		};

		/**
		 * @brief Рассчитывает статистику для указанного маршрута.
		 * @param route_name Имя маршрута
		 * @return RouteStat, если маршрут найден; std::nullopt — если нет
		 *
		 * @note Длина маршрута — сумма дорожных расстояний между соседними остановками.
		 *       Прямое расстояние — сумма географических расстояний между соседними остановками.
		 */
		std::optional<RouteStat> GetRouteStat(std::string_view route_name) const;

		/**
		 * @brief Возвращает множество названий маршрутов, проходящих через остановку.
		 * @param stop_name Имя остановки
		 * @return Отсортированный набор названий маршрутов (лексикографически)
		 *
		 * @note Если остановка не найдена, возвращается пустой набор.
		 */
		std::set<std::string> GetBusesByStop(std::string_view stop_name) const;

		/**
		 * @brief Устанавливает дорожное расстояние между двумя остановками.
		 *
		 * Расстояние одностороннее: от from до to.
		 * @param from Имя начальной остановки
		 * @param to Имя конечной остановки
		 * @param distance Расстояние в метрах
		 */
		void SetDistance(std::string_view from, std::string_view to, int distance);

		/**
		 * @brief Возвращает дорожное расстояние между двумя остановками.
		 * @param from Указатель на начальную остановку
		 * @param to Указатель на конечную остановку
		 * @return Расстояние в метрах
		 */
		int GetDistance(const Stop* from, const Stop* to) const;

	private:
		// Хранилища (владеют объектами)
		std::deque<Stop> stops_;
		std::deque<Route> routes_;

		// Индексы: имя → указатель
		std::unordered_map<std::string_view, const Stop*> stopname_to_stop_;
		std::unordered_map<std::string_view, const Route*> routename_to_route_;

		// Остановка → множество маршрутов, проходящих через неё
		std::unordered_map<const Stop*, std::unordered_set<const Route*>> stop_to_routes_;

		// Хэшер для пар указателей
		struct PairHash {
			size_t operator()(const std::pair<const Stop*, const Stop*>& p) const;
		};

		// Граф дорожных расстояний: (from, to) → meters
		std::unordered_map<std::pair<const Stop*, const Stop*>, int, PairHash> distances_;
	};

	// Вспомогательная функция для комбинирования хэшей (C++17/20)
	template<typename T>
	void hash_combine(size_t& seed, const T& value) {
		seed ^= std::hash<T>{}(value)+2654435761U + (seed << 6) + (seed >> 2);
	}
} // namespace trans_cat
