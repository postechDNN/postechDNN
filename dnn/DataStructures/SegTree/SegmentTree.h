#pragma once

#include <iostream>

#define MIN -99999999

using namespace std;

typedef struct Node {
	int a;
	int b;
	int c;

	int l;
	int r;
	
	struct Node* lchild;
	struct Node* rchild;
} Node;

class SegmentTree {
public:
	Node* root;
	Node* makeSegmentTree(int left, int right);
	pair<int, int> max(Node* node, int left, int right);
	void add(Node* node, int l, int r, int x);
	void set(Node* node, int idx, int x);
	void update(Node* node);
	void clearTree(Node* node);
	void postorderout(Node* node);
};