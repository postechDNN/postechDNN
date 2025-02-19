#pragma once
#include "Space.h"
#define EPS 1e-5

class AVLTreeList
{
	struct Node
	{
		Edge data;
		Node* left;
		Node* right;
	};

	enum RotateDir
	{
		NONE,
		LL,
		RR,
		LR,
		RL,
	};

	class NodeStack
	{
		struct StackNode
		{
			Node* data;
			StackNode* next;
		};

	private:
		StackNode head;
		StackNode tail;
	public:
		NodeStack()
		{
			head.next = &tail;
			tail.next = &tail;
		}
		bool Push(Node* value)
		{
			StackNode* newStackNode = new StackNode;
			newStackNode->data = value;
			newStackNode->next = head.next;
			head.next = newStackNode;
			return true;
		}
		bool Pop(Node** pop)
		{
			if (head.next == &tail)
				return false;

			StackNode* deleteStackNode = head.next;
			head.next = deleteStackNode->next;
			*pop = deleteStackNode->data;
			delete deleteStackNode;

			return true;
		}
	};

private:
	Node root;			//root 는 더미노드, 실직적 root 는 root.left


	NodeStack stack;

	void Rebalance();
	void RebalancingPivot(Node* node);
	bool RebalancingNode(Node* node, Node** rotatedNode);
	int GetHeight(Node* node);
	bool Rotate(Node* node, RotateDir dir, Node** rotatedNode);

public:
	AVLTreeList();

	bool Is_empty();
	Edge Search();
	bool Search(Edge data, Point v1);
	bool Insert(Edge data, Point v1, Point v2);
	bool Delete(Edge data, Point v1, Point v2);

};
