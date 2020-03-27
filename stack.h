#include <cstdint>
#include <array_list.h>

namespace structures {

template <typename T, typename Container>
class StackWrapper {
public:
	void push(const T& data) { return cont.push_at_back(data); }
	T pop() { return cont.pop_at_back(); }
	T& top() { return cont.back(); }
	const T& top() const { return cont.back(); }
	void clear() { return cont.clear(); }
	std::size_t size() const { return cont.size(); }

private:
	Container cont;
};

template <typename T>
class Stack : public StackWrapper<T, Arraylist<T>> {};

}  

/* name trait */
template <>
const std::string traits::type<structures::Stack>::name = "Stack";