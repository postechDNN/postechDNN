#pragma once 
#include <set>

template <typename T, typename Comp = std::less<T>>
class SimpleBBSTNode{
	public :
		typename std::multiset<T, Comp>::iterator it;
		SimpleBBSTNode(typename std::multiset<T, Comp>::iterator it);//store the iterator of this node.
		 T getData();
};

template <typename T, typename Comp = std::less<T>>
class SimpleBBST{
	private:
		std::multiset<T, Comp> mt;
	public :
		bool isEmpty();//return true when the tree is empty
		bool isLeftMost(SimpleBBSTNode<T, Comp>);//return true when the node has the smallest value.
		bool isRightMost(SimpleBBSTNode<T, Comp>);//return true when the node has the largest value.
		void insert(T);//insert a node storing value t into the tree.
		void erase(T);//erase the node storing value t into the tree.
		SimpleBBSTNode<T, Comp> findLeft(T);//return the node left to the node	storing value t into the tree.
		SimpleBBSTNode<T, Comp> findRight(T);//return the node right to the node storing value t into the tree.
		SimpleBBSTNode<T, Comp> getLeftNode(SimpleBBSTNode<T, Comp>);//return the node left to the node nt.
		SimpleBBSTNode<T, Comp> getRightNode(SimpleBBSTNode<T, Comp>);//return the node right to the node nt.
};

template <typename T, typename Comp>
SimpleBBSTNode<T, Comp>::SimpleBBSTNode(typename std::multiset<T, Comp>::iterator its) {
	it = its;
}

template <typename T, typename Comp>
T SimpleBBSTNode<T, Comp>::getData() {
	return *it;
}

template <typename T, typename Comp>
bool SimpleBBST<T, Comp>::isEmpty() {
	return mt.empty();
}

template <typename T, typename Comp>
void SimpleBBST<T, Comp>::insert(T t) {
	mt.insert(t);
}

template <typename T, typename Comp>
SimpleBBSTNode<T, Comp> SimpleBBST<T, Comp>::findLeft(T t) {
	typename std::multiset<T, Comp>::iterator lb = mt.lower_bound(t);
	return SimpleBBSTNode<T, Comp>(lb);
}

template <typename T, typename Comp>
SimpleBBSTNode<T, Comp> SimpleBBST<T, Comp>::findRight(T t) {
	typename std::multiset<T, Comp>::iterator rb = mt.upper_bound(t);
	return SimpleBBSTNode<T, Comp>(rb);
}

template <typename T, typename Comp>
SimpleBBSTNode<T, Comp> SimpleBBST<T, Comp>::getLeftNode(SimpleBBSTNode<T, Comp> nt) {
	return SimpleBBSTNode<T, Comp>(nt.it--);
}

template <typename T, typename Comp>
SimpleBBSTNode<T, Comp> SimpleBBST<T, Comp>::getRightNode(SimpleBBSTNode<T, Comp> nt) {
	return SimpleBBSTNode<T, Comp>(nt.it++);
}

template <typename T, typename Comp>
bool SimpleBBST<T, Comp>::isLeftMost(SimpleBBSTNode<T, Comp> nt) {
	return nt.it == mt.begin();
}

template <typename T, typename Comp>
bool SimpleBBST<T, Comp>::isRightMost(SimpleBBSTNode<T, Comp> nt) {
	return nt.it == mt.rbegin();
}