#include "List.cpp"

int main() {
	List<int> lst;
	lst.insert(0, 40);
	lst.insert(1, 50);
	lst.insert(2, 60);
	lst.push_back(70);
	lst.push_back(80);
	lst.push_back(90);
	lst.insert(6, 100);
	std::cout << lst[4];
	return 0;
}