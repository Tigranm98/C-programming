#pragma once
#include <iostream>
#include <cassert>

template<typename T>
struct Node {
	template<typename U>
	friend class List;
private:
	T data;
	Node* next;
	Node* prev;
	Node(T ndata) {
		next = prev = nullptr;
		data = ndata;
	}
};

template<typename T>
class List{
private: 
	Node<T>* head, *tail;
	int Size;

public:
	List();
	~List();
	void push_back(T ndata);
	void pop_back();
	void push_front(T ndata);
	void pop_front();
	void insert(int index, T ndata);
	void erase(int index);
	void clear();
	int GetSize();
	T& operator[](const int index);
};

