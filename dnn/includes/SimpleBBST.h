#pragma once 
#include <set>

template <typename T>
class SimpleBBSTNode{
	public :
		typename std::multiset<T>::iterator it;
		SimpleBBSTNode(typename std::multiset<T>::iterator it);//store the iterator of this node.
		~SimpleBBSTNode();
		T getData();
};


template <typename T>
class SimpleBBST{
	private:
		std::multiset<T> mt;
	public :
		SimpleBBST();
		~SimpleBBST();
		
		bool isEmpty();//return true when the tree is empty
		bool isLeftMost(SimpleBBSTNode<T>);//return true when the node has the smallest value.
		bool isRightMost(SimpleBBSTNode<T>);//return true when the node has the largest value.
		void insert(T);//insert a node storing value t into the tree.
		void erase(T);//erase the node storing value t into the tree.
		SimpleBBSTNode<T> findLeft(T);//return the node left to the node	storing value t into the tree.
		SimpleBBSTNode<T> findRight(T);//return the node right to the node storing value t into the tree.
		SimpleBBSTNode<T> getLeftNode(SimpleBBSTNode<T>);//return the node left to the node nt.
		SimpleBBSTNode<T> getRightNode(SimpleBBSTNode<T>);//return the node right to the node nt.
};

