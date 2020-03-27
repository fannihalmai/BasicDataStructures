#include <cstdint>
#include <memory>
#include <stdexcept>

#include <utils.h>

namespace structures {
/* Implement a list(data structure) using arrays */
	
template <typename T>
class Arraylist {
public:
	Arraylist() = default;

	Arraylist(const Arraylist<T>& other)
		: contents{copy_array(other.contents, other.size_, other.max_size_)}
		, size_{other.size_}
		, max_size_{other.max_size_} {}

	Arraylist(Arraylist<T>&& other)
		: contents{std::move(other.contents)}
		, size_{std::move(other.size_)}
		, ma	x_size_{std::move(other.max_size_)} {
		other.size_ = 0;
		other.max_size_ = 0;
	}

	Arraylist<T>& operator=(const Arraylist<T>& other) {
		Arraylist<T> copy{other};
		std::swap(contents, copy.contents);
		std::swap(size_, copy.size_);
		std::swap(max_size_, copy.max_size_);
		return *this;
	}

	Arraylist<T>& operator=(Arraylist<T>&& other) {
		Arraylist<T> copy{std::move(other)};
		std::swap(contents, copy.contents);
		std::swap(size_, copy.size_);
		std::swap(max_size_, copy.max_size_);
		return *this;
	}

	virtual ~Arraylist() = default;
	/* Construct given max_size = maximum # of elements */
	explicit Arraylist(std::size_t max_size)
		: contents{new T[max_size]}, max_size_{max_size} {}
	/* To clear all elements */
	void clear() { size_ = 0; }

	/* Add 'data' at the end of the list */
	void push_at_back(const T& data) { insert(data, size_); }

	/* Add 'data' at the beginning of the list */
	void push_at_front(const T& data) { insert(data, 0); }

	/* Insert an element ('data') at a given position ('index') of the list */
	void insert(const T& data, std::size_t index) {
		if (index > size_) {
			throw std::out_of_range("Index out of bounds");
		} else {
			for (std::size_t i = size_; i > index; i--) {
				contents[i] = contents[i - 1];
			}
			contents[index] = data;
			size_++;

			if (max_size_ == size_)
				expand(2);
		}
	}

	/* Insert an element 'data' sorted to the list */
	void insert_sorted(const T& data) {
		std::size_t i = 0;
		while (i < size_ && data >= contents[i])
			i++;
		insert(data, i);
	}

	/* Remove an element from a given position ('index') */
	T erase(std::size_t index) {
		if (empty()) {
			throw std::out_of_range("List is empty");
		} else if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		} else {
			T deleted = contents[index];
			for (std::size_t i = index; i < size_ - 1; ++i) {
				contents[i] = contents[i + 1];
			}
			size_--;

			if (max_size_ / 4 > size_)
				expand(0.5);

			return deleted;
		}
	}

	/* Remove the element at the end of the list*/
	T pop_at_back() { return erase(size_ - 1); }

	/* Remove first element of a list */
	T pop_at_front() { return erase(0); }

	/* Remove an element 'data' from the list */
	void remove(const T& data) { erase(find(data)); }

	/* Return True if list is empty */
	bool empty() const { return size_ == 0; }

	/* return True if the list contains 'data' */
	bool contains(const T& data) const { return find(data) != size_; }

	/* Return the position of 'data' in the list */
	std::size_t find(const T& data) const {
		for (std::size_t i = 0; i < size_; ++i) {
			if (contents[i] == data)
				return i;
		}
		return size_;
	}

	/* Return the list's size */
	std::size_t size() const { return size_; }

	/* Check for the 'index' that it is valid in the 
	list and then return the reference to the element 
	at the given index position */
	T& at(std::size_t index) {
		return const_cast<T&>(static_cast<const Arraylist*>(this)->at(index));
	}

	const T& at(std::size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		} else {
			return contents[index];
		}
	}

	/* Returns the reference to the element at 'index' position of the list*/
	T& operator[](std::size_t index) {
		return const_cast<T&>(
			static_cast<const Arraylist*>(this)->operator[](index));
	}

	const T& operator[](std::size_t index) const { return contents[index]; }

	T& front() { return contents[0]; }

	const T& front() const { return contents[0]; }

	T& back() { return contents[size_ - 1]; }

	const T& back() const { return contents[size_ - 1]; }

private:
	void expand(float ratio) {
		contents = copy_array(contents, size_, max_size_ * ratio);
		max_size_ *= ratio;
	}

	static std::unique_ptr<T[]> copy_array(
		const std::unique_ptr<T[]>& original, std::size_t size,
		std::size_t new_size) {
		std::unique_ptr<T[]> copy{new T[new_size]};
		for (std::size_t i = 0; i < size; i++) {
			copy[i] = original[i];
		}
		return copy;
	}

	const static std::size_t starting_size{8};

	std::unique_ptr<T[]> contents = make_unique<T[]>(starting_size);
	std::size_t size_{0u};
	std::size_t max_size_{starting_size};
};

}  
