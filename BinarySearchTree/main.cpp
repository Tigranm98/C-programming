#include "BST.cpp"

int main() {
	BST<int> tree;
	tree.insertNode(7);
	tree.insertNode(4);
	tree.insertNode(10);
	tree.insertNode(2);
	tree.insertNode(6);
	tree.insertNode(8);
	tree.insertNode(15);
	tree.insertNode(11);
	tree.deleteNode(4);
	tree.printPreorder();
	std::cout << std::endl;
	tree.printPostorder();
	std::cout << std::endl;
	tree.printInorder();
	std::cout << std::endl;
	tree.printLevelorder();
	return 0;
}
