#include "../algorithm/errorTest.h"
// #include <Eigen/Dense>
#include "../Polytope.h"
#include "fstream"
// #include "filesystem"

using namespace std;

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

	int dimension = 4;
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

	std::vector<long long> speedSum = { 0, 0, 0, 0, 0 };
	std::vector<double> avgSpeed = { 0.0, 0.0, 0.0, 0.0, 0.0 };

	std::vector<double> distErrorSumsAll = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	std::vector<double> numErrorSumsAll = { 0.0, 0.0, 0.0, 0.0, 0.0 };

	std::vector<double> distErrorSumsAllDijk = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	std::vector<double> numErrorSumsAllDijk = { 0.0, 0.0, 0.0, 0.0, 0.0 };

	for (auto& i00 = iterDirDir; i00 != fs::end(iterDirDir); ++i00) {

		curDataSetId++;
		if (curDataSetId != useDataSetId) {
			// cout << "not same " << curDataSetId << " " << useDataSetId << endl;
			continue;
		}
		// else cout << "same " << curDataSetId;

		// if (speedFlag) {}
			// if (curDataSetId % 20 != 10) continue;
			// if (curDataSetId % 20 != 0) continue;

		// if (curDataSetId <= 5) continue;
		// 폴더가 2개 있음. polytopes 그리고 points

		fs::path dataDir = (*i00).path();
		// string polytopesDirStr = dataDir.string() +  "\\polytopes";
		// string pointsDirStr = dataDir.string() + "\\points";
		fs::path polytopesDir(dataDir.string() + "\\polytopes");

		std::vector<std::vector<Polytope*>> multiTopes;
		// vector<Polytope*> singleTope
		// vector<vector<Polytope>> multiTopesNoPtr;
		// multiTopesNoPtr.assign(multiTopes.size(), {});

		std::vector<Polytope*> multiTopesNoPtr;

		// 각각의 polytope이 폴더 안에 있음
		fs::directory_iterator iterTopes(polytopesDir);

		// ex) \\polytopes\\0\\ 니까 매 폴더마다 하나의 polytope,
		// 해서 총 10개의 polytope 체크.

		std::cout << "before reading inputs" << std::endl;

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
				multiTopesNoPtr.push_back(singleConvexTope);
			}
		}

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

		// 준비된 free points와 polytopes 이용해 epsilon graph 생성

		// Eps_Graph_nD epsGraph(dimension, frpts, multiTopesNoPtr, epsilon, resultDir + "\\" + "EG1");
		auto* epsGraph =
			new Eps_Graph_nD(dimension, frpts, multiTopesNoPtr,
				epsilon, resultDir + "\\" + "EG1");

		if (checkMemory) {
			epsGraph->checkMemory("C:\\Users\\HWI\\Desktop\\finalResult");
			return;
		}

		std::cout << "constructed epsGraph1" << std::endl;

		// copy for epsGraph2
		vector<Polytope*> multiTopesNoPtr2;

		list<Free_Point*> frpts2;

		Eps_Graph_nD* epsGraphOpt = nullptr;
		if (!speedFlag) {

			for (auto& tope : multiTopesNoPtr) {
				multiTopesNoPtr2.push_back(new Polytope(*tope));
			}

			for (auto& p : frpts) {
				frpts2.push_back(new Free_Point(*p));
			}

			epsGraphOpt =
				new Eps_Graph_nD(dimension, frpts2, multiTopesNoPtr2,
					epsilon * 0.5, resultDir + "\\" + "EG2");
			std::cout << "constructed epsGraph2" << std::endl;
		}


		// 비교할 near-optimal epsilon graph 생성
		// Eps_Graph_nD epsGraphOpt(dimension, frpts, multiTopesNoPtr, epsilon * 0.5, resultDir + "\\" + "EG2");

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

		if (speedFlag) {

			//auto start = chrono::high_resolution_clock::now();
			//for (int j = 0; j < numQueries; j++) epsGraph.kNN(queryPoints[j], 10, "");
			//auto stop = chrono::high_resolution_clock::now();

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

pair<double, double> printErrorDijk(Eps_Graph_nD& epsGraph, pVV& pr,
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


pair<double, double> printError(Eps_Graph_nD* epsGraph1, Eps_Graph_nD* epsGraph2,
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