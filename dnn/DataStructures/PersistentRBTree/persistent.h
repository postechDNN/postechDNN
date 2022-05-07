#pragma once
#include<vector>
#include<array>

using namespace std;

struct NodeP;

struct NodeC
{
	NodeC* prev;
	NodeC* next;
	NodeP* parent;
	long long tag;
	int version;
	NodeC(int v)
	{
		prev = nullptr;
		next = nullptr;
		parent = nullptr;
		tag = 0;
		version = v;
	}
};

struct NodeP
{
	NodeP* prev;
	NodeP* next;
	NodeC* child; //head of the child list
	long long tag;
	int size;
	NodeP()
	{
		prev = nullptr;
		next = nullptr;
		child = nullptr;
		tag = 0;
		size = 1;
	}
	~NodeP()
	{
		NodeC* cur = child;
		while (cur)
		{
			NodeC* next = cur->next;
			delete cur;
			cur = next;
		}
	}
};



struct Version
{
	array<NodeC*, 2> adds{ nullptr,nullptr }; //addresss of second level for two lists
	int parent;
	//Node* root;
	Version(int p)
	{
		parent = p;
	}
};



class VersionTree
{
public:
	~VersionTree();
	void newVersion(int pindex);
	bool testAncestor(int v1, int v2);
private:
	vector<Version> v;
	array<NodeP*, 2> heads{nullptr,nullptr};
	bool testOrder(int list, int v1, int v2);
};


VersionTree::~VersionTree()
{
	for (size_t i = 0; i < 2; i++)
	{
		NodeP* cur = heads[i];
		while (cur)
		{
			NodeP* next = cur->next;
			delete cur;
			cur = next;
		}
	}
}




