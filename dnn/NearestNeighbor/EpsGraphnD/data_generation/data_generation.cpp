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

	// ���Ŀ� ���缭 output�ؾ� �ϹǷ�.
}

// ------------------------------------------------------------------------------------------------------------------------
// halfplane ���� �Լ���
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

// center(�ְ��� ����)�� ������ halfplane �ϳ��� ������
// ������ ����ϹǷ�, ���⼭�� ���� bound�� �ʿ�� ����.
halfplane* hp_thru_p(Point* center) {
	halfplane* hp = new halfplane;
	hp->d = center->n;

	std::uniform_real_distribution<double> dist(0.0, 1.0);

	for (int i = 0; i < hp->d - 1; i++) {
		hp->vals[i] = dist(gen);
	}
	
	// ������ ���� �ڵ����� ������
	hp->vals[hp->d - 1] = 0;
	for (int i = 0; i < hp->d - 1; i++) {
		hp->vals[hp->d - 1] -= hp->vals[i] * center->getx(i);
	}
	
	return hp;
}

// slice the bounding box with parallel hyperplanes
// bounding box�� main�Լ����� ���õǾ� ����
//  x�� (ù��° coordinate) �������� �ڸ���.
// �̷��� ������� ����: �׾Ƹ� ���
// ���ϰ��� center���� vector
// num_cuts: ������ cut�� ����. subpart�� ������ ���� num_cuts + 1�� ��.
std::vector<halfplane*> x_slice(int num_cuts) {
	return {};
}

// ------------------------------------------------------------------------------------------------------------------------
// ������ ���� �Լ���

// ���ǿ� �¾ƶ����� ������ ��� �������� ���� �̾Ƽ� �������� ��ȯ�ϴ� �Լ�
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
// ������ �и� �Լ���
// bounding box�� ������ �ʿ� ����
// halfplane �ϳ� ������, 3���� subset���� ����
// 0, ���, ����
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
// ��Ÿ �⺻ �Լ���

// halfplane �������� ���� ��� ���� ��ġ�ϴ��� �׽�Ʈ�ϴ� �Լ� 
// ���ϰ��� ���, 0, ���� �� �ϳ�. halfplane ���� ���� ��� 0�� ��ȯ
double hp_result(halfplane* hp, Point* p) {
	// halfplane�� point�� ���� ����Ű�� ������ �ٸ� ��� ������ ���� �߻���Ű��
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