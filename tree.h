#include <array_list.h>

namespace structures {

/* Binary search tree
 * param T: data type of the elements 
 * param Node: node class */
	
template <typename T, typename Node>
class Tree {
public:
	Tree() = default;

	Tree(const Tree<T, Node>& other) {
		auto list = other.items();
		for (std::size_t i = 0; i < list.size(); i++) {
			insert(list[i]);
		}
	}

	Tree(Tree<T, Node>&& other) : root{other.root}, size_{other.size_} {
		other.root = nullptr;
		other.size_ = 0;
	}

	Tree<T, Node>& operator=(const Tree<T, Node>& other) {
		Tree copy{other};
		std::swap(root, copy.root);
		std::swap(size_, copy.size_);
		return *this;
	}
	
	/* Inserts 'x' into the tree */
	bool insert(const T& x) {
		if (root) {
			if (!Node::insert(root, x))
				return false;
		} else {
			root = new Node(x);
		}
		++size_;
		return true;
	}

	Tree<T, Node>& operator=(Tree<T, Node>&& other) {
		Tree copy{std::move(other)};
		std::swap(root, copy.root);
		std::swap(size_, copy.size_);
		return *this;
	}

	~Tree() { delete root; }

	/* Returns true if the tree contains 'x' */
	bool contains(const T& x) const {
		return root ? root->contains(x) : false;
	}

	void clear() {
		while (size_ > 0)
			remove(root->x);
	}
	
	/* Removes 'x' from the tree, if it exists else return false*/
	bool remove(const T& x) {
		if (root) {
			if (root->x == x) {
				if (root->right && root->left) {
					root->x = root->substitute();
					Node::remove((Node*) root->right, root->x);
				} else {
					Node* n;
					if (root->right) {
						n = (Node*) root->right;
						root->right = nullptr;  
					} else {
						n = (Node*) root->left;
						root->left = nullptr; 
					}

					delete root;
					root = (Node*) n;
					if (root)
						root->parent = nullptr;
				}
				--size_;
				return true;
			} else if (Node::remove(root, x)) {
				--size_;
				return true;
			} else {
				return false;
			}
		}
		return false;
	}

	/* Returns the size of the tree */
	std::size_t size() const { return size_; }

	Arraylist<T> items() const { return std::move(Arraylist<T>{pre_order()}); }

	/* Returns a pre-ordered list of the tree */
	Arraylist<T> pre_order() const {
		Arraylist<T> out{size_};
		if (root)
			root->pre_order(out);
		return out;
	}

	/*  Returns a in-ordered list of the tree */
	Arraylist<T> in_order() const {
		Arraylist<T> out{size_};
		if (root)
			root->in_order(out);
		return out;
	}

	/* Returns a post-ordered list of the tree */
	Arraylist<T> post_order() const {
		Arraylist<T> out{size_};
		if (root)
			root->post_order(out);
		return out;
	}

	/* Prints tree sideways */
	void print() const {
		std::cout << "Tree size = " << size_;
		if (root) {
			std::cout << std::endl << std::endl;
			root->print(0);
			for (int i = 0; i < 80; ++i) {
				std::cout << "-";
			}
		}
		std::cout << std::endl;
	}

protected:
	Node* root{nullptr};
	std::size_t size_{0u};
};

}
