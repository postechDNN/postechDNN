#include "data_generation.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <tuple>
#include <string>
#include "../algorithm/EpsGraphnD.h"
#include "../algorithm/Polytope.h"
#include "../algorithm/Point.h"
#include <typeinfo>
#include <filesystem>
#include <sstream>

#include <stdio.h>
#include <io.h>
#include <conio.h>

std::random_device rd;
std::mt19937 gen(rd());

bool PRINT_POINT_FLAG = false;

// 이 flag가 true라면 data를 만드는 
// 이 flag가 false라면 (만들어진 data를 가지고) nearest neighbor를 찾음
bool DATA_GENERATION = true;

void data_generation() {
	std::cout.precision(3);
	std::cout << std::fixed;

	// config를 입력받기


	// dummy 없이 그냥 해도 될 듯?

	std::string config("C:\\Users\\HWI\\Desktop\\StarLab_1017\\postechDNN\\dnn\\NearestNeighbor\\EpsGraphnD\\data_generation\\config.ini");
	// std::string config("./config.ini");
	// std::string config("config.ini");

	ifstream readFile;
	readFile.open(config);

	// 1을 입력 시 자동으로 DataGeneration사용법.docx에 해당하는 입력
	// 0을 입력 시 수동으로 입력받음
	int auto_answer;
	int d;
	double u_bound;
	int halfplane_constraint;
	int num_parts;
	// int dis_pts;
	int num_subspace;
	int dis_pts;
	int dis_pts_cell;

	// 파일 입출력
	if (readFile.is_open()) {
		// string dum1, dum2, str;
		// readFile >> dum1 >> dum2 >> s;

		string dum, str;
		// dimension
		readFile >> dum >> str;
		d = stoi(str);

		readFile >> dum >> str;
		u_bound = stod(str);

		readFile >> dum >> str;
		halfplane_constraint = stoi(str);

		readFile >> dum >> str;
		num_parts = stoi(str);

		readFile >> dum >> str;
		num_subspace = stoi(str);

		readFile >> dum >> str;
		dis_pts = stoi(str);

		readFile >> dum >> str;
		dis_pts_cell = stoi(str);
	}
	// 파일이 존재하지 않을 경우 직접 input 입력
	else {
		std::cout << "Auto: ";
		std::cin >> auto_answer;

		// DataGeneration사용법.docx에 해당하는 입력 예시: 4, 10.0, 0, 2, 2, 3, 4.
		if (auto_answer == 1) {
			d = 4; u_bound = 10.0; halfplane_constraint = 0; num_parts = 2; num_subspace = 2;
			dis_pts = 3; dis_pts_cell = 4;
		}
		else {

			std::cout << "Enter the dimension: ";
			std::cin >> d;

			// defines a bounding box (cube) around the origin
			std::cout << std::endl;
			std::cout << "Maximum value for each coordinate: ";
			std::cin >> u_bound;

			// determines if the halfplanes are generated along the axes (grid)
			std::cout << std::endl;
			std::cout << "Halfplane constraint (0: Griddy, 1: Arbitrary)" << std::endl;
			std::cout << "Select: ";
			std::cin >> halfplane_constraint;

			std::cout << std::endl;
			std::cout << "Number of 'maximum' convex subparts: ";
			std::cin >> num_parts;

			// 기존에 axis별로 입력받았다면 지금은 일괄적으로, 하나의 값을 k 입력받아서 k^d개의 cell을 만듬
			// std::cout << "Number of subspaces along the " << i + 1 << "-th axis: ";
			std::cout << std::endl;
			std::cout << "Number of subspaces along each axis: ";
			std::cin >> num_subspace;

			std::cout << "Number of points to disperse on each halfplane: ";
			std::cin >> dis_pts;

			std::cout << "Number of points to locate inside each cell: ";
			std::cin >> dis_pts_cell;
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------
	// generate a rectangular bounding box
	std::vector<std::pair<double, double>> bbx;

	for (int i = 0; i < d; i++) bbx.push_back(std::make_pair(-u_bound, u_bound));
	// double upper_bound = 10.0; // maximum value for each coordinate
	// double lower_bound = -upper_bound; // minimum value for each coordinate

	switch (halfplane_constraint) {
	case 0: {
		// for each axis

		std::cout << std::endl;
		vector<int> num_subspaces_over_axes;
		for (int i = 0; i < d; i++) {
			num_subspaces_over_axes.push_back(num_subspace);
		}
		// generate grid (cells)
		// set neighbors inside generate_grid( )
		auto cells = generate_grid(bbx, num_subspaces_over_axes); // 
		activate_cells(cells, num_parts);

		// 각 hp에 흩뿌리고 나서
		// 임의의 active한 pair 사이에서 발생하는 건가? 그럼 각 cell에 대해서 neighbor 다 뒤져서 restricted_halfplane을 가져와야 하나?
		disperse_pts_between_active_cells(bbx, num_subspaces_over_axes, cells, dis_pts);

		for (int i = 0; i + 1 < cells.size(); i++) {
			if (cells[i]->active) {
				for (auto nb : cells[i]->neighbors) {
					if (nb.first->active &&
						cells[i]->total_index < nb.first->total_index) {
						for (int j = 0; j < nb.second->on_points.size(); j++) {
							if (PRINT_POINT_FLAG) print_pt(nb.second->on_points[j], j);
						}
					}
				}
			}
		}

		for (int i = 0; i < cells.size(); i++) {
			generate_points_on_bb(cells[i], dis_pts_cell);
			for (int j = 0; j < cells[i]->in_points.size(); j++) {
				if (PRINT_POINT_FLAG) print_pt(cells[i]->in_points[j], j);
			}
		}

		std::string dir("C:\\qhull\\bin\\A\\");
		// user-defined directory end

		int count = 0;

		for (int i = 0; i < cells.size(); i++) {
			if (!cells[i]->active) continue;

			vector<Point*> pts = cells[i]->in_points;
			for (auto nb : cells[i]->neighbors) {
				if (!nb.first->active) continue;

				pts.insert(pts.end(), nb.second->on_points.begin(), nb.second->on_points.end());
			}

			std::string res = "result";
			if (count < 10) {
				res += "00";
			}
			else if (count < 100) {
				res += "0";
			}
			res += std::to_string(count) + ".txt";

			std::ofstream fout(dir + res);

			// first line contains the dimension
			fout << d << std::endl;
			// second line contains the number of input points
			fout << pts.size() << std::endl;
			// remaining lines contain point coordinates
			for (auto pt : pts) {
				for (int j = 0; j < d - 1; j++) {
					fout << pt->getx(j) << " ";
				}
				fout << pt->getx(d - 1) << std::endl;
			}
			count++;

			fout.close();
		}
	}
	}
}

// int d - dimension
// double u_bound - max_value 
// int num_parts - # maximum convex subparts
// halfplane_constraint - griddy or arbitrary
// num_subspaces - # subspaces along each axis
// dis_pts - # points on each halfplane
// dis_pts_cell - # points on within each cell
void data_generation(int d, double u_bound, int halfplane_constraint, int num_parts, int num_subspace, int dis_pts, int dis_pts_cell) {
	std::cout.precision(3);
	std::cout << std::fixed;

	// config를 입력받기

	// dummy 없이 그냥 해도 될 듯?

	std::string config("C:\\Users\\HWI\\Desktop\\StarLab_1017\\postechDNN\\dnn\\NearestNeighbor\\EpsGraphnD\\data_generation\\config.ini");
	// std::string config("./config.ini");
	// std::string config("config.ini");

	ifstream readFile;
	readFile.open(config);

	// 1을 입력 시 자동으로 DataGeneration사용법.docx에 해당하는 입력
	// 0을 입력 시 수동으로 입력받음
	int auto_answer;

	// 파일 입출력
	if (readFile.is_open()) {
		// string dum1, dum2, str;
		// readFile >> dum1 >> dum2 >> s;

		string dum, str;
		// dimension
		readFile >> dum >> str;
		d = stoi(str);

		readFile >> dum >> str;
		u_bound = stod(str);

		readFile >> dum >> str;
		halfplane_constraint = stoi(str);

		readFile >> dum >> str;
		num_parts = stoi(str);

		readFile >> dum >> str;
		num_subspace = stoi(str);

		readFile >> dum >> str;
		dis_pts = stoi(str);

		readFile >> dum >> str;
		dis_pts_cell = stoi(str);
	}
	// 파일이 존재하지 않을 경우 직접 input 입력
	else {
		std::cout << "Auto: ";
		std::cin >> auto_answer;

		// DataGeneration사용법.docx에 해당하는 입력 예시: 4, 10.0, 0, 2, 2, 3, 4.
		if (auto_answer == 1) {
			d = 4; u_bound = 10.0; halfplane_constraint = 0; num_parts = 2; num_subspace = 2;
			dis_pts = 3; dis_pts_cell = 4;
		}
		else {

			std::cout << "Enter the dimension: ";
			std::cin >> d;

			// defines a bounding box (cube) around the origin
			std::cout << std::endl;
			std::cout << "Maximum value for each coordinate: ";
			std::cin >> u_bound;

			// determines if the halfplanes are generated along the axes (grid)
			std::cout << std::endl;
			std::cout << "Halfplane constraint (0: Griddy, 1: Arbitrary)" << std::endl;
			std::cout << "Select: ";
			std::cin >> halfplane_constraint;

			std::cout << std::endl;
			std::cout << "Number of 'maximum' convex subparts: ";
			std::cin >> num_parts;

			// 기존에 axis별로 입력받았다면 지금은 일괄적으로, 하나의 값을 k 입력받아서 k^d개의 cell을 만듬
			// std::cout << "Number of subspaces along the " << i + 1 << "-th axis: ";
			std::cout << std::endl;
			std::cout << "Number of subspaces along each axis: ";
			std::cin >> num_subspace;

			std::cout << "Number of points to disperse on each halfplane: ";
			std::cin >> dis_pts;

			std::cout << "Number of points to locate inside each cell: ";
			std::cin >> dis_pts_cell;
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------
	// generate a rectangular bounding box
	std::vector<std::pair<double, double>> bbx;

	for (int i = 0; i < d; i++) bbx.push_back(std::make_pair(-u_bound, u_bound));
	// double upper_bound = 10.0; // maximum value for each coordinate
	// double lower_bound = -upper_bound; // minimum value for each coordinate

	switch (halfplane_constraint) {
	case 0: {
		// for each axis

		std::cout << std::endl;
		vector<int> num_subspaces_over_axes;
		for (int i = 0; i < d; i++) {
			num_subspaces_over_axes.push_back(num_subspace);
		}
		// generate grid (cells)
		// set neighbors inside generate_grid( )
		auto cells = generate_grid(bbx, num_subspaces_over_axes); // 
		activate_cells(cells, num_parts);

		// 각 hp에 흩뿌리고 나서
		// 임의의 active한 pair 사이에서 발생하는 건가? 그럼 각 cell에 대해서 neighbor 다 뒤져서 restricted_halfplane을 가져와야 하나?
		disperse_pts_between_active_cells(bbx, num_subspaces_over_axes, cells, dis_pts);

		for (int i = 0; i + 1 < cells.size(); i++) {
			if (cells[i]->active) {
				for (auto nb : cells[i]->neighbors) {
					if (nb.first->active &&
						cells[i]->total_index < nb.first->total_index) {
						for (int j = 0; j < nb.second->on_points.size(); j++) {
							if (PRINT_POINT_FLAG) print_pt(nb.second->on_points[j], j);
						}
					}
				}
			}
		}

		for (int i = 0; i < cells.size(); i++) {
			generate_points_on_bb(cells[i], dis_pts_cell);
			for (int j = 0; j < cells[i]->in_points.size(); j++) {
				if (PRINT_POINT_FLAG) print_pt(cells[i]->in_points[j], j);
			}
		}

		std::string dir("C:\\qhull\\bin\\A\\");
		// user-defined directory end

		int count = 0;

		for (int i = 0; i < cells.size(); i++) {
			if (!cells[i]->active) continue;

			vector<Point*> pts = cells[i]->in_points;
			for (auto nb : cells[i]->neighbors) {
				if (!nb.first->active) continue;

				pts.insert(pts.end(), nb.second->on_points.begin(), nb.second->on_points.end());
			}

			std::string res = "result";
			if (count < 10) {
				res += "00";
			}
			else if (count < 100) {
				res += "0";
			}
			res += std::to_string(count) + ".txt";

			std::ofstream fout(dir + res);

			// first line contains the dimension
			fout << d << std::endl;
			// second line contains the number of input points
			fout << pts.size() << std::endl;
			// remaining lines contain point coordinates
			for (auto pt : pts) {
				for (int j = 0; j < d - 1; j++) {
					fout << pt->getx(j) << " ";
				}
				fout << pt->getx(d - 1) << std::endl;
			}
			count++;

			fout.close();
		}
	}
	}
}

void run_nearest_neighbor() {
	// string my_dir = "C:\\Users\\Jagun\\source\\repos\\";
	// string my_dir = "C:\\Users\\ALGO\\Desktop\\StarLab\\";

	// 깃허브 스타랩 위치(사용자 직접 설정)
	string my_dir = "C:\\Users\\hwikim\\Desktop\\GitHub\\";
	// polytope 정보를 저장할 하위 디렉토리
	string dir = my_dir + "postechDNN\\dnn\\NearestNeighbor\\EpsGraphnD\\data_generation\\polytopes\\D";


	ifstream fin;

	int d = 4;
	int num_polytopes = 1;

	std::vector<std::pair<double, double>> bbx;

	int u_bound = 50;
	for (int i = 0; i < d; i++) bbx.push_back(std::make_pair(-u_bound, u_bound));
	// double upper_bound = 10.0; // maximum value for each coordinate
	// double lower_bound = -upper_bound; // minimum value for each coordinate

	vector<vector<Polytope*>> topes;
	for (int i = 1; i < num_polytopes + 1; i++) {
		string final_dir = dir;

		if (i == 1) {
			// str
		}
		else {
			final_dir += " (" + to_string(i) + ")";
		}

		final_dir += "\\";

		//_finddata_t fd;
		//long handle;
		//int result = 1;

		//final_dir += "*.*";
		//const char* c = final_dir.c_str();

		//handle = _findfirst(c, &fd);  //현재 폴더 내 모든 파일을 찾는다.

		//int num_files = 0;

		//while (result != -1)
		//{
		//	// printf("File: %s\n", fd.name);
		//	result = _findnext(handle, &fd);
		//	num_files++;
		//}

		//_findclose(handle);

		// topes.push_back(dels2polytopes(dir, num_polytopes));
		// topes.push_back(dels2polytopes(final_dir.substr(0, final_dir.size()-3), (num_files - 2) / 2));
		topes.push_back(dels2polytopes(final_dir, 1));
		//vector<Polytope*> _rst = dels2polytopes(final_dir, 13);
		//vector<Polytope*>::iterator it = topes.insert(topes.end(), _rst.begin(), _rst.end());

	}

	vector<Polytope> plts;
	for (auto tps : topes) {
		for (auto ts : tps) {
			plts.push_back(*ts);
		}
	}
	//vector<Polytope> _plts;
	/*
	Free_Point* q1 = new Free_Point({ 0., -10. ,0.,0. });
	Free_Point* q2 = new Free_Point({ 0., 0. ,0.,0. });
	Free_Point* q3 = new Free_Point({ 0., 10. ,0.,0. });
	Free_Point* q4 = new Free_Point({ 0., 20. ,0.,0. });
	Free_Point* q5 = new Free_Point({ 0., 30. ,0.,0. });
	list<Free_Point> _frpts = {*q1,*q2 ,*q3 ,*q4 ,*q5 };

	vector<Point*> pts = generate_point_sites(bbx, d, 10, 0);
	list<Free_Point> frpts(pts.begin(), pts.end());
	cout << "Number of Polytopes: " << plts.size() << endl;
	Eps_Graph_nD grid(4, _frpts, plts, 20.0);
	*/

	d = 2;
	Free_Point* q1 = new Free_Point({ 0., -10. });
	Free_Point* q2 = new Free_Point({ 0., 40. });
	Free_Point* q3 = new Free_Point({ 0., 10. });
	Free_Point* q4 = new Free_Point({ 0., 20. });
	Free_Point* q5 = new Free_Point({ 0., 30. });
	Free_Point* q6 = new Free_Point({ -21., 0. });
	list<Free_Point> _frpts = { *q1,*q2 ,*q3 ,*q4 ,*q5, *q6 };

	int num_pt = 0;
	vector<Point*> pts;
	while (num_pt < 50) {
		vector<Point*> ps = generate_point_sites(bbx, 2, 1, 0);
		for (auto tp : plts) {
			if (tp.isIn(ps[0])) continue;
		}
		pts.push_back(ps[0]);
		num_pt++;
	}

	//vector<Point*> pts = generate_point_sites(bbx, 2, 10, 0);
	list<Free_Point> frpts(pts.begin(), pts.end());
	cout << "Number of Polytopes: " << plts.size() << endl;
	cout << "Number of Points: " << _frpts.size() << endl;

	//Eps_Graph_nD grid(2, _frpts, plts, 5);
	//Free_Point* q = new Free_Point({ 0., -5. });
	//grid.add_freepts(q);
	//for (int j = 0; j < d; j++) {
	//	cout << q->getx(j) << ' ';
	//}
	//cout << endl;
	////grid.print_free_point();


	//grid.print_kNN(*q, 3);
	//grid.Dijkstra(*q, 3);

}

// polytope 저장하는 디렉토리
string polytopeDir = "C:\\1polytopes";

namespace fs = std::filesystem;

void dataGeneration() {
	
}

vector<Point*> makePointSet(std::string dir) {
	
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
		ret.push_back(pt);
	}

	return ret;
}

vector<Point> makePointSetNoPtr(std::string dir) {

	vector<Point> ret;

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

		ret.push_back(Point(values, true));
	}

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

		//Free_Point newP(values);
		//ret.push_back(newP);
	}

	return ret;
}

vector<Free_Point> makeFreePointSetNoPtr(std::string dir) {

	vector<Free_Point> ret;

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
		
		Free_Point newP(values);
		ret.push_back(newP);
		// ret.push_back(Free_Point(values)); // , true));
	}

	return ret;
}

bool isIn(vector<int> vec, int val) {
	for (auto& vecVal : vec) {
		if (vecVal == val) return true;
	}
	return false;
}

// Free_Point getFreePointFromList()

pair<double, double> printErrorDijk(Eps_Graph_nD& epsGraph, pVV& pr,
	vector<pair<Free_Point, double>>& prDijk, Free_Point& q, int& k, string& dir) {

	auto& myDists = pr.second;
	vector<double> optDists;
	for (int i = 0; i < prDijk.size(); i++) optDists.push_back(prDijk[i].second);
	// auto& optDists = pr2.second;

	auto& myNbs = pr.first;
	vector<int> myIDs;
	for (auto& myNb : myNbs) {
		myIDs.push_back(myNb.id);
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

pair<double, double> printError(Eps_Graph_nD& epsGraph1, Eps_Graph_nD& epsGraph2,
	pVV& pr1, pVV& pr2, Free_Point& q, int& k, string& dir) {
	// std::string resultDir = "C:\\Users\\\HWI\\Documents\\epsGraphTestResult";

	bool printFlag = false;

	vector<Grid_Point>& gr1 = epsGraph1.grid;
	vector<Grid_Point>& gr2 = epsGraph2.grid;

	// if (k != 10) return {};

	ifstream fin;
	ofstream fout;
	
	auto& myDists = pr1.second;
	auto& optDists = pr2.second;

	//(dir + "\\" + "distanceError.txt");
	//// Acc stands for accuracy
	//ifstream finAcc(dir + "\\" + "accuracyError.txt");

	string printDir = dir + "\\" + "accuracyError.txt";

	fout.open(printDir, ios::app);

	if (printFlag) {
		fout << "k: " << k << endl;
		fout << "q: "; 
		for (auto& x : q.xs) fout << x << " ";
		fout << endl;
	}
	
	// q.print(printDir);
	auto& myNbs = pr1.first;
	vector<int> myIDs;
	for (auto& myNb : myNbs) {
		myIDs.push_back(myNb.id);
	}

	auto& optNbs = pr2.first;
	vector<int> optIDs;
	for (auto& optNb : optNbs) {
		optIDs.push_back(optNb.id);
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
		for (auto& val : myNbs[i].xs) fout << val << " ";
		
		fout << "dist: " << myDists[i] << endl;
		fout << "host: " << "#" << myNbs[i].host << ", ";
		for (auto& val : gr1[myNbs[i].host].xs) fout << val << " ";
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
			auto now = epsGraph1.fr_pts.begin();
			std::advance(now, id);

			auto now2 = epsGraph2.fr_pts.begin();
			std::advance(now2, id);

			for (auto& val : (*now).xs) fout << val << " ";

			fout << "host: " << "#" << (*now).host << ", ";
			for (auto& val : gr1[(*now).host].xs) fout << val << " ";

			fout << endl;

			fout << "distance to current host: " << distanceBtwGPandFP(gr1[(*now).host], (*now)) << ", ";
			fout << "distance to host in coarser grid" << distanceBtwGPandFP(gr2[(*now2).host], (*now2));

			fout << endl;
		}
		fout << endl;

		fout << "optIDs: " << endl;
		for (int i = 0; i < optIDs.size(); i++) {
			fout << "#" << optIDs[i] << " "; 
			for (auto& val : optNbs[i].xs) fout << val << " ";

			fout << "dist: " << optDists[i] << endl;
			fout << "host: " << "#" << optNbs[i].host << ", ";
			for (auto& val : gr2[optNbs[i].host].xs) fout << val << " ";
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
		fout << "Accuracy Error: " << noIDnum << " / " << double(k) << ", Ratio: " << noIDnum/double(k) << endl;
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
		int _= 0;
	}

	// if ()
	if (printFlag) {
		fout << "Distance Error: " << myDistSum - optDistSum <<  " / " << optDistSum << ", Ratio: " << (myDistSum - optDistSum) / optDistSum << endl;
	}

	fout.close();

	return make_pair(noIDnum /double(k), ((myDistSum - optDistSum)) / optDistSum);
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
	fout.open(dir);

	for (int i = 0; i < avgSpeed.size(); i++) {
		fout << "DataSet " << i+1 << endl;
		fout << avgSpeed[i] << " ms" << endl;
	}
	

	fout.close();
}

// printSpeedFinal(resultDir + "\\" + "speedSum.txt", speedSum);
void printSpeedFinal(string dir, vector<long long int> speedSum) {
	ofstream fout;
	fout.open(dir);

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
	fout.open(dir);
	// fout.open(resultDir + "\\" + "result.txt");

	fout << fixed << setprecision(10);

	fout << "Num Error: ";
	for (auto& er : numErrorSumsAll) fout << er / (numQueries * numDatasets) * 100.0 << " ";
	fout << endl;

	fout << "Dist Error: ";
	for (auto& er : distErrorSumsAll) fout << er / (numQueries * numDatasets) * 100.0 << " ";
	fout << endl;

	fout.close();
}

void autoTest() {
	
	remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\accuracyError.txt");
	remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\distanceError.txt");
	remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\result.txt");
	remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\log.txt");
	remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\speed.txt");
	remove("C:\\Users\\HWI\\Documents\\epsGraphTestResult\\speedPerEach.txt");

	bool speedFlag = true;

	int dimension = 4;
	// double epsilon = 50.0;
	double epsilon = 65.0;
	// double epsilon = 12.5;
	// double bound = ;

	// 100개의 데이터셋
	int numDatasets = 100;
	// 1000번의 랜덤 쿼리
	int numQueries = 1000;

	// kNN에서 k값
	vector<int> ks = {10, 50, 100, 500, 1000};

	std::string resultDir = "C:\\Users\\\HWI\\Documents\\epsGraphTestResult";
	fs::path dataDirDir = "C:\\Users\\\HWI\\Documents\\epsGraphTestData";
	if (speedFlag) dataDirDir = "C:\\Users\\\HWI\\Documents\\epsGraphTestDataSpeed";

	fs::directory_iterator iterDirDir(dataDirDir);

	int curDataSetId = 0;

	vector<long long> speedSum = {0, 0, 0, 0, 0};
	vector<double> avgSpeed = {0.0, 0.0, 0.0, 0.0, 0.0};

	vector<double> distErrorSumsAll = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	vector<double> numErrorSumsAll = { 0.0, 0.0, 0.0, 0.0, 0.0 };

	vector<double> distErrorSumsAllDijk = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	vector<double> numErrorSumsAllDijk = { 0.0, 0.0, 0.0, 0.0, 0.0 };

	for (auto& i00 = iterDirDir; i00 != fs::end(iterDirDir); ++i00) {

		curDataSetId++;
		if (speedFlag) {}
			// if (curDataSetId % 20 != 10) continue;
			// if (curDataSetId % 20 != 0) continue;

		// if (curDataSetId <= 5) continue;
		// 폴더가 2개 있음. polytopes 그리고 points

		fs::path dataDir = (*i00).path();
		// string polytopesDirStr = dataDir.string() +  "\\polytopes";
		// string pointsDirStr = dataDir.string() + "\\points";
		fs::path polytopesDir(dataDir.string() + "\\polytopes");

		vector<vector<Polytope*>> multiTopes;
		// vector<Polytope*> singleTope
		// vector<vector<Polytope>> multiTopesNoPtr;
		// multiTopesNoPtr.assign(multiTopes.size(), {});

		vector<Polytope> multiTopesNoPtr;

		// 각각의 polytope이 폴더 안에 있음
		fs::directory_iterator iterTopes(polytopesDir);

		// ex) \\polytopes\\0\\ 니까 매 폴더마다 하나의 polytope,
		// 해서 총 10개의 polytope 체크.

		for (auto& i01 = iterTopes; i01 != fs::end(iterTopes); ++i01) {
			// break;

			fs::path topeDir = (*i01).path();
			// topeDir.string();

			multiTopes.push_back(dels2polytopes(topeDir.string(), 1));
		}

		// multiTopes에서 multiTopesNoPtr로 변경
		for (int j = 0; j < multiTopes.size(); j++) {
			for (auto singleConvexTope : multiTopes[j]) {
				// multiTopesNoPtr[j].push_back(*singleConvexTope);
				multiTopesNoPtr.push_back(*singleConvexTope);
			}
		}

		// ex) C:\Users\HWI\Documents\epsGraphTestData\000\points
		fs::path pointsDir(dataDir.string() + "\\points");

		std::string pointsSpecificDir = dataDir.string() + "\\points\\points.txt";

		// list<Free_Point> frpts(pts.begin(), pts.end());
		// vector<Point*> pts = makePointSet(pointsSpecificDir);
		// vector<Point> pts = makePointSetNoPtr(pointsSpecificDir);
		vector<Free_Point*> pts = makeFreePointSet(pointsSpecificDir);
		// vector<Free_Point> pts = makeFreePointSetNoPtr(pointsSpecificDir);

		// list<Free_Point> frpts(pts.begin(), pts.end());

		list<Free_Point> frpts;
		for (auto& pt : pts) {
			frpts.push_back(*pt);
		}

		// 준비된 free points와 polytopes 이용해 epsilon graph 생성
		Eps_Graph_nD epsGraph(dimension, frpts, multiTopesNoPtr, epsilon, resultDir + "\\" + "EG1");

		// 비교할 near-optimal epsilon graph 생성
		Eps_Graph_nD epsGraphOpt(dimension, frpts, multiTopesNoPtr, epsilon * 0.5, resultDir + "\\" + "EG2");

		// 쿼리 점들도 읽어 오기
		// convex polytope을 피해야 하기 때문에?

		std::string queriesSpecificDir = dataDir.string() + "\\points\\queries.txt";

		// size of queryPoints = numQueries
		vector<Free_Point> queryPoints = makeFreePointSetNoPtr(queriesSpecificDir);

		//  ks = {10, 50, 100, 500, 1000}의 각각에 해당
		vector<double> distErrorSums = {0.0, 0.0, 0.0, 0.0, 0.0};
		vector<double> numErrorSums = { 0.0, 0.0, 0.0, 0.0, 0.0 };

		if (speedFlag) {

			//auto start = chrono::high_resolution_clock::now();
			//for (int j = 0; j < numQueries; j++) epsGraph.kNN(queryPoints[j], 10, "");
			//auto stop = chrono::high_resolution_clock::now();

			auto start = chrono::high_resolution_clock::now();
			for (int j = 0; j < numQueries; j++) {
				if (j == 0) epsGraph.kNN(queryPoints[j], 10, "C:\\Users\\HWI\\Documents\\epsGraphTestResult\\kNNtimeline");
				else epsGraph.kNN(queryPoints[j], 10, "");
			}
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
			auto exe_time = duration.count();
			// speedSum[curDataSetId / 20] += exe_time;
			speedSum[curDataSetId / 20] += exe_time;
			// avgSpeed[curDataSetId / 20 - 1] += exe_time;
			printSpeedTemp(resultDir + "\\" + "speedPerEach.txt", curDataSetId, exe_time);
			// printSpeedTemp(resultDir + "\\" + "speedSum.txt", curDataSetId, exe_time);
		}
		else {
			for (int j = 0; j < numQueries; j++) {

				for (int ii = 0; ii < ks.size(); ii++) {
					auto& k = ks[ii];

					// 시간 측정
					auto pr = epsGraph.kNN(queryPoints[j], k, "");
					auto pr2 = epsGraphOpt.kNN(queryPoints[j], k, "");

					// auto prDijk = epsGraph.Dijkstra(queryPoints[j], k);
					// grid.Dijkstra(*q, 3);

					auto pr3 = printError(epsGraph, epsGraphOpt, pr, pr2, queryPoints[j], k, resultDir);

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


		if (speedFlag) {
			// for (auto& elem : avgSpeed) elem / numQueries;
			printSpeedFinal(resultDir + "\\" + "speed.txt", avgSpeed);
			printSpeedFinal(resultDir + "\\" + "speedSum.txt", speedSum);
		}
		else 
			printErrorFinal(resultDir + "\\" + "result.txt", numErrorSumsAll, distErrorSumsAll, numQueries, numDatasets);
		
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

int main() {
	
	// menu
	// 1 - 

	cout << "menu\n";
	cout << "0. auto test\n"; // data generation\n";
	cout << "1. generate point sites\n";
	cout << "2. read point sites\n";
	cout << "3. read polytopes\n";
	cout << "4. kNN query\n";
	cout << "Enter: ";
	int inVal; cin >> inVal;
	switch (inVal) {
		case 0:
			autoTest();
			break;
		case 1:
			// generatePoints();
			break;
		case 2:
			// readPoints();
			break;
		case 3:
			// readPolytopes();
			break;
		case 4:
			
			break;
	}



	// read input points

	// generate input points

	// read polytopes generated by python code




	//int data_g;
	//cin >> data_g;

	//if (data_g == 1) {
	//	data_generation();
	//}
	//else {
	//	run_nearest_neighbor();
	//}

	/*
	std::cout << "enter any word: ";
	string strrr;
	std::cin >> strrr;
	*/
	// vector<Polytope*> plts_p = dels2polytopes(num_parts);

	/*
	vector<Polytope> plts;

	for (vector<Polytope*>::iterator it = plts_p.begin(); it != plts_p.end(); it++) {
		plts.push_back(**it);
	}

	vector<Polytope> plts;
	Free_Point* p1 = new Free_Point({ 0., 20. });

	Free_Point* p2 = new Free_Point({ 0, -65. });
	Free_Point* p3 = new Free_Point({ 0., 1.5 });
	Free_Point* p4 = new Free_Point({ 100., 100. });
	Free_Point* p5 = new Free_Point({ -100., -100. });
	list<Free_Point> frpts = { *p1,*p2,*p3,*p4,*p5 };
	//Eps_Graph_nD g(3);
	//cout << "Number of Polytopes: " << plts.size() << endl;

	Eps_Graph_nD grid(2, frpts, plts, 10.0);
	
	Free_Point* q = new Free_Point({ 0., -20. });
	grid.add_freepts(q);
	grid.print_free_point();
	
	grid.print_kNN(*q, 2);
	grid.Dijkstra(*q);
	*/

	return 0;
	
}

// 리턴값 자체가 하나의 nonconvex polytope임
vector<Polytope*> dels2polytopes(string dir, int num_topes) {
	vector<Polytope*> ret;

	int dummy;

	//if (iter_num == 0) {
	//	dir = "C:\\qhull\\bin\\AAA" + s;
	//}
	//else {
	//	dir = "C:\\qhull\\bin\\AAA" + s +  " (" + iter_num + ")";
	//}

	// std::string index;
	std::vector<string> filenames = {""};

	// for (int count = 0; count < num_topes; count++) {
	// 
	//	/*
	//	std::cout << "Enter the input file index: ";
	//	std::cin >> index; filenames.push_back(index);
	//	*/

	//	string res = "";
	//	if (count < 10) {
	//		res += "00";
	//	}
	//	else if (count < 100) {
	//		res += "0";
	//	}
	//	res += std::to_string(count);//  + ".txt";
	//	filenames.push_back(res);
	//}

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

bool isNumeric(std::string const& str)
{
	auto it = str.begin();
	while (it != str.end() && std::isdigit(*it)) {
		it++;
	}
	return !str.empty() && it == str.end();
}

// ------------------------------------------------------------------------------------------------------------------------
// halfplane 생성 함수들
// dim: dimension
halfplane* gen_hp_random(int dim, int index) {
	halfplane* H = new halfplane;
	H->d = dim;

	std::uniform_real_distribution<double> uni_dist(0.0, 1.0);

	// constructing halfplane by generating d+1 random numbers
	// ax + by + c = 0 if 2d, ax + by + cz + d = 0 if 3d, and so on.

	for (int i = 0; i < dim + 1; i++) {
		// H->vals.push_back(round(dist(gen) * 100));
		H->vals.push_back(uni_dist(gen));
	}

	return H;
}

// center(주관적 원점)을 지나는 halfplane 하나를 만들어내기
// 어차피 비례하므로, 여기서는 수를 bound할 필요는 없음.
halfplane* gen_hp_thru_p(Point* center) {
	halfplane* hp = new halfplane;
	hp->d = center->n;

	std::uniform_real_distribution<double> uni_dist(0.0, 1.0);

	for (int i = 0; i < hp->d - 1; i++) {
		hp->vals[i] = uni_dist(gen);
	}
	
	// 마지막 값은 자동으로 결정됨
	hp->vals[hp->d - 1] = 0;
	for (int i = 0; i < hp->d - 1; i++) {
		hp->vals[hp->d - 1] -= hp->vals[i] * center->getx(i);
	}
	
	return hp;
}

// slice the bounding box with parallel hyperplanes
// bounding box는 main함수에서 제시되어 있음
//  x축 (첫번째 coordinate) 기준으로 자르기.
// 이렇게 만들어진 예시: 항아리 모양
// 리턴값은 center들의 vector
// num_cuts: 나누는 cut의 개수. subpart의 개수는 따라서 num_cuts + 1이 됨.
std::vector<halfplane*> x_slice(int u_bnd, int dim, int num_cuts) {
	std::vector<halfplane*> ret;

	double x_start = -u_bnd;
	double x_move = 2 * u_bnd / double(num_cuts + 1);

	for (int i = 1; i <= num_cuts; i++) {
		halfplane* H = new halfplane();
		for (int j = 0; j < dim; j++) {
			if (j == 0) H->vals[j] = 1.0;
			else H->vals[j] = 0.0;
		}
		H->vals[dim] = x_start + x_move * i;

		ret.push_back(H);
	}

	return ret;
}

std::vector<halfplane*> axis_slice(int u_bnd, int dim, int my_dim, int num_cuts) {
	return {};
}

// ------------------------------------------------------------------------------------------------------------------------
// 점집합 생성 함수들


// void generate_points_on_bb(std::vector<std::pair<double, double>> bbx, int dis_pts, int max_iter) {
void generate_points_on_bb(cell* c, int dis_pts, int max_iter) {
	int dim = c->index.size();
	double real_max = DBL_MIN;
	for (const auto val : c->bbx) {
		double temp = max(abs(val.first), abs(val.second));
		if (temp > real_max) real_max = temp;
	}

	std::uniform_real_distribution<double> uni_dist(-real_max, real_max); // uniform distribution
	int iter = 0;
	while (iter < max_iter && dis_pts > c->in_points.size()) {
		iter++;

		Point* pt = new Point(dim);
		for (int i = 0; i < dim; i++) {
			pt->setx(i, uni_dist(gen));
		}

		// strict inequality 필요
		for (int i = 0; i < dim; i++) {
			double x = pt->getx(i);	
			if (x <= c->bbx[i].first || x >= c->bbx[i].second) continue;
		}
		
		c->in_points.push_back(pt);
	}
}

std::vector<Point*> gen_pts(int u_bound, halfplane* H, bool positive, int num, int max_iter) {
	int dim = H->vals.size() - 1;
	std::uniform_real_distribution<double> uni_dist(-u_bound, u_bound); // uniform distribution
	std::vector<Point*> ret = {};

	int iter = 0;
	while (ret.size() < num && iter < max_iter) {
		iter++;

		Point* pt = new Point(dim);
		for (int i = 0; i < dim; i++) {
			pt->setx(i, uni_dist(gen));
		}

		if ((positive && (hp_result(H, pt) > 0)) || (!positive && !(hp_result(H, pt) > 0))) ret.push_back(pt);
	}

	return ret;
}

/*
// 조건에 맞아떨어질 때까지 계속 랜덤으로 점을 뽑아서 점집합을 반환하는 함수
std::vector<Point*> brute_force(double bound, int num, halfplane* hp, bool b) {
	std::vector<Point*> ret;

	while (ret.size() < num) {
		Point* p = new Point(hp->d);

		// generate a random point

		if (b) {
			if (hp_result > 0) ret.push_back(p);
		}
		else {
			if (hp_result < 0) ret.push_back(p);
		}
	}

	return ret;
}
*/

// all points lie on n-dimensional sphere
// num_pts: size of the returned vector of 'Point'
// radius: radius of the sphere
// center: center of the sphere
std::vector<Point*> pts_on_sphere(int num_pts, Point* center, double radius, DIST_FLAG DF) {
	std::vector<Point*> ret; int num = 0;

	while (num < num_pts) {
		Point* p = new Point(center->n);
		// double val_left = pow(radius, 2);

		// std::random_device rd2;
		// std::mt19937 gen(rd2());
		// std::uniform_real_distribution<double> dist(); // uniform distribution
		if (DF == UNIFORM) {

			// need to improve
			// double revised_value = 2 * (radius / sqrt(center->n));
			// std::uniform_real_distribution<double> dist(-revised_value, revised_value);
			std::uniform_real_distribution<double> uni_dist(0.0, 1.0);

			std::vector<double> vec;
			for (int i = 0; i < center->n; i++) {
				vec.push_back(uni_dist(gen));
			}

			double square_sum = 0;
			for (auto val : vec) {
				square_sum += pow(val, 2);
			}

			double ratio = pow(radius, 2) / square_sum;

			for (auto val : vec) {
				val = sqrt(pow(val, 2) * ratio);
			}
			
			for (int i = 0; i < center->n; i++) {
				if (uni_dist(gen) > 0.5) {
					p->setx(i, center->getx(i) + vec[i]);
				}
				else {
					p->setx(i, center->getx(i) - vec[i]);
				}
			}

		}
		else {
			std::normal_distribution<double> norm_dist(-radius, radius);

			std::vector<double> vec;
			for (int i = 0; i < center->n; i++) {
				vec.push_back(norm_dist(gen));
			}

			double square_sum = 0;
			for (auto val : vec) {
				square_sum += pow(val, 2);
			}

			double ratio = pow(radius, 2) / square_sum;

			for (auto val : vec) {
				val = sqrt(pow(val, 2) * ratio);
			}

			for (int i = 0; i < center->n; i++) {
				if (norm_dist(gen) > 0.5) {
					p->setx(i, center->getx(i) + vec[i]);
				}
				else {
					p->setx(i, center->getx(i) - vec[i]);
				}
			}
		}

		ret.push_back(p);
	}

	return ret;
}

// halfplane 위에 점 생성. bounding box도 아닌데 왜 bb?
void generate_points_on_bb(std::vector<std::pair<double, double>> bbx, halfplane* H, int dim, int con_pts, int max_iter) {
	auto& ret = H->on_points;
	// std::vector<Point*> ret;

	int u_bound = bbx[0].second;
	int num = 0; int iter = 0;

	while (num < con_pts && iter < max_iter) {
		iter++;

		// 앞 d-1개의 값을 랜덤하게 고름
		// u_bound / 5 와 같은 식으로, 범위를 조금 줄이기?
		// 위의 점을 잡은 다음 평면 위에서 움직여서 점 찍기?
		std::uniform_real_distribution<double> uni_dist(-u_bound, u_bound); // uniform distribution
		std::vector<double> vec;

		for (int i = 0 ; i < dim-1; i++) {
			vec.push_back(uni_dist(gen));
		}

		// halfplane 정보 이용하여 마지막 1개 값을 계산
		double val = H->vals[dim];
		for (int i = 0; i < dim - 1; i++) {
			val += H->vals[i] * vec[i];
		}
		val = -val;
		val /= H->vals[dim-1];

		// 계산된 값 역시 bounding box 안에 있다면 해당 점을 ret에 추가, num 증가
		if (abs(val) < abs(u_bound)) {
			vec.push_back(val);
			Point* pt = new Point(vec);

			ret.push_back(pt);
			num++;
		}
	}
}

// for grid
void generate_points_on_restricted_hp(std::vector<std::pair<double, double>> bbx, halfplane* H, vector<int> grid_size, vector<int> index, int con_pts, int max_iter) {
	// 대응되는 point set이 비어있지 않다면(이미 한번 불렸다면) 바로 종료
	if (!H->on_points.empty()) return;
	
	int dim = index.size();
	
	// compute restriction
	// vector<pair<double, double>> restriction;
	for (int i = 0; i < index.size(); i++) {
		if (index[i] == -1) {
			H->restriction[i] = make_pair(H->vals[dim] - EPS_DG, H->vals[dim] + EPS_DG);
			//for (int j = 0; j < dim; j++) {
			//	if (abs(H->vals[j]) > EPS_DG) 
			//	H->restriction[i] = make_pair(-H->vals[dim] - EPS_DG, -H->vals[dim] + EPS_DG);
			//}
		}
		else {
			H->restriction[i] = make_pair(bbx[i].first + (bbx[i].second - bbx[i].first) * index[i] / grid_size[i], 
				bbx[i].first + (bbx[i].second - bbx[i].first) * (index[i] + 1) / grid_size[i]);
		}
	}
	
	auto& ret = H->on_points;

	int u_bound = bbx[0].second;
	int num = 0; int iter = 0;

	while (num < con_pts && iter < max_iter) {
		iter++;

		// 앞 d-1개의 값을 랜덤하게 고름
		// u_bound / 5 와 같은 식으로, 범위를 조금 줄이기?
		// 위의 점을 잡은 다음 평면 위에서 움직여서 점 찍기?
		std::uniform_real_distribution<double> uni_dist(-u_bound, u_bound); // uniform distribution
		std::vector<double> vec;

		for (int i = 0; i < dim; i++) {
			if (index[i] == -1) vec.push_back(H->vals[dim]);
			else vec.push_back(uni_dist(gen));
		}

		for (int j = 0; j < H->restriction.size(); j++) { //  auto pr : restriction) {
			if (H->restriction[j].first > vec[j] || H->restriction[j].second < vec[j]) continue;
		}
		Point* pt = new Point(vec);

		ret.push_back(pt);
		num++;
	}
}

// ------------------------------------------------------------------------------------------------------------------------
// 점집합 분리 함수들
// bounding box의 정보는 필요 없음
// halfplane 하나 가지고, 3개의 subset으로 나눔
// 0, 양수, 음수
std::pair<std::vector<Point*>, std::vector<Point*>> separate_pts(halfplane* hp, std::vector<Point*> pts) {

	std::pair<std::vector<Point*>, std::vector<Point*>> ret;
	// std::vector<std::vector<int>> ret;

	for (int i = 0; i < pts.size(); i++) {
		double val = hp_result(hp, pts[i]);

		if (abs(val) < EPS_DG) {}
		else if (val > 0) ret.first.push_back(pts[i]);
		else ret.second.push_back(pts[i]);

		// if (abs(val) < EPS_DG) ret[0].push_back(i);
		// else if (val > 0) ret[1].push_back(i);
		// else ret[2].push_back(i);
	}

	return ret;
}

// ------------------------------------------------------------------------------------------------------------------------
// 기타 기본 함수들

// halfplane 기준으로 점이 어느 곳에 위치하는지 테스트하는 함수 
// 리턴값은 양수, 0, 음수 중 하나. halfplane 위에 있을 경우 0을 반환
double hp_result(halfplane* hp, Point* p) {
	// halfplane과 point가 각각 가리키는 차원이 다른 경우 강제로 오류 발생시키기
	if (p->n != hp->d) exit(1);

	double ret = 0.0;

	for (int i = 0; i < p->n; i++) {
		ret += hp->vals[i] * p->getx(i);
	}

	return ret;
}

// ------------------------------------------------------------------------------------------------------------------------
// print 함수들

// halfplane 출력 함수
void print_halfplane(halfplane* H, int index) {
	int dim = H->vals.size() - 1;
	// std::cout << dim << "-dimensional halfplane" << " \#" << index << " generated: " << "(";
	std::cout << dim << "-dimensional halfplane" << " \#" << index << " (";
	for (int i = 0; i < dim; i++) {
		std::cout << H->vals[i] << ", ";
	}
	std::cout << H->vals[dim] << ")" << std::endl;
}

// point 출력 함수
void print_pt(Point* p, int index) {
	int dim = p->getxs().size();
	std::cout << dim << "-dimensional point" << " \#" << index << " (";
	for (int i = 0; i < dim-1; i++) {
		std::cout << p->getx(i) << ", ";
	}
	std::cout << p->getx(dim-1) << ")" << std::endl;
}

// ------------------------------------------------------------------------------------------------------------------------
// 점 변경 함수

// 주어진 점이 bounding box 안에 들어가도록 scaling
void scale_pt(int u_bound, Point* pt) {
	double max_coord = DBL_MIN;
	for (auto coord : pt->getxs()) {
		if (max_coord < abs(coord)) {
			coord = max_coord;
		}
	}

	if (max_coord > u_bound) {
		auto vec = pt->getxs();
		for (auto val : vec) val *= u_bound / (2 * max_coord);
	}
}

// ------------------------------------------------------------------------------------------------------------------------
// grid 관련 함수
vector<cell*> generate_grid(std::vector<std::pair<double, double>> bbx, vector<int> num_subspaces_over_axes) {
	int dim = num_subspaces_over_axes.size();
	vector<cell*> cells;
	int total_num_cells = 1;
	for (auto num : num_subspaces_over_axes) total_num_cells *= num;

	vector<int> denominators = compute_denominators(num_subspaces_over_axes);

	// generate all cells
	for (int i = 0; i < total_num_cells; i++) {
		auto c = new cell(i);
		c->index = num2index(num_subspaces_over_axes, denominators, i);
		for (int j = 0; j < c->index.size(); j++) { //  auto val : bbx) {
			c->bbx.push_back(make_pair(bbx[j].first + (bbx[j].second - bbx[j].first) * c->index[j] / num_subspaces_over_axes[j],
				bbx[j].first + (bbx[j].second - bbx[j].first) * (c->index[j] + 1) / num_subspaces_over_axes[j]));
		}
		cells.push_back(c);
	}

	// set neighbor infos (at most 2d)
	for (int i = 0; i < total_num_cells; i++) {
		auto c = cells[i];
		auto index = cells[i]->index;
		for (int j = 0; j < dim; j++) {
			auto index_copy1 = index; index_copy1[j]++;
			auto index_copy2 = index; index_copy2[j]--;
			vector<vector<int>> index_copies = { index_copy1, index_copy2 };
			
			for (auto index_copy : index_copies) {
				int new_num = index2num(num_subspaces_over_axes, denominators, index_copy);
				if (new_num != -1 && i < new_num) {
					auto H = new halfplane(dim);
					for (int j = 0; j < dim; j++) {
						// 부등호가 있으므로 한쪽 방으로만 cell 만들어짐
						if (c->index[j] != cells[new_num]->index[j]) {
							// 해당하는 halfplane을 만듬
							H->vals[j] = 1.0;
							H->vals[dim] = -(bbx[j].first + (bbx[j].second - bbx[j].first) * max(c->index[j], cells[new_num]->index[j]) / num_subspaces_over_axes[j]);
							break;
						}
					}
					c->neighbors.push_back(make_pair(cells[new_num], H));
					cells[new_num]->neighbors.push_back(make_pair(c, H));
				}
			}
		}
	}

	return cells;
}

vector<int> compute_denominators(vector<int> num_subspaces_over_axes) {
	// of dim d-1
	vector<int> denominators;
	int denominator = 1;
	for (int i = num_subspaces_over_axes.size() - 1; i > 0; i--) {
		denominator *= num_subspaces_over_axes[i];
		denominators.push_back(denominator);
	}
	reverse(denominators.begin(), denominators.end());

	return denominators;
}

vector<int> num2index(vector<int> num_subspaces_over_axes, vector<int> denominators, int num) {
	vector<int> index;

	// copy i to compute index
	for (int j = 0; j < denominators.size(); j++) {
		if (j == denominators.size() - 1) {
			index.push_back(num / denominators[j]);
			index.push_back(num % denominators[j]);
		}
		else {
			index.push_back(num / denominators[j]);
			num %= denominators[j];
		}
	}

	return index;
}

int index2num(vector<int> num_subspaces_over_axes, vector<int> multipliers, vector<int> index) {
	int dim = num_subspaces_over_axes.size();

	// index 벗어난 경우는 -1 리턴
	for (int i = 0; i < index.size(); i++) {
		if (index[i] < 0 || index[i] >= num_subspaces_over_axes[i]) return -1;
	}

	int num = 0;
	for (int i = 0; i < dim - 1; i++) {
		num += index[i] * multipliers[i];
	}
	num += index[dim - 1];

	return num;
}

void activate_cells(vector<cell*> cells, int num_parts) {
	for (int i = 0; i < num_parts; i++) cells[i]->active = true;
}

void disperse_pts_between_active_cells(std::vector<std::pair<double, double>> bbx, vector<int> num_subspaces_over_axes, vector<cell*> cells, int num) {
	int dim = num_subspaces_over_axes.size();

	for (int i = 0; i < cells.size(); i++) {
		auto& c = cells[i];
		for (auto& nb : c->neighbors) {
			// c->index 대신에 nb->index 넣어도 결과는 같음
			// 두 번 불릴 텐데, 그 중에 한 번만 실행될 것이라 문제는 없음
			if (c->active && nb.first->active) {

				auto copy_index = c->index;
				for (int j = 0; j < copy_index.size(); j++) {
					if (c->index[j] != nb.first->index[j]) copy_index[j] = -1;
				}

				if (find(copy_index.begin(), copy_index.end(), -1) == copy_index.end()) continue;
				generate_points_on_restricted_hp(bbx, nb.second, num_subspaces_over_axes, copy_index, num);
			}
		}
	}
}

//// ------------------------------------------------------------------------------------------------------------------------

// random point generation
// bounding box (d-dimensional rectangular box) 내에서 (uniformly) random하게 point 만들기
vector<Point*> generate_point_sites(std::vector<std::pair<double, double>> bbx, int dim, int num_points, int TYPE) {
	vector<Point*> ret;

	int cur_num_points = 0;
	while (cur_num_points < num_points) {
		vector<double> coords;
		for (auto axis_val_pair : bbx) {
			// uniform
			if (TYPE == 0) {
				std::uniform_real_distribution<double> uni_dist(axis_val_pair.first, axis_val_pair.second);
				coords.push_back(uni_dist(gen));
			}
			// clustered
			if (TYPE == 1) {
				
			}
		}
		auto pt = new Point(coords);
		pt->is_Free_Point = true;
		ret.push_back(pt);
		cur_num_points++;
	}

	return ret;
}


//// ------------------------------------------------------------------------------------------------------------------------

/*


	//case 1: {
	//	std::uniform_real_distribution<double> uni_dist(-u_bound, u_bound); // uniform distribution
	//	break;
	//	}
	//}

	//// ------------------------------------------------------------------------------------------------------------------------
	//// generate halfplanes

	//// for now, only 1 halfplane
	//// std::cout << "Number of halfplanes to generate: ";
	//// int num_h;
	//// std::cin >> num_h;

	//std::cout << std::endl;
	//int num_h = num_parts - 1;

	//std::cout << "Method for halfplane generation (0. Random, 1. Centered)" << std::endl;
	//std::cout << "Select: ";
	//int gen_method; std::cin >> gen_method;

	//// std::vector<HP>; // halfplanes
	//std::vector<halfplane*> halfplanes; // halfplane

	//switch (gen_method) {
	//	// generate halfplanes at random
	//	case 0:
	//		for (int j = 0; j < num_h; j++) {
	//			auto H = gen_hp_random(d, j);
	//			halfplanes.push_back(H);
	//			print_halfplane(H, j);
	//			std::cout << "Halfplane \#" << j << " generated" << std::endl;
	//		}
	//		break;

	//	// generate halfplanes, each centered at a point
	//	case 1:
	//		for (int j = 0; j < num_h; j++) {
	//			std::cout << "Dimension: " << d << std::endl;
	//			std::cout << "- Enter Point\#" << j << "Info -" << std::endl;

	//			Point* pt = new Point;
	//			for (int i = 0; i < d; i++) {
	//				std::cout << "Enter " << i << "-th value: ";
	//				double val; std::cin >> val;
	//				pt->setx(i, val);
	//			}
	//			auto H = gen_hp_thru_p(pt);
	//			halfplanes.push_back(H);
	//			print_halfplane(H, j);
	//			std::cout << "Halfplane \#" << j << " generated" << std::endl;
	//		}
	//		break;
	//}

	//// ------------------------------------------------------------------------------------------------------------------------
	//// disperse certain amount of points on the halfplanes

	//// con_pts stands for convex points
	//std::cout << std::endl;
	//std::cout << "Disperse points onto halfplanes (0. General, 1. Each)" << std::endl;
	//std::cout << "Select: ";
	//int num_method; std::cin >> num_method;
	//switch (num_method) {
	//case 0:
	//	std::cout << "Number of points: ";
	//	int dis_pts; std::cin >> dis_pts;
	//	for (int i = 0; i < num_h; i++) {
	//		auto H = halfplanes[i];
	//		generate_points_on_bb(bbx, halfplanes[i], d, dis_pts);
	//		for (int j = 0; j < H->on_points.size(); j++) {
	//			print_pt(H->on_points[j], j);
	//		}
	//	}
	//	break;
	//case 1:
	//	for (int i = 0; i < num_h; i++) {
	//		auto H = halfplanes[i];

	//		std::cout << "Number of points for halfplane \#";
	//		int dis_pts; std::cin >> dis_pts;
	//		generate_points_on_bb(bbx, halfplanes[i], d, dis_pts);
	//		for (int j = 0; j < H->on_points.size(); j++) {
	//			print_pt(H->on_points[j], j);
	//		}
	//	}
	//	break;
	//}

	//std::vector<std::vector<Point*>> pos_parts;
	//std::vector<std::vector<Point*>> neg_parts;

	//// point sets on both sides, union them with the dispersed ones
	//std::cout << std::endl;
	//for (int i = 0; i < num_h; i++) {
	//	auto H = halfplanes[i];

	//	std::cout << "Number of points on the positive side: ";
	//	int pos_num; std::cin >> pos_num;
	//	auto pos_pts = gen_pts(u_bound, H, true, pos_num);
	//	pos_pts.insert(pos_pts.end(), H->on_points.begin(), H->on_points.end());
	//	for (int j = 0; j < pos_pts.size(); j++) {
	//		print_pt(pos_pts[j], j);
	//	}

	//	std::cout << std::endl;
	//	std::cout << "Number of points on the negative side: ";
	//	int neg_num; std::cin >> neg_num;
	//	auto neg_pts = gen_pts(u_bound, H, false, neg_num);
	//	neg_pts.insert(neg_pts.end(), H->on_points.begin(), H->on_points.end());
	//	for (int j = 0; j < neg_pts.size(); j++) {
	//		print_pt(neg_pts[j], j);
	//	}

	//	pos_parts.push_back(pos_pts);
	//	neg_parts.push_back(neg_pts);
	//}

	//// user-defined directory start
	//std::string dir("C:\\Users\\HWI\\Desktop\\qhull\\bin\\");
	//// std::string dir("C:\\Users\\hwikim\\Desktop\\ALLTAG\\2023\\202307\\20230719\\qhull-2020.2\\bin\\");
	//// std::string dir("C:\\Users\\HWI\\Desktop\\ALLTAG\\2023\\202307\\20230703\\postechDNN\\dnn\\NearestNeighbor\\EpsGraphnD\\data_generation");
	//// user-defined directory end

	//// std::string myst("pts.txt");
	//// std::ifstream fin(dir + myst);
	//int count = 0;

	//pos_parts.insert(pos_parts.end(), neg_parts.begin(), neg_parts.end());
	////for (int i = 0; i < num_parts; i++) {
	////
	////	auto pos_pts = pos_parts[i];
	////	auto neg_pts = neg_parts[i];
	//for (int i = 0; i < pos_parts.size(); i++) {
	//	auto pts = pos_parts[i];

	//	std::string res = "results.out";
	//	if (count < 10) {
	//		res += "00";
	//	}
	//	else if (count < 100) {
	//		res += "0";
	//	}
	//	res += std::to_string(count) + ".txt";

	//	std::ofstream fout(dir + res);

	//	// first line contains the dimension
	//	fout << d << std::endl;
	//	// second line contains the number of input points
	//	fout << pts.size() << std::endl;
	//	// remaining lines contain point coordinates
	//	for (auto pt : pts) {
	//		for (int j = 0; j < d-1; j++) {
	//			fout << pt->getx(j) << " ";
	//		}
	//		fout << pt->getx(d-1) << std::endl;
	//	}
	//	count++;

	//	fout.close();
	//}

	// std::string res; // connectivity graph
*/

