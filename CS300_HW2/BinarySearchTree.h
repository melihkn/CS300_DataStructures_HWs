

using namespace std;
template <class Key, class Value>
struct TreeNode {
	Key name;
	Value* data;
	TreeNode* left;
	TreeNode* right;

	TreeNode() {}

	TreeNode(Key name, Value* data, TreeNode* left, TreeNode* right)
		: name(name), data(data), left(left), right(right)
	{}

	~TreeNode() {
		delete data;
	}

};



template <class Key, class Value>
class BinarySearchTree {
private:

	TreeNode<Key, Value>* root;

	//this private function searchs the tree and finds the key's node's pointer, if there is no returns nullptr
	TreeNode<Key, Value>* find(const Key& key, TreeNode<Key, Value>* node) const {
		if (!node)
			return nullptr;
		else if (key < node->name)
			return find(key, node->left);
		else if (node->name < key)
			return find(key, node->right);
		else
			return node;    // Match
	}

	//this private function inserts to given data to tree
	TreeNode<Key, Value>* insert_(TreeNode<Key, Value>* start, const Key& key, Value* new_data) {

		if (start) {
			if (key < start->name) {
				start->left = insert_(start->left, key, new_data);
			}
			else {
				start->right = insert_(start->right, key, new_data);
			}
		}
		else {
			return new TreeNode<Key, Value>(key, new_data, nullptr, nullptr);
		}

		return start;
	}

	TreeNode<Key, Value>* find(const Key& key, TreeNode<Key, Value>* node) {
		if (node == nullptr)
			return nullptr;
		else if (key < node->name)
			return find(key, node->left);
		else if (node->name < key)
			return find(key, node->right);
		else
			return node;    // Match
	}



	TreeNode<Key, Value>* remove(TreeNode<Key, Value>* start, TreeNode<Key, Value>* tbd) {
		if (start) {
			if (tbd->name < start->name) {
				start->left = remove(start->left, tbd);
			}
			else if (start->name < tbd->name) {
				start->right = remove(start->right, tbd);
			}
			else if (start->name == tbd->name) {
				TreeNode<Key, Value>* temp = nullptr;

				if (start->left && start->right) {
					temp = GetMin(start->right);
					remove(start, temp);
					temp->left = start->left;
					temp->right = start->right;

				}
				else if (start->left) {
					temp = start->left;
				}
				else if (start->right) {
					temp = start->right;
				}

				return temp;
			}
			else {
				start->right = remove(start->right, tbd);
			}
		}
		return start;
	}

	TreeNode<Key, Value>* GetMin(TreeNode<Key, Value>* start) {
		while (start && start->left) {
			start = start->left;
		}

		return start;
	}

	TreeNode<Key, Value>* GetMax(TreeNode<Key, Value>* start) {
		while (start && start->right) {
			start = start->right;
		}

		return start;
	}

	void printTree_(TreeNode<Key, Value>* start) {
		if (!start)
			return;

		printTree_(start->left);
		cout << start->data << endl;
		printTree_(start->right);

	}

	void destroy(TreeNode<Key, Value>* node) {
		if (node != nullptr) {
			destroy(node->left);
			destroy(node->right);
			delete node;
		}
	}



public:

	BinarySearchTree(TreeNode<Key, Value>* first = nullptr)
		: root(first)
	{}

	BinarySearchTree(const BinarySearchTree<Key, Value>& rhs) {
		*this = rhs;
	}

	~BinarySearchTree() {
		destroy(root);
	}

	void insert(const Key& key, Value* data) {
		root = insert_(root, key, data);
	}

	Value find(const Value& x) const
	{
		return *elementAt(find(x, root));
	}

	TreeNode<Key, Value>* find(const Key& key) {
		return find(key, root);
	}


	void remove(TreeNode<Key, Value>* tbd) {
		root = remove(root, tbd);
		delete tbd;
	}

	bool isEmpty() {
		return root != nullptr;
	}

	TreeNode<Key, Value>* GetMax() {
		return GetMax(root);
	}

	TreeNode<Key, Value>* GetMin() {
		return GetMin(root);
	}

	void printTree() {
		printTree_(root);
	}
};


