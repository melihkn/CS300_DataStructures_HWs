#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#include "BinarySearchTree.h"

using namespace std;

template <class Key, class Value>
struct AVLNode {
	Key name;
	Value* data;
	int height;
	AVLNode* left;
	AVLNode* right;


	AVLNode() {}

	AVLNode(Key name, Value* data, int height, AVLNode* left, AVLNode* right)
		: name(name), data(data), height(height), left(left), right(right) {}

	~AVLNode() {
		delete data;
	}
};


struct DocumentItem {
private:
	string documentName;
	int count;

public:
	void incrementCount() { count++; }
	int getCount() { return count; }

	DocumentItem(const string& documentName = "", const int& count = 1)
		: documentName(documentName), count(count) {}
};


struct WordItem {
	string word;
	BinarySearchTree<string, DocumentItem> list;

	WordItem() {}

	WordItem(string word = "")
		: word(word)
	{
		list = BinarySearchTree<string, DocumentItem>();
	}

	~WordItem() {}
};



template <class Key, class Value>
class AVLSearchTree {

private:
	AVLNode<Key, Value>* root;

	int getBalance(AVLNode<Key, Value>* node) {
		if (node == nullptr) return 0;

		int leftHeight = (node->left != nullptr) ? node->left->height : -1;
		int rightHeight = (node->right != nullptr) ? node->right->height : -1;

		return leftHeight - rightHeight;
	}


	// this private function inserts the given data to AVL tree with making rotations if necessary
	void insert_(Key& key_item, Value* value_item, AVLNode<Key, Value>*& start) {
		if (start == nullptr)
			start = new AVLNode<Key, Value>(key_item, value_item, 0, nullptr, nullptr);

		else if (key_item < start->name) {
			// X should be inserted to the left tree!
			insert_(key_item, value_item, start->left);

			// Check if the left tree is out of balance (left subtree grew in height!)
			if (height(start->left) - height(start->right) == 2) {
				if (key_item < start->left->name)  // X was inserted to the left-left subtree!
					rotateWithLeftChild(start);
				else			     // X was inserted to the left-right subtree!
					doubleWithLeftChild(start);
			}
		}
		else if (start->name < key_item)
		{    // Otherwise X is inserted to the right subtree
			insert_(key_item, value_item, start->right);
			if (height(start->right) - height(start->left) == 2) {
				// height of the right subtree increased
				if (start->right->name < key_item)
					// X was inserted to right-right subtree
					rotateWithRightChild(start);
				else // X was inserted to right-left subtree
					doubleWithRightChild(start);
			}
		}
		else
			;  // Duplicate; do nothing

	  // update the height the node
		start->height = max(height(start->left), height(start->right)) + 1;
	}

	//this function removes the given key from tree with making necessary rotations
	AVLNode<Key, Value>* remove_(const Key& key, AVLNode<Key, Value>* start) {

		if (!start) {
			return start;
		}
		if (key < start->name)
			start->left = remove_(key, start->left);
		else if (key > start->name)
			start->right = remove_(key, start->right);
		else {
			if (!start->left || !start->right) { //has one or zero child
				AVLNode<Key, Value>* temp = start->left ? start->left : start->right;

				// No child case
				if (!temp) {
					temp = start;
					start = nullptr;
				}
				else {
					// One child case
					*start = *temp; // Copy the contents of the non-empty child
				}
				delete temp;
			}
			else {
				// Node with two children: Get the inorder successor
				AVLNode<Key, Value>* temp = findMin(start->right);
				start->name = temp->name;
				start->data = temp->data;
				start->right = remove_(temp->name, start->right);
			}
		}

		if (!start) return start;


		start->height = 1 + max(height(start->left), height(start->right));

		int balance;

		balance = getBalance(start);

		//checking balance and doing necessary rotations
		if (balance > 1 && getBalance(start->left) >= 0) {
			return rotateWithRightChild(start);
		}
		if (balance > 1 && getBalance(start->left) < 0) {
			start->left = rotateWithLeftChild(start->left);
			return rotateWithRightChild(start);
		}
		if (balance < -1 && getBalance(start->right) <= 0) {
			return rotateWithLeftChild(start);
		}
		if (balance < -1 && getBalance(start->right) > 0) {
			start->right = rotateWithRightChild(start->right);
			return rotateWithLeftChild(start);
		}

		return start;
	}

	//this private function searchs the tree and finds the key's node's pointer, if there is no returns nullptr
	AVLNode<Key, Value>* find_(Key& key, AVLNode<Key, Value>* start) {
		if (start == nullptr)
			return nullptr;
		else if (key < start->name)
			return find_(key, start->left);
		else if (start->name < key)
			return find_(key, start->right);
		else
			return start;    // Match
	}

	// this function is to check whether tree created correctly or not, only prints key by in-order traversal
	void printTree_(AVLNode<Key, Value>* start) const {

		if (!start) {
			return;
		}

		// this is in order traversal
		printTree_(start->left);
		cout << start->name << endl;
		printTree_(start->right);
	}

	//this function is called inside of the destructor
	void destroy(AVLNode<Key, Value>* node) {
		if (node != nullptr) {
			destroy(node->left);
			destroy(node->right);
			delete node;
		}
	}

public:

	//constructor
	AVLSearchTree(AVLNode<Key, Value>* first = nullptr)
		: root(first)
	{}

	AVLSearchTree(const AVLSearchTree& rhs) { //copy constructor
		*this = rhs;
	}

	~AVLSearchTree() { // deconstructor
		destroy(root);
	}

	//calls private find function and returns the address if there is a node exist
	AVLNode<Key, Value>* find(Key& key) {
		return find_(key, root);
	}

	//returns true if tree is empty
	bool isEmpty() const {
		return root == nullptr;
	}

	//calls private print function and accesible for users
	void printTree() const {
		printTree_(root);
	}

	//calls private remove function and accesible for users
	void remove(const Key& key) {
		remove_(key, root);
	}

	//calls private insert function and accesible for users
	void insert(Key& key_item, Value* value_item) {
		insert_(key_item, value_item, root);
	}

	//find min and max function, returns addresses
	AVLNode<Key, Value>* findMin(AVLNode<Key, Value>* start) const {
		while (start && start->left) {
			start = start->left;
		}

		return start;
	}
	AVLNode<Key, Value>* findMax(AVLNode<Key, Value>* start) const {
		while (start && start->right) {
			start = start->right;
		}

		return start;
	}

	void makeEmpty(AVLNode<Key, Value>*& t) const {
		destroy(root);
	}

	//returns height of the given node
	int height(AVLNode<Key, Value>* t = root) const{
		if (t == nullptr)
			return -1;

		return t->height;
	}

	int max(int lhs, int rhs) const {
		if (lhs > rhs)
			return lhs;

		return rhs;
	}


	//rotation functions
	AVLNode<Key, Value>* rotateWithLeftChild(AVLNode<Key, Value>*& k2) const {
		if (k2 == nullptr || k2->left == nullptr) {
			return k2; // Cannot perform rotation if k2 or its left child is null
		}
		AVLNode<Key, Value>* k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2->height = max(height(k2->left), height(k2->right)) + 1;
		k1->height = max(height(k1->left), k2->height) + 1;
		k2 = k1;
		return k1;
	}

	AVLNode<Key, Value>* rotateWithRightChild(AVLNode<Key, Value>*& k1) const {
		if (k1 == nullptr || k1->right == nullptr) {
			return k1; // Cannot perform rotation if k2 or its left child is null
		}
		AVLNode<Key, Value>* k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1->height = max(height(k1->left), height(k1->right)) + 1;
		k2->height = max(height(k2->right), k1->height) + 1;
		k1 = k2;
		return k2;
	}

	AVLNode<Key, Value>* doubleWithLeftChild(AVLNode<Key, Value>*& k3) const {
		rotateWithRightChild(k3->left);
		return rotateWithLeftChild(k3);
	}

	AVLNode<Key, Value>* doubleWithRightChild(AVLNode<Key, Value>*& k1) const {
		rotateWithLeftChild(k1->right);
		return rotateWithRightChild(k1);
	}


};
