#pragma once

#include "transport_catalogue.h"
#include "json.h"
#include "map_renderer.h"

#include <string>
#include <string_view>

namespace json_reader {
	
	/**
	 * @brief Настройки маршрутизации: время ожидания и скорость автобусов.
	 */
	struct RoutingSettings {
		int bus_wait_time = 0;        ///< Время ожидания автобуса (минуты)
		double bus_velocity = 0.0;    ///< Скорость автобусов (км/ч)
	};

	/**
	 * @brief Утилита для парсинга JSON-входа и заполнения транспортного каталога.
	 *
	 * Класс отвечает за:
	 * - Загрузку "base_requests" (остановки, маршруты, расстояния)
	 * - Извлечение "stat_requests" (запросы Bus, Stop, Map)
	 * - Парсинг "render_settings" (настройки визуализации карты)
	 *
	 * Все методы потоконебезопасны.
	 */
	class JSONReader {
	public:
		/**
		 * @brief Создаёт читатель, привязанный к транспортному каталогу.
		 * @param tc Ссылка на каталог, который будет заполняться
		 */
		explicit JSONReader(trans_cat::TransportCatalogue& tc);

		/**
		 * @brief Загружает данные транспортного справочника из JSON-документа.
		 *
		 * Парсит массив "base_requests" и:
		 * - добавляет остановки (тип "Stop")
		 * - добавляет маршруты (тип "Bus")
		 * - устанавливает дорожные расстояния между остановками
		 *
		 * @param input JSON-документ с ключом "base_requests"
		 * @throw json::ParsingError если формат JSON нарушен или отсутствуют обязательные поля
		 *
		 */
		void LoadFromJson(const json::Document& input);

		/**
		 * @brief Извлекает массив статистических запросов из входного документа.
		 *
		 * Возвращает массив запросов типа "Bus", "Stop", "Map" из ключа "stat_requests".
		 * Если ключ отсутствует, возвращается пустой массив.
		 *
		 * @param input Полный входной JSON-документ
		 * @return json::Array — копия массива статистических запросов
		 *
		 */
		static json::Array GetStatRequests(const json::Document& input);

		/**
		 * @brief Парсит настройки визуализации карты из JSON.
		 *
		 * Читает словарь "render_settings" и заполняет структуру renderer::RenderSettings.
		 * Если "render_settings" отсутствует, поведение не определено (вызов должен быть защищён проверкой).
		 *
		 * @param input Входной JSON-документ
		 * @return Объект RenderSettings с настройками карты
		 *
		 * @throw json::ParsingError если формат "render_settings" нарушен
		 *
		 * @note Все значения должны присутствовать (кроме цвета underlayer_color, который может быть массивом или строкой).
		 *
		 * @example
		 * "render_settings": {
		 *   "width": 1000,
		 *   "height": 500,
		 *   "padding": 50,
		 *   "stop_radius": 5,
		 *   "line_width": 4,
		 *   "bus_label_font_size": 20,
		 *   "bus_label_offset": [7, 15],
		 *   "stop_label_font_size": 16,
		 *   "stop_label_offset": [7, -3],
		 *   "underlayer_color": "white",
		 *   "underlayer_width": 3.0,
		 *   "color_palette": ["blue", "red", "green"]
		 * }
		 */
		static renderer::RenderSettings GetRenderSettings(const json::Document& input);

		/**
		 * @brief Парсит настройки маршрутизации из JSON.
		 *
		 * Ожидает словарь "routing_settings" с полями:
		 * - bus_wait_time: int — время ожидания автобуса в минутах
		 * - bus_velocity: double — скорость автобуса в км/ч
		 *
		 * @param input Входной JSON-документ
		 * @return RoutingSettings — заполненная структура
		 * @throw json::ParsingError если поля отсутствуют или имеют неверный тип
		 */
		static RoutingSettings GetRoutingSettings(const json::Document& input);

	private:

		// Внутренние методы
		void AddStopFromJSON(const json::Dict& stop_node);
		void AddRouteFromJSON(const json::Dict& route_node);

		// Утилита: безопасное извлечение значения по ключу
		template<typename T>
		T GetJsonValue(const json::Dict& dict, std::string_view key) const;

		trans_cat::TransportCatalogue& catalogue_;	///< Ссылка на каталог — заполняется при загрузке
	};

	template<typename T>
	inline T JSONReader::GetJsonValue(const json::Dict& dict, std::string_view key) const {
		auto it = dict.find(std::string(key));
		if (it == dict.end()) {
			throw json::ParsingError{ std::string("Key not found: ") + std::string(key) };
		}
		try {
			return it->second.As<T>();
		}
		catch (const std::bad_variant_access&) {
			throw json::ParsingError{ std::string("Key '")
				+ std::string(key) + "' has wrong type" };
		}
	}

} // namespace json_reader
