#include "BST.hpp"
#include <queue>

template<class T>
BST<T>::BST() : root{ nullptr } {}

template<class T>
void BST<T>::insertNode(const T& data)
{
	if (root == nullptr)
	{
		root = new Node(data);
	}
	else
	{
		insertNode(root, data);
	}
}

template<class T>
void BST<T>::searchNode(const T& data)
{
	if (root == nullptr)
	{
		std::cout << "Tree hasn't element\n";
	}
	else
	{
		searchNode(root, data);
	}
}

template<class T>
void BST<T>::insertNode(Node* node, const T& data)
{
	if (node->data == data)
	{
		std::cout << "Value already exists\n";
		return;
	}
	if (data < node->data)
	{
		if (node->left == nullptr)
		{
			node->left = new Node(data);
		}
		else
		{
			insertNode(node->left, data);
		}
	}
	else {
		if (node->right == nullptr)
		{
			node->right = new Node(data);
		}
		else
		{
			insertNode(node->right, data);
		}
	}
}

template<class T>
void BST<T>::searchNode(Node* node, const T& data)
{
	if (data == node->data)
	{
		std::cout << node->data << std::endl;
		return;
	}
	if (data > node->data)
	{
		searchNode(node->right, data);
	}
	else
	{
		searchNode(node->left, data);
	}
}

template<class T>
void BST<T>::destroyTree(Node* root)
{
	if (root != nullptr)
	{
		destroyTree(root->left);
		destroyTree(root->right);
		delete root;
	}
}

template<class T>
void BST<T>::deleteNode(const T& data)
{
	deleteNode(root, data);
}

template<class T>
void BST<T>::printPreorder()
{
	printPreorder(root);
}

template<class T>
void BST<T>::printPostorder()
{
	printPostorder(root);
}

template<class T>
void BST<T>::printInorder()
{
	printInorder(root);
}

template<class T>
void BST<T>::printLevelorder()
{
	printLevelorder(root);
}

template<class T>
void BST<T>::deleteNode(Node* node, const T& data)
{
	if (root == nullptr)
	{
		std::cout << "Tree havn't element\n";
	}
	else if (data > node->data)
	{
		deleteNode(node->right, data);
		if (node->right->data < 0)
		{
			node->right = nullptr;
		}
	}
	else if (data < node->data)
	{
		deleteNode(node->left, data);
		if (node->left->data < 0)
		{
			node->left = nullptr;
		}
	}
	else
	{
		if (node->right == nullptr && node->left == nullptr)
		{
			delete node;
			node = nullptr;
		}
		else if (node->right == nullptr)
		{
			Node* tempNode = node;
			node = node->left;
			delete tempNode;
		}
		else if (node->left == nullptr)
		{
			Node* tempNode = node;
			node = node->right;
			delete tempNode;
		}
		else
		{
			Node* tempNode = findMinNode(node->right);
			node->data = tempNode->data;
			deleteNode(node->right, tempNode->data);
			if (node->right->data < 0)
			{
				node->right = nullptr;
			}
		}
	}
}

template<class T>
void BST<T>::printPreorder(Node* node)
{
	if (node != nullptr)
	{
		std::cout << node->data << ' ';
		printPreorder(node->left);
		printPreorder(node->right);
	}
	return;
}

template<class T>
void BST<T>::printPostorder(Node* node)
{
	if (node != nullptr)
	{
		printPreorder(node->left);
		printPreorder(node->right);
		std::cout << node->data << ' ';
	}
	return;
}

template<class T>
void BST<T>::printInorder(Node* node)
{
	if (node != nullptr)
	{
		printPreorder(node->left);
		std::cout << node->data << ' ';
		printPreorder(node->right);
	}
	return;
}

template<class T>
void BST<T>::printLevelorder(Node* node)
{
	std::queue<Node*> q;
	if (node)
	{
		q.push(node);
	}
	while (!q.empty())
	{
		const Node* const tempNode = q.front();
		q.pop();
		std::cout << tempNode->data << ' ';
		if (tempNode->left)
		{
			q.push(tempNode->left);
		}
		if (tempNode->right)
		{
			q.push(tempNode->right);
		}
	}
}

template<class T>
typename BST<T>::Node* BST<T>::findMinNode(Node* node)
{
	Node* tempNode = node;
	while (tempNode->left != nullptr)
	{
		tempNode = tempNode->left;
	}
	return tempNode;
}

template<class T>
BST<T>::~BST()
{
	destroyTree(root);
}