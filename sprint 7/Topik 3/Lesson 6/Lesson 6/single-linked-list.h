#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <assert.h>
#include <iterator>
#include <compare>
#include <algorithm>

template <typename Type>
class SingleLinkedList {

	// Узел списка
	struct Node {
		Node() = default;
		Node(const Type& val, Node* next)
			: value(val)
			, next_node(next) {
		}
		Type value{};
		Node* next_node = nullptr;
	};

	// Шаблон класса «Базовый Итератор».
	// Определяет поведение итератора на элементы односвязного списка
	// ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
	template <typename ValueType>
	class BasicIterator {
		// Класс списка объявляется дружественным, чтобы из методов списка
		// был доступ к приватной области итератора
		friend class SingleLinkedList;

		// Конвертирующий конструктор итератора из указателя на узел списка
		explicit BasicIterator(Node* node) : node_(node) {
		}

	public:
		// Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

		// Категория итератора — forward iterator
		// (итератор, который поддерживает операции инкремента и многократное разыменование)
		using iterator_category = std::forward_iterator_tag;
		// Тип элементов, по которым перемещается итератор
		using value_type = Type;
		// Тип, используемый для хранения смещения между итераторами
		using difference_type = std::ptrdiff_t;
		// Тип указателя на итерируемое значение
		using pointer = ValueType*;
		// Тип ссылки на итерируемое значение
		using reference = ValueType&;

		BasicIterator() = default;

		// Конвертирующий конструктор/конструктор копирования
		// При ValueType, совпадающем с Type, играет роль копирующего конструктора
		// При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
		BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {
		}

		// Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
		// пользовательского конструктора копирования, явно объявим оператор = и
		// попросим компилятор сгенерировать его за нас
		BasicIterator& operator=(const BasicIterator& rhs) = default;

		// Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
		// Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
		[[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
			return node_ == rhs.node_;
		}

		// Оператор проверки итераторов на неравенство
		// Противоположен ==
		[[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
			return node_ != rhs.node_;
		}

		// Оператор сравнения итераторов (в роли второго аргумента итератор)
		// Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
		[[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
			return node_ == rhs.node_;
		}

		// Оператор проверки итераторов на неравенство
		// Противоположен ==
		[[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
			return node_ != rhs.node_;
		}

		// Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
		// Возвращает ссылку на самого себя
		// Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
		BasicIterator& operator++() noexcept {
			node_ = node_->next_node;
			return *this;
		}

		// Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
		// Возвращает прежнее значение итератора
		// Инкремент итератора, не указывающего на существующий элемент списка,
		// приводит к неопределённому поведению
		BasicIterator operator++(int) noexcept {
			auto this_save = *this;
			node_ = node_->next_node;
			return this_save;
		}

		// Операция разыменования. Возвращает ссылку на текущий элемент
		// Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
		// приводит к неопределённому поведению
		[[nodiscard]] reference operator*() const noexcept {
			assert(node_ != nullptr);
			return node_->value;
		}

		// Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
		// Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
		// приводит к неопределённому поведению
		[[nodiscard]] pointer operator->() const noexcept {
			assert(node_ != nullptr);
			return &node_->value;
		}

	private:
		Node* node_ = nullptr;
	};

public:	// Конструкторы и деструкторы

	SingleLinkedList() = default;

	SingleLinkedList(std::initializer_list<Type> values) {
		auto slider = begin();
		for (auto it = values.begin(); it != values.end(); ++it) {
			slider = InsertAfter(slider, *it);
		}
	}

	SingleLinkedList(const SingleLinkedList& other) {
		auto slider = begin();
		for (auto it = other.begin(); it != other.end(); ++it) {
			slider = InsertAfter(slider, *it);
		}
	}

	~SingleLinkedList() {
		Clear();
	}

public:
	using value_type = Type;
	using reference = value_type&;
	using const_reference = const value_type&;

	// Итератор, допускающий изменение элементов списка
	using Iterator = BasicIterator<Type>;
	// Константный итератор, предоставляющий доступ для чтения к элементам списка
	using ConstIterator = BasicIterator<const Type>;

	// Возвращает итератор, ссылающийся на первый элемент
	// Если список пустой, возвращённый итератор будет равен end()
	[[nodiscard]] Iterator begin() noexcept {
		return Iterator(head_.next_node);
	}

	// Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
	// Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] Iterator end() noexcept {
		return Iterator(nullptr);
	}

	// Возвращает константный итератор, ссылающийся на первый элемент
	// Если список пустой, возвращённый итератор будет равен end()
	// Результат вызова эквивалентен вызову метода cbegin()
	[[nodiscard]] ConstIterator begin() const noexcept {
		return ConstIterator{ head_.next_node };
	}

	// Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
	// Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
	// Результат вызова эквивалентен вызову метода cend()
	[[nodiscard]] ConstIterator end() const noexcept {
		return ConstIterator(nullptr);
	}

	// Возвращает константный итератор, ссылающийся на первый элемент
	// Если список пустой, возвращённый итератор будет равен cend()
	[[nodiscard]] ConstIterator cbegin() const noexcept {
		return ConstIterator{ head_.next_node };
	}

	// Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
	// Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] ConstIterator cend() const noexcept {
		return ConstIterator(nullptr);
	}

	// Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] Iterator before_begin() noexcept {
		return Iterator{ &head_ };
	}

	// Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] ConstIterator before_begin() const noexcept {
		return ConstIterator{ const_cast<Node*>(&head_) };
	}

	// Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
		return ConstIterator{ const_cast<Node*>(&head_) };
	}

	// Возвращает количество элементов в списке
	[[nodiscard]] size_t GetSize() const noexcept {
		return size_;
	}

	// Сообщает, пустой ли список
	[[nodiscard]] bool IsEmpty() const noexcept {
		return size_ == 0;
	}

	// Вставляет элемент value в начало списка за время O(1)
	void PushFront(const Type& value) {
		head_.next_node = new Node(value, head_.next_node);	// Вставка элемента в начало списка 
		++size_;	// Увеличение размера списка
	}

	// Вставляет элемент value после элемента, на который указывает pos.
	// Возвращает итератор на вставленный элемент
	// Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
	Iterator InsertAfter(ConstIterator pos, const Type& value) {
		if (size_ == 0) {
			PushFront(value);
			return Iterator{ head_.next_node };
		}
		else {
			Node* new_node = new Node(value, pos.node_->next_node);
			pos.node_->next_node = new_node;
			++size_;
			return Iterator{ new_node };
		}
	}

	// Удаляет первый элемента непустого списка.Не выбрасывает исключений.
	void PopFront() noexcept {
		if (size_ > 0) {
			Node* to_del = head_.next_node;
			head_.next_node = to_del->next_node;
			delete to_del;
			--size_;
		}
	}

	// Удаляет элемент, следующий за pos.
	// Возвращает итератор на элемент, следующий за удалённым
	Iterator EraseAfter(ConstIterator pos) noexcept {
		Node* to_del = pos.node_->next_node;
		pos.node_->next_node = to_del->next_node;
		delete to_del;
		--size_;
		return Iterator{ pos.node_->next_node };
	}

	// Очищает список за время O(N)
	void Clear() noexcept {
		while (head_.next_node != nullptr) {
			auto node = head_.next_node;
			head_.next_node = node->next_node;
			delete node;
		}
		size_ = 0;
	}

	void swap(SingleLinkedList& other) noexcept {
		auto src_size = this->size_;
		auto src_first_ptr = this->head_.next_node;

		this->size_ = other.size_;
		this->head_.next_node = other.head_.next_node;

		other.size_ = src_size;
		other.head_.next_node = src_first_ptr;
	}

	SingleLinkedList& operator=(const SingleLinkedList& rhs) {
		SingleLinkedList buffer(rhs);
		this->swap(buffer);
		return *this;
	}

private:
	Node head_;				// Фиктивный узел, используется для вставки "перед первым элементом"
	size_t size_ = 0;		// Размер списка
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
	lhs.swap(rhs);
}

//
// Операции сравнения
//

template <typename Type>
auto operator<=>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	std::strong_ordering res = (lhs.GetSize() <=> rhs.GetSize());

	if (res == std::strong_ordering::equal) {

		auto lhs_it = lhs.cbegin();
		auto rhs_it = rhs.cbegin();

		auto lhs_end_it = lhs.cend();

		while (lhs_it != lhs_end_it && res == std::strong_ordering::equal) {
			res = *lhs_it <=> *rhs_it;
			++lhs_it;
			++rhs_it;
		}
	}

	return res;
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs <=> rhs) == std::strong_ordering::equal;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs <=> rhs) != std::strong_ordering::equal;
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs <=> rhs) == std::strong_ordering::less;
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs <=> rhs) != std::strong_ordering::greater;
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs <=> rhs) == std::strong_ordering::greater;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs <=> rhs) != std::strong_ordering::less;
}

