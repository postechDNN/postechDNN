#include "Polygon.h"
#include <random>
#include <iostream>
#include <fstream>
#include <string>

double MIN_X = 0.0, MAX_X = 1000.0;
double MIN_Y = 0.0, MAX_Y = 1000.0;
std::random_device rd;
std::mt19937 gen(rd());

double __clip(double x, double min, double max);
SimplePolygon generatePolygon(double ctrX, double ctrY, double aveRadius, double irregularity, double spikeyness, int numVerts);
void generate_input(int type, int K, int n_points, int n_polys, int n_query, std::string fname , std::string fname_query);

int main() {
	int n_points;
	int n_polys;
	std::cout << "The number of points and the number of polygons(e.g 150000 100) : ";
	std::cin >> n_points >> n_polys;
	std::cout << "Distribution of points(1 : uniform, 2 : K-cluster) : ";
	int type;
	std::cin >> type;
	int n_query;
	std::cout << "The number of query points : ";
	std::cin >> n_query;
	
	int n_files;
	std::cout << "The number of files : ";
	
	std::cin >> n_files;
	std::string uniform_fname("uniform_distribution");
	std::string k_cluster_fname("k_cluster_distribution");
	std::uniform_int_distribution<int> dis_K(1,10);
	for (int i = 1;i <= n_files;i++) {
		if (type == 1) {
			generate_input(type, 1, n_points, n_polys,n_query ,uniform_fname + std::to_string(i) + std::string(".txt"), uniform_fname+ std::to_string(i) + std::string("_query") + std::string(".txt"));
		}
		else {
			int K = dis_K(gen);
			generate_input(type, K, n_points, n_polys, n_query,k_cluster_fname + std::to_string(i) + std::string(".txt"), k_cluster_fname + std::to_string(i) + std::string("_query") + std::string(".txt"));
		}
		std::cout << i << " files is completed" << std::endl;
	}

	
	return 0;
}

void generate_input(int type,int K, int n_points, int n_polys,int n_query, std::string fname, std::string fname_query) {
	int m = int(sqrt(n_polys)) + 1;
	int MAX_NUM_VERT = 10;
	double grid_width = (MAX_X - MIN_X) / m;
	double grid_height = (MAX_Y - MIN_Y) / m;

	std::uniform_real_distribution<double> dis_unit(0.0, 1);
	std::uniform_int_distribution<int> dis_numVerts(3, MAX_NUM_VERT);
	std::vector<SimplePolygon> polys;
	for (int i = 0; i < n_polys;i++) {
		int row = i / m;
		int col = i % m;
		double ctrX = row * grid_width + grid_width / 2;
		double ctrY = col * grid_height + grid_height / 2;
		double aveRadius = std::min(grid_height, grid_width) / 4;
		double irregurality = dis_unit(gen) * 0.7;
		double spikeness = dis_unit(gen) * 0.7;
		int numVerts = dis_numVerts(gen);
		SimplePolygon poly = generatePolygon(ctrX, ctrY, aveRadius, irregurality, spikeness, numVerts);
		polys.push_back(poly);
	}

	int added_pts = 0;
	std::vector<Point> pts;

	std::vector<double> ctrX(K), ctrY(K);
	double dev = std::min(MAX_X - MIN_X, MAX_Y - MIN_Y) / 20.0;
	for (int i = 0; i < K;i++) {
		ctrX[i] = dis_unit(gen) * (MAX_X - MIN_X) + MIN_X;
		ctrY[i] = dis_unit(gen) * (MAX_Y - MIN_Y) + MIN_Y;
	}
	std::normal_distribution<double> dis_normal(0, dev);

	int idx = 0;
	while (added_pts < n_points) {
		double x, y;
		if (type == 1) {	//Uniform distribution
			x = dis_unit(gen) * (MAX_X - MIN_X) + MIN_X;
			y = dis_unit(gen) * (MAX_Y - MIN_Y) + MIN_Y;
		}
		else {	//K-Cluster distribution
			idx = (idx + 1) % K;
			x = ctrX[idx] + dis_normal(gen);
			y = ctrY[idx] + dis_normal(gen);
			if (x < MIN_X || x > MAX_X || y < MIN_Y || y > MAX_Y)
				continue;
			//std::cout << x << " " << y << std::endl;
		}
		Point p(x, y);
		bool flag = true;
		for (int i = 0; i < n_polys;i++) {
			if (polys[i].inPolygon(p) >= 0) {
				flag = false;
				break;
			}
		}
		if (flag) {
			pts.push_back(p);
			added_pts++;
		}
	}
	std::ofstream out(fname);
	out << n_points << " " << n_polys << std::endl;
	for (auto i : pts)
		out << i.getx() << " " << i.gety() << std::endl;

	for (auto i : polys) {
		std::vector<Point> vertices = i.getVertices();
		out << vertices.size() << std::endl;
		for (auto j : vertices) out << j.getx() << " " << j.gety() << std::endl;
	}

	std::ofstream out_query(fname_query);
	out_query << n_query << std::endl;
	int added_query = 0;
	while (added_query < n_query) {
		double x, y;
		x = dis_unit(gen) * (MAX_X - MIN_X) + MIN_X;
		y = dis_unit(gen) * (MAX_Y - MIN_Y) + MIN_Y;
		int flag = true;
		Point p(x, y);
		for (int i = 0; i < n_polys;i++) {
			if (polys[i].inPolygon(p) >= 0) {
				flag = false;
				break;
			}
		}
		if (flag) {
			out_query << x << " " << y << std::endl;
			added_query++;
		}
	}
}

double __clip(double x, double min, double max) {
	if (min > max) return x;
	else if (x < min) return min;
	else if (x > max) return max;
	else return x;
}

SimplePolygon generatePolygon(double ctrX, double ctrY, double aveRadius, double irregularity, double spikeyness, int numVerts) {
	double PI = 3.1415926535897;
	irregularity = __clip(irregularity, 0, 1) * 2 * PI / numVerts;
	spikeyness = __clip(spikeyness, 0, 1) * aveRadius;

	std::vector<double> angleSteps;
	double lower = (2 * PI / numVerts) - irregularity;
	double upper = (2 * PI / numVerts) + irregularity;
	double sum = 0;
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int i = 0; i < numVerts;i++) {
		std::uniform_real_distribution<double> dis(lower, upper);
		double tmp = dis(gen);
		angleSteps.push_back(tmp);
		sum += tmp;
	}

	double k = sum / 2 / PI;
	for (int i = 0; i < numVerts;i++)
		angleSteps[i] /= k;

	std::vector<Point> points;
	std::uniform_real_distribution<double> dis(0, 2 * PI);
	double angle = dis(gen);
	for (int i = 0; i < numVerts;i++) {
		std::normal_distribution<double> dis(aveRadius, spikeyness);
		double r_i = __clip(dis(gen), 0, 2 * aveRadius);
		double x = ctrX + r_i * cos(angle);
		double y = ctrY + r_i * sin(angle);
		points.push_back(Point(x, y));
		angle = angle + angleSteps[i];
	}
	std::reverse(points.begin(), points.end());	//Make Clockwise order.
	SimplePolygon poly(points);
	return poly;
}