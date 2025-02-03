#include <queue>
#include <random>
#include <map>

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

	return ret;
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

void kDQuadTreeNode::updateNumNodesSubtree() {
	kDQuadTreeNode* nowNode = this;

	while (true) {
		nowNode->numNodesSubtree++;
		
		// if (nowNode->isRoot) break;
		if (nowNode->parent == nullptr) break;
		else nowNode = nowNode->parent;
	}
}

Node* kDQuadTree::build(vector<Point*> _points, int _dim, vector<pair<double, double>> _boundingBox, double _eps, int _depth, kDQuadTreeNode* parent) { // vector<Polytope*>

	cout << "current depth: " << _depth << ", # points:" << _points.size() << endl;

	int maxDepth = 4;

	// debug
	// if (_points.empty()) { cout << "point set empty. return" << endl; return new kDQuadTreeLeafNode({}); }
	// // else if (_depth == maxDepth) { cout << "reached maximum depth of " << _depth << " achieved. return" << endl; return nullptr; }
	// else if (_depth == maxDepth) { cout << "reached maximum depth of " << _depth << " achieved. return" << endl; return new kDQuadTreeLeafNode(_points); }
	// else if (_points.size() == 1) { cout << "Single point. return" << endl; return new kDQuadTreeLeafNode(_points); }

	// 포인트가 비어 있으면 리프 노드 반환
	if (_points.empty()) {
		// cout << "point set empty. return" << endl;
		kDQuadTreeNode* leafNode = new kDQuadTreeNode(vector<Point*>{}, parent);
		leafNode->isLeaf = true;
		leafNode->boundingBox = _boundingBox;

		leafNode->updateNumNodesSubtree();

		return leafNode;
	}

	// 최대 깊이 도달
	else if (_depth == maxDepth) {
		// cout << "Reached max depth. return" << endl;
		kDQuadTreeNode* leafNode = new kDQuadTreeNode(_points, parent);
		leafNode->isLeaf = true;
		leafNode->boundingBox = _boundingBox;

		leafNode->updateNumNodesSubtree();

		return leafNode;
	}

	// 포인트가 하나만 있으면 리프 노드 반환
	if (_points.size() == 1) {
		// cout << "Single point. return" << endl;
		kDQuadTreeNode* leafNode = new kDQuadTreeNode(_points, parent);
		leafNode->isLeaf = true;
		leafNode->boundingBox = _boundingBox;

		leafNode->updateNumNodesSubtree();

		return leafNode;
	}

	kDQuadTreeNode* internalNode = new kDQuadTreeNode;
	internalNode->parent = parent;

	vector<Node*> childNodes;

	// child nodes 생성
	int powerNum = 1;
	for (int j = 0; j < _dim; j++) { powerNum *= 2; }

	for (int j = 0; j < powerNum; j++) {
		Node* newNode = new Node();

		vector<int> nowBinary = dec2bin(powerNum, j);

		vector<Point*> nowCellPoints;
		for (auto p : _points) if (isContained(p, _boundingBox, nowBinary)) nowCellPoints.push_back(p);

		vector<pair<double, double>> newBoundingBox;

		for (int axis = 0; axis < _dim; axis++) {

			double axisMin = _boundingBox[axis].first;
			double axisMax = _boundingBox[axis].second;
			double axisMid = (axisMin + axisMax) / 2;

			// 0 for the lower part, 1 for the upper part
			if (nowBinary[axis] == 0) newBoundingBox.push_back(make_pair(axisMin, axisMid));
			else newBoundingBox.push_back(make_pair(axisMid, axisMax));

		}

		// 재귀적으로 child node 생성
		childNodes.push_back(build(nowCellPoints, _dim, newBoundingBox, _eps, _depth + 1, internalNode));
	}

	// kDQuadTreeNode* internalNode = new kDQuadTreeNode(childNodes, parent);
	internalNode->childNodes = childNodes;
	// for (auto& n : childNodes) n->parent = internalNode;

	internalNode->isLeaf = false;
	internalNode->boundingBox = _boundingBox;
	// if (parent == nullptr) internalNode->isRoot = true;

	return internalNode;
	// return new Node(childNodes);
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

	vector<CPolytope*> pols;

	auto qT = new kDQuadTree(points, pols, 4, boundingBox, eps);

	// debug
	exit(1);

}

// point location 함수 구현 - 컴파일 잘됨!
// 용도: 한 Point에 대해, 해당 Point가 quadtree의 어느 cell(region)에 위치하는지 찾고 반환
// input: Point 형식의 한 점
// output: input point가 위치하는 leafnode 반환

Node* pointLocation(Node* node, Point* point) {
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
            if (isContained(point, node->boundingBox, binary)) {
                return pointLocation(child, point); // 재귀적으로 탐색
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

Node* addPoint(Node* node, Point* point, int maxDepth) {
	// 예외) node나 point가 nullptr인 경우
    if (node == nullptr || point == nullptr) {
        return nullptr;
    }

    // pointLocation을 사용하여 target leafnode 찾기
    Node* targetNode = pointLocation(node, point);

	// 예외) leafnode를 찾을 수 없는 경우 (pointLocation 결과가 nullptr인 경우)
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
	double depth = myLog2(128/cubeLen);
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

Node* deletePoint(Node* node, Point* point) {
	// 예외) node나 point가 nullptr인 경우
    if (node == nullptr || point == nullptr) {
        return node;
    }

    // pointLocation을 사용하여 target leafnode 찾기
    Node* targetNode = pointLocation(node, point);

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

Point* generateRandomPoint(int dim, vector<pair<double, double>> boundingBox) {
	random_device rd;
	mt19937 mt(rd());

	vector<double> xs;

	for (auto& interval : boundingBox) {
		uniform_real_distribution<double> uni(interval.first, interval.second);
		xs.push_back(uni(mt));
	}
	// uniform_real_distribution(interval)  

	return new Point(xs);
}

Point* generateRandomPoint(int dim, pair<double, double> boundingBox) {
	vector<pair<double, double>> fullBoundingBox;

	for (int i = 0; i < dim; i++) fullBoundingBox.push_back(boundingBox);

	return generateRandomPoint(dim, fullBoundingBox);
}

// 현재는 cell 내부에 균일하게 뿌리기. vs 경계(cell edge)에다 뿌리기?
void spreadPoints(Node* node, int dim, int numPoints) {

	int nowNum = 0;
	while (nowNum < numPoints) {
		Point* p = generateRandomPoint(dim, node->boundingBox);

		// false 대신, "p가 input convex polytope 중 하나에 포함될 경우"로 수정 
		if (false) continue;
		else {node->spreadPoints.push_back(p); nowNum += 1;}
	}

}

// spreadPoints

void constructLocalGraph(Node* root, int dim) {
	
	// 큐가 빌 때까지
	queue<Node*> Q;
	while (!Q.empty()) {
		Node* nowNode = Q.front();
		Q.pop();

		// 현재 노드에 local graph 점 추가
		spreadPoints(nowNode, dim);

		// 자식 노드 push
		for (auto& child : nowNode->childNodes) Q.push(child);

	}

	while (!Q.empty()) {
		Node* nowNode = Q.front();
		Q.pop();

		// 인접한 quadtree cell pair (c1, c2)에 해당하는 point set (P1, P2)
		// each p1 \in P1, p2 \in P2에 대해 p1과 p2를 연결
		for (auto& iNode : nowNode->incidentNodes) {
			for (auto& p1 : nowNode->spreadPoints) {
				for (auto& p2 : iNode->spreadPoints) {

					// true 대신 polytope 조건 필요
					if (true) {
						p1->neighbors.push_back(p2);
						// p1->neighbors.push_back(p1);
					}

				}
			}
		}

		// 자식 노드 push
		for (auto& child : nowNode->childNodes) Q.push(child);
	}

}

// returns { (n_1, dist(query,  n_1)), ..., (n_k, dist(query,  n_k)) }
vector<pair<double, Point*>> kDQuadTree::kNN(Point* query, int k) {
	
	vector<pair<double, Point*>> ret;

	Node* startNode = pointLocation(root, query);

	// 지금까지 찾은 neighbor 수
	int numFind = 0;

	// Dijkstra on the local graph
	priority_queue<pair<double, Point*>> pq;

	map<Point*, double> dist;
	map<Point*, bool> visited;

	visited[query] = false;
	dist[query] = 0;

	// startNode의 각 spreadPoint에서 쿼리 점까지 자동으로 연결
	for (auto p : startNode->spreadPoints) {
		pq.push(make_pair(distance(query, p), p));
		visited[p] = false;
		dist[query] = distance(query, p);
	}

	// point location의 결과 노드 v뿐만이 아니라,
	// v + (v의 인접 노드)까지 모두 고려해야 하나?

	while (!pq.empty()) {
		auto& nowPair = pq.top();
		pq.pop();
		
		ret.push_back(nowPair);
		numFind += 1;

		if (numFind == k) return ret;

		auto& nowPoint = nowPair.second;
		for (auto n : nowPoint->neighbors) {
			
			// 대신, 노드 n을 이미 방문한 경우는 continue로 넘어감

			if (visited.contains(n)) continue;

			// 노드 n을 방문하지는 않았지만, 거리는 계산된 적이 있으면
			if (dist.contains(n)) {
				double newDist = dist[nowPoint] + distance(n, nowPoint);
				if (dist[n] > newDist) dist[n] = newDist;
			}
			// 거리 계산도 이루어지지 않았으면 map에 새로 추가
			else {
				dist[n] = dist[nowPoint] + distance(n, nowPoint);
			}

			// 위의 if-else 두 경우 모두에서, 큐에는 n까지의 최단거리 candidate을 넣음.
			pq.push(make_pair(dist[n], n));

		}
	}

}

void buildPointGraphOnQuadTree(kDQuadTree* quadtree) {
	std::vector<Node*> leafs;
	double EPS = 0.001;

	auto& pols = quadtree->pols;

	// std::cout << "buildPointGraphOnQuadTree Start\n";

	std::queue<Node*> queue;
	queue.push(quadtree->root);
	while (!queue.empty()) {
		Node* cur = queue.front();
		queue.pop();

		for (Node* child : cur->childNodes) {
			queue.push(child);
		}

		if (cur->isLeaf) {
			leafs.push_back(cur);
		}
	}

	for (Node* leaf : leafs) {
		std::vector<Node*> adjacentNodes;
		vector<pair<double, double>> box = leaf->boundingBox;
		double radius = std::abs((box[0].second - box[0].first) / 2.0);
		std::vector<double> center;

		// std::cout << "box\n";
		for (std::pair<double, double> interval : box) {
			center.push_back((interval.first + interval.second) / 2.0);

			// std::cout << interval.first << ' ' << interval.second << '\n';
		}

		//std::cout << "center\n";
		//for (double x : center) {
		//	std::cout << x << ' ';
		//}
		//std::cout << '\n';

		for (int i = 0; i < box.size(); i++) {
			vector<double> pos1, pos2;
			pos1 = pos2 = center;
			pos1[i] += (radius + EPS);
			pos2[i] -= (radius + EPS);
			Point p1(pos1), p2(pos2);
			Node* adj1 = pointLocation(quadtree->root, &p1);
			Node* adj2 = pointLocation(quadtree->root, &p2);

			if (adj1 != nullptr) {
				adjacentNodes.push_back(adj1);
			}
			if (adj2 != nullptr) {
				adjacentNodes.push_back(adj2);
			}
		}


		for (Node* adj : adjacentNodes) {
			//std::cout << "adj" << '\n';
			//for (std::pair<double, double> interval : adj->boundingBox) {
			//	std::cout << interval.first << ' ' << interval.second << '\n';
			//}
			for (Point* p1 : leaf->points) {
				for (Point* p2 : adj->points) {
					for (auto& pol : pols) {
						if (!pol->is_intersect(p1, p2)) {

							p1->neighbors.push_back(p2);
							p2->neighbors.push_back(p1);

							/*
							if (p1->isPolytopeVertex && !(p2->isPolytopeVertex)) { // p1만 polytope vertex
							}
							else if (!(p1->isPolytopeVertex) && p2->isPolytopeVertex) { // p2만 polytope vertex
							}
							else if (!(p1->isPolytopeVertex) && !(p2->isPolytopeVertex)) { // 둘 다 polytope vertex가 아님
							}
							else { // 둘 다 polytope vertex임
								// if (isSameFacet(p1, p2)) { } // 같은 facet 위에 있는 경우만 연결?
							}
							*/
						}
					}
					
				}
			}
		}
	}
}

// 디버그용 함수
// node를 루트로 하는 그래프 G = (V, E)에 대하여, |V|와 |E|를 계산
// 점 많이 찍을수록, or incidency를 결정하는 radius가 커질수록 |E|가 달라지는 것 확인
void checkPointGraphSize(Node* node) {
	
}