#include <cstdint>
#include <stdexcept>

namespace structures {

/* Singly linked list, with first pointer: head, with last node points to 'lastnodenull'.*/
template <typename T>
class Singlelinkedlist {
public:
	Singlelinkedlist() = default;

	Singlelinkedlist(const Singlelinkedlist<T>& other)
		: head{copy_list(other.head)}, size_{other.size_} {}

	Singlelinkedlist(Singlelinkedlist<T>&& other) : head{other.head}, size_{other.size_} {
		other.head = lastnodenull;
		other.size_ = 0;
	}

	Singlelinkedlist<T>& operator=(const Singlelinkedlist<T>& other) {
		Singlelinkedlist<T> copy{other};
		std::swap(head, copy.head);
		std::swap(size_, copy.size_);
		return *this;
	}

	Singlelinkedlist<T>& operator=(Singlelinkedlist<T>&& other) {
		Singlelinkedlist<T> copy{std::move(other)};
		std::swap(head, copy.head);
		std::swap(size_, copy.size_);
		return *this;
	}

	virtual ~Singlelinkedlist() { clear(); }

	/* Clears the list */
	void clear() {
		while (!empty())
			pop_front();
	}

	/* Inserts the element 'x' at the end of the list */
	void push_back(const T& x) { insert(x, size_); }

	/* Inserts the element 'x' at the beginning of the list */
	void push_front(const T& x) {
		head = new Node(x, head);
		++size_;
	}

	/* Inserts an element 'x' at a position 'index' of the list */
	void insert(const T& x, std::size_t index) {
		if (index == 0) {
			return push_front(x);
		} else if (index > size_) {
			throw std::out_of_range("Invalid index");
		} else {
			Node* it = head;
			for (std::size_t i = 0; i < index - 1; ++i) {
				it = it->next;
			}
			it->next = new Node(x, it->next);

			++size_;
		}
	}

	/* Inserts an element into sorted list */
	void insert_sorted(const T& x) {
		if (empty() || x <= head->x) {
			return push_front(x);
		} else {
			Node* it = head;
			while (it->next != lastnodenull && x > it->next->x) {
				it = it->next;
			}
			it->next = new Node(x, it->next);

			++size_;
		}
	}

	/* Returns the reference of the element on the 'index' position of the list */
	T& at(std::size_t index) {
		return const_cast<T&>(static_cast<const Singlelinkedlist*>(this)->at(index));
	}

	const T& at(std::size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		} else {
			Node* it = head;
			for (std::size_t i = 0; i < index; i++) {
				it = it->next;
			}
			return it->x;
		}
	}

	/* Return an element removed at a given index */
	T erase(std::size_t index) {
		if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		} else if (index == 0) {
			return pop_front();
		} else {
			Node* it = head;
			for (std::size_t i = 0; i < index - 1; ++i) {
				it = it->next;
			}

			T removed = it->next->x;
			Node* p_removed = it->next;
			it->next = it->next->next;

			--size_;
			delete p_removed;
			return removed;
		}
	}

	/* Removes an element at the end of the list and returns it */
	T pop_back() { return erase(size_ - 1); }

	/* Removes the first element of the list and returns the removed element */
	T pop_front() {
		if (empty()) {
			throw std::out_of_range("List is empty");
		} else {
			T removed = head->x;
			Node* old_head = head;
			head = head->next;
			delete old_head;
			--size_;
			return removed;
		}
	}

	/* Returns an element 'x' from the list */
	void remove(const T& x) {
		if (head->x == x) {
			pop_front();
			return;
		} else {
			Node* it;
			for (it = head; it->next->x != x; it = it->next) {
				if (it->next == lastnodenull)
					return;
			}

			Node* p_removed = it->next;
			it->next = it->next->next;
			delete p_removed;

			--size_;
		}
	}

	/* Return true if list is empty */
	bool empty() const { return size_ == 0; }

	/* Returns true if the list contains an element(x) */
	bool contains(const T& x) const { return find(x) != size_; }

	/* Returns the position of 'x' on the list if it is in the list */
	std::size_t find(const T& x) const {
		std::size_t index = 0;
		for (Node* it = head; it != lastnodenull; it = it->next) {
			if (it->x == x)
				break;
			++index;
		}
		return index;
	}

	/* Return the size of the list*/
	std::size_t size() const { return size_; }

	T& front() { return head->x; }

	const T& front() const { return head->x; }

	T& back() {
		Node* it = head;
		for (std::size_t i = 1; i < size_; ++i) {
			it = it->next;
		}
		return it->x;
	}

	const T& back() const {
		Node* it = head;
		for (std::size_t i = 1; i < size_; ++i) {
			it = it->next;
		}
		return it->x;
	}

private:
	struct Node {
		explicit Node(const T& x) : x{x} {}
		Node(const T& x, Node* next) : x{x}, next{next} {}

		T x;
		Node* next{lastnodenull};
	};

	static Node* copy_list(const Node* other_head) {
		auto new_tail = new Node(other_head->x);
		auto new_head = new_tail;
		auto it = other_head->next;

		while (it != lastnodenull) {
			new_tail->next = new Node(it->x);
			new_tail = new_tail->next;
			it = it->next;
		}

		return new_head;
	}

	Node* head{lastnodenull};
	std::size_t size_{0u};
};

}  