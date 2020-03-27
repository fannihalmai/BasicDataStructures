#include <functional>
#include <array_list.h>
#include <linked_list.h>
#include <utils.h>

namespace structures {

/* HashTable implementation
   params T: Data type of the elements
   param Hash: Class that implements the hash function */
template <typename T, typename Hash = std::hash<T>>
class Hashtablewrapper {
public:
	Hashtablewrapper() = default;

	Hashtablewrapper(const Hashtablewrapper<T>& other)
		: Hashtablewrapper(other.buckets_size) {
		auto list = other.items();
		for (std::size_t i = 0; i < list.size(); i++) {
			insert(list[i]);
		}
	}

	Hashtablewrapper(Hashtablewrapper<T>&& other)
		: buckets{std::move(other.buckets)}
		, buckets_size{std::move(other.buckets_size)}
		, _size{std::move(other._size)} {}

	Hashtablewrapper<T>& operator=(const Hashtablewrapper<T>& other) {
		Hashtablewrapper<T> copy{other};
		std::swap(buckets_size, copy.buckets_size);
		std::swap(buckets, copy.buckets);
		std::swap(_size, copy._size);
		return *this;
	}

	Hashtablewrapper<T>& operator=(Hashtablewrapper<T>&& other) {
		Hashtablewrapper<T> copy{std::move(other)};
		std::swap(buckets_size, copy.buckets_size);
		std::swap(buckets, copy.buckets);
		std::swap(_size, copy._size);
		return *this;
	}

	~Hashtablewrapper() = default;

	/* Inserts the element 'x' into the respective bucket of the table. 
	   If needed, it grows the table. Returns false if element is already in the 
	   table (unique elements) */
	bool insert(const T& x) {
		auto& bucket = buckets[hash(x)];
		if (bucket.contains(x)) {
			return false;
		} else {
			bucket.push_front(x);
			_size++;

			if (_size == buckets_size) {
				resize_table(buckets_size * 2);
			}

			return true;
		}
	}

	/* Removes `x` from the table. Return true if x is found, else return false. */
	bool remove(const T& x) {
		try {
			auto& bucket = buckets[hash(x)];
			auto i = bucket.find(x);
			bucket.erase(i);
			_size--;

			if (_size <= buckets_size / 4) {
				std::size_t new_size = buckets_size / 2;
				if (new_size >= starting_size)
					resize_table(new_size);
			}

			return true;
		} catch (const std::out_of_range& e) {
			return false;
		}
	}

	/* Returns true if the element 'x' is in the table */
	bool contains(const T& x) const {
		return buckets[hash(x)].contains(x);
	}

	void clear() {
		Hashtablewrapper<T> ht;
		*this = std::move(ht);
	}

	std::size_t size() const { return _size; }

	/* Returns a list of items that are in the table */
	Arraylist<T> items() const {
		Arraylist<T> al{_size};

		for (std::size_t i = 0; i < buckets_size; i++) {
			for (std::size_t j = 0; j < buckets[i].size(); j++) {
				al.push_back(buckets[i].at(j));
			}
		}

		return std::move(al);
	}

private:
	explicit Hashtablewrapper(std::size_t buckets_size_)
		: buckets{new LinkedList<T>[buckets_size_]}
		, buckets_size{buckets_size_} {}

	std::size_t hash(const T& x) const { return hashf(x) % buckets_size; }

	void resize_table(std::size_t new_size) {
		Hashtablewrapper new_ht{new_size};

		auto list = items();
		for (std::size_t i = 0; i < list.size(); i++) {
			new_ht.insert(list.at(i));
		}

		*this = std::move(new_ht);
	}

	const static std::size_t starting_size{8};

	std::unique_ptr<LinkedList<T>[]> buckets =
		get_unique_ptr<LinkedList<T>[]>(starting_size);
	std::size_t buckets_size{starting_size};
	std::size_t _size{0};

	Hash hashf{};
};

template <typename T>
class HashTable : public Hashtablewrapper<T> {};

} 