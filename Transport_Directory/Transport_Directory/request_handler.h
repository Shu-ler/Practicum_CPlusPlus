#pragma once

#include "transport_catalogue.h"
#include "json.h"
#include "map_renderer.h"

#include <optional>
#include <string>
#include <map>
#include <functional>

/**
 * @brief Модуль обработки запросов к транспортному справочнику.
 *
 * Содержит класс RequestHandler — фасад, упрощающий взаимодействие между
 * JSON-парсером и подсистемами приложения (каталог, визуализация).
 *
 * Позволяет:
 * - Получать статистику по автобусным маршрутам
 * - Получать список маршрутов, проходящих через остановку
 * - Генерировать карту маршрутов (если заданы render_settings)
 */
namespace request_handler {

	/// Статистика по автобусному маршруту
	struct RouteStat {
		std::string name;               ///< Название маршрута
		size_t stop_count = 0;          ///< Число остановок (с повторениями)
		size_t unique_stop_count = 0;   ///< Число уникальных остановок
		double route_length = 0.0;      ///< Длина маршрута в метрах
		double curvature = 0.0;         ///< Коэффициент извилистости
	};

	/// Статистика по остановке
	struct StopStat {
		std::string name;                   ///< Название остановки
		std::vector<std::string> bus_names;	///< Маршруты через остановку
	};

	/**
	 * @brief Фасад для обработки запросов к транспортному справочнику.
	 *
	 * Инкапсулирует логику маршрутизации запросов:
	 * - "Bus" → статистика маршрута
	 * - "Stop" → статистика остановки
	 * - "Map" → SVG-карта
	 *
	 * Использует RequestProcessor для обработки запросов.
	 */
	class RequestHandler {
	public:
		// Создаёт обработчик на основе каталога и входных данных.
		static RequestHandler Create(const trans_cat::TransportCatalogue& catalogue,
			const json::Document& input);

		// Обрабатывает запросы и выводит результат в поток.
		void ProcessRequests(std::ostream& out) const;

		// Получает статистику по маршруту.
		// nullopt, если маршрут не найден
		std::optional<RouteStat> GetBusStat(const std::string& bus_name) const;

		// Получает статистику по остановке.
		// nullopt, если остановка не найдена
		std::optional<StopStat> GetStopStat(const std::string& stop_name) const;

	private:
		/// Функция-обработчик запроса
		using Handler = std::function<json::Dict(const json::Dict&)>;

		explicit RequestHandler(const trans_cat::TransportCatalogue& catalogue,
			std::optional<renderer::MapRenderer> renderer,
			std::optional<json::Array> stat_requests);

		// Обработчики запросов
		json::Dict ProcessBusRequest(const json::Dict& req) const;
		json::Dict ProcessStopRequest(const json::Dict& req) const;
		json::Dict ProcessMapRequest(const json::Dict& req) const;

		// Универсальный генератор ответа об ошибке
		static json::Dict MakeErrorResponse(int id, std::string_view message);

		// Командный процессор — обрабатывает запросы по типу
		struct RequestProcessor {
			std::map<std::string, Handler> handlers;

			void AddHandler(std::string type, Handler handler) {
				handlers[std::move(type)] = std::move(handler);
			}

			std::vector<json::Dict> Process(const json::Array& requests) const {
				std::vector<json::Dict> responses;
				responses.reserve(requests.size());

				for (const auto& node : requests) {
					const auto& req = node.AsDict();
					std::string type = req.at("type").AsString();
					int id = req.at("id").AsInt();

					auto it = handlers.find(type);
					if (it != handlers.end()) {
						try {
							responses.push_back(it->second(req));
						}
						catch (...) {
							responses.push_back(MakeErrorResponse(id, "internal error"));
						}
					}
					else {
						responses.push_back(MakeErrorResponse(id, "unknown request type"));
					}
				}
				return responses;
			}

			void Print(const std::vector<json::Dict>& responses, std::ostream& out) const {
				json::Print(json::Document(json::Node(json::Array{ responses.begin(), responses.end() })), out);
				out << '\n';
			}
		};

		const trans_cat::TransportCatalogue& catalogue_;
		std::optional<renderer::MapRenderer> map_renderer_;
		std::optional<json::Array> stat_requests_;
		RequestProcessor processor_;  // диспетчер запросов
	};

} // namespace request_handler
