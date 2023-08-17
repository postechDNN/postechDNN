#include "data_generation.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <tuple>
#include <string>

std::random_device rd;
std::mt19937 gen(rd());

int main() {
	std::cout.precision(3);
	std::cout << std::fixed;
	// std::string config("config.ini");

	// ------------------------------------------------------------------------------------------------------------------------
	// dimension d
	std::cout << "Enter the dimension: ";
	int d; std::cin >> d;

	// ------------------------------------------------------------------------------------------------------------------------
	// generate a bounding box

	// defines a bounding box around the origin
	std::cout << std::endl;
	std::cout << "Maximum value for each coordinate: ";
	double u_bound; std::cin >> u_bound;

	// rectangular bounding box
	std::vector<std::pair<double, double>> bbx;

	for (int i = 0; i < d; i++) bbx.push_back(std::make_pair(-u_bound, u_bound));
	// double upper_bound = 10.0; // maximum value for each coordinate
	// double lower_bound = -upper_bound; // minimum value for each coordinate

	std::cout << std::endl;
	std::cout << "Number of convex subparts: ";
	int num_parts; std::cin >> num_parts;

	std::uniform_real_distribution<double> uni_dist(-u_bound, u_bound); // uniform distribution

	// ------------------------------------------------------------------------------------------------------------------------
	// generate halfplanes

	// for now, only 1 halfplane
	// std::cout << "Number of halfplanes to generate: ";
	// int num_h; 
	// std::cin >> num_h;

	std::cout << std::endl;
	int num_h = num_parts - 1;

	std::cout << "Method for halfplane generation (0. Random, 1. Centered)" << std::endl;
	std::cout << "Select: ";
	int gen_method; std::cin >> gen_method;

	// std::vector<HP>; // halfplanes
	std::vector<halfplane*> halfplanes; // halfplane

	switch (gen_method) {
		// generate halfplanes at random
		case 0:
			for (int j = 0; j < num_h; j++) {
				auto H = gen_hp_random(d, j);
				halfplanes.push_back(H);
				print_halfplane(H, j);
			}
			break;

		// generate halfplanes, each centered at a point
		case 1:
			for (int j = 0; j < num_h; j++) {
				std::cout << "Dimension: " << d << std::endl;
				std::cout << "- Enter Point\#" << j << "Info -" << std::endl;

				Point* pt = new Point;
				for (int i = 0; i < d; i++) {
					std::cout << "Enter " << i << "-th value: ";
					double val; std::cin >> val;
					pt->setx(i, val);
				}
				auto H = gen_hp_thru_p(pt);
				halfplanes.push_back(H);
				print_halfplane(H, j);
				std::cout << "Halfplane \#" << j << " generated" << std::endl;
			}
			break;
	}

	// ------------------------------------------------------------------------------------------------------------------------
	// disperse certain amount of points on the halfplanes

	// con_pts stands for convex points
	std::cout << std::endl;
	std::cout << "Disperse points onto halfplanes (0. general, 1. each)" << std::endl;
	std::cout << "Select: ";
	int num_method; std::cin >> num_method;
	switch (num_method) {
	case 0:
		std::cout << "Number of points: ";
		int dis_pts; std::cin >> dis_pts;
		for (int i = 0; i < num_h; i++) {
			auto H = halfplanes[i];
			generate_points_on_bb(bbx, halfplanes[i], d, dis_pts);
			for (int j = 0; j < H->on_points.size(); j++) {
				print_pt(H->on_points[j], j);
			}
		}
		break;
	case 1:
		for (int i = 0; i < num_h; i++) {
			auto H = halfplanes[i];

			std::cout << "Number of points for halfplane \#";
			int dis_pts; std::cin >> dis_pts;
			generate_points_on_bb(bbx, halfplanes[i], d, dis_pts);
			for (int j = 0; j < H->on_points.size(); j++) {
				print_pt(H->on_points[j], j);
			}
		}
		break;
	}

	std::vector<std::vector<Point*>> pos_parts;
	std::vector<std::vector<Point*>> neg_parts;

	// point sets on both sides, union them with the dispersed ones
	std::cout << std::endl;
	for (int i = 0; i < num_h; i++) {
		auto H = halfplanes[i];

		std::cout << "Number of points on the positive side: ";
		int pos_num; std::cin >> pos_num;
		auto pos_pts = gen_pts(u_bound, H, true, pos_num);
		pos_pts.insert(pos_pts.end(), H->on_points.begin(), H->on_points.end());
		for (int j = 0; j < pos_pts.size(); j++) {
			print_pt(pos_pts[j], j);
		}

		std::cout << "Number of points on the negative side: ";
		int neg_num; std::cin >> neg_num;
		auto neg_pts = gen_pts(u_bound, H, false, neg_num);
		neg_pts.insert(neg_pts.end(), H->on_points.begin(), H->on_points.end());
		for (int j = 0; j < neg_pts.size(); j++) {
			print_pt(neg_pts[j], j);
		}

		pos_parts.push_back(pos_pts);
		neg_parts.push_back(neg_pts);
	}

	// user-defined start
	std::string dir("C:\\Users\\HWI\\Desktop\\qhull\\bin\\");
	// std::string dir("C:\\Users\\hwikim\\Desktop\\ALLTAG\\2023\\202307\\20230719\\qhull-2020.2\\bin\\");
	// std::string dir("C:\\Users\\HWI\\Desktop\\ALLTAG\\2023\\202307\\20230703\\postechDNN\\dnn\\NearestNeighbor\\EpsGraphnD\\data_generation");
	// user-defined end

	// std::string myst("pts.txt");
	// std::ifstream fin(dir + myst);
	int count = 0;

	/*
	for (int i = 0; i < num_parts; i++) {
		
		auto pos_pts = pos_parts[i];
		auto neg_pts = neg_parts[i];

		std::string res = "results";
		if (count < 10) {

			std::string res( + std::to_string(i) + ".txt");

		}
		else if (count < 100) {

		}
		std::ofstream fout(dir + res);
		std::string res("results" + std::to_string(i) + ".txt");
		std::ofstream fout(dir + res);

		// first line contains the dimension
		fout << d << std::endl;
		// second line contains the number of input points
		fout << num_parts;
		// remaining lines contain point coordinates
		fout << "POINT_"
		fout << 
	}
	*/
}

// ------------------------------------------------------------------------------------------------------------------------
// halfplane ���� �Լ���
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

// center(�ְ��� ����)�� ������ halfplane �ϳ��� ������
// ������ ����ϹǷ�, ���⼭�� ���� bound�� �ʿ�� ����.
halfplane* gen_hp_thru_p(Point* center) {
	halfplane* hp = new halfplane;
	hp->d = center->n;

	std::uniform_real_distribution<double> uni_dist(0.0, 1.0);

	for (int i = 0; i < hp->d - 1; i++) {
		hp->vals[i] = uni_dist(gen);
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

// ------------------------------------------------------------------------------------------------------------------------
// ������ ���� �Լ���

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
// ���ǿ� �¾ƶ����� ������ ��� �������� ���� �̾Ƽ� �������� ��ȯ�ϴ� �Լ�
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

// halfplane ���� �� ����
void generate_points_on_bb(std::vector<std::pair<double, double>> bbx, halfplane* H, int dim, int con_pts, int max_iter) {
	auto& ret = H->on_points;
	// std::vector<Point*> ret;

	int u_bound = bbx[0].second;
	int num = 0; int iter = 0;

	while (num < con_pts && iter < max_iter) {
		iter++;

		// �� d-1���� ���� �����ϰ� ��
		// u_bound / 5 �� ���� ������, ������ ���� ���̱�?
		// ���� ���� ���� ���� ��� ������ �������� �� ���?
		std::uniform_real_distribution<double> uni_dist(-u_bound, u_bound); // uniform distribution
		std::vector<double> vec;

		for (int i = 0 ; i < dim-1; i++) {
			vec.push_back(uni_dist(gen));
		}

		// halfplane ���� �̿��Ͽ� ������ 1�� ���� ���
		double val = H->vals[dim];
		for (int i = 0; i < dim - 1; i++) {
			val += H->vals[i] * vec[i];
		}
		val = -val;
		val /= H->vals[dim-1];

		// ���� �� ���� bounding box �ȿ� �ִٸ� �ش� ���� ret�� �߰�, num ����
		if (abs(val) < abs(u_bound)) {
			vec.push_back(val);
			Point* pt = new Point(vec);

			ret.push_back(pt);
			num++;
		}
	}

	// return ret;
}

// ------------------------------------------------------------------------------------------------------------------------
// ������ �и� �Լ���
// bounding box�� ������ �ʿ� ����
// halfplane �ϳ� ������, 3���� subset���� ����
// 0, ���, ����
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

// ------------------------------------------------------------------------------------------------------------------------
// print �Լ���

// halfplane ��� �Լ�
void print_halfplane(halfplane* H, int index) {
	int dim = H->vals.size() - 1;
	// std::cout << dim << "-dimensional halfplane" << " \#" << index << " generated: " << "(";
	std::cout << dim << "-dimensional halfplane" << " \#" << index << " (";
	for (int i = 0; i < dim; i++) {
		std::cout << H->vals[i] << ", ";
	}
	std::cout << H->vals[dim] << ")" << std::endl;
}

// point ��� �Լ�
void print_pt(Point* p, int index) {
	int dim = p->getxs().size();
	std::cout << dim << "-dimensional point" << " \#" << index << " (";
	for (int i = 0; i < dim-1; i++) {
		std::cout << p->getx(i) << ", ";
	}
	std::cout << p->getx(dim-1) << ")" << std::endl;
}

// ------------------------------------------------------------------------------------------------------------------------
// �� ���� �Լ�

// �־��� ���� bounding box �ȿ� ������ scaling
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