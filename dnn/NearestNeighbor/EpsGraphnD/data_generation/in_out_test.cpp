#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include<sstream>

int main() {

	std::cout << "Enter the dimension: ";
	int d; std::cin >> d;

	for (int i = 0; i < d - 1; i++) { // d-1개의 숫자 랜덤으로 생성해서 halfplane 만들기
		
	}

	// halfplane 위에 적절한(일단은 5로 fix) 개수의 공유하는 점 뿌리기



	std::string myst("pts.txt");
	std::string dir("C:\\Users\\hwikim\\Desktop\\ALLTAG\\2023\\202305\\20230509\\qhull-2020.2\\bin\\");
	std::string res("results.txt");

	std::ifstream fin(dir + myst);
	std::ofstream fout(dir + res);
	
	std::string line;
	while (!fin.eof()) {
		getline(fin, line);

		std::string word;

		std::istringstream ss(line);
		while (ss >> word) {
			std::cout << word << std::endl;
		}
		
		// std::cout << line << std::endl;
	}
}

// bounding box
// qhull에서 포인트 가져오기

// halfplane intersection?

// qhull input 가져오기
// for int 
