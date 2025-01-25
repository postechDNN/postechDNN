#include <string>
#include <iostream>
#include "../data_generation/data_generation.h"
#include "quadtree.h"

int main(int argc, char* argv[]) {

	/*
	// �Ʒ� switch�� �޴� ����
	int inVal = std::stoi(argv[1]); // *(argv[1]) - '0';

	// ���� ������ ����� ���丮
	auto argv2 = argv[2];
	std::string dir = argv[2];

	// ���� �ӵ� üũ(T), ���� ���� üũ(F)
	bool speedFlag = true;
	if (std::string(argv[3]) == "F") speedFlag = false;

	// grid step size
	std::string epsilonStr = argv[4];
	double epsilon = std::stod(epsilonStr);

	// �Է����� ����� ������ ���� �ε���
	int useDataSetId = std::stoi(argv[5]);

	// �Է��� polytope�� ��
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

	// manual �Է�
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