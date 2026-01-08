#pragma once

#include "graph.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * @file router.h
 * @brief Модуль расчёта кратчайших путей в графе.
 *
 * Реализует класс Router, который позволяет находить кратчайшие пути
 * между парами вершин в ориентированном взвешенном графе.
 *
 * Используется алгоритм Флойда-Уоршелла с оптимизацией восстановления путей.
 * Подходит для графов с неотрицательными весами рёбер.
 *
 * @tparam Weight — тип веса рёбер (должен поддерживать операции +, <, =)
 */

namespace graph {

	/**
	 * @brief Маршрутизатор для поиска кратчайших путей в графе.
	 *
	 * Вычисляет кратчайшие пути между всеми парами вершин при создании.
	 * Поддерживает запросы на построение маршрута между двумя вершинами.
	 *
	 * @note Используется алгоритм Флойда-Уоршелла.
	 * @note Граф должен иметь неотрицательные веса рёбер.
	 * @note Сложность построения: O(V³), запроса: O(L), где L — длина маршрута.
	 */
	template <typename Weight>
	class Router {
	private:
		using Graph = DirectedWeightedGraph<Weight>;  ///< Удобный псевдоним

	public:
		/// Данные о найденном маршруте
		struct RouteInfo {
			Weight weight;				///< Суммарный вес маршрута
			std::vector<EdgeId> edges;	///< Последовательность рёбер маршрута
		};

		/**
		 * @brief Создаёт маршрутизатор для заданного графа.
		 * @param graph Константная ссылка на граф
		 * @throw std::domain_error, если вес любого ребра отрицательный
		 *
		 * При создании вычисляет все кратчайшие пути (алгоритм Флойда-Уоршелла).
		 */
		explicit Router(const Graph& graph);

		/**
		 * @brief Строит маршрут между двумя вершинами.
		 * @param from Начальная вершина
		 * @param to Конечная вершина
		 * @return RouteInfo — если путь существует; std::nullopt — если недостижим
		 *
		 * @note Время выполнения: O(L), где L — количество рёбер в маршруте.
		 */
		std::optional<RouteInfo> BuildRoute(VertexId from, VertexId to) const;

	private:
		/**
		 * @brief Внутренние данные для построения маршрута.
		 *
		 * Хранит:
		 * - weight: суммарный вес кратчайшего пути
		 * - prev_edge: ID последнего ребра в кратчайшем пути (для восстановления)
		 */
		struct RouteInternalData {
			Weight weight;
			std::optional<EdgeId> prev_edge;
		};

		/// Двумерный массив: routes_internal_data_[from][to] — данные о пути
		using RoutesInternalData = std::vector<std::vector<std::optional<RouteInternalData>>>;

		/**
		 * @brief Инициализирует начальные данные для алгоритма.
		 *
		 * Заполняет диагональ (путь из вершины в себя = 0)
		 * и прямые рёбра (если есть ребро from→to, это кандидат на кратчайший путь).
		 *
		 * @note Также проверяет, что веса неотрицательны.
		 */
		void InitializeRoutesInternalData(const Graph& graph);

		/**
		 * @brief Пытается улучшить путь из vertex_from в vertex_to через вершину.
		 *
		 * Если путь vertex_from → vertex_through → vertex_to короче текущего,
		 * обновляет маршрут и сохраняет предыдущее ребро.
		 */
		void RelaxRoute(VertexId vertex_from, VertexId vertex_to,
			const RouteInternalData& route_from,
			const RouteInternalData& route_to);

		/**
		 * @brief Обновляет все кратчайшие пути, проходящие через вершину vertex_through.
		 *
		 * Реализует основной шаг алгоритма Флойда-Уоршелла.
		 */
		void RelaxRoutesInternalDataThroughVertex(size_t vertex_count, VertexId vertex_through);

		static constexpr Weight ZERO_WEIGHT{};  ///< Нулевой вес (для начальной инициализации)

		const Graph& graph_;                    ///< Граф, для которого строим маршруты
		RoutesInternalData routes_internal_data_; ///< Таблица кратчайших путей
	};

	// ====================================================
	// Реализация методов
	// ====================================================

	/**
	 * Конструктор: инициализирует данные и запускает алгоритм Флойда-Уоршелла.
	 */
	template <typename Weight>
	Router<Weight>::Router(const Graph& graph)
		: graph_(graph)
		, routes_internal_data_(graph.GetVertexCount(),
			std::vector<std::optional<RouteInternalData>>(graph.GetVertexCount()))
	{
		InitializeRoutesInternalData(graph);

		const size_t vertex_count = graph.GetVertexCount();
		for (VertexId vertex_through = 0; vertex_through < vertex_count; ++vertex_through) {
			RelaxRoutesInternalDataThroughVertex(vertex_count, vertex_through);
		}
	}

	/**
	 * Инициализирует начальные расстояния:
	 * - путь из вершины в себя = 0
	 * - прямое ребро → кандидат на кратчайший путь
	 */
	template <typename Weight>
	void Router<Weight>::InitializeRoutesInternalData(const Graph& graph) {
		const size_t vertex_count = graph.GetVertexCount();
		for (VertexId vertex = 0; vertex < vertex_count; ++vertex) {
			// Путь из вершины в себя
			routes_internal_data_[vertex][vertex] = RouteInternalData{ ZERO_WEIGHT, std::nullopt };

			// Прямые рёбра
			for (const EdgeId edge_id : graph.GetIncidentEdges(vertex)) {
				const auto& edge = graph.GetEdge(edge_id);
				if (edge.weight < ZERO_WEIGHT) {
					throw std::domain_error("Edges' weights should be non-negative");
				}
				auto& route_internal_data = routes_internal_data_[vertex][edge.to];
				if (!route_internal_data || route_internal_data->weight > edge.weight) {
					route_internal_data = RouteInternalData{ edge.weight, edge_id };
				}
			}
		}
	}

	/**
	 * Пытается улучшить путь vertex_from → vertex_to через вершину vertex_through.
	 */
	template <typename Weight>
	void Router<Weight>::RelaxRoute(VertexId vertex_from, VertexId vertex_to,
		const RouteInternalData& route_from,
		const RouteInternalData& route_to) {
		auto& route_relaxing = routes_internal_data_[vertex_from][vertex_to];
		const Weight candidate_weight = route_from.weight + route_to.weight;
		if (!route_relaxing || candidate_weight < route_relaxing->weight) {
			route_relaxing = { candidate_weight,
							  route_to.prev_edge ? route_to.prev_edge : route_from.prev_edge };
		}
	}

	/**
	 * Обновляет все пути, проходящие через вершину vertex_through.
	 * Основной шаг алгоритма Флойда-Уоршелла.
	 */
	template <typename Weight>
	void Router<Weight>::RelaxRoutesInternalDataThroughVertex(size_t vertex_count, VertexId vertex_through) {
		for (VertexId vertex_from = 0; vertex_from < vertex_count; ++vertex_from) {
			if (const auto& route_from = routes_internal_data_[vertex_from][vertex_through]) {
				for (VertexId vertex_to = 0; vertex_to < vertex_count; ++vertex_to) {
					if (const auto& route_to = routes_internal_data_[vertex_through][vertex_to]) {
						RelaxRoute(vertex_from, vertex_to, *route_from, *route_to);
					}
				}
			}
		}
	}

	/**
	 * Восстанавливает маршрут между двумя вершинами.
	 * Собирает рёбра, идя по prev_edge, и возвращает их в правильном порядке.
	 */
	template <typename Weight>
	std::optional<typename Router<Weight>::RouteInfo> Router<Weight>::BuildRoute(VertexId from, VertexId to) const {
		const auto& route_internal_data = routes_internal_data_.at(from).at(to);
		if (!route_internal_data) {
			return std::nullopt;  // Путь не существует
		}

		const Weight weight = route_internal_data->weight;
		std::vector<EdgeId> edges;

		// Собираем рёбра, идя по цепочке prev_edge
		for (std::optional<EdgeId> edge_id = route_internal_data->prev_edge;
			edge_id;
			edge_id = routes_internal_data_[from][graph_.GetEdge(*edge_id).from]->prev_edge) {
			edges.push_back(*edge_id);
		}

		// Рёбра собраны в обратном порядке — разворачиваем
		std::reverse(edges.begin(), edges.end());

		return RouteInfo{ weight, std::move(edges) };
	}

}  // namespace graph
