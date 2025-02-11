#include "pch.h"
#include "AVLTreeList.h"
#include <algorithm>
#include <math.h>

#define EPS 1e-4

AVLTreeList::AVLTreeList()
{
    root.left = nullptr;
    root.right = nullptr;
}

bool AVLTreeList::Is_empty()
{
    Node* currentNode = root.left;
    if (currentNode == nullptr) { return true;}
    return false;
}

Edge AVLTreeList::Search()
{
    Node* currentNode = root.left;
    while (currentNode->left != nullptr)
    {
        currentNode = currentNode->left;
    }
    return currentNode->data;
}

bool AVLTreeList::Search(Edge data, Point v1)
{
    Node* parentNode = &root;	//지울노드가 맨처음 노드 일경우를 대비해서 가상노드 루트가 있다.
    Node* deleteNode = root.left;
    bool isLeft = true;
    Point x(v1.getx() + 1 / EPS, v1.gety());
    Edge E(v1, x);
    bool right = true;
    if (data.crossing(E, true) == nullptr) return false;

    double key = v1.distance(data.crossing(E, true)->gets());

    while (deleteNode != nullptr)	//지울 노드를 찾는 과정
    {
        if (deleteNode->data.gets() == data.gets() && deleteNode->data.gett() == data.gett())
            break;

        stack.Push(parentNode);	//리밸런싱을 위해 저장
        parentNode = deleteNode;

        double del_key = v1.distance(deleteNode->data.crossing(E, true)->gets());;

        if (del_key <= key)
        {
            isLeft = false;
            deleteNode = deleteNode->right;
        }
        else if (del_key > key)
        {
            isLeft = true;
            deleteNode = deleteNode->left;
        }
    }

    if (deleteNode == nullptr)	//노드를 못찾았다면
    {
        return false;
    }
    else return true; // 노드를 찾았다면
}

bool AVLTreeList::Insert(Edge data, Point v1, Point v2)
{
    Node* parentNode = &root;
    Node* currentNode = root.left;
    bool isLeft = true;
    double x = v2.getx() - v1.getx();
    double y = v2.gety() - v1.gety();
    Point V(v1.getx() + x / EPS, v1.gety() + y / EPS);
    Point VR(v1.getx() + (x * cos(ERR*10) - y * sin(ERR*10)) / EPS, v1.gety() + (x * sin(ERR*10) + y * cos(ERR*10)) / EPS);
    Edge E(v1, V);
    Edge ER(v1, VR);
    double key = v1.distance(data.crossing(E, true)->gets());

    while (currentNode != nullptr)		//(왼쪽 혹은 오른쪽 자식이 비어있는)부모 노드를 찾는 과정
    {

        double cur_key = v1.distance(currentNode->data.crossing(E, true)->gets());
        if (currentNode->data.gets() == data.gets() && currentNode->data.gett() == data.gett())
            return false;

        stack.Push(parentNode);	//리밸런싱을 위해 저장
        parentNode = currentNode;

        if (key > cur_key + ERR)
        {
            isLeft = false;
            currentNode = currentNode->right;
        }
        else if (key + ERR < cur_key)
        {
            isLeft = true;
            currentNode = currentNode->left;
        }
        else {
            double cur_rkey = v1.distance(currentNode->data.crossing(ER, true)->gets());
            double rkey = v1.distance(data.crossing(ER, true)->gets());
            if (cur_rkey < rkey)
            {
                isLeft = false;
                currentNode = currentNode->right;
            }
            else if (cur_rkey > rkey)
            {
                isLeft = true;
                currentNode = currentNode->left;
            }
        }
    }
    currentNode = new Node;		//새로운 노드를 만들고
    currentNode->data = data;
    currentNode->left = nullptr;
    currentNode->right = nullptr;

    isLeft == true ? parentNode->left = currentNode : parentNode->right = currentNode;

    Rebalance();

    return true;
}

bool AVLTreeList::Delete(Edge data, Point v1, Point v2)
{
    Node* parentNode = &root;	//지울노드가 맨처음 노드 일경우를 대비해서 가상노드 루트가 있다.
    Node* deleteNode = root.left;
    bool isLeft = true;

    double x = v2.getx() - v1.getx();
    double y = v2.gety() - v1.gety();
    Point V(v1.getx() + x / EPS, v1.gety() + y / EPS);
    Point VR(v1.getx() + (x * cos(-ERR*10) - y * sin(-ERR*10)) / EPS, v1.gety() + (x * sin(-ERR*10) + y * cos(-ERR*10)) / EPS);
    Edge E(v1, V);
    Edge ER(v1, VR);
    double key = v1.distance(data.crossing(E, true)->gets());

    while (deleteNode != nullptr)	//지울 노드를 찾는 과정
    {
        if (deleteNode->data.gets() == data.gets() && deleteNode->data.gett() == data.gett())
            break;

        stack.Push(parentNode);	//리밸런싱을 위해 저장
        parentNode = deleteNode;

        double del_key = v1.distance(deleteNode->data.crossing(E, true)->gets());

        if (del_key + ERR < key)
        {
            isLeft = false;
            deleteNode = deleteNode->right;
        }
        else if (del_key > key + ERR)
        {
            isLeft = true;
            deleteNode = deleteNode->left;
        }
        else {
            double rkey = v1.distance(data.crossing(ER, true)->gets());
            double del_rkey = v1.distance(deleteNode->data.crossing(ER, true)->gets());
            if (del_rkey < rkey)
            {
                isLeft = false;
                deleteNode = deleteNode->right;
            }
            else if (del_rkey > rkey)
            {
                isLeft = true;
                deleteNode = deleteNode->left;
            }
        }
    }

    if (deleteNode == nullptr)	//지울 노드를 못찾았다면
    {
        return false;
    }
    else if (deleteNode->data.gets() == data.gets() && deleteNode->data.gett() == data.gett())	//지울 노드를 찾았다면
    {
        Node* currentParentNode = nullptr;
        Node* currentNode = nullptr;
        if (deleteNode->left != nullptr && deleteNode->right != nullptr)	//지울 노드가 자식 둘 있는 노드
        {
            currentNode = deleteNode->right;
            if (currentNode->left != nullptr)				//오른쪽 노드가 제일 작은 값이 아닌 경우
            {
                while (currentNode->left != nullptr)
                {
                    currentParentNode = currentNode;
                    currentNode = currentNode->left;	//오른쪽 노드에서 가장 작은 값(가장 왼쪽 노드)을 찾는다.
                }
                currentParentNode->left = currentNode->right;	//작은 노드의 오른쪽 값을 작은노드 부모 노드가 받는다.
                currentNode->right = deleteNode->right;			//지울 노드의 오른쪽을 받는다.
            }

            currentNode->left = deleteNode->left;	//지울 노드의 왼쪽을 받는다.
        }
        else		//지울노드가 하나 혹은 없는 경우
        {
            //왼쪽 자식이 있으면 왼쪽, 오른쪽 자식이 있거나 자식이 하나도 없으면 오른쪽
            //있으면 (왼쪽 혹은 오른쪽)자식 노드가 들어가고 없으면 (자식 오른쪽에 있던)nullptr 값이 들어간다.
            currentNode = deleteNode->left != nullptr ? deleteNode->left : deleteNode->right;
        }

        isLeft == true ? parentNode->left = currentNode : parentNode->right = currentNode; //찾은 값을 지울 노드 위치로

        delete deleteNode;

        Rebalance();

        return true;
    }
    return false;
}

void AVLTreeList::Rebalance()
{
    Node* balanceCheckNode = nullptr;
    while (stack.Pop(&balanceCheckNode))	//스택을 돌면서 
    {
        RebalancingPivot(balanceCheckNode);
    }
}

void AVLTreeList::RebalancingPivot(Node* node)
{
    Node* rotatedNode = nullptr;
    if (RebalancingNode(node->left, &rotatedNode))
    {
        node->left = rotatedNode;
    }
    else if (RebalancingNode(node->right, &rotatedNode))
    {
        node->right = rotatedNode;
    }
}

bool AVLTreeList::RebalancingNode(Node* node, Node** rotatedNode)
{
    if (node == nullptr)
        return false;

    int rootDif = GetHeight(node->left) - GetHeight(node->right);	//균형도를 검사한다.
    int childDif;

    if (rootDif >= 2)	//왼쪽이 더 많다.
    {
        childDif = GetHeight(node->left->left) - GetHeight(node->left->right);	//LL 회전인지 LR 회전인지 판단
        Rotate(node, childDif >= 1 ? LL : LR, rotatedNode);	//회전
    }
    else if (rootDif <= -2)	//오른쪽이 더 많다.
    {
        childDif = GetHeight(node->right->left) - GetHeight(node->right->right);	//RR 회전인지 RL 회전인지 판단
        Rotate(node, childDif <= -1 ? RR : RL, rotatedNode);	//회전
    }
    else				//균형도가 맞는다.
        return false;
    return true;
}

int AVLTreeList::GetHeight(Node* node)	//노드의 높이를 계산하는 함수
{
    if (node == nullptr)
        return 0;

    int left = GetHeight(node->left);
    int right = GetHeight(node->right);

    return (left > right ? left : right) + 1;
}

bool AVLTreeList::Rotate(Node* node, RotateDir dir, Node** rotatedNode)
{
    if (node == nullptr)
        return false;

    Node* childNode = nullptr;
    Node* grandChildNode = nullptr;

    switch (dir)
    {
    case LL:
        //node 기준 L 회전
        childNode = node->left;
        node->left = childNode->right;
        childNode->right = node;
        break;
    case RR:
        //node 기준 R 회전
        childNode = node->right;
        node->right = childNode->left;
        childNode->left = node;
        break;
    case LR:		//LR 의 경우 자식노드를 RR 회전통해 LL 상태로 만들어야한다.
        childNode = node->left;

        //node->right 기준 R 회전
        grandChildNode = childNode->right;
        childNode->right = grandChildNode->left;
        grandChildNode->left = childNode;
        node->left = grandChildNode;

        //node 기준 L 회전
        childNode = node->left;
        node->left = childNode->right;
        childNode->right = node;
        break;
    case RL:		//RL 의 경우 자식노드를 LL 회전통해 RR 상태로 만들어야한다.
        childNode = node->right;

        //node->left 기준 L 회전
        grandChildNode = childNode->left;
        childNode->left = grandChildNode->right;
        grandChildNode->right = childNode;
        node->right = grandChildNode;

        //node 기준 R 회전
        childNode = node->right;
        node->right = childNode->left;
        childNode->left = node;
        break;
    default:
        return false;
    }

    *rotatedNode = childNode;	//회전한 노드 (부모노드와 연결이 안된상태)
    return true;
}