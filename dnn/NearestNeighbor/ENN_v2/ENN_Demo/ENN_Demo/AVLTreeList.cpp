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
    Node* parentNode = &root;	//�����尡 ��ó�� ��� �ϰ�츦 ����ؼ� ������ ��Ʈ�� �ִ�.
    Node* deleteNode = root.left;
    bool isLeft = true;
    Point x(v1.getx() + 1 / EPS, v1.gety());
    Edge E(v1, x);
    bool right = true;
    if (data.crossing(E, true) == nullptr) return false;

    double key = v1.distance(data.crossing(E, true)->gets());

    while (deleteNode != nullptr)	//���� ��带 ã�� ����
    {
        if (deleteNode->data.gets() == data.gets() && deleteNode->data.gett() == data.gett())
            break;

        stack.Push(parentNode);	//���뷱���� ���� ����
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

    if (deleteNode == nullptr)	//��带 ��ã�Ҵٸ�
    {
        return false;
    }
    else return true; // ��带 ã�Ҵٸ�
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

    while (currentNode != nullptr)		//(���� Ȥ�� ������ �ڽ��� ����ִ�)�θ� ��带 ã�� ����
    {

        double cur_key = v1.distance(currentNode->data.crossing(E, true)->gets());
        if (currentNode->data.gets() == data.gets() && currentNode->data.gett() == data.gett())
            return false;

        stack.Push(parentNode);	//���뷱���� ���� ����
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
    currentNode = new Node;		//���ο� ��带 �����
    currentNode->data = data;
    currentNode->left = nullptr;
    currentNode->right = nullptr;

    isLeft == true ? parentNode->left = currentNode : parentNode->right = currentNode;

    Rebalance();

    return true;
}

bool AVLTreeList::Delete(Edge data, Point v1, Point v2)
{
    Node* parentNode = &root;	//�����尡 ��ó�� ��� �ϰ�츦 ����ؼ� ������ ��Ʈ�� �ִ�.
    Node* deleteNode = root.left;
    bool isLeft = true;

    double x = v2.getx() - v1.getx();
    double y = v2.gety() - v1.gety();
    Point V(v1.getx() + x / EPS, v1.gety() + y / EPS);
    Point VR(v1.getx() + (x * cos(-ERR*10) - y * sin(-ERR*10)) / EPS, v1.gety() + (x * sin(-ERR*10) + y * cos(-ERR*10)) / EPS);
    Edge E(v1, V);
    Edge ER(v1, VR);
    double key = v1.distance(data.crossing(E, true)->gets());

    while (deleteNode != nullptr)	//���� ��带 ã�� ����
    {
        if (deleteNode->data.gets() == data.gets() && deleteNode->data.gett() == data.gett())
            break;

        stack.Push(parentNode);	//���뷱���� ���� ����
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

    if (deleteNode == nullptr)	//���� ��带 ��ã�Ҵٸ�
    {
        return false;
    }
    else if (deleteNode->data.gets() == data.gets() && deleteNode->data.gett() == data.gett())	//���� ��带 ã�Ҵٸ�
    {
        Node* currentParentNode = nullptr;
        Node* currentNode = nullptr;
        if (deleteNode->left != nullptr && deleteNode->right != nullptr)	//���� ��尡 �ڽ� �� �ִ� ���
        {
            currentNode = deleteNode->right;
            if (currentNode->left != nullptr)				//������ ��尡 ���� ���� ���� �ƴ� ���
            {
                while (currentNode->left != nullptr)
                {
                    currentParentNode = currentNode;
                    currentNode = currentNode->left;	//������ ��忡�� ���� ���� ��(���� ���� ���)�� ã�´�.
                }
                currentParentNode->left = currentNode->right;	//���� ����� ������ ���� ������� �θ� ��尡 �޴´�.
                currentNode->right = deleteNode->right;			//���� ����� �������� �޴´�.
            }

            currentNode->left = deleteNode->left;	//���� ����� ������ �޴´�.
        }
        else		//�����尡 �ϳ� Ȥ�� ���� ���
        {
            //���� �ڽ��� ������ ����, ������ �ڽ��� �ְų� �ڽ��� �ϳ��� ������ ������
            //������ (���� Ȥ�� ������)�ڽ� ��尡 ���� ������ (�ڽ� �����ʿ� �ִ�)nullptr ���� ����.
            currentNode = deleteNode->left != nullptr ? deleteNode->left : deleteNode->right;
        }

        isLeft == true ? parentNode->left = currentNode : parentNode->right = currentNode; //ã�� ���� ���� ��� ��ġ��

        delete deleteNode;

        Rebalance();

        return true;
    }
    return false;
}

void AVLTreeList::Rebalance()
{
    Node* balanceCheckNode = nullptr;
    while (stack.Pop(&balanceCheckNode))	//������ ���鼭 
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

    int rootDif = GetHeight(node->left) - GetHeight(node->right);	//�������� �˻��Ѵ�.
    int childDif;

    if (rootDif >= 2)	//������ �� ����.
    {
        childDif = GetHeight(node->left->left) - GetHeight(node->left->right);	//LL ȸ������ LR ȸ������ �Ǵ�
        Rotate(node, childDif >= 1 ? LL : LR, rotatedNode);	//ȸ��
    }
    else if (rootDif <= -2)	//�������� �� ����.
    {
        childDif = GetHeight(node->right->left) - GetHeight(node->right->right);	//RR ȸ������ RL ȸ������ �Ǵ�
        Rotate(node, childDif <= -1 ? RR : RL, rotatedNode);	//ȸ��
    }
    else				//�������� �´´�.
        return false;
    return true;
}

int AVLTreeList::GetHeight(Node* node)	//����� ���̸� ����ϴ� �Լ�
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
        //node ���� L ȸ��
        childNode = node->left;
        node->left = childNode->right;
        childNode->right = node;
        break;
    case RR:
        //node ���� R ȸ��
        childNode = node->right;
        node->right = childNode->left;
        childNode->left = node;
        break;
    case LR:		//LR �� ��� �ڽĳ�带 RR ȸ������ LL ���·� �������Ѵ�.
        childNode = node->left;

        //node->right ���� R ȸ��
        grandChildNode = childNode->right;
        childNode->right = grandChildNode->left;
        grandChildNode->left = childNode;
        node->left = grandChildNode;

        //node ���� L ȸ��
        childNode = node->left;
        node->left = childNode->right;
        childNode->right = node;
        break;
    case RL:		//RL �� ��� �ڽĳ�带 LL ȸ������ RR ���·� �������Ѵ�.
        childNode = node->right;

        //node->left ���� L ȸ��
        grandChildNode = childNode->left;
        childNode->left = grandChildNode->right;
        grandChildNode->right = childNode;
        node->right = grandChildNode;

        //node ���� R ȸ��
        childNode = node->right;
        node->right = childNode->left;
        childNode->left = node;
        break;
    default:
        return false;
    }

    *rotatedNode = childNode;	//ȸ���� ��� (�θ���� ������ �ȵȻ���)
    return true;
}