#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include<sstream>
#include <random>

typedef struct halfplane {
	int d; // dimension
	std::vector<float> vals; // values to represent the halfplane. For example, (d=2) ax + by + c = 0, (d=3) ax + by + cz + d = 0.

} halfplane;

int main() {

	std::cout << "Enter the dimension: ";
	int d; std::cin >> d;
	std::cout << "Number of halfplanes to generate: ";
	int num_h; std::cin >> num_h;

	float upper_bound = 10.0; // maximum value for each coordinate
	float lower_bound = -10.0; // minimum value for each coordinate
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(lower_bound, upper_bound); // uniform distribution

	// std::vector<HP>; // halfplanes
	std::vector<halfplane*> halfplanes; // halfplane


	for (int j = 0; j < num_h; j++) {
		halfplane* H = new halfplane{ d };
		for (int i = 0; i < d + 1; i++) { // constructing halfplane by generating d+1 random numbers
			H->vals.push_back(dist(gen));
		}
		halfplanes.push_back(H); 
	}

	// disperse certain amount of points on the halfplane (for now, fixed as 5)

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

// bounding box?
// import points information from qhull

// halfplane intersection from qhull
