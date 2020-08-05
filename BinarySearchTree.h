#ifndef BinarySearchTree_h
#define BinarySearchTree_h

#include "Tree.h"
#include <memory>
#include <iostream>
#include <exception>

// Custom exception for if the BST is empty
class EmptyTreeException : std::exception
{
public:
	EmptyTreeException() {};

	const char* what() const noexcept override
	{
		return "BinarySearchTree is empty()";
	}
};

template<typename t_Data>
class BinarySearchTree : public Tree<t_Data>
{
	// Nodes will store their left and right child as well as their data
	// struct because we do not need a full class for this
	struct Node
	{
		// Store data + two child nodes
		t_Data data;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;

		Node(const t_Data& _data) : data(_data) {}
	};

	std::unique_ptr<Node> root;

public: 

	// insert node into tree in O(logN) time complexity
	void insert(const t_Data& data) override 
	{
		if (root)
		{
			// if there is already a root node, insert into the tree
			insertNode(data, root.get());
		}
		else
		{
			// if there is no root node, make data the new root node
			root = std::make_unique<Node>(data);
		}
	}

	void traversal() const override
	{
		// if no root, return
		if (!root)
			return;

		// travers starting at root node
		inOrderTraversal(root.get());
		std::cout << '\n';
	}

	void remove(const t_Data& data) override
	{
		// if no root, return
		if (!root)
			return;

		// search for and remove data starting with root node
		remove(data, root);
	}

	const t_Data& getMaxValue() const override
	{
		// if no root, throw exception (because there is no return value)
		if (!root)
			throw EmptyTreeException{};

		// search for max value starting with root node
		return getMax(root.get());
	}

	const t_Data& getMinValue() const override
	{
		// if no root, throw exception (because there is no return value)
		if (!root)
			throw EmptyTreeException{};

		// search for max value starting with root node
		return getMin(root.get());
	}

private:

	void insertNode(const t_Data& data, Node* node)
	{
		// if data is smaller than the data in the current node, investigate left child (smaller values go to the left)
		if (data < node->data)
		{
			// if there is a left child (node), recursively check its data and children
			if(node->left)
				insertNode(data, node->left.get());
			// otherwise, new left node is created and data is stored there
			else
				node->left = std::make_unique<Node>(data);
		}
		// if data is larger than the data in the current node, investigate right child (larger values go to the right)
		else
		{
			// if there is a right child (node), recursively check its data and children
			if (node->right)
				insertNode(data, node->right.get());
			// otherwise, new right node is created and data is stored there
			else
				node->right = std::make_unique<Node>(data);
		}
	}

	void inOrderTraversal(Node* node) const
	{
		// if node has a left node (child), recursively go through it and all other left nodes
		// left first because left is smaller values
		if (node->left)
			inOrderTraversal(node->left.get());

		// Display data within the current node
		std::cout << node->data << "  -->  ";

		// if node has a right node (child), recursively go through it and all other right nodes
		// right second because right is larger values
		if (node->right)
			inOrderTraversal(node->right.get());
	}

	void remove(const t_Data& data, std::unique_ptr<Node>& nodeRef)
	{
		// recursively search for the node to remove
		if (data < nodeRef->data)
		{
			if (nodeRef->left)
				remove(data, nodeRef->left);
		}
		else if (data > nodeRef->data)
		{
			if (nodeRef->right)
				remove(data, nodeRef->right);
		}
		else
		{
			// We found the node to remove!
			// to remove a leaf node, simple reset the struct to null 
			if (!nodeRef->left && !nodeRef->right)
			{
				std::cout << "Removeing a leaf node...\n";
				nodeRef.reset();
				return;
			}

			// removing the right child
			if (!nodeRef->left)
			{
				std::cout << "Remving the right child...\n";
				std::unique_ptr<Node> temp = std::move(nodeRef->right);
				nodeRef.reset();
				nodeRef = std::move(temp);
				return;
			}
			//removing the left child
			if (!nodeRef->right)
			{
				std::cout << "Removing the left child...\n";
				std::unique_ptr<Node> temp = std::move(nodeRef->right);
				nodeRef.reset();
				nodeRef = std::move(temp);
				return;
			}

			// Case where node to remove has two children
			std::cout << "Removing two children\n";
			// Find largest (rightmost) item in leftmost tree and detatch it to become the root node for new side tree
			std::unique_ptr<Node> temp = detatchRightMost(nodeRef->left);

			// Set new temp node's right node (child) to be the remove node's right node (child)
			temp->right = std::move(nodeRef->right);
			std::unique_ptr<Node>& tempLeftMost = getLeftMost(temp);
			tempLeftMost->left = std::move(nodeRef->left);
			nodeRef.reset();
			nodeRef = std::move(temp);
		}
	}

	std::unique_ptr<Node> detatchRightMost(std::unique_ptr<Node>& nodeRef)
	{
		// recursively go through nodes until finding a node with no right node (child) and return that node while also removing it from tree
		if (nodeRef->right)
			return detatchRightMost(nodeRef->right);

		return std::move(nodeRef);
	}

	std::unique_ptr<Node>& getLeftMost(std::unique_ptr<Node>& nodeRef)
	{
		// Recursively go through all nodes with a left node (child) and return reference too leftmost node
		if (nodeRef->left)
			return getLeftMost(nodeRef->left);

		return nodeRef;
	}

	const t_Data& getMax(Node* node) const
	{
		if (node->right)
			return getMax(node->right.get());

		return node->data;
	}

	const t_Data& getMin(Node* node) const
	{
		if (node->left)
			return getMin(node->left.get());

		return node->data;
	}
};

#endif