#pragma once

#include "ranges.h"

#include <cstdlib>
#include <vector>

/**
 * @file graph.h
 * @brief Реализация ориентированного взвешенного графа.
 *
 * Предоставляет шаблонный класс DirectedWeightedGraph для эффективного
 * хранения и обхода графа с весами рёбер. Используется, например,
 * в системах маршрутизации и расчёта оптимальных путей.
 *
 * Реализован как список смежности на основе массива рёбер и списков инцидентности.
 * Позволяет быстро получать исходящие рёбра из вершины.
 *
 * @tparam Weight — тип веса рёбер (например, int, double)
 */

namespace graph {

	/// Идентификатор вершины
	using VertexId = size_t;

	/// Идентификатор ребра
	using EdgeId = size_t;

	/**
	 * @brief Описание ребра ориентированного взвешенного графа.
	 *
	 * Каждое ребро соединяет две вершины и имеет вес.
	 * Рёбра хранятся в отдельном векторе, а их ID используются в списках смежности.
	 */
	template <typename Weight>
	struct Edge {
		VertexId from;    ///< Начало ребра
		VertexId to;      ///< Конец ребра
		Weight weight;    ///< Вес ребра (длина, время и т.п.)
	};

	/**
	 * @brief Ориентированный взвешенный граф.
	 *
	 * Граф реализован по принципу "списки смежности", где:
	 * - Все рёбра хранятся в векторе edges_
	 * - Для каждой вершины хранится список ID рёбер, из неё исходящих (incidence_lists_)
	 *
	 * Это позволяет:
	 * - Быстро добавлять рёбра — O(1)
	 * - Быстро получать исходящие рёбра — O(выходная степень)
	 * - Эффективно использовать память
	 *
	 * @tparam Weight — тип веса рёбер
	 */
	template <typename Weight>
	class DirectedWeightedGraph {
	private:
		/// Список ID рёбер, исходящих из вершины
		using IncidenceList = std::vector<EdgeId>;

		/// Диапазон итераторов по списку инцидентности (для range-based for)
		using IncidentEdgesRange = ranges::Range<typename IncidenceList::const_iterator>;

	public:
		/// Конструктор по умолчанию — создаёт пустой граф
		DirectedWeightedGraph() = default;

		/**
		 * @brief Создаёт граф с заданным числом вершин (без рёбер).
		 * @param vertex_count Число вершин
		 */
		explicit DirectedWeightedGraph(size_t vertex_count);

		/**
		 * @brief Добавляет ребро в граф.
		 * @param edge Описание ребра (from, to, weight)
		 * @return Уникальный идентификатор добавленного ребра
		 * @note Сложность: O(1)
		 */
		EdgeId AddEdge(const Edge<Weight>& edge);

		/**
		 * @brief Возвращает количество вершин в графе.
		 * @note Сложность: O(1)
		 */
		size_t GetVertexCount() const;

		/**
		 * @brief Возвращает количество рёбер в графе.
		 * @note Сложность: O(1)
		 */
		size_t GetEdgeCount() const;

		/**
		 * @brief Возвращает ссылку на ребро по его ID.
		 * @param edge_id Идентификатор ребра
		 * @return Константная ссылка на ребро
		 * @note Сложность: O(1), выбросит исключение, если ID некорректен
		 */
		const Edge<Weight>& GetEdge(EdgeId edge_id) const;

		/**
		 * @brief Возвращает диапазон ID рёбер, исходящих из вершины.
		 * @param vertex Идентификатор вершины
		 * @return Диапазон (range) с итераторами на ID рёбер
		 * @note Сложность: O(1), обход — O(выходная степень)
		 * @note Используется в range-based for:
		 *       for (EdgeId id : graph.GetIncidentEdges(v)) { ... }
		 */
		IncidentEdgesRange GetIncidentEdges(VertexId vertex) const;

	private:
		std::vector<Edge<Weight>> edges_;				///< Хранение всех рёбер
		std::vector<IncidenceList> incidence_lists_;	///< Списки ID рёбер для каждой вершины
	};

	// ====================================================
	// Реализация методов
	// ====================================================

	template <typename Weight>
	DirectedWeightedGraph<Weight>::DirectedWeightedGraph(size_t vertex_count)
		: incidence_lists_(vertex_count) {
		// Инициализируем списки смежности для каждой вершины
		// Рёбра пока не добавляются
	}

	template <typename Weight>
	EdgeId DirectedWeightedGraph<Weight>::AddEdge(const Edge<Weight>& edge) {
		// Добавляем ребро в общий список
		edges_.push_back(edge);
		const EdgeId id = edges_.size() - 1;

		// Добавляем ID ребра в список исходящих рёбер начальной вершины
		incidence_lists_.at(edge.from).push_back(id);

		return id;
	}

	template <typename Weight>
	size_t DirectedWeightedGraph<Weight>::GetVertexCount() const {
		return incidence_lists_.size();
	}

	template <typename Weight>
	size_t DirectedWeightedGraph<Weight>::GetEdgeCount() const {
		return edges_.size();
	}

	template <typename Weight>
	const Edge<Weight>& DirectedWeightedGraph<Weight>::GetEdge(EdgeId edge_id) const {
		return edges_.at(edge_id); // at() — с проверкой диапазона
	}

	template <typename Weight>
	typename DirectedWeightedGraph<Weight>::IncidentEdgesRange
		DirectedWeightedGraph<Weight>::GetIncidentEdges(VertexId vertex) const {
		return ranges::AsRange(incidence_lists_.at(vertex));
		// Возвращает диапазон по вектору ID рёбер — удобно для итерации
	}

}  // namespace graph