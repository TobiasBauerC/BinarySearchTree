#include "BinarySearchTree.h"
#include <iostream>

using namespace std;

int main()
{
	shared_ptr<Tree<int>> bst = make_shared<BinarySearchTree<int>>();

	bst->insert(12);
	bst->insert(4);
	bst->insert(20);
	bst->insert(1);
	//bst->insert(7);
	bst->insert(23);
	//bst->insert(5);

	bst->traversal();
	bst->remove(12);
	bst->traversal();

	return 0;
}