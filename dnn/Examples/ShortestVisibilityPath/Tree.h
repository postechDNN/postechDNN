#pragma once
#include "Node.h"

double absTolerance = (1.0e-8);
class Tree {
private:
	Node * root;
public:
	Tree() {
		root = NULL;
	}
	Node * find_left_edge(point_type x, point_type y) {
		Node * left_edge = NULL;
		Node * current = root;
		while (current) {
			if (abs(current->get_x(y)-x)<absTolerance || current->get_x(y)>x){
				current = current->left;
			}
			else {
				left_edge = current;
				current = current->right;
			}
		}
		return left_edge;
	}
	Node * delete_node(point_type x, point_type y) {
		if (root == NULL) return NULL;
		else {
			root = internal_delete(root, x, y);
			if (root != NULL) root->parent = NULL;
			return NULL;
		}
	}
	Node * internal_delete(Node * current, point_type x, point_type y) {
		
		if (current == NULL) {
			cout << "Delete error (" << x<<", "<<y<< ")"<<endl;
			return NULL;
		}
		else if (abs(current->get_x(y)-x)<absTolerance) {
			if (current->right == NULL && current->left == NULL) {
				if (current == root) root = NULL;
				delete(current);
				return NULL;
			}
			else if (current->right == NULL) {
				Node * current_left = current->left;
				if (current == root) root = current_left;
				delete(current);
				current = current_left;
			}
			else if (current->left == NULL) {
				Node * current_right = current->right;
				if (current == root) root = current_right;
				delete(current);
				current = current_right;
			}
			else {
				Node * min_x = find_min_x(current->right);
				current->copy(min_x);
				Node * new_right_node = internal_delete(current->right, min_x->get_x(y), y);
				if (new_right_node != NULL) new_right_node->parent = current;
				current->right = new_right_node;
			}
		}
		else if (current->get_x(y) < x) {
			Node * new_right_node = internal_delete(current->right, x, y);
			if (new_right_node != NULL) new_right_node->parent = current;
			current->right = new_right_node;
		}
		else if (current->get_x(y) > x) {
			Node * new_left_node = internal_delete(current->left, x, y);
			if (new_left_node != NULL) new_left_node->parent = current;
			current->left = new_left_node;
		}
		

		set_height(current);
		return rebalance(current, y, x);
	}

	Node * find_min_x(Node * current) {
		if (current == NULL) return NULL;
		else {
			if (current->left == NULL) {
				return current;
			}
			else {
				return find_min_x(current->left);
			}
		}
	}

	Node * insert_node(int helper, point_type angle, Point point, point_type y) {
		Node * value = new Node(helper, angle, point);
		if (root == NULL) {
			root = value;
			return value;
		}
		else {
			root = internal_insert(root, value, y);
			root->parent = NULL;
			return NULL;
		}
	}
	/*

	return val : current_position node
	*/
	Node* internal_insert(Node* current, Node* value, point_type y) {
		if (current == NULL) {
			return value;
		}
		else if (comp(current, value, y) >= 0) {
			Node * new_right_node = internal_insert(current->right, value, y);
			new_right_node->parent = current;
			current->right = new_right_node;
		}
		else {
			Node * new_left_node = internal_insert(current->left, value, y);
			new_left_node->parent = current;
			current->left = new_left_node;
		}

		set_height(current);

		return rebalance(current, y, value->get_x(y));

	}

	Node *  rebalance(Node * current, point_type y, point_type target_x) {
		int balance = get_balance(current);

		//LL case
		if (balance > 1 && get_balance(current->left) >= 0) {
			return rightRotate(current);
		}
		//LR case
		else if (balance > 1 && get_balance(current->left) < 0) {
			Node * new_left_node = leftRotate(current->left);
			new_left_node->parent = current;
			current->left = new_left_node;
			return rightRotate(current);
		}
		//RR case
		else if (balance < -1 && get_balance(current->right) <= 0) {
			return leftRotate(current);
		}
		//RL case
		else if (balance <-1 && get_balance(current->right) > 0) {
			Node * new_right_node = rightRotate(current->right);
			new_right_node->parent = current;
			current->right = new_right_node;
			return leftRotate(current);
		}

		return current;
	}

	void set_height(Node * current) {
		current->height = 1 + max(get_height(current->left), get_height(current->right));
	}
	Node * rightRotate(Node*& current) {
		Node * current_left = current->left;
		Node * current_left_right = current_left->right;

		current_left->right = current;
		current->left = current_left_right;

		//change_parent(current, current_left);
		if (current_left_right != NULL) current_left_right->parent = current;
		current->parent = current_left;

		set_height(current);
		set_height(current_left);

		if (current == root)
			root = current_left;

		return current_left;

	}
	Node* leftRotate(Node*& current) {
		Node * current_right = current->right;
		Node * current_right_left = current_right->left;

		current_right->left = current;
		current->right = current_right_left;

		//change_parent(current, current_right);
		if (current_right_left != NULL) current_right_left->parent = current;
		current->parent = current_right;

		set_height(current);
		set_height(current_right);

		if (current == root)
			root = current_right;

		return current_right;

	}

	//TO DELETE
	// current의 parent를 current_left에게 양보한다.
	void change_parent(Node * current, Node * current_left) {
		if (current->parent == NULL) {
			current_left->parent = NULL;
			root = current_left;
		}
		else {
			current->parent->change_child(current, current_left);
		}
	}
	int get_balance(Node * current) {
		if (current == NULL) return 0;
		else return get_height(current->left) - get_height(current->right);
	}
	int get_height(Node * value) {
		if (value == NULL) return 0;
		else  return value->height;
	}

	int comp(Node * current, Node * value, point_type y) {
		point_type current_x = current->get_x(y);
		point_type value_x = value->get_x(y);

		if ( abs(current_x-value_x) <= absTolerance ) return 0;
		else if (current_x > value_x) return -1;
		else return 1;
	}


};