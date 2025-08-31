/*
 |  Филимонов И.В.
 |  +7 913 240 81 77
 |  +7 905 084 20 06 (Telegram)
 |  https://github.com/Shu-ler
*/

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
		for (auto it = values.begin(); it != values.end(); ++it) {
			PushBack(*it);
		}
	}

	SingleLinkedList(const SingleLinkedList& other) {
		for (auto it = other.begin(); it != other.end(); ++it) {
			PushBack(*it);
		}
	}

	~SingleLinkedList() {
		Clear();
	}

public: // Итераторы
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

public:	// Методы

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
		// Вставка элемента в начало списка и увеличение размера списка
		head_.next_node = new Node(value, head_.next_node);
		++size_;

		// Для первой вставки - привязка элемента к "хвостику"
		if (tail_ == nullptr) {
			tail_ = head_.next_node;

		}
	}

	// Вставляет элемент value в конец списка за время O(1)
	void PushBack(const Type& value) {
		if (tail_ == nullptr) {
			PushFront(value);
		}
		else {
			tail_->next_node = new Node(value, nullptr);
			tail_ = tail_->next_node;
			++size_;
		}
	}

	// Очищает список за время O(N)
	void Clear() noexcept {
		while (head_.next_node != nullptr) {
			auto node = head_.next_node;
			head_.next_node = node->next_node;
			delete node;
		}
		size_ = 0;
		tail_ = nullptr;
	}

	void swap(SingleLinkedList& other) noexcept {
		auto src_size = this->size_;
		auto src_first_ptr = this->head_.next_node;
		
		this->size_ = other.size_;
		this->head_.next_node = other.head_.next_node;

		other.size_ = src_size;
		other.head_.next_node = src_first_ptr;
	}

public:		// Операторы

	SingleLinkedList& operator=(const SingleLinkedList& rhs) {
		SingleLinkedList buffer(rhs);
		this->swap(buffer);
		return *this;
	}

private: // Члены класса
	Node head_;				// Фиктивный узел, используется для вставки "перед первым элементом"
	Node* tail_ = nullptr;	// Последний узел списка
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

// Эта функция проверяет работу класса SingleLinkedList
void Test3() {
	// Проверка списков на равенство и неравенство
	{
		SingleLinkedList<int> list_1;
		list_1.PushFront(1);
		list_1.PushFront(2);

		SingleLinkedList<int> list_2;
		list_2.PushFront(1);
		list_2.PushFront(2);
		list_2.PushFront(3);

		SingleLinkedList<int> list_1_copy;
		list_1_copy.PushFront(1);
		list_1_copy.PushFront(2);

		SingleLinkedList<int> empty_list;
		SingleLinkedList<int> another_empty_list;

		// Список равен самому себе
		assert(list_1 == list_1);
		assert(empty_list == empty_list);

		// Списки с одинаковым содержимым равны, а с разным - не равны
		assert(list_1 == list_1_copy);
		assert(list_1 != list_2);
		assert(list_2 != list_1);
		assert(empty_list == another_empty_list);
	}

	// Обмен содержимого списков
	{
		SingleLinkedList<int> first;
		first.PushFront(1);
		first.PushFront(2);

		SingleLinkedList<int> second;
		second.PushFront(10);
		second.PushFront(11);
		second.PushFront(15);

		const auto old_first_begin = first.begin();
		const auto old_second_begin = second.begin();
		const auto old_first_size = first.GetSize();
		const auto old_second_size = second.GetSize();

		first.swap(second);

		assert(second.begin() == old_first_begin);
		assert(first.begin() == old_second_begin);
		assert(second.GetSize() == old_first_size);
		assert(first.GetSize() == old_second_size);

		// Обмен при помощи функции swap
		{
			using std::swap;

			// В отсутствие пользовательской перегрузки будет вызвана функция std::swap, которая
			// выполнит обмен через создание временной копии
			swap(first, second);

			// Убеждаемся, что используется не std::swap, а пользовательская перегрузка

			// Если бы обмен был выполнен с созданием временной копии,
			// то итератор first.begin() не будет равен ранее сохранённому значению,
			// так как копия будет хранить свои узлы по иным адресам
			assert(first.begin() == old_first_begin);
			assert(second.begin() == old_second_begin);
			assert(first.GetSize() == old_first_size);
			assert(second.GetSize() == old_second_size);
		}
	}

	// Инициализация списка при помощи std::initializer_list
	{
		SingleLinkedList<int> list{ 1, 2, 3, 4, 5 };
		assert(list.GetSize() == 5);
		assert(!list.IsEmpty());
		assert(std::equal(list.begin(), list.end(), std::begin({ 1, 2, 3, 4, 5 })));
	}

	// Лексикографическое сравнение списков
	{
		using IntList = SingleLinkedList<int>;

		assert((IntList{ 1, 2, 3 } < IntList{ 1, 2, 3, 1 }));
		assert((IntList{ 1, 2, 3 } <= IntList{ 1, 2, 3 }));
		assert((IntList{ 1, 2, 4 } > IntList{ 1, 2, 3 }));
		assert((IntList{ 1, 2, 3 } >= IntList{ 1, 2, 3 }));
	}

	// Копирование списков
	{
		const SingleLinkedList<int> empty_list{};
		// Копирование пустого списка
		{
			auto list_copy(empty_list);
			assert(list_copy.IsEmpty());
		}

		SingleLinkedList<int> non_empty_list{ 1, 2, 3, 4 };
		// Копирование непустого списка
		{
			auto list_copy(non_empty_list);

			assert(non_empty_list.begin() != list_copy.begin());
			assert(list_copy == non_empty_list);
		}
	}

	// Присваивание списков
	{
		const SingleLinkedList<int> source_list{ 1, 2, 3, 4 };

		SingleLinkedList<int> receiver{ 5, 4, 3, 2, 1 };
		receiver = source_list;
		assert(receiver.begin() != source_list.begin());
		assert(receiver == source_list);
	}

	// Вспомогательный класс, бросающий исключение после создания N-копии
	struct ThrowOnCopy {
		ThrowOnCopy() = default;
		explicit ThrowOnCopy(int& copy_counter) noexcept
			: countdown_ptr(&copy_counter) {
		}
		ThrowOnCopy(const ThrowOnCopy& other)
			: countdown_ptr(other.countdown_ptr)  //
		{
			if (countdown_ptr) {
				if (*countdown_ptr == 0) {
					throw std::bad_alloc();
				}
				else {
					--(*countdown_ptr);
				}
			}
		}
		// Присваивание элементов этого типа не требуется
		ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
		// Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
		// Как только обнулится, конструктор копирования выбросит исключение
		int* countdown_ptr = nullptr;
	};

	// Безопасное присваивание списков
	{
		SingleLinkedList<ThrowOnCopy> src_list;
		src_list.PushFront(ThrowOnCopy{});
		src_list.PushFront(ThrowOnCopy{});
		auto thrower = src_list.begin();
		src_list.PushFront(ThrowOnCopy{});

		int copy_counter = 0;  // при первом же копировании будет выброшено исключение
		thrower->countdown_ptr = &copy_counter;

		SingleLinkedList<ThrowOnCopy> dst_list;
		dst_list.PushFront(ThrowOnCopy{});
		int dst_counter = 10;
		dst_list.begin()->countdown_ptr = &dst_counter;
		dst_list.PushFront(ThrowOnCopy{});

		try {
			dst_list = src_list;
			// Ожидается исключение при присваивании
			assert(false);
		}
		catch (const std::bad_alloc&) {
			// Проверяем, что состояние списка-приёмника не изменилось
			// при выбрасывании исключений
			assert(dst_list.GetSize() == 2);
			auto it = dst_list.begin();
			assert(it != dst_list.end());
			assert(it->countdown_ptr == nullptr);
			++it;
			assert(it != dst_list.end());
			assert(it->countdown_ptr == &dst_counter);
			assert(dst_counter == 10);
		}
		catch (...) {
			// Других типов исключений не ожидается
			assert(false);
		}
	}
}

int main() {
	Test3();
}