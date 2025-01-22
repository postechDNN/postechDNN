#include "quadtree.h"

vector<int> dec2bin(int powerNum, int num) {
	vector<int> ret;

	int size = int(myLog2(double(powerNum)));
	for (int i = 0; i < size; i++) ret.push_back(0);
	// ret.assign(size, 0);
	// ret.assign(4, 0);

	int nowDigit = 0;
	while (num > 0) {
		ret[nowDigit] = num % 2;
		num = num / 2;
		nowDigit += 1;
	}

	reverse(ret.begin(), ret.end());

	return ret;
}

// need to test
int bin2dec(vector<int> bin) {
	int ret = 0;

	reverse(bin.begin(), bin.end());

	int factor = 1;

	for (int i = 0; i < bin.size(); i++) {
		ret += bin[i] * factor;
		factor *= 2;
	}
}

// minimum side length of the 
double minCellSize(vector<pair<double, double>>& _boundingBox) {
	double ret = DBL_MAX;
	for (auto slab : _boundingBox) {
		if (slab.second - slab.first < DBL_MAX)
			ret = slab.second - slab.first;
	}

	return ret;
}

bool isContained(Point* p, vector<pair<double, double>> boundingBox, vector<int> binary) {

	for (int axis = 0; axis < binary.size(); axis++) {
		double axisMin = boundingBox[axis].first;
		double axisMax = boundingBox[axis].second;
		double axisMid = (axisMin + axisMax) / 2;

		double axisVal = p->xs[axis];

		if (binary[axis] == 0 && !(axisMin <= axisVal && axisVal <= axisMid)) return false;
		else if (binary[axis] == 1 && !(axisMid < axisVal && axisVal <= axisMax)) return false;
	}

	return true;

}

double myLog2(double num) {
	return log(num) / log(2.0);
}

void buildEpsilonGraph() {
	// debug: check if some nodes are of depth 2, while others are of depth 1

	int dim = 4;
	double val = 128.0;
	// double middleVal = val / 2.0;
	double middleVal = 70.0;

	int pointNum = 1;
	for (int i = 0; i < dim; i++) pointNum *= 2; // 2 cases for each axis

	vector<Point*> points;
	for (int i = 0; i < pointNum; i++) points.push_back(new Point(dim, true));

	for (auto i = 0; i < pointNum; i++) {
		if (i == 0) continue;

		vector<int> nowBinary = dec2bin(pointNum, i);
		for (int j = 0; j < nowBinary.size(); j++) {
			if (nowBinary[j] == 0) points[i]->xs[j] = -1 * middleVal;
			else points[i]->xs[j] = middleVal;
		}
	}

	vector<pair<double, double >> boundingBox;
	for (int i = 0; i < dim; i++) boundingBox.push_back(make_pair(-val, val));

	double eps = 64;

	auto qT = new kDQuadTree(points, 4, boundingBox, eps);

	exit(1);

}

// point location 함수 구현 - 컴파일 잘됨!
// 용도: 한 Point에 대해, 해당 Point가 quadtree의 어느 cell(region)에 위치하는지 찾고 반환
// input: Point 형식의 한 점
// output: input point가 위치하는 leafnode 반환

Node* PointLocation(Node* node, Point* point) {
    if (node == nullptr) {
        return nullptr; // 노드가 null인 경우 null 반환
    }

    // 현재 노드가 리프 노드인 경우, 해당 노드를 반환
    if (node->isLeaf) {
        return node;
    }

    // Internal Node인 경우 자식 노드를 탐색
    for (int i = 0; i < node->childNodes.size(); i++) {
        Node* child = node->childNodes[i];
        if (child != nullptr) {
            // 자식 노드의 인덱스를 binary로 변환
            vector<int> binary = dec2bin(node->childNodes.size(), i);

            // 현재 점이 자식 노드의 영역에 포함되는지 확인
            if (isContained(point, child->boundingBox, binary)) {
                return PointLocation(child, point); // 재귀적으로 탐색
            }
        }
    }

    // 예외: 점이 어떤 자식 노드에도 포함되지 않는 경우 null 반환
    return nullptr;
}



// point 추가 함수 구현
// 용도: quadtree에 Point 하나를 추가하고 바뀐 quadtree 반환
// input: Point 형식의 한 점
// output: Point 한 점을 추가한 quadtree

Node* PointAdd(Node* node, Point* point, int maxDepth) {
	// 예외) node나 point가 nullptr인 경우
    if (node == nullptr || point == nullptr) {
        return nullptr;
    }

    // PointLocation을 사용하여 target leafnode 찾기
    Node* targetNode = PointLocation(node, point);

	// 예외) leafnode를 찾을 수 없는 경우 (PointLocation 결과가 nullptr인 경우)
	// 새로 leafnode 만들기
	// 근데 targetNode == nullptr일 경우가 거의 없을 듯?
    if (targetNode == nullptr) {
        vector<pair<double, double>> boundingBox = node->boundingBox;
        targetNode = new kDQuadTreeNode(vector<Point*>{point});
        targetNode->boundingBox = boundingBox;
        targetNode->isLeaf = true;
		targetNode->parent = node;
        node->childNodes.push_back(targetNode);
        return node;
    }

    // 1. point 삽입 위치에 leafnode가 empty
    if (targetNode->points.empty()) {
        targetNode->points.push_back(point);
        return node;
    }

    // 2. point 삽입 위치에 leafnode가 이미 뭔가 있음
    targetNode->points.push_back(point);

	// 2-1. targetNode depth가 max depth에 해당하는 경우
	// targetNode에 point push만 하기 (이미 윗줄에서 완료)
	// 기본 박스는 128 size
	// boundingbox 한 변 길이 확인해서 depth 계산할 수 있음
	double cubeLen = abs(targetNode->boundingBox[0].second - targetNode->boundingBox[0].first);
	double depth = (128 / cubeLen) - 1;
	if (depth == maxDepth) {
		return node;
	}

    // 2-2. max depth에 해당하지 않아서 targetNode 분할
    if (targetNode->points.size() > 1) {
        vector<Node*> childNodes;

		int dim = targetNode->boundingBox.size();
		int powerNum = 1 << dim; // 하나의 node에 대해 생성되어야 하는 child node 개수 (dimension에 따라 달라지니까)

		// child node를 하나씩 만들어나가는 과정
        for (int i = 0; i < powerNum; i++) {
            vector<int> binary = dec2bin(powerNum, i);
            vector<pair<double, double>> newBoundingBox;

			// child node의 boundingbox 설정
            for (int axis = 0; axis < dim; axis++) {
                double axisMin = targetNode->boundingBox[axis].first;
                double axisMax = targetNode->boundingBox[axis].second;
                double axisMid = (axisMin + axisMax) / 2;

                if (binary[axis] == 0) {
                    newBoundingBox.push_back(make_pair(axisMin, axisMid));
                } else {
                    newBoundingBox.push_back(make_pair(axisMid, axisMax));
                }
            }

			// child node의 points 설정
            vector<Point*> newCellPoints;
            for (Point* p : targetNode->points) {
                if (isContained(p, newBoundingBox, binary)) {
                    newCellPoints.push_back(p);
                }
            }

            Node* childNode = new kDQuadTreeNode(newCellPoints);
            childNode->boundingBox = newBoundingBox;
            childNode->isLeaf = true;
			childNode->parent = targetNode;
            childNodes.push_back(childNode);
        }
		
		// targetNode가 기존에 leafnode였지만, child node가 생기면서 internal node로 바뀜
        targetNode->childNodes = childNodes;
        targetNode->isLeaf = false;
        targetNode->points.clear();
    }

    return node;
}



// point 삭제 함수 구현
// 용도: quadtree에 Point 하나를 삭제하고 바뀐 quadtree 반환
// input: Point 형식의 한 점
// output: Point 한 점을 삭제한 quadtree

Node* PointDel(Node* node, Point* point) {
	// 예외) node나 point가 nullptr인 경우
    if (node == nullptr || point == nullptr) {
        return node;
    }

    // PointLocation을 사용하여 target leafnode 찾기
    Node* targetNode = PointLocation(node, point);

	// 예외) targetNode가 nullptr인 경우
    if (targetNode == nullptr) {
        return node;
    }

    // targetNode의 points에서 삭제해야할 점 찾아서 삭제
    auto it = std::find(targetNode->points.begin(), targetNode->points.end(), point);
    if (it != targetNode->points.end()) {
        targetNode->points.erase(it);
    }

    if (targetNode->points.empty()) {
        Node* parent = targetNode->parent;

        if (parent != nullptr) {
            // 부모 노드의 모든 자식 노드가 비어있는지 확인
            bool allEmpty = true;
            for (Node* sibling : parent->childNodes) {
                if (sibling != nullptr && !sibling->points.empty()) {
                    allEmpty = false;
                    break;
                }
            }

			// 같은 parent를 가진 child들이 모두 비어있는 경우
			// childnode 모두 없애고 parent를 leafnode로 변경
			if (allEmpty) {
				for (Node*& sibling : parent->childNodes) {
                    delete sibling;
                    sibling = nullptr;
                }
                parent->childNodes.clear();
                parent->isLeaf = true;
				parent->points = {};
			}
        }
    }

    return node;
}