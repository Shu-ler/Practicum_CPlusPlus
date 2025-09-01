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

	// ���� ������
	struct Node {
		Node() = default;
		Node(const Type& val, Node* next)
			: value(val)
			, next_node(next) {
		}
		Type value{};
		Node* next_node = nullptr;
	};

	// ������ ������ �������� ��������.
	// ���������� ��������� ��������� �� �������� ������������ ������
	// ValueType � ��������� � Type (��� Iterator) ���� � const Type (��� ConstIterator)
	template <typename ValueType>
	class BasicIterator {
		// ����� ������ ����������� �������������, ����� �� ������� ������
		// ��� ������ � ��������� ������� ���������
		friend class SingleLinkedList;

		// �������������� ����������� ��������� �� ��������� �� ���� ������
		explicit BasicIterator(Node* node) : node_(node) {
		}

	public:
		// ����������� ���� ���� �������� ����������� ���������� � ��������� ����� ���������

		// ��������� ��������� � forward iterator
		// (��������, ������� ������������ �������� ���������� � ������������ �������������)
		using iterator_category = std::forward_iterator_tag;
		// ��� ���������, �� ������� ������������ ��������
		using value_type = Type;
		// ���, ������������ ��� �������� �������� ����� �����������
		using difference_type = std::ptrdiff_t;
		// ��� ��������� �� ����������� ��������
		using pointer = ValueType*;
		// ��� ������ �� ����������� ��������
		using reference = ValueType&;

		BasicIterator() = default;

		// �������������� �����������/����������� �����������
		// ��� ValueType, ����������� � Type, ������ ���� ����������� ������������
		// ��� ValueType, ����������� � const Type, ������ ���� ��������������� ������������
		BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {
		}

		// ����� ���������� �� ������� �������������� �� ���������� ��������� = ��� �������
		// ����������������� ������������ �����������, ���� ������� �������� = �
		// �������� ���������� ������������� ��� �� ���
		BasicIterator& operator=(const BasicIterator& rhs) = default;

		// �������� ��������� ���������� (� ���� ������� ��������� ��������� ����������� ��������)
		// ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������ ���� �� end()
		[[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
			return node_ == rhs.node_;
		}

		// �������� �������� ���������� �� �����������
		// �������������� ==
		[[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
			return node_ != rhs.node_;
		}

		// �������� ��������� ���������� (� ���� ������� ��������� ��������)
		// ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������ ���� �� end()
		[[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
			return node_ == rhs.node_;
		}

		// �������� �������� ���������� �� �����������
		// �������������� ==
		[[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
			return node_ != rhs.node_;
		}

		// �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
		// ���������� ������ �� ������ ����
		// ��������� ���������, �� ������������ �� ������������ ������� ������, �������� � �������������� ���������
		BasicIterator& operator++() noexcept {
			node_ = node_->next_node;
			return *this;
		}

		// �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
		// ���������� ������� �������� ���������
		// ��������� ���������, �� ������������ �� ������������ ������� ������,
		// �������� � �������������� ���������
		BasicIterator operator++(int) noexcept {
			auto this_save = *this;
			node_ = node_->next_node;
			return this_save;
		}

		// �������� �������������. ���������� ������ �� ������� �������
		// ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
		// �������� � �������������� ���������
		[[nodiscard]] reference operator*() const noexcept {
			assert(node_ != nullptr);
			return node_->value;
		}

		// �������� ������� � ����� ������. ���������� ��������� �� ������� ������� ������
		// ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
		// �������� � �������������� ���������
		[[nodiscard]] pointer operator->() const noexcept {
			assert(node_ != nullptr);
			return &node_->value;
		}

	private:
		Node* node_ = nullptr;
	};

public:	// ������������ � �����������

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

	// ��������, ����������� ��������� ��������� ������
	using Iterator = BasicIterator<Type>;
	// ����������� ��������, ��������������� ������ ��� ������ � ��������� ������
	using ConstIterator = BasicIterator<const Type>;

	// ���������� ��������, ����������� �� ������ �������
	// ���� ������ ������, ������������ �������� ����� ����� end()
	[[nodiscard]] Iterator begin() noexcept {
		return Iterator(head_.next_node);
	}

	// ���������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
	// �������������� ���� �������� ������ � ������� ������������� ������� � �������������� ���������
	[[nodiscard]] Iterator end() noexcept {
		return Iterator(nullptr);
	}

	// ���������� ����������� ��������, ����������� �� ������ �������
	// ���� ������ ������, ������������ �������� ����� ����� end()
	// ��������� ������ ������������ ������ ������ cbegin()
	[[nodiscard]] ConstIterator begin() const noexcept {
		return ConstIterator{ head_.next_node };
	}

	// ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
	// �������������� ���� �������� ������ � ������� ������������� ������� � �������������� ���������
	// ��������� ������ ������������ ������ ������ cend()
	[[nodiscard]] ConstIterator end() const noexcept {
		return ConstIterator(nullptr);
	}

	// ���������� ����������� ��������, ����������� �� ������ �������
	// ���� ������ ������, ������������ �������� ����� ����� cend()
	[[nodiscard]] ConstIterator cbegin() const noexcept {
		return ConstIterator{ head_.next_node };
	}

	// ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
	// �������������� ���� �������� ������ � ������� ������������� ������� � �������������� ���������
	[[nodiscard]] ConstIterator cend() const noexcept {
		return ConstIterator(nullptr);
	}

	// ���������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
	// �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
	[[nodiscard]] Iterator before_begin() noexcept {
		return Iterator{ &head_ };
	}

	// ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
	// �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
	[[nodiscard]] ConstIterator before_begin() const noexcept {
		return ConstIterator{ const_cast<Node*>(&head_) };
	}

	// ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
	// �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
	[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
		return ConstIterator{ const_cast<Node*>(&head_) };
	}

	// ���������� ���������� ��������� � ������
	[[nodiscard]] size_t GetSize() const noexcept {
		return size_;
	}

	// ��������, ������ �� ������
	[[nodiscard]] bool IsEmpty() const noexcept {
		return size_ == 0;
	}

	// ��������� ������� value � ������ ������ �� ����� O(1)
	void PushFront(const Type& value) {
		head_.next_node = new Node(value, head_.next_node);	// ������� �������� � ������ ������ 
		++size_;	// ���������� ������� ������
	}

	// ��������� ������� value ����� ��������, �� ������� ��������� pos.
	// ���������� �������� �� ����������� �������
	// ���� ��� �������� �������� ����� ��������� ����������, ������ ��������� � ������� ���������
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

	// ������� ������ �������� ��������� ������.�� ����������� ����������.
	void PopFront() noexcept {
		if (size_ > 0) {
			Node* to_del = head_.next_node;
			head_.next_node = to_del->next_node;
			delete to_del;
			--size_;
		}
	}

	// ������� �������, ��������� �� pos.
	// ���������� �������� �� �������, ��������� �� ��������
	Iterator EraseAfter(ConstIterator pos) noexcept {
		Node* to_del = pos.node_->next_node;
		pos.node_->next_node = to_del->next_node;
		delete to_del;
		--size_;
		return Iterator{ pos.node_->next_node };
	}

	// ������� ������ �� ����� O(N)
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
	Node head_;				// ��������� ����, ������������ ��� ������� "����� ������ ���������"
	size_t size_ = 0;		// ������ ������
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
	lhs.swap(rhs);
}

//
// �������� ���������
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

