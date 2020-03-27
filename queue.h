#include <cstdint>
#include <circular_list.h>

namespace structures {

template <typename T, typename Container>
class QueueWrapper {
public:
	void push(const T& x) { return cont.push_at_back(x); }
	T pop() { return cont.pop_at_front(); }
	T& front() { return cont.front(); }
	const T& front() const { return cont.front(); }
	T& back() { return cont.back(); }
	const T& back() const { return cont.back(); }
	void clear() { return cont.clear(); }
	std::size_t size() const { return cont.size(); }

private:
	Container cont;
};

template <typename T>
class Queue : public QueueWrapper<T, Circularlist<T>> {};

}  

template <>
const std::string traits::type<structures::Queue>::name = "Queue";

