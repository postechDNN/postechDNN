#define _CRT_SECURE_NO_WARNINGS

#include "../algorithm/errorTest.h"
// #include <Eigen/Dense>
#include "../Polytope.h"
#include "fstream"
#include "quadtree.h"
#include <ctime>
// #include "filesystem"

using namespace std;

// point set 
vector<Point*> makePointSet(std::string dir, int start_idx) {

	vector<Point*> ret;

	ifstream fin(dir);

	string line;
	getline(fin, line); // dimension
	getline(fin, line); // number of points
	int numPoints = stoi(line);

	// 
	for (int j = 0; j < numPoints; j++) {
		getline(fin, line);
		stringstream ss(line);
		ss.str(line);

		vector<double> values;
		vector<string> words;

		string word;
		while (ss >> word) {
			// words.push_back(word);
			values.push_back(stod(word));
		}

		auto pt = new Point(values);
		pt->is_Free_Point = true;
		pt->nowIndex = start_idx + j;
		ret.push_back(pt);
	}

	fin.close();

	return ret;
}

vector<Free_Point*> makeFreePointSet(std::string dir) {

	vector<Free_Point*> ret;

	ifstream fin(dir);

	string line;
	getline(fin, line); // dimension
	getline(fin, line); // number of points
	int numPoints = stoi(line);

	for (int j = 0; j < numPoints; j++) {
		getline(fin, line);
		stringstream ss(line);
		ss.str(line);

		vector<double> values;
		vector<string> words;

		string word;
		while (ss >> word) {
			// words.push_back(word);
			values.push_back(stod(word));
		}


		auto newP = new Free_Point(values);
		// pt->is_Free_Point = true;
		ret.push_back(newP);

	}

	return ret;
}

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

	outputTXT << "q" << " ";
	for (auto& x : queries[0]->xs)
		outputTXT << x << ' ';

	auto ret = qT->kNN(queries[0], 10, true);

	for (int jj = 0; jj < ret.size(); jj++) { // pr : ret) {
		auto pr = ret[jj]; // (dist, neighbor)
	}

	outputTXT << 10 << ' ' << ret.size() << "\n";
	for (auto [dist, kp] : ret) {
		outputTXT << kp->nowIndex << ' ' << dist << ' ' << kp->getx(0) << ' ' << kp->getx(1) << "\n";
	}

}

void visualize(std::string dir) {

	int dim = 2;

	std::string pointsDir = "C:\\epsGraphData\\003\\points\\points.txt";
	std::string queryDir = "C:\\epsGraphData\\003\\points\\queries.txt";

	std::vector<Point*> pts = makePointSet(pointsDir); // input sites
	std::vector<Point*> q_pts = makePointSet(queryDir); // query points

	namespace fs = std::filesystem;
	fs::path topeDir = "C:\\epsGraphData\\003\\polytopes";
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

		std::string queryDir = dataDir.string() + "\\points\\queries.txt"; // for queries
		std::vector<Point*> q_pts = makePointSet(queryDir); // query points

		// ********************************************************** read (input) points end

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
		cout << "numPoints: " << numPoints << " numPolytopes: " << Ctopes.size() << " numQueries: " << numQueries << "\n";
		cout << "time" << numQueries << "queries: " << second << "\n\n";

		outputTXT << t->tm_year + 1900 << "/" << t->tm_mon + 1 << "/" << t->tm_mday << " " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "\n";
		outputTXT << "numPoints: " << numPoints << ", numPolytopes: " << Ctopes.size() << " numQueries: " << numQueries << "\n";
		outputTXT << "time" << numQueries << "queries: " << second << "\n\n";
	}

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

			int numExtraPoints =  double(1) / eps * pts.size();

			auto qT = new EpsGraphNd(pts, Ctopes, dim, boundingBox, maxDepth);

			for (int _ = 0; _ < numExtraPoints; _++) {
				Point* p = generateRandomPoint(dim, boundingBox);
				p->isExtraPoint = true;

				auto node = pointLocation(qT->root, p);
				node->points.push_back(p);
			}
			
			fillEmptyCells(dim, qT);
			auto edge_list = buildPointGraphOnQuadTree(qT);

			std::cout << "epsilon: " << eps << "\n";
			std::cout << "num cells: " << getLeafs(qT->root).size() << "\n";
			std::cout << "num edges: " << edge_list.size() << "\n";

			for (int ii = 0; ii < numQueries; ii++) {
				auto& q = q_pts[ii];

				for (int jj = 0; jj < kVals.size(); jj++) {
					auto k = kVals[jj];

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

bool isIn(vector<int> vec, int val) {
	for (auto& vecVal : vec) {
		if (vecVal == val) return true;
	}
	return false;
}

// 파일 input을 통해, 하나의 nonConvex polytope을 vector<convex polytope>으로 변환하는 함수  
vector<Polytope*> dels2polytopes(string dir, int num_topes) {
	vector<Polytope*> ret;

	int dummy;

	std::vector<string> filenames = { "" };

	for (auto index : filenames) {
		std::ifstream fin;

		// string str = dir + "result" + index + ".txt";
		string str = dir + "\\" + "points" + index + ".txt";
		fin.open(str);

		string s;
		getline(fin, s);
		int dim = stoi(s);
		getline(fin, s);
		int num_pts = stoi(s);

		std::vector<Point*> pts;
		std::vector<double> vec(num_pts, 0.0);
		double val;

		for (int i = 0; i < num_pts; i++) {
			Point* pt = new Point(dim);
			for (int j = 0; j < dim; j++) {
				fin >> val;
				pt->setx(j, val);
			}
			pts.push_back(pt);
		}

		fin.close();

		Polytope* tope = new Polytope(dim);
		tope->set_vertices(pts);

		std::vector<simplex> sims;

		// std::ifstream fin2(dir + "CH" + to_string(index));
		string str2 = dir + "\\" + "tets" + index + ".txt";
		std::ifstream fin2;

		fin.open(str2);

		getline(fin, s);
		int size = stoi(s);
		for (int index = 0; index < size; index++) {
			fin >> dummy;
			vector<Point*> vec;
			for (int index2 = 0; index2 < dim + 1; index2++) {
				fin >> s;
				vec.push_back(pts[stoi(s)]);
			}
			auto sim = new simplex(dim, vec);
			sims.push_back(*sim);
		}

		tope->set_simplices(sims);
		ret.push_back(tope);
	}

	return ret;
}

// 파일 input을 통해, (하나의) convex polytope을 만드는 함수
// dir의 예시: 
CPolytope* dels2cpolytope(string dir, int dim, bool isSimplex) {

	int dummy;

	std::ifstream fin;

	// point 정보 읽기 start
	// string str = dir + "\\" + "points" + index + ".txt";
	string str = dir + "\\" + "points.txt";
	fin.open(str);

	string s;
	// 차원 입력
	getline(fin, s);
	// 점 개수 입력
	getline(fin, s);
	int num_pts = stoi(s);

	// 포인트 읽어들이기
	// std::vector<Point*> pts;
	std::vector<Point> pts;
	double val;

	for (int i = 0; i < num_pts; i++) {
		// Point* pt = new Point(dim);
		Point pt(dim);
		for (int j = 0; j < dim; j++) {
			fin >> val;
			// pt->setx(j, val);
			pt.setx(j, val);
		}
		pts.push_back(pt);
	}

	fin.close();
	// point 정보 읽기 end

	// facet 정보 계산 start
	std::vector< std::vector<int>> facets;

	if (isSimplex) {
		// 0, 1, ..., dim

		for (int j = 0; j < dim + 1; j++) {
			vector<int> temp;
			for (int v = 0; v < dim + 1; v++) {
				if (v == j) continue;
				temp.push_back(v);
			}
			facets.push_back(temp);
		}
	}
	else {
		std::ifstream fin2;

		// string str2 = dir + "\\" + "tets" + index + ".txt";
		string str2 = dir + "\\" + "tets.txt";
		fin.open(str2);

		getline(fin, s);
		// facet의 개수
		int size = stoi(s);
		for (int index = 0; index < size; index++) {
			// 차원 정보 (필요 없는 정보)
			fin >> dummy;

			std::vector<int> facet;

			for (int j = 0; j < dim + 1; j++) {
				fin >> s;
				facet.push_back(stoi(s));
			}

			facets.push_back(facet);
		}

		fin2.close();
	}

	// facet 정보 읽기 end

	return new CPolytope(dim, pts, facets);
}