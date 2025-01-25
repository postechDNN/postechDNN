#include <string>
#include <iostream>
#include "../data_generation/data_generation.h"
#include "quadtree.h"

int main(int argc, char* argv[]) {

	/*
	// 아래 switch문 메뉴 선택
	int inVal = std::stoi(argv[1]); // *(argv[1]) - '0';

	// 최종 데이터 저장될 디렉토리
	auto argv2 = argv[2];
	std::string dir = argv[2];

	// 쿼리 속도 체크(T), 쿼리 오차 체크(F)
	bool speedFlag = true;
	if (std::string(argv[3]) == "F") speedFlag = false;

	// grid step size
	std::string epsilonStr = argv[4];
	double epsilon = std::stod(epsilonStr);

	// 입력으로 사용할 데이터 셋의 인덱스
	int useDataSetId = std::stoi(argv[5]);

	// 입력할 polytope의 수
	int numPolytopes = std::stoi(argv[6]);

	std::cout << "read arguments" << std::endl;
	*/

	//cout << "menu\n";
	//cout << "0. auto test\n"; // data generation\n";
	//cout << "1. generate point sites\n";
	//cout << "2. read point sites\n";
	//cout << "3. read polytopes\n";
	//cout << "4. kNN query\n";
	//cout << "Enter: ";

	// manual 입력
	int inVal = 0;
	string dir = "C:\\Users\\HWI\\Documents\\epsGraphTest";
	bool speedFlag = true;
	int useDataSetId = 0;
	double epsilon = 1;

	switch (inVal) {
	case 0:
		// buildEpsilonGraph();
		autoTest(dir, epsilon, speedFlag, useDataSetId);
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

	std::cout << "escaped from switch statement";

	return 0;

}