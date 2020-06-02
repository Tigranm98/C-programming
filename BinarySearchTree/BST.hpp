#ifndef _BST_H_
#define _BST_H_

#include <iostream>

template<class T>
class BST {
public:
	BST();
	void insertNode(const T& data);
	void searchNode(const T& data);
	void deleteNode(const T& data);
	void printPreorder();
	void printPostorder();
	void printInorder();
	void printLevelorder();
	~BST();
private:
	struct Node {
	public:
		Node(const T& ndata) : left{ nullptr }, right{ nullptr }, data{ ndata } {}
		Node* left;
		Node* right;
		T data;
	};
	Node* root;
	void insertNode(Node* node, const T& data);
	void searchNode(Node* node, const T& data);
	void deleteNode(Node* node, const T& data);
	void destroyTree(Node* root);
	void printPreorder(Node* node);
	void printPostorder(Node* node);
	void printInorder(Node* node);
	void printLevelorder(Node* node);
	Node* findMinNode(Node* node);
};

#endif