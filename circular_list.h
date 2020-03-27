#include <stdexcept>

namespace structures {

/* Double linked circular list */
template <typename T>
class Circularlist {
public:
	Circularlist() = default;

	Circularlist(const Circularlist<T>& other)
		: head{copy_list(other.head)}, size_{other.size_} {}

	Circularlist(Circularlist<T>&& other)
		: head{other.head}, size_{other.size_} {
		other.head = nullptr;
		other.size_ = 0;
	}

	Circularlist<T>& operator=(const Circularlist<T>& other) {
		Circularlist<T> copy{other};
		std::swap(head, copy.head);
		std::swap(size_, copy.size_);
		return *this;
	}

	Circularlist<T>& operator=(Circularlist<T>&& other) {
		Circularlist<T> copy{std::move(other)};
		std::swap(head, copy.head);
		std::swap(size_, copy.size_);
		return *this;
	}

	~Circularlist() { clear(); }

	/* Clears all elements of the list */
	void clear() {
		while (!empty()) {
			pop_back();
		}
	}

	/* Insert the element 'x' at the end of the list  */
	void push_at_back(const T& x) {
		if (empty()) {
			head = new Node(x);
			head->next = head;
			head->prev = head;
		} else {
			auto newNode = new Node(x, head->prev, head);
			newNode->prev->next = newNode;
			head->prev = newNode;
		}
		++size_;
	}

	/* Insert the element 'x' at the beginning of the list*/
	void push_at_front(const T& x) {
		push_back(x);
		head = head->prev;
	}

	/* Insert the element 'x' at the given position 'index' of the list */
	void insert(const T& x, std::size_t index) {
		if (index == 0) {
			push_back(x);
			head = head->prev;
		} else if (index > size_) {
			throw std::out_of_range("Invalid index (insert())");
		} else {
			auto it = head;
			for (std::size_t i = 0; i < index - 1; ++i) {
				it = it->next;
			}
			it->next = new Node(x, it, it->next);
			it->next->next->prev = it->next;
			++size_;
		}
	}

	/* Insert the element 'x' sorted in the list  */
	void insert_sorted(const T& x) {
		if (empty() || x <= head->x)
			return push_front(x);
		auto it = head;
		while (it->next != head && x > it->next->x) {
			it = it->next;
		}
		auto newNode = new Node(x, it, it->next);
		it->next->prev = newNode;
		it->next = newNode;
		++size_;
	}

	/* Remove an element from the given index 'index'*/
	T erase(std::size_t index) {
		if (index >= size_)
			throw std::out_of_range("Index out of bounds (pop())");
		auto oldhead = head;
		for (std::size_t i = 0; i < index + 1; ++i) {
			head = head->next;
		}
		auto out = pop_back();
		head = oldhead;
		return out;
	}

	/* Remove an element at the end of the list */
	T pop_at_back() {
		if (empty())
			throw std::out_of_range("List is empty (pop_back())");
		auto toDelete = head->prev;
		head->prev = toDelete->prev;
		toDelete->prev->next = head;
		T out = toDelete->x;
		delete toDelete;
		--size_;
		return out;
	}

	/* Remove an element at the beginning of the list */
	T pop_at_front() {
		if (empty())
			throw std::out_of_range("List is empty (pop_front())");
		head = head->next;
		return pop_back();
	}

	/* If 'x' is in list, then removes it */
	void remove(const T& x) {
		auto it = head->next;
		for (; it->x != x; it = it->next) {
			if (it == head)
				return;  // Reached end of the list
		}
		auto oldhead = head;
		head = it->next;
		pop_back();
		head = oldhead;
	}

	/* Return true if list is empty*/
	bool empty() const { return size_ == 0; }

	/* Return true if the list contains the element 'x' */
	bool contains(const T& x) const {
		if (empty())
			return false;

		if (head->x == x)
			return true;

		for (auto it = head->next; it != head; it = it->next) {
			if (it->x == x)
				return true;
		}
		return false;
	}

	/* Return reference of element in given index 'index'*/
	T& at(std::size_t index) {
		return const_cast<T&>(
			static_cast<const Circularlist*>(this)->at(index));
	}

	const T& at(std::size_t index) const {
		if (index >= size_)
			throw std::out_of_range("Index out of bounds");
		auto it = head;
		for (std::size_t i = 0; i < index; ++i) {
			it = it->next;
		}
		return it->x;
	}

	/* Return the position of element 'x' in the list */
	std::size_t find(const T& x) const {
		if (size_ == 0)
			return 0;

		if (head->x == x)
			return 0;

		std::size_t index = 1;
		for (auto it = head->next; it != head; it = it->next) {
			if (it->x == x)
				break;
			++index;
		}
		return index;
	}

	/* Return size of list*/
	std::size_t size() const { return size_; }

	T& front() { return head->x; }

	const T& front() const { return head->x; }

	T& back() { return head->prev->x; }

	const T& back() const { return head->prev->x; }

private:
	struct Node {
		explicit Node(const T& x) : x{x} {}
		Node(const T& x, Node* next) : x{x}, next{next} {}
		Node(const T& x, Node* prev, Node* next)
			: x{x}, prev{prev}, next{next} {}

		T x;
		Node* prev{nullptr};
		Node* next{nullptr};
	};

	static Node* copy_list(const Node* other_head) {
		Circularlist<T> copy;
		copy.push_back(other_head->x);

		for (auto it = other_head->next; it != other_head; it = it->next) {
			copy.push_back(it->x);
		}

		auto p = copy.head;
		copy.head = nullptr;
		copy.size_ = 0;

		return p;
	}

	Node* head{nullptr};
	std::size_t size_{0u};
};

}  
