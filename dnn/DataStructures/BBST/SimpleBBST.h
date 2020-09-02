#pragma once 
#include <set>

template <typename T>
class SimpleBBSTNode{
	public :
		typename std::multiset<T>::iterator it;
		SimpleBBSTNode(typename std::multiset<T>::iterator it);
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
		
		bool isEmpty();
		
		bool isLeftMost(SimpleBBSTNode<T>);
		bool isRightMost(SimpleBBSTNode<T>);
		void insert(T);
		void erase(T);
		SimpleBBSTNode<T> findLeft(T);	
		SimpleBBSTNode<T> findRight(T);
		SimpleBBSTNode<T> getLeftNode(SimpleBBSTNode<T>);
		SimpleBBSTNode<T> getRightNode(SimpleBBSTNode<T>);
};

