#include "data_generation.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <tuple>

std::random_device rd;
std::mt19937 gen(rd());

int main() {
	// std::string config("config.ini");

	// ------------------------------------------------------------------------------------------------------------------------
	// dimension d
	std::cout << "Enter the dimension: ";
	int d; std::cin >> d;

	// ------------------------------------------------------------------------------------------------------------------------
	// generate a bounding box

	// defines a bounding box around the origin
	std::cout << "Maximum value for each coordinate: ";
	double u_bound; std::cin >> u_bound;

	// rectangular bounding box
	std::vector<std::pair<double, double>> bbx;

	for (int i = 0; i < d; i++) bbx.push_back(std::make_pair(-u_bound, u_bound));
	// double upper_bound = 10.0; // maximum value for each coordinate
	// double lower_bound = -upper_bound; // minimum value for each coordinate

	std::cout << "Number of convex subparts: ";
	int num_parts; std::cin >> num_parts;

	std::uniform_real_distribution<double> dist(-u_bound, u_bound); // uniform distribution

	// ------------------------------------------------------------------------------------------------------------------------
	// generate halfplanes

	// for now, only 1 halfplane
	std::cout << "Number of halfplanes to generate: ";
	int num_h; std::cin >> num_h;

	std::cout << "1. Random, 2: Centered"<< std::endl;
	std::cout << "Method for halfplane generation: ";
	int gen_method; std::cin >> gen_method;

	// std::vector<HP>; // halfplanes
	std::vector<halfplane*> halfplanes; // halfplane

	switch (gen_method) {
		// generate halfplanes at random
		case 1:
			for (int j = 0; j < num_h; j++) {
				halfplanes.push_back(hp_random(d));
				break;
			}

		// generate halfplanes, each centered at a point
		case 2:
			for (int j = 0; j < num_h; j++) {
				std::cout << "Dimension: " << d << std::endl;
				std::cout << "- Enter Point\#" << j << "Info -" << std::endl;

				Point* pt = new Point;
				for (int i = 0; i < d; i++) {
					std::cout << "Enter " << i << "-th value: ";
					double val; std::cin >> val;
					pt->setx(i, val);
				}
				halfplanes.push_back(hp_thru_p(pt));
				std::cout << "Halfplane \#" << j << " generated" << std::endl;
			}
			break;
	}

	// for debug
	// print the halfplane infos
	for (auto H : halfplanes) {
		for (auto v : H->vals) {
			std::cout << v << " ";
		}
		std::cout << std::endl;
	}

	// ------------------------------------------------------------------------------------------------------------------------
	// disperse certain amount of points on the halfplanes

	// con_pts stands for convex points
	int con_pts; std::cin >> con_pts;

	for (int i = 0; i < halfplanes.size(); i++) {
		
	}


	// user-defined
	std::string myst("pts.txt");
	std::string dir("C:\\Users\\hwikim\\Desktop\\ALLTAG\\2023\\202307\\20230719\\qhull-2020.2\\bin\\");
	// std::string dir("C:\\Users\\HWI\\Desktop\\ALLTAG\\2023\\202307\\20230703\\postechDNN\\dnn\\NearestNeighbor\\EpsGraphnD\\data_generation");
	std::string res("results.txt");
	// user-defined

	std::ifstream fin(dir + myst);
	std::ofstream fout(dir + res);

	std::string line;
	while (!fin.eof()) {
		getline(fin, line);

		std::string word;

		// print the contents of "pts.txt"
		std::istringstream ss(line);
		while (ss >> word) {
			std::cout << word << std::endl;
		}

		// std::cout << line << std::endl;
	}

	// 형식에 맞춰서 output해야 하므로.
}

// ------------------------------------------------------------------------------------------------------------------------
// halfplane 생성 함수들
// dim: dimension
halfplane* hp_random(int dim) {
	halfplane* H = new halfplane{ dim };

	std::uniform_real_distribution<double> dist(0.0, 1.0);

	// constructing halfplane by generating d+1 random numbers
	// ax + by + c = 0 if 2d, ax + by + cz + d = 0 if 3d, and so on.
	for (int i = 0; i < dim + 1; i++) {
		H->vals.push_back(round(dist(gen)));
	}

	return H;

}

// center(주관적 원점)을 지나는 halfplane 하나를 만들어내기
// 어차피 비례하므로, 여기서는 수를 bound할 필요는 없음.
halfplane* hp_thru_p(Point* center) {
	halfplane* hp = new halfplane;
	hp->d = center->n;

	std::uniform_real_distribution<double> dist(0.0, 1.0);

	for (int i = 0; i < hp->d - 1; i++) {
		hp->vals[i] = dist(gen);
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
std::vector<halfplane*> x_slice(int num_cuts) {
	return {};
}

// ------------------------------------------------------------------------------------------------------------------------
// 점집합 생성 함수들

// 조건에 맞아떨어질 때까지 계속 랜덤으로 점을 뽑아서 점집합을 반환하는 함수
std::vector<Point*> brute_force(Point* center, double bound, int num, halfplane* hp, bool b) {
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
			std::uniform_real_distribution<double> dist(0.0, 1.0);

			std::vector<double> vec;
			for (int i = 0; i < center->n; i++) {
				vec.push_back(dist(gen));
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
				if (dist(gen) > 0.5) {
					p->setx(i, center->getx(i) + vec[i]);
				}
				else {
					p->setx(i, center->getx(i) - vec[i]);
				}
			}

		}
		else {
			std::normal_distribution<double> dist(-radius, radius);

			std::vector<double> vec;
			for (int i = 0; i < center->n; i++) {
				vec.push_back(dist(gen));
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
				if (dist(gen) > 0.5) {
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

// ------------------------------------------------------------------------------------------------------------------------
// 점집합 분리 함수들
// bounding box의 정보는 필요 없음
// halfplane 하나 가지고, 3개의 subset으로 나눔
// 0, 양수, 음수
std::vector<std::vector<int>> separate_pts(halfplane* hp, std::vector<Point*> pts) {

	std::vector<std::vector<int>> ret;

	for (int i = 0; i < pts.size(); i++) {
		double val = hp_result(hp, pts[i]);

		if (abs(val) < EPS_DG) ret[0].push_back(i);
		else if (val > 0) ret[1].push_back(i);
		else ret[2].push_back(i);
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

// 
struct graph_input {
	// std::vector<s>
};