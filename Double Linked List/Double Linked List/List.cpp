#include "List.hpp"

template<typename T>
List<T>::List()
{
	head = tail = nullptr;
	Size = 0;
}

template<typename T>
List<T>::~List()
{
	clear();
}

template<typename T>
int List<T>::GetSize()
{
	return Size;
}

template<typename T>
void List<T>::push_back(T ndata)
{
	Node<T>* tmp = new Node<T>(ndata);
	if (tail == nullptr) {
		tail = tmp;
		head = tmp;
	}
	else {
		tail->next = tmp;
		tmp->prev = tail;
		tail = tmp;
	}
	Size++;
}

template<typename T>
void List<T>::pop_back()
{
	if (tail == nullptr) {
		return;
	}
	else if (head->next == nullptr) {
		Node<T>* tmp = tail;
		tail = head = nullptr;
		delete tmp;
		Size--;
	}
	else {
		Node<T>* tmp = tail;
		tail = tmp->prev;
		tail->next = nullptr;
		delete tmp;
		Size--;
	}
}

template<typename T>
void List<T>::push_front(T ndata)
{
	Node<T>* tmp = new Node<T>(ndata);
	if (head == nullptr) {
		head = tmp;
		tail = tmp;
	}
	else {
		tmp->next = head;
		head->prev = tmp;
		head = tmp;
	}
	Size++;

}

template<typename T>
void List<T>::pop_front()
{
	if (head == nullptr) {
		return;
	}
	else if (head->next == nullptr) {
		Node<T>* tmp = head;
		head = tail = nullptr;
		delete tmp;
	}
	else {
		Node<T>* tmp = head;
		head = tmp->next;
		head->prev = nullptr;
		delete tmp;
	}
	Size--;
}

template<typename T>
void List<T>::insert(int index, T ndata)
{
	assert(index <= GetSize() && index >= 0 && "Index must be in range [0;Size]");
	Node<T>* tmp = new Node<T>(ndata);
	if (index == 0) {
		push_front(ndata);
	}
	else if (index == GetSize()) {
		push_back(ndata);
	}
	else if (index <= GetSize() / 2) {
		Node<T>* node = head;
		for (int i = 0; i < index - 1; i++) {
			node = node->next;
		}
		tmp->next = node->next;
		tmp->next->prev = tmp;
		tmp->prev = node;
		node->next = tmp;
		Size++;
	}
	else {
		Node<T>* node = tail;
		for (int i = GetSize() - 1; i > index; i--) {
			node = node->prev;
		}
		tmp->next = node;
		node->prev->next = tmp;
		tmp->next = node;
		node->prev = tmp;
		Size++;
	}
}

template<typename T>
void List<T>::erase(int index)
{
	assert(index < GetSize() && index >= 0 && "Index must be in range [0;Size-1]");
	if (index == 0) {
		pop_front();
	}
	else if (index == GetSize() - 1) {
		pop_back();
	}
	else if (index <= GetSize() / 2) {
		Node<T>* tmp = head;
		for (int i = 0; i < index - 1; i++) {
			tmp = tmp->next;
		}
		Node<T>* node = tmp->next;
		tmp->next = node->next;
		node->next->prev = tmp;
		delete node;
		Size--;
	}
	else {
		Node<T>* tmp = tail;
		for (int i = GetSize() - 1; i > index + 1; i--) {
			tmp = tmp->prev;
		}
		Node<T>* node = tmp->prev;
		tmp->prev = node->prev;
		node->prev->next = tmp;
		delete node;
		Size--;
	}

}

template<typename T>
void List<T>::clear()
{
	while (Size) {
		pop_front();
	}
}

template<typename T>
T& List<T>::operator[](const int index)
{
	assert(index < GetSize() && index >= 0 && "Index must be in range [0;Size-1]");
	int count = 0;
	Node<T>* tmp = head;
	while (count != index) {
		tmp = tmp->next;
		count++;
	}
	return tmp->data;
}