#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include<sstream>

int main() {

	std::cout << "Enter the dimension: ";
	int d; std::cin >> d;

	for (int i = 0; i < d - 1; i++) { // d-1���� ���� �������� �����ؼ� halfplane �����
		
	}

	// halfplane ���� ������(�ϴ��� 5�� fix) ������ �����ϴ� �� �Ѹ���



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
// qhull���� ����Ʈ ��������

// halfplane intersection?

// qhull input ��������
// for int 
