#define _CRT_SECURE_NO_WARNINGS

#include "../algorithm/errorTest.h"
// #include <Eigen/Dense>
#include "../Polytope.h"
#include "fstream"
#include "quadtree.h"
#include <ctime>
// #include "filesystem"

using namespace std;

Point* translate(Point* p, int axis, double val) {
	assert(axis < p->xs.size());
	auto ret = new Point(p);
	p->xs[axis] += val;
	return ret;
}

void visualizeTXT(EpsGraphNd* qT, vector<CPolytope*> Ctopes, vector<pair<int, int>> edge_list, vector<Point*> queries) {
	string outputFileName = "quadtree.txt";
	ofstream outputTXT(outputFileName);
	outputTXT.clear();

	std::queue<Node*> q;
	q.push(qT->root);
	if (outputTXT.is_open()) {
		while (!q.empty()) {
			Node* cur = q.front();
			q.pop();

			// std::cout << "size: " << (cur->boundingBox).size() << '\n';

			outputTXT << "b\n";
			for (pair<double, double> p : cur->boundingBox) {
				outputTXT << p.first << " " << p.second << '\n';
			}

			if (cur->isLeaf) {
				outputTXT << "p " << (cur->points).size() << '\n';
				for (Point* p : cur->points) {
					for (double x : p->getxs()) {
						outputTXT << x << ' ';
					}
					outputTXT << p->nowIndex << ' ';

					if (p->isExtraPoint || p->isPolytopeVertex) outputTXT << 'x';
					else outputTXT << 'o';

					outputTXT << '\n';
				}

			}
			else {
				for (Node* child : cur->childNodes) {
					q.push(child);
				}
			}
		}
	}
	else {
		std::cout << "output.txt error!\n";
	}

	for (auto& edge : edge_list) {
		outputTXT << "e" << "\n";
		outputTXT << edge.first << " " << edge.second << "\n";
	}

	for (auto& tope : Ctopes) {
		outputTXT << "t" << " " << tope->vertices.size() << "\n";
		for (auto& ver : tope->vertices) {
			for (auto& x : ver.xs) {
				outputTXT << x << " ";
			}
			outputTXT << "\n";
		}

	}

	/*
		for (auto q : q_pts) {

		for (auto k : ks) {
			for (auto& x : q->xs)
				outputTXT << x << ' ';

			auto ret = qT->kNN(q, k, false);

			for (int jj = 0; jj < ret.size(); jj++) { // pr : ret) {
				auto pr = ret[jj]; // (dist, neighbor)
				// cout << k << "-NN result" << endl;
				// cout << jj << "-th neighbor index: " << pr.second->nowIndex << ", graph distance: " << pr.first << endl;
			}

			outputTXT << k << ' ' << ret.size() << "\n";
			for (auto [dist, kp] : ret) {
				outputTXT << kp->nowIndex << ' ' << dist << ' ' << kp->getx(0) << ' ' << kp->getx(1) << "\n";
			}
		}

	}
	*/

	outputTXT << "q" << " ";
	for (auto& x : queries[0]->xs)
		outputTXT << x << ' ';

	auto ret = qT->kNN(queries[0], 10, true);

	for (int jj = 0; jj < ret.size(); jj++) { // pr : ret) {
		auto pr = ret[jj]; // (dist, neighbor)
		// cout << k << "-NN result" << endl;
		// cout << jj << "-th neighbor index: " << pr.second->nowIndex << ", graph distance: " << pr.first << endl;
	}

	outputTXT << 10 << ' ' << ret.size() << "\n";
	for (auto [dist, kp] : ret) {
		outputTXT << kp->nowIndex << ' ' << dist << ' ' << kp->getx(0) << ' ' << kp->getx(1) << "\n";
	}

}

void visualize(std::string dir) {

	int dim = 2;

	std::string pointsDir = "C:\\epsGraphData\\000\\points\\points.txt";
	std::string queryDir = "C:\\epsGraphData\\000\\points\\queries.txt";

	std::vector<Point*> pts = makePointSet(pointsDir); // input sites
	std::vector<Point*> q_pts = makePointSet(queryDir); // query points

	namespace fs = std::filesystem;
	fs::path topeDir = "C:\\epsGraphData\\000\\polytopes";
	fs::directory_iterator iterTopes(topeDir);

	std::vector<CPolytope*> Ctopes;

	for (auto& i01 = iterTopes; i01 != fs::end(iterTopes); ++i01) {
		fs::path topeDir = (*i01).path();
		Ctopes.push_back(dels2cpolytope(topeDir.string(), dim, true));
	}

	double maxValue = 128.0;
	vector<pair<double, double >> boundingBox;
	for (int i = 0; i < dim; i++) boundingBox.push_back(make_pair(-maxValue, maxValue));

	int maxDepth = 10;
	auto qT = new EpsGraphNd(pts, Ctopes, dim, boundingBox, maxDepth);

	fillEmptyCells(dim, qT);
	auto edge_list = buildPointGraphOnQuadTree(qT);

	visualizeTXT(qT, Ctopes, edge_list, q_pts);

	// void visualizeTXT(EpsGraphNd* qT, vector<CPolytope*> Ctopes, vector<pair<int, int>> edge_list, vector<Point*> queries)

	// qT->kNN(q_pts[0], 10, true);

}

// ex) dir = ""
void querySpeedTest(std::string dir, int startID, int endID, int numPoints, int numQueries) {

	int dim = 4;
	// int numQueries = 1000;
	int k = 10;
	int epsilon = 1.0;
	int maxDepth = INT_MAX; // user-defined

	// bounding box
	double maxValue = 128.0;
	vector<pair<double, double >> boundingBox;
	for (int i = 0; i < dim; i++) boundingBox.push_back(make_pair(-maxValue, maxValue));

	// uniform 4개 dataset
	vector<double> uniform;
	uniform.assign(4, 0.0);

	// clustered 4개 dataset
	vector<double> clustered;
	clustered.assign(4, 0.0);

	namespace fs = std::filesystem;
	fs::path dataDirDir = dir + "_Speed";
	fs::directory_iterator iterDirDir(dataDirDir);

	int curDataSetID = -1;

	// int startID = 70;
	// int endID = 79;

	string outputFileName = "speed_" + to_string(startID) + "_" + to_string(endID) + "_" + to_string(numPoints) + "_" + to_string(numQueries) + ".txt";
	ofstream outputTXT(outputFileName);
	outputTXT.clear();

	if (0 <= startID && endID <= 9 || 20 <= startID && endID <= 29 || 40 <= startID && endID <= 49 || 60 <= startID && endID <= 69) {
		cout << "uniform\n\n";  outputTXT << "uniform\n\n";
	}
	if (10 <= startID && endID <= 19 || 30 <= startID && endID <= 39 || 50 <= startID && endID <= 59 || 70 <= startID && endID <= 79) {
		cout << "cluster\n\n";  outputTXT << "cluster\n\n";
	}

	for (auto& i00 = iterDirDir; i00 != fs::end(iterDirDir); ++i00) {

		curDataSetID += 1;

		if (startID <= curDataSetID && curDataSetID <= endID) {
			cout << "dataset #" << curDataSetID << " start\n";
			outputTXT << "dataset #" << curDataSetID << " start\n";
		}
		else { continue; }
		// else { cout << "dataset #" << curDataSetID << " skipped\n"; continue; }

		// if (startID <= curDataSetId && curDataSetId <= endID) 
		// if (startID > curDataSetID || curDataSetID > endID) continue;

		fs::path dataDir = (*i00).path();

		// ********************************************************** read CPolytope start
		fs::path polytopesDir(dataDir.string() + "\\polytopes");
		fs::directory_iterator iterTopes(polytopesDir);

		std::vector<CPolytope*> Ctopes;

		for (auto& i01 = iterTopes; i01 != fs::end(iterTopes); ++i01) {
			fs::path topeDir = (*i01).path();
			Ctopes.push_back(dels2cpolytope(topeDir.string(), dim, true));
		}
		// ********************************************************** read CPolytope end

		std::string pointsDir = dataDir.string() + "\\points\\points.txt"; // for input sites
		std::vector<Point*> pts = makePointSet(pointsDir);

		// int numPoints = pts.size();
		// pts = vector<Point*>(pts.begin(), pts.begin() + 1000);

		std::string queryDir = dataDir.string() + "\\points\\queries.txt"; // for queries
		std::vector<Point*> q_pts = makePointSet(queryDir); // query points

		// ********************************************************** read (input) points end

		// kDQuadTree(vector<Point*> _points, vector<CPolytope*> _pols, int _dim, 
		// vector<pair<double, double >> _boundingBox, int _maxDepth, double _eps = 1.0)
		auto qT = new EpsGraphNd(pts, Ctopes, dim, boundingBox, maxDepth);

		int numExtraPoints = double(1) / epsilon * pts.size();

		for (int _ = 0; _ < numExtraPoints; _++) {
			Point* p = generateRandomPoint(dim, boundingBox);
			p->isExtraPoint = true;

			auto node = pointLocation(qT->root, p);
			node->points.push_back(p);
		}

		auto edge_list = buildPointGraphOnQuadTree(qT);

		// std::cout << "epsilon: " << epsilon << "\n";
		std::cout << "num cells: " << getLeafs(qT->root).size() << "\n";
		std::cout << "num edges: " << edge_list.size() << "\n";

		outputTXT << "num cells: " << getLeafs(qT->root).size() << "\n";
		outputTXT << "num edges: " << edge_list.size() << "\n";

		/*
		vector<Point*> queries;
		for (int j = 0; j < numQueries; j++) {
			queries.push_back(generateRandomPoint(dim, boundingBox));
		}
		*/

		// cout << "query start\n";
		auto start = chrono::high_resolution_clock::now();
		for (int j = 0; j < q_pts.size(); j++) {
			qT->kNN(q_pts[j], k, true);
		}
		auto stop = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
		auto microsecond = duration.count();
		double second = double(microsecond) / 1000000.0;
		// cout << "query end\n";

		for (auto& pt : pts) delete pt;
		for (auto& q_pt : q_pts) delete q_pt;
		for (auto& tope : Ctopes) delete tope;
		delete qT;

		int index;

		// uniform
		if (curDataSetID / 10 % 2 == 0) {
			index = curDataSetID / 20;
			uniform[index] += microsecond;
		}
		else {
			index  = (curDataSetID - 10) / 20;
			clustered[index] += microsecond;
		}

		time_t timer = time(NULL);
		struct tm* t = localtime(&timer);

		cout << "currentTime: " << t->tm_year + 1900 << "/" << t->tm_mon + 1 << "/" << t->tm_mday << " " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "\n";
		// cout << "dataset ID: " << curDataSetID;
		// << ", type: ";
		// if (curDataSetID / 10 % 2 == 0) cout << " uniform, ";
		// else cout << " clustered, ";
		cout << "numPoints: " << numPoints << " numPolytopes: " << Ctopes.size() << " numQueries: " << numQueries << "\n";
		cout << "time" << numQueries << "queries: " << second << "\n\n";
		//  taken for 

		outputTXT << t->tm_year + 1900 << "/" << t->tm_mon + 1 << "/" << t->tm_mday << " " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "\n";
		// outputTXT << "dataset ID: " << curDataSetID;
		// << ", type: ";
		// if (curDataSetID / 10 % 2 == 0) outputTXT << " uniform, ";
		// else outputTXT << " clustered, ";
		outputTXT << "numPoints: " << numPoints << ", numPolytopes: " << Ctopes.size() << " numQueries: " << numQueries << "\n";
		outputTXT << "time" << numQueries << "queries: " << second << "\n\n";
		//  taken for 
	}

	/*
	cout << "uniform - average query time (ms)\n";
	for (auto val : uniform) {
		cout << val / 10.0 / numQueries * 1000.0 << " ";
	}
	cout << "\nclustered - average query time (ms)\n";
	for (auto val : clustered) {
		cout << val / 10.0 / numQueries * 1000.0 << " ";
	}
	cout << "\n";

	outputTXT << "uniform - average query time (ms)\n";
	for (auto val : uniform) {
		outputTXT << val / 10.0 / numQueries * 1000.0 << " ";
	}
	outputTXT << "\nclustered - average query time (ms)\n";
	for (auto val : clustered) {
		outputTXT << val / 10.0 / numQueries * 1000.0 << " ";
	}
	outputTXT << "\n";
	*/

	outputTXT.close();
}

void distanceSumTest(std::string dir, int startID, int endID, int numSites, int numQueries) {

	int dim = 4;
	// int numSites = 1000;
	// int numQueries = 100;

	vector<int> kVals;
	if (numSites == 1000) kVals = { 5, 10, 50, 100 };
	else if (numSites == 10000) kVals = { 10, 50, 100, 500, 1000};
	else throw std::invalid_argument("numSites should be 1000 or 10000");

	// vector<double> epsVals = { 0.33, 0.2, 0.14, 0.09 };
	vector<double> epsVals = { 10.0, 5.0, 2.0, 1.0, 0.5, 0.2, 0.1};
	int maxDepth = 10; // user-defined

	// bounding box
	double maxValue = 128.0;
	vector<pair<double, double >> boundingBox;
	for (int i = 0; i < dim; i++) boundingBox.push_back(make_pair(-maxValue, maxValue));

	namespace fs = std::filesystem;
	fs::path dataDirDir = dir;
	fs::directory_iterator iterDirDir(dataDirDir);

	int curDataSetID = -1;

	// int startID = 0;
	// int endID = 9;

	string outputFileName = "dist_" + to_string(startID) + "_" + to_string(endID) + "_" + 
		to_string(numSites) + "_" + to_string(numQueries) + ".txt";
	ofstream outputTXT(outputFileName);
	outputTXT.clear();

	if (startID >= 50) outputTXT << "cluster\n";

	vector<vector<double>> arr;
	arr.assign(epsVals.size(), vector<double>(kVals.size(), 0.0));
	// vec.assign(3, vector<int>(4, 0));
	
	for (auto& i00 = iterDirDir; i00 != fs::end(iterDirDir); ++i00) {

		curDataSetID += 1;

		if (startID <= curDataSetID && curDataSetID <= endID) cout << "dataset #" << curDataSetID << " start\n";
		else { cout << "dataset #" << curDataSetID << " skipped\n"; }

		// if (startID <= curDataSetId && curDataSetId <= endID) 
		if (startID > curDataSetID || curDataSetID > endID) continue;

		fs::path dataDir = (*i00).path();

		// ********************************************************** read CPolytope start
		fs::path polytopesDir(dataDir.string() + "\\polytopes");
		fs::directory_iterator iterTopes(polytopesDir);

		std::vector<CPolytope*> Ctopes;

		for (auto& i01 = iterTopes; i01 != fs::end(iterTopes); ++i01) {
			fs::path topeDir = (*i01).path();
			Ctopes.push_back(dels2cpolytope(topeDir.string(), dim, true));
		}
		// ********************************************************** read CPolytope end

		std::string pointsDir = dataDir.string() + "\\points\\points.txt"; // for input sites
		std::vector<Point*> pts = makePointSet(pointsDir);

		pts = vector<Point*>(pts.begin(), pts.begin() + numSites);

		std::string queryDir = dataDir.string() + "\\points\\queries.txt"; // for queries
		std::vector<Point*> q_pts = makePointSet(queryDir); // query points

		q_pts = vector<Point*>(q_pts.begin(), q_pts.begin() + numQueries);

		// ********************************************************** read (input) points end

		for (int j = 0; j < epsVals.size(); j++) {
			auto eps = epsVals[j];

			/*
			vector<Point*> pts2;

			for (int ii = 0; ii < pts.size(); ii++) {
				pts2.push_back(new Point(pts[ii]));
			}
			*/

			int numExtraPoints =  double(1) / eps * pts.size();
			/*
			for (int k = 0; k < numExtraPoints; k++) {
				Point* p = generateRandomPoint(dim, boundingBox);
				p->isExtraPoint = true;
				pts.push_back(p);
			}
			*/

			auto qT = new EpsGraphNd(pts, Ctopes, dim, boundingBox, maxDepth);

			for (int _ = 0; _ < numExtraPoints; _++) {
				Point* p = generateRandomPoint(dim, boundingBox);
				p->isExtraPoint = true;

				auto node = pointLocation(qT->root, p);
				node->points.push_back(p);
			}

			/*
			* 
			// find a number n such that n^2 <= 1 / eps < (n+1)^2
			int n = 1;

			while (true) {
				if (pow(n, 4) < numExtraPoints && numExtraPoints <= pow(n + 1, 4)) {
					break;
				}
				else n++;
			}

			// 놓는 점의 개수가 n^4
			double gap = (boundingBox[0].second - boundingBox[0].first) / (n + 1);
			double start = boundingBox[0].first + gap;

			vector<int> vec; vec.assign(dim, 0);

			// debug
			vector<Point*> testPoints;

			// n진수 - 각 자리수는 0부터 (n-1)까지
			for (int ii = 0; ii < pow(n, 4); ii++) {

				int nowDigit = dim - 1;
				while (true) {
					if (vec[nowDigit] == n - 1) {
						vec[nowDigit] = 0;
						nowDigit -= 1;
					}

					else {
						vec[nowDigit] += 1;
						break;
					}
				}

				// for (auto x : vec)
				// 	cout << x << " ";

				// cout << "\n";

				vector<double> xs; xs.assign(dim, start);
				for (int jj = 0; jj < dim; jj++) {
					xs[jj] += vec[jj] * gap;
				}

				auto p = new Point(xs);
				p->isExtraPoint = true;

				auto node = pointLocation(qT->root, p);
				node->points.push_back(p);

				// debug
				testPoints.push_back(p);
			}

			// place additional points (not as quadtree sites but as "points") on the uniform grid of size n * n (as vertices)

			*/
			
			fillEmptyCells(dim, qT);
			auto edge_list = buildPointGraphOnQuadTree(qT);

			std::cout << "epsilon: " << eps << "\n";
			std::cout << "num cells: " << getLeafs(qT->root).size() << "\n";
			std::cout << "num edges: " << edge_list.size() << "\n";

			for (int ii = 0; ii < numQueries; ii++) {
				auto& q = q_pts[ii];

				for (int jj = 0; jj < kVals.size(); jj++) {
					auto k = kVals[jj];
					// auto ret = qT->kNN(q, k, false);

					double distSum = 0;

					// auto ret = qT->kNN(q, k, true);
					auto ret = qT->kNN(q, k, false);

					for (auto& v : ret) {
						distSum += v.first;
					}
					
					arr[j][jj] += distSum;
				}
			}

			delete qT;
			/*
			for (int ii = numSites; ii < numSites + numExtraPoints; ii++) {
				delete pts[ii];
			}
			*/

			pts.resize(numSites);
			// pts = vector<Point*>(pts.begin(), pts.begin() + numSites);
			// for (auto & pt : pts) delete pt;
		}

		for (auto& pt : pts) delete pt;
		for (auto& q_pt : q_pts) delete q_pt;
		for (auto& tope : Ctopes) delete tope;

		for (int j = 0; j < arr.size(); j++) {
			auto& vals = arr[j];
			cout << "eps: " << epsVals[j] << "\n";
			
			for (int ii = 0; ii < vals.size(); ii++) {
				auto& val = vals[ii];
				cout << val << " for k: " << kVals[ii] << "\n";
			}
			cout << "\n";
		}
		cout << "\n\n";

		for (int j = 0; j < arr.size(); j++) {
			auto& vals = arr[j];
			outputTXT << "eps: " << epsVals[j] << "\n";

			for (int ii = 0; ii < vals.size(); ii++) {
				auto& val = vals[ii];
				outputTXT << val << " for k: " << kVals[ii] << "\n";
			}
			outputTXT << "\n";
		}
		outputTXT << "\n\n";

	}
}

void Test(std::string dir, bool speedFlag, int useDataSetId) {
	
	// very small value
	// double EPS = 0.0000001;

	bool plotting = true;

	int dim = 4;

	// 100개의 데이터셋
	// int numDatasets = 100;

	// kNN에서 k값
	// std::vector<int> ks = { 10, 50, 100, 500, 1000 };
	std::vector<int> ks = { 50 };

	// if (speedFlag) ks = {10};

	// std::vector<double> epsValues = { 0.1, 0.25, 0.5, 1.0}; // (0, \infty) 범위 내에서 well-defined 되는 것이 좋을 듯.
	std::vector<double> epsValues = { 0.1, 0.2, 0.3 }; 
	if (speedFlag) {
		epsValues = {0.0};
	}

	int maxDepth = INT_MAX; 
	if (speedFlag) {
		maxDepth = 3;
	}

	namespace fs = std::filesystem;

	std::string resultDir = dir + "Result";
	// fs::path dataDirDir = dir + "Data";
	// 	if (speedFlag) dataDirDir = dir + "DataSpeed";
	fs::path dataDirDir = dir;

	vector<Point*> speedPts;
	if (speedFlag) {
		dataDirDir = dir + "_Speed";
	}

	fs::directory_iterator iterDirDir(dataDirDir);

	int curDataSetId = -1;
	// useDataSetId = 0;

	// 각 dataSet에 대해서
	for (auto& i00 = iterDirDir; i00 != fs::end(iterDirDir); ++i00) {

		curDataSetId++;
		// 디버그용. 특정 하나의 dataSet만 테스트하도록
		if (curDataSetId != useDataSetId) {
			continue;
		}

		fs::path dataDir = (*i00).path();

		// ********************************************************** read CPolytope start
		fs::path polytopesDir(dataDir.string() + "\\polytopes");
		fs::directory_iterator iterTopes(polytopesDir);

		std::vector<CPolytope*> Ctopes;

		std::cout << "before reading inputs" << std::endl;

		for (auto& i01 = iterTopes; i01 != fs::end(iterTopes); ++i01) {
			fs::path topeDir = (*i01).path();
			Ctopes.push_back(dels2cpolytope(topeDir.string(), dim, true));
		}
		// ********************************************************** read CPolytope end

		std::string pointsDir = dataDir.string() + "\\points\\points.txt"; // for input sites
		std::vector<Point*> pts = makePointSet(pointsDir);

		pts = vector<Point*>(pts.begin(), pts.begin() + 100);

		std::string queryDir = dataDir.string() + "\\points\\queries.txt"; // for queries
		std::vector<Point*> q_pts = makePointSet(queryDir); // query points

		// ********************************************************** read (input) points end

		std::cout << "finished reading inputs" << std::endl;

		// ************** quadtree debug start

		for (auto epsVal : epsValues) {  // 여러 개의 epsilon 값에 대해 반복

			double maxValue = 128.0;
			// bounding box
			vector<pair<double, double >> boundingBox;
			for (int i = 0; i < dim; i++) boundingBox.push_back(make_pair(-maxValue, maxValue));
			// ********************************************************** 파이썬을 이용한 쿼드트리 시각화를 위한 txt 파일 생성
			string outputFileName = "output_" + to_string(epsVal) + ".txt";
			ofstream outputTXT(outputFileName);
			outputTXT.clear();

			if (!speedFlag) {
				int numExtraPoints = epsVal * pts.size();
				for (int k = 0; k < numExtraPoints; k++) {
					Point* p = generateRandomPoint(dim, boundingBox);
					p->isExtraPoint = true;
					pts.push_back(p);
				}
			}

			auto qT = new EpsGraphNd(pts, Ctopes, dim, boundingBox, epsVal, maxDepth);
			
			auto query = q_pts[0];
			// insertionTest(qT, query, maxDepth);
			// deletionTest(qT, query, maxDepth);

			if (!speedFlag) {
				fillEmptyCells(dim, qT);
			}

			std::queue<Node*> q;
			q.push(qT->root);
			if (outputTXT.is_open()) {
				while (!q.empty()) {
					Node* cur = q.front();
					q.pop();

					 // std::cout << "size: " << (cur->boundingBox).size() << '\n';

					outputTXT << "b\n";
					for (pair<double, double> p : cur->boundingBox) {
						outputTXT << p.first << " " << p.second << '\n';
					}

					if (cur->isLeaf) {
						outputTXT << "p " << (cur->points).size() << '\n';
						for (Point* p : cur->points) {
							for (double x : p->getxs()) {
								outputTXT << x << ' ';
							}
							outputTXT << p->nowIndex;
							outputTXT << '\n';
						}

					}
					else {
						for (Node* child : cur->childNodes) {
							q.push(child);
						}
					}
				}
			}
			else {
				std::cout << "output.txt error!\n";
			}

			// vector<pair<int, int>> edge_list = buildPointGraphOnQuadTree(qT, -1, 0.5);
			vector<pair<int, int>> edge_list = buildPointGraphOnQuadTree(qT, maxValue * epsVal, -1);

			std::cout << "epsilon: " << epsVal << std::endl;
			std::cout << "num cells: " << getLeafs(qT->root).size() << std::endl;
			std::cout << "num edges: " << edge_list.size() << std::endl;
			// checkPointGraphSize(eps, qT);

			for (auto& edge : edge_list) {
				outputTXT << "e" << "\n";
				outputTXT << edge.first << " " << edge.second << "\n";
			}

			for (auto& tope : Ctopes) {
				outputTXT << "t" << " " << tope->vertices.size() << "\n";
				for (auto& ver : tope->vertices) {
					for (auto& x : ver.xs) {
						outputTXT << x << " ";
					}
					outputTXT << "\n";
				}

			}
			/*std::vector<Point*> pts = makePointSet(pointsDir);
			std::vector<Point*> q_pts = makePointSet(queryDir);*/
			outputTXT << 'o' << ' ' << epsVal << ' ' << pts.size() << ' ' << q_pts.size() << "\n";

			if (speedFlag) {
	
				cout << "number of points: " << pts.size() << endl;
		
				auto q = generateRandomPoint(dim, boundingBox);

				auto start = chrono::high_resolution_clock::now();
				qT->kNN(q, 10, true);

				auto stop = chrono::high_resolution_clock::now();
				auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
				auto exe_time = duration.count();
				cout << "query time: " << exe_time / 1000000.0 << "s\n";

			}
			else {
				
				for (auto q : q_pts) {

					for (auto k : ks) {
						for (auto& x : q->xs)
							outputTXT << x << ' ';

						auto ret = qT->kNN(q, k, false);

						for (int jj = 0; jj < ret.size(); jj++) { // pr : ret) {
							auto pr = ret[jj]; // (dist, neighbor)
							// cout << k << "-NN result" << endl;
							// cout << jj << "-th neighbor index: " << pr.second->nowIndex << ", graph distance: " << pr.first << endl;
						}

						outputTXT << k << ' ' << ret.size() << "\n";
						for (auto [dist, kp] : ret) {
							outputTXT << kp->nowIndex << ' ' << dist << ' ' << kp->getx(0) << ' ' << kp->getx(1) << "\n";
						}
					}

				}

			}
				
			outputTXT.close();
		}


		// ********************************************************** 파이썬을 이용한 쿼드트리 시각화를 위한 txt 파일 생성
	}
}

void insertionTest(EpsGraphNd* qT, Point* q, int maxDepth, double EPS) {

	auto ret = qT->kNN(q, 1, false);
	cout << "nn index:" << ret[0].second->nowIndex << "\n";

	auto nn = translate(q, 0, EPS);
	addPoint(qT->root, q, maxDepth);

	ret = qT->kNN(q, 1, false);
	cout << "nn index:" << ret[0].second->nowIndex << "\n";

}

void deletionTest(EpsGraphNd* qT, Point* q, double EPS) {

	auto ret = qT->kNN(q, 1, false);
	cout << "nn index:" << ret[0].second->nowIndex << "\n";

	deletePoint(qT->root, q);

	ret = qT->kNN(q, 1, false);
	cout << "nn index:" << ret[0].second->nowIndex << "\n";

}

void autoTest(std::string dir, double epsilon, bool speedFlag, int useDataSetId) {

	bool checkMemory = false;

	//remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\accuracyError.txt");
	//remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\distanceError.txt");
	//remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\result.txt");
	//remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\log.txt");
	//remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\speed.txt");
	//remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\speedPerEach.txt");

	std::cout << "started autotest" << std::endl;

	// bool speedFlag = true;

	int dim = 2;
	// double epsilon = 200.;

	std::cout << "epsilon: " << epsilon << std::endl;

	// double epsilon = 12.5;
	// double bound = ;

	// 100개의 데이터셋
	int numDatasets = 100;
	// 1000번의 랜덤 쿼리
	// int numQueries = 1000;

	// kNN에서 k값
	std::vector<int> ks = { 10, 50, 100, 500, 1000 };

	//std::string resultDir = "C:\\Users\\\HWI\\Documents\\epsGraphTestResult";
	//fs::path dataDirDir = "C:\\Users\\\HWI\\Documents\\epsGraphTestData";
	//if (speedFlag) dataDirDir = "C:\\Users\\\HWI\\Documents\\epsGraphTestDataSpeed";
	
	namespace fs = std::filesystem;

	std::string resultDir = dir + "Result";
	fs::path dataDirDir = dir + "Data"; // "C:\\Users\\\HWI\\Documents\\epsGraphTestData";
	if (speedFlag) dataDirDir = dir + "DataSpeed"; // C:\\Users\\\HWI\\Documents\\epsGraphTestDataSpeed";

	fs::directory_iterator iterDirDir(dataDirDir);

	int curDataSetId = -1;

	// 테스트케이스 5가지. (point 수, polytope 수에 따라)
	std::vector<long long> speedSum = { 0, 0, 0, 0, 0 };
	std::vector<double> avgSpeed = { 0.0, 0.0, 0.0, 0.0, 0.0 };

	std::vector<double> distErrorSumsAll = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	std::vector<double> numErrorSumsAll = { 0.0, 0.0, 0.0, 0.0, 0.0 };

	std::vector<double> distErrorSumsAllDijk = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	std::vector<double> numErrorSumsAllDijk = { 0.0, 0.0, 0.0, 0.0, 0.0 };

	for (auto& i00 = iterDirDir; i00 != fs::end(iterDirDir); ++i00) {

		curDataSetId++;
		if (curDataSetId != useDataSetId) {
			continue;
		}

		fs::path dataDir = (*i00).path();
		fs::path polytopesDir(dataDir.string() + "\\polytopes");

		// 각각의 polytope이 폴더 안에 있음
		// ex) \\polytopes\\0\\ 니까 매 폴더마다 하나의 polytope,
		// 해서 총 10개의 polytope 체크.
		fs::directory_iterator iterTopes(polytopesDir);

		// std::vector<std::vector<Polytope*>> multiTopes;

		// std::vector<Polytope*> multiTopesNoPtr;

		// ********************************************************** Polytope input reading start
		std::vector<CPolytope*> Ctopes;

		std::cout << "before reading inputs" << std::endl;

		for (auto& i01 = iterTopes; i01 != fs::end(iterTopes); ++i01) {
			// break;

			fs::path topeDir = (*i01).path();
			// topeDir.string();

			Ctopes.push_back(dels2cpolytope(topeDir.string(), dim));

			// multiTopes.push_back(dels2polytopes(topeDir.string(), 1));
		}

		// ********************************************************** Polytope input reading end

		//// multiTopes에서 multiTopesNoPtr로 변경
		//for (int j = 0; j < multiTopes.size(); j++) {
		//	for (auto singleConvexTope : multiTopes[j]) {
		//		// multiTopesNoPtr[j].push_back(*singleConvexTope);
		//		multiTopesNoPtr.push_back(singleConvexTope);
		//	}
		//}

		// ex) C:\Users\HWI\Documents\epsGraphTestData\000\points
		fs::path pointsDir(dataDir.string() + "\\points");

		std::string pointsSpecificDir = dataDir.string() + "\\points\\points.txt";

		// list<Free_Point> frpts(pts.begin(), pts.end());
		// vector<Point*> pts = makePointSet(pointsSpecificDir);
		// vector<Point> pts = makePointSetNoPtr(pointsSpecificDir);
		std::vector<Free_Point*> pts = makeFreePointSet(pointsSpecificDir);
		// vector<Free_Point> pts = makeFreePointSetNoPtr(pointsSpecificDir);

		// list<Free_Point> frpts(pts.begin(), pts.end());

		std::list<Free_Point*> frpts;
		for (auto& pt : pts) {
			frpts.push_back(pt);
		}

		std::cout << "finished reading inputs" << std::endl;

		// ************** quadtree debug start

		double maxValue = 128.0;
			// bounding box
		vector<pair<double, double >> boundingBox;
		for (int i = 0; i < dim; i++) boundingBox.push_back(make_pair(-maxValue, maxValue));

		// std::vector<Point*> pts2 = makePointSet(pointsSpecificDir);
		// 현재는 10000개 포인트 중에서 맨 앞 100개만 확인
		// auto slicedPoints = vector<Point*>(pts2.begin(), pts2.begin() + 100); 

		vector<Point*> pts2;
		for (int j = 0; j < 100; j++) pts2.push_back(generateRandomPoint(dim, make_pair(-maxValue, maxValue)));
		
		// vector<CPolytope*> pols;
		
		auto qT = new EpsGraphNd(pts2, Ctopes, dim, boundingBox, epsilon, INT_MAX);
		// buildEpsilonGraph(pts2);

		// 파이썬에서 옮겨서 테스트 할 output 생성

		ofstream outputTXT("output.txt");
		outputTXT.clear();

		std::queue<Node*> q;
		q.push(qT->root);
		if (outputTXT.is_open()) {
			while (!q.empty()) {
				Node* cur = q.front();
				q.pop();

				// std::cout << "size: " << (cur->boundingBox).size() << '\n';

				outputTXT << "b\n";
				for (pair<double, double> p : cur->boundingBox) {
					outputTXT << p.first << " " << p.second << '\n';
				}


				if (cur->isLeaf) {
					outputTXT << "p " << (cur->points).size() << '\n';
					for (Point* p : cur->points) {
						for (double x : p->getxs()) {
							outputTXT << x << ' ';
						}
						outputTXT << p->nowIndex;
						outputTXT << '\n';
					}

				}
				else {
					for (Node* child : cur->childNodes) {
						q.push(child);
					}
				}
			}
		}
		else {
			std::cout << "output.txt error!\n";
		}

		vector<pair<int, int>> edge_list = buildPointGraphOnQuadTree(qT);

		for (auto& edge : edge_list) {
			outputTXT << "e" << "\n";
			outputTXT << edge.first << " " << edge.second << "\n";
		}
		// 왜 10000 넘는 인덱스가 만들어지나, 복사 생성자 호출해서?
		
		outputTXT.close();
		//Point p(std::vector<double>({ 10.0, 10.0 }));
		//Node* n = pointLocation(qT->root, &p);
		//if (n != nullptr) {
		//	std::cout << "hi\n";
		//}
		exit(1);

		// ************** quadtree debug end

		// 준비된 free points와 polytopes 이용해 epsilon graph 생성

		std::vector<Polytope*> multiTopesNoPtr;

		// Eps_Graph_nD epsGraph(dim, frpts, multiTopesNoPtr, epsilon, resultDir + "\\" + "EG1");
		auto* epsGraph = new GridGraph(dim, frpts, multiTopesNoPtr, epsilon, resultDir + "\\" + "EG1");
		// auto* epsGraph = new Eps_Graph_nD(-1);

		if (checkMemory) {
			epsGraph->checkMemory("C:\\Users\\HWI\\Desktop\\finalResult");
			return;
		}

		std::cout << "constructed epsGraph1" << std::endl;

		// copy for epsGraph2
		vector<Polytope*> multiTopesNoPtr2;

		list<Free_Point*> frpts2;

		GridGraph* epsGraphOpt = nullptr;
		if (!speedFlag) {

			for (auto& tope : multiTopesNoPtr) {
				multiTopesNoPtr2.push_back(new Polytope(*tope));
			}

			for (auto& p : frpts) {
				frpts2.push_back(new Free_Point(*p));
			}

			epsGraphOpt =
				new GridGraph(dim, frpts2, multiTopesNoPtr2,
					epsilon * 0.5, resultDir + "\\" + "EG2");
			std::cout << "constructed epsGraph2" << std::endl;
		}


		// 비교할 near-optimal epsilon graph 생성
		// Eps_Graph_nD epsGraphOpt(dim, frpts, multiTopesNoPtr, epsilon * 0.5, resultDir + "\\" + "EG2");

		// 쿼리 점들도 읽어 오기
		// convex polytope을 피해야 하기 때문에?

		std::string queriesSpecificDir = dataDir.string() + "\\points\\queries.txt";

		// size of queryPoints = numQueries
		vector<Free_Point*> queryPoints = makeFreePointSet(queriesSpecificDir);

		int numQueries = queryPoints.size();

		vector<Free_Point*> queryPoints2;
		for (auto& qP : queryPoints) {
			queryPoints2.push_back(new Free_Point(*qP));
			// multiTopesNoPtr2.push_back(new Polytope(*tope));
		}

		//  ks = {10, 50, 100, 500, 1000}의 각각에 해당
		vector<double> distErrorSums = { 0.0, 0.0, 0.0, 0.0, 0.0 };
		vector<double> numErrorSums = { 0.0, 0.0, 0.0, 0.0, 0.0 };

		std::cout << "ready to start" << endl;

		std::cout << numQueries << std::endl;
		if (speedFlag) {

			//auto start = chrono::high_resolution_clock::now();
			//for (int j = 0; j < numQueries; j++) epsGraph.kNN(queryPoints[j], 10, "");
			//auto stop = chrono::high_resolution_clock::now();;
			auto start = chrono::high_resolution_clock::now();
			for (int j = 0; j < numQueries; j++) {
				if (j % 100 == 0) std::cout << j << "-th query" << endl;

				if (j == 0) epsGraph->kNN(queryPoints[j], 10, resultDir + "\\" + "kNNtimeline.txt");
				//"C:\\Users\\HWI\\Documents\\epsGraphTestResult\\kNNtimeline.txt");
				else epsGraph->kNN(queryPoints[j], 10, "");
			}
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
			auto exe_time = duration.count();

			// speedSum[curDataSetId / 20] += exe_time;
			// speedSum[curDataSetId / 20] += exe_time;
			// avgSpeed[curDataSetId / 20 - 1] += exe_time;
			printSpeedTemp(resultDir + "\\" + "speedPerEach.txt", useDataSetId, exe_time);
			// printSpeedTemp(resultDir + "\\" + "speedSum.txt", curDataSetId, exe_time);
		}
		else {
			for (int j = 0; j < numQueries; j++) {

				if (j % 100 == 0) std::cout << j << "-th query" << endl;

				for (int ii = 0; ii < ks.size(); ii++) {
					auto& k = ks[ii];

					// 시간 측정
					auto pr = epsGraph->kNN(queryPoints[j], k, "");
					auto pr2 = epsGraphOpt->kNN(queryPoints2[j], k, "");

					// auto prDijk = epsGraph.Dijkstra(queryPoints[j], k);
					// grid.Dijkstra(*q, 3);

					auto pr3 = printError(epsGraph, epsGraphOpt, useDataSetId, pr, pr2,
						queryPoints[j], k, resultDir);

					// auto pr4 = printErrorDijk(epsGraph, pr, prDijk, queryPoints[j], k, resultDir);

					//numErrorSums[ii] += pr3.first;
					//distErrorSums[ii] += pr3.second;
					numErrorSumsAll[ii] += pr3.first;
					distErrorSumsAll[ii] += pr3.second;

					//numErrorSumsAllDijk[ii] += pr4.first;
					//distErrorSumsAllDijk[ii] += pr4.second;
				}

			}
		}

		std::cout << "finished computing errors" << endl;

		if (speedFlag) {
			// for (auto& elem : avgSpeed) elem / numQueries;
			// printSpeedFinal(resultDir + "\\" + "speed.txt", avgSpeed);
			// printSpeedFinal(resultDir + "\\" + "speedSum.txt", speedSum);

		}
		else

		std::cout << "before printErrorFinal" << endl;

		printErrorFinal(resultDir + "\\" + "result.txt", numErrorSumsAll, distErrorSumsAll, numQueries, numDatasets);
		std::cout << "finished printErrorFinal" << endl;

		break;
		//printErrorFinal(resultDir + "\\" + "resultDijk.txt", numErrorSumsAllDijk, distErrorSumsAllDijk, numQueries, numDatasets);

		//if (speedFlag) {}
		//else break;

		//for (int j = 0; j < ks.size(); j++) {
		//	distErrorSums += 
		//}
	}



	// fout << dataSet / numQueries;

	//auto start = chrono::high_resolution_clock::now();
	//auto stop = chrono::high_resolution_clock::now();
	//auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	//auto exe_time = duration.count();
}

pair<double, double> printErrorDijk(GridGraph& epsGraph, pVV& pr,
	vector<pair<Free_Point, double>>& prDijk, Free_Point& q, int& k, string& dir) {

	auto& myDists = pr.second;
	vector<double> optDists;
	for (int i = 0; i < prDijk.size(); i++) optDists.push_back(prDijk[i].second);
	// auto& optDists = pr2.second;

	auto& myNbs = pr.first;
	vector<int> myIDs;
	for (auto& myNb : myNbs) {
		myIDs.push_back(myNb->id);
	}

	// auto& optNbs = prDijk first;
	vector<int> optIDs;
	for (int i = 0; i < prDijk.size(); i++) { // auto& optNb : optNbs) {
		optIDs.push_back(prDijk[i].first.id);
	}

	vector<int> overlapIDs;

	double yesIDnum = 0.0;
	double noIDnum = 0.0;
	for (auto& myID : myIDs) {
		if (isIn(optIDs, myID)) {
			yesIDnum = yesIDnum + 1;
			overlapIDs.push_back(myID);
		}
		else noIDnum = noIDnum + 1;
	}

	double myDistSum = 0;
	for (auto& myDist : myDists) myDistSum += myDist;

	double optDistSum = 0;
	for (auto& optDist : optDists) optDistSum += optDist;

	return make_pair(noIDnum / double(k), ((myDistSum - optDistSum)) / optDistSum);
}

//pair<double, double> printError(Eps_Graph_nD& epsGraph1, Eps_Graph_nD& epsGraph2,
//	pVV& pr1, pVV& pr2, Free_Point& q, int& k, string& dir) {
//	// std::string resultDir = "C:\\Users\\\HWI\\Documents\\epsGraphTestResult";
//
//	bool printFlag = false;
//
//	vector<Grid_Point>& gr1 = epsGraph1.grid;
//	vector<Grid_Point>& gr2 = epsGraph2.grid;

//auto now = epsGraph1.fr_pts.begin();
//std::advance(now, id);
//
//auto now2 = epsGraph2.fr_pts.begin();
//std::advance(now2, id);


pair<double, double> printError(GridGraph* epsGraph1, GridGraph* epsGraph2,
	int useDataSetId,
	pVV& pr1, pVV& pr2, Free_Point* q, int& k, string& dir) {
	// std::string resultDir = "C:\\Users\\\HWI\\Documents\\epsGraphTestResult";

	bool printFlag = false;

	vector<Grid_Point*>& gr1 = epsGraph1->grid;
	vector<Grid_Point*>& gr2 = epsGraph2->grid;

	// if (k != 10) return {};

	// ifstream fin;
	ofstream fout;

	auto& myDists = pr1.second;
	auto& optDists = pr2.second;

	//(dir + "\\" + "distanceError.txt");
	//// Acc stands for accuracy
	//ifstream finAcc(dir + "\\" + "accuracyError.txt");

	string printDir = dir + "\\" + "accuracyError.txt";

	fout.open(printDir, ios::app);

	// fout << "dataset #" << useDataSetId << endl;

	if (printFlag) {
		fout << "k: " << k << endl;
		fout << "q: ";
		for (auto& x : q->xs) fout << x << " ";
		fout << endl;
	}

	// q.print(printDir);
	auto& myNbs = pr1.first;
	vector<int> myIDs;
	for (auto& myNb : myNbs) {
		myIDs.push_back(myNb->id);
	}

	auto& optNbs = pr2.first;
	vector<int> optIDs;
	for (auto& optNb : optNbs) {
		optIDs.push_back(optNb->id);
	}

	// opt에는 있는데 my에 없는 애들 계산.
	vector<int> inOPTbutNotInMys;

	for (auto& optID : optIDs) {
		if (!isIn(myIDs, optID)) {
			inOPTbutNotInMys.push_back(optID);
		}
	}

	if (printFlag) {
		fout << "myIDs: " << endl;
		for (int i = 0; i < myIDs.size(); i++) {
			fout << "#" << myIDs[i] << " ";
			for (auto& val : myNbs[i]->xs) fout << val << " ";

			fout << "dist: " << myDists[i] << endl;
			fout << "host: " << "#" << myNbs[i]->host << ", ";
			for (auto& val : gr1[myNbs[i]->host]->xs) fout << val << " ";
			fout << endl;
		}
		// for (auto& myID : myIDs) fout << myID << " ";
		fout << endl;
	}
	/*
	#include <iterator>
	#include <list>

		....

	std::list<Student> l; // look, no pointers!
	auto l_front = l.begin();

	std::advance(l_front, 4);

	std::cout << *l_front << '\n';
	*/

	if (printFlag) {

		fout << "Missed Targets" << endl;
		for (auto& id : inOPTbutNotInMys) {
			fout << "#" << id << " ";

			// id 번만큼 뒤로 이동
			auto now = epsGraph1->fr_pts.begin();
			std::advance(now, id);

			auto now2 = epsGraph2->fr_pts.begin();
			std::advance(now2, id);

			for (auto& val : (*now)->xs) fout << val << " ";

			fout << "host: " << "#" << (*now)->host << ", ";
			for (auto& val : gr1[(*now)->host]->xs) fout << val << " ";

			fout << endl;

			fout << "distance to current host: " <<
				distanceBtwGPandFP(gr1[(*now)->host], (*now)) << ", ";
			fout << "distance to host in coarser grid" <<
				distanceBtwGPandFP(gr2[(*now2)->host], (*now2));

			fout << endl;
		}
		fout << endl;

		fout << "optIDs: " << endl;
		for (int i = 0; i < optIDs.size(); i++) {
			fout << "#" << optIDs[i] << " ";
			for (auto& val : optNbs[i]->xs) fout << val << " ";

			fout << "dist: " << optDists[i] << endl;
			fout << "host: " << "#" << optNbs[i]->host << ", ";
			for (auto& val : gr2[optNbs[i]->host]->xs) fout << val << " ";
			fout << endl;
		}
		// for (auto& optID : optIDs) fout << optID << " ";
		fout << endl;

	}
	//int yesID = 0;
	//for (auto& myID : myIDs) {
	//	if (isIn(optIDs, myID)) yesID++;
	//}

	vector<int> overlapIDs;

	double yesIDnum = 0.0;
	double noIDnum = 0.0;
	for (auto& myID : myIDs) {
		if (isIn(optIDs, myID)) {
			yesIDnum = yesIDnum + 1;
			overlapIDs.push_back(myID);
		}
		else noIDnum = noIDnum + 1;
	}

	if (printFlag) {

		fout << "overlapIDs: " << " ";
		for (auto& overlapID : overlapIDs) fout << overlapID << " ";
		fout << endl;
		fout << endl;

		// fout << "Accuracy Error: " << yesID << " / " << double(k) << ", Ratio: " << yesID/k << endl;
		fout << "Accuracy Error: " << noIDnum << " / " << double(k) << ", Ratio: " << noIDnum / double(k) << endl;
		fout << endl;
	}

	fout.close();

	fout.open(dir + "\\" + "distanceError.txt", ios::app);

	if (printFlag) {
		fout << "k: " << k << endl;
		fout << "q: "; // q.print();
	}

	double myDistSum = 0;
	for (auto& myDist : myDists) myDistSum += myDist;

	double optDistSum = 0;
	for (auto& optDist : optDists) optDistSum += optDist;

	if (myDistSum < optDistSum) {
		int _ = 0;
	}

	// if ()
	if (printFlag) {
		fout << "Distance Error: " << myDistSum - optDistSum << " / " << optDistSum << ", Ratio: " << (myDistSum - optDistSum) / optDistSum << endl;
	}

	fout.close();

	return make_pair(noIDnum / double(k), ((myDistSum - optDistSum)) / optDistSum);
}


//// 
//double computeDistanceError(pVV& pr1, pVV& pr2, string dir) {
//	
//}
//
//// 
//double computeNeighborError(pVV& pr1, pVV& pr2, string dir) {
//	
//}

// resultDir + "\\" + "speed.txt", avgSpeed);
void printSpeedFinal(string dir, vector<double> avgSpeed) {

	ofstream fout;
	fout.open(dir, ios::app);

	for (int i = 0; i < avgSpeed.size(); i++) {
		fout << "DataSet " << i + 1 << endl;
		fout << avgSpeed[i] << " ms" << endl;
	}


	fout.close();
}

// printSpeedFinal(resultDir + "\\" + "speedSum.txt", speedSum);
void printSpeedFinal(string dir, vector<long long int> speedSum) {
	ofstream fout;
	fout.open(dir, ios::app);

	for (int i = 0; i < speedSum.size(); i++) {
		fout << "DataSet " << i + 1 << endl;
		fout << speedSum[i] << endl;
	}

	fout.close();
}

// printSpeedTemp(exe_time, curDataSetId, resultDir + "\\" + "speedPerEach.txt");
void printSpeedTemp(string dir, int id, long long time) {
	ofstream fout;
	fout.open(dir, ios::app);

	fout << "id: " << id << ", time: " << time << endl;

	fout.close();
}


void printErrorFinal(string dir, vector<double> numErrorSumsAll, vector<double> distErrorSumsAll,
	int numQueries, int numDatasets) {
	ofstream fout;
	fout.open(dir, ios::app);
	// fout.open(resultDir + "\\" + "result.txt");

	std::time_t now1 = std::time(nullptr);
	tm tm_1;
	localtime_s(&tm_1, &now1);
	// fout << "현재 날짜:" << tm_1.tm_mday << "/" << (tm_1.tm_mon + 1) << "/" << (tm_1.tm_year + 1900) << std::endl;
	fout << "현재 날짜:" << (tm_1.tm_year + 1900) << "/" << (tm_1.tm_mon + 1) << "/" << tm_1.tm_mday << " ";
	// fout << "현재 시간:" << tm_1.tm_hour << ":" << tm_1.tm_min << ":" << tm_1.tm_sec << std::endl;
	fout << "현재 시간:" << tm_1.tm_hour << ":" << tm_1.tm_min << ":" << tm_1.tm_sec << std::endl;

	fout << fixed << setprecision(10);

	fout << "Num Error: ";
	for (auto& er : numErrorSumsAll) fout << er / (numQueries * numDatasets) * 100.0 << " ";
	fout << endl;

	fout << "Dist Error: ";
	for (auto& er : distErrorSumsAll) fout << er / (numQueries * numDatasets) * 100.0 << " ";
	fout << endl;

	fout.close();
}

bool isIn(vector<int> vec, int val) {
	for (auto& vecVal : vec) {
		if (vecVal == val) return true;
	}
	return false;
}