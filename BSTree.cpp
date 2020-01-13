#include "BSTree.h"
#include <algorithm>

//Protected inner class BSTreeNode
template< typename DataType, typename KeyType> //Constructor for tree node
BSTree<DataType, KeyType>::BSTreeNode::BSTreeNode(const DataType& nodeDataItem, BSTreeNode* leftPtr, BSTreeNode* rightPtr) {
	dataItem = nodeDataItem;
	left = leftPtr;
	right = rightPtr;
}

//Public member functions

template< typename DataType, typename KeyType > //Default constructor for tree
BSTree<DataType, KeyType>::BSTree() { root = nullptr; }

template < typename DataType, typename KeyType > //Copy constructor for tree
BSTree<DataType, KeyType>::BSTree(const BSTree<DataType, KeyType>& other) {
	root = nullptr;
	*this = other;
}

template< typename DataType, typename KeyType > //Assignment operator overload
BSTree<DataType, KeyType>& BSTree<DataType, KeyType>::operator=(const BSTree<DataType, KeyType>& other) {
	clear();
	if (!other.isEmpty()) { copyHelper(other.root); }
	return *this;
}

template< typename DataType, typename KeyType > //Destructor
BSTree<DataType, KeyType>::~BSTree() {
	clear();
	delete root;
}

template< typename DataType, typename KeyType > //Insert a node
void BSTree<DataType, KeyType>::insert(const DataType& newDataItem) {
	if (isEmpty()) { root = new BSTreeNode(newDataItem, nullptr, nullptr); }
	else { insertHelper(newDataItem, root); }
}

template< typename DataType, typename KeyType > //Retrieve a node
bool BSTree<DataType, KeyType>::retrieve(const KeyType& searchKey, DataType& searchDataItem) {
	if (isEmpty()) { return false; }
	else { return retrieveHelper(searchKey, searchDataItem, root); }
}

template< typename DataType, typename KeyType > //Remove a node
bool BSTree<DataType, KeyType>::remove(const KeyType& deleteKey) {
	bool keyFound = false;
	if (!isEmpty()) { keyFound = removeHelper(deleteKey, root, root); }
	return keyFound;
}

template < typename DataType, typename KeyType > //Write keys in ascending order
void BSTree<DataType, KeyType>::writeKeys() const {
	if (isEmpty()) { cout << "Tree is empty" << endl; }
	else { writeKeysHelper(root); }
	cout << endl;
}

template < typename DataType, typename KeyType > //Clear nodes from tree
void BSTree<DataType, KeyType>::clear() {
	if (!isEmpty()) { clearHelper(root); }
	root = nullptr;
}

template < typename DataType, typename KeyType > //Check if tree is empty
bool BSTree<DataType, KeyType>::isEmpty() const { return (root == nullptr); }

//Public in-lab operations

template < typename DataType, typename KeyType > //Return number of tree nodes
int BSTree<DataType, KeyType>::getCount() {
	int count = 0;
	if (!isEmpty()) { countHelper(root, count); }
	return count;
}

template < typename DataType, typename KeyType > //Return height of tree
int BSTree<DataType, KeyType>::getHeight() const { return heightHelper(root); }

//Private helper functions

template < typename DataType, typename KeyType > //Helper for copy constructor
void BSTree<DataType, KeyType>::copyHelper(const BSTreeNode* otherCurrent) {
	insert(otherCurrent->dataItem);
	if (otherCurrent->left != nullptr) { copyHelper(otherCurrent->left); }
	if (otherCurrent->right != nullptr) { copyHelper(otherCurrent->right); }
}

template < typename DataType, typename KeyType > //Helper for insert method
void BSTree<DataType, KeyType>::insertHelper(const DataType& newDataItem, BSTreeNode* current) {
	if (newDataItem.getKey() < current->dataItem.getKey()) {
		if (current->left == nullptr) { current->left = new BSTreeNode(newDataItem, nullptr, nullptr); }
		else { insertHelper(newDataItem, current->left); }
	}
	else if (newDataItem.getKey() > current->dataItem.getKey()) {
		if (current->right == nullptr) { current->right = new BSTreeNode(newDataItem, nullptr, nullptr); }
		else { insertHelper(newDataItem, current->right); }
	}
	else { current->dataItem = newDataItem; }
}

template < typename DataType, typename KeyType > //Helper for retrieve method
bool BSTree<DataType, KeyType>::retrieveHelper(const KeyType& searchKey, DataType& searchDataItem, BSTreeNode* current) {
	bool keyFound = false;

	if (searchKey < current->dataItem.getKey() && current->left != nullptr) {
		keyFound = retrieveHelper(searchKey, searchDataItem, current->left);
	}
	else if (searchKey > current->dataItem.getKey() && current->right != nullptr) {
		keyFound = retrieveHelper(searchKey, searchDataItem, current->right);
	}
	else if (searchKey == current->dataItem.getKey()) { 
		keyFound = true;
		searchDataItem = current->dataItem;
	}
	return keyFound;
}

template< typename DataType, typename KeyType > //Helper for writeKeys method
void BSTree<DataType, KeyType>::writeKeysHelper(BSTreeNode* current) const {
	if (current->left != nullptr) { writeKeysHelper(current->left); }
	cout << current->dataItem.getKey() << " ";
	if (current->right != nullptr) { writeKeysHelper(current->right); }
}

template< typename DataType, typename KeyType >
void BSTree<DataType, KeyType>::clearHelper(BSTreeNode* current) {
	if (current->left != nullptr) { clearHelper(current->left); }
	if (current->right != nullptr) { clearHelper(current->right); }
	delete current;
}

template < typename DataType, typename KeyType > //Helper for remove method
bool BSTree<DataType, KeyType>::removeHelper(const KeyType& deleteKey, BSTreeNode* last, BSTreeNode* current) {
	bool keyFound = false;

	if (deleteKey < current->dataItem.getKey() && current->left != nullptr) {
		keyFound = removeHelper(deleteKey, current, current->left);
	}
	else if (deleteKey > current->dataItem.getKey() && current->right != nullptr) {
		keyFound = removeHelper(deleteKey, current, current->right);
	}
	else if (deleteKey == current->dataItem.getKey()) { //0 or 1 child node deletion
		keyFound = true;

		if (!(current->left != nullptr && current->right != nullptr)) {
			if (last->left == current) {
				if (current->right == nullptr) { last->left = current->left; }
				else if (current->left == nullptr) { last->left = current->right; }
			}
			else if (last->right == current) {
				if (current->right == nullptr) { last->right = current->left; }
				else if (current->left == nullptr) { last->right = current->right; }
			}
			else if (last == current) {
				if (root->right == nullptr) { root = root->left; }
				else if (root->left == nullptr) { root = root->right; }
			}
			delete current;
		}
		else { //2 child node deletion
			KeyType copyKey;
			retrieveMinKey(copyKey, current->right);
			removeHelper(copyKey, root, root);
			current->dataItem.setKey(copyKey);
		}
	}
	return keyFound;
}

template < class DataType, class KeyType > //Another helper for remove method; retrieves the minimum key in a subtree
void BSTree<DataType, KeyType>::retrieveMinKey(KeyType& searchKey, BSTreeNode* current) {
	if (current->left != nullptr) { retrieveMinKey(searchKey, current->left); }
	else { searchKey = current->dataItem.getKey(); }
}

template< typename DataType, typename KeyType > //Helper for getCount method
void BSTree<DataType, KeyType>::countHelper(BSTreeNode* current, int& count) {
	if (current->left != nullptr) { countHelper(current->left, count); }
	count++;
	if (current->right != nullptr) { countHelper(current->right, count); }
}

template< typename DataType, typename KeyType > //Helper for getHeight method
int BSTree<DataType, KeyType>::heightHelper(BSTreeNode* current) const {
	if (current == nullptr) { return 0; }
	return max(heightHelper(current->left), heightHelper(current->right)) + 1;
}