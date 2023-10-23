#include "data_generation.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <tuple>
#include <string>
#include "../algorithm/EpsGraphnD.h"
#include "../algorithm/Polytope.h"
#include "../algorithm/Point.h"
#include <typeinfo>

std::random_device rd;
std::mt19937 gen(rd());

bool PRINT_POINT_FLAG = false;

/*
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

	// defines a bounding box (cube) around the origin
	std::cout << std::endl;
	std::cout << "Maximum value for each coordinate: ";
	double u_bound; std::cin >> u_bound;

	// rectangular bounding box
	std::vector<std::pair<double, double>> bbx;

	for (int i = 0; i < d; i++) bbx.push_back(std::make_pair(-u_bound, u_bound));
	// double upper_bound = 10.0; // maximum value for each coordinate
	// double lower_bound = -upper_bound; // minimum value for each coordinate

	// determines if the halfplanes are generated along the axes (grid)
	std::cout << std::endl;
	std::cout << "Halfplane constraint (0: Griddy, 1: Arbitrary)" << std::endl;
	std::cout << "Select: ";
	int halfplane_constraint; std::cin >> halfplane_constraint;

	std::cout << std::endl;
	std::cout << "Number of 'maximum' convex subparts: ";
	int num_parts; std::cin >> num_parts;

	switch (halfplane_constraint) {
	case 0: {
		// for each axis

		std::cout << std::endl;
		vector<int> num_subspaces_over_axes;
		for (int i = 0; i < d; i++) {
			std::cout << "Number of subspaces along the " << i + 1 << "-th axis: ";
			int num_subspace;  std::cin >> num_subspace;
			num_subspaces_over_axes.push_back(num_subspace);
		}
		// generate grid (cells)
		// set neighbors inside generate_grid( )
		auto cells = generate_grid(bbx, num_subspaces_over_axes); // 
		activate_cells(cells, num_parts);

		// 각 hp에 흩뿌리고 나서
		// 임의의 active한 pair 사이에서 발생하는 건가? 그럼 각 cell에 대해서 neighbor 다 뒤져서 restricted_halfplane을 가져와야 하나?

		std::cout << "Number of points to disperse on each halfplane: ";
		int dis_pts; std::cin >> dis_pts;
		disperse_pts_between_active_cells(bbx, num_subspaces_over_axes, cells, dis_pts);

		for (int i = 0; i + 1 < cells.size(); i++) {
			if (cells[i]->active) {
				for (auto nb : cells[i]->neighbors) {
					if (nb.first->active &&
						cells[i]->total_index < nb.first->total_index) {
						for (int j = 0; j < nb.second->on_points.size(); j++) {
							print_pt(nb.second->on_points[j], j);
						}
					}
				}
			}
		}

		std::cout << "Number of points to locate inside each cell: ";
		std::cin >> dis_pts;
		for (int i = 0; i < cells.size(); i++) {
			generate_points_on_bb(cells[i], dis_pts);
			for (int j = 0; j < cells[i]->in_points.size(); j++) print_pt(cells[i]->in_points[j], j);
		}

		std::string dir("C:\\qhull\\bin\\");
		// user-defined directory end

		int count = 0;

		for (int i = 0; i < cells.size(); i++) {
			if (!cells[i]->active) continue;

			vector<Point*> pts = cells[i]->in_points;
			for (auto nb : cells[i]->neighbors) {
				if (!nb.first->active) continue;

				pts.insert(pts.end(), nb.second->on_points.begin(), nb.second->on_points.end());
			}

			std::string res = "result";
			if (count < 10) {
				res += "00";
			}
			else if (count < 100) {
				res += "0";
			}
			res += std::to_string(count) + ".txt";

			std::ofstream fout(dir + res);

			// first line contains the dimension
			fout << d << std::endl;
			// second line contains the number of input points
			fout << pts.size() << std::endl;
			// remaining lines contain point coordinates
			for (auto pt : pts) {
				for (int j = 0; j < d - 1; j++) {
					fout << pt->getx(j) << " ";
				}
				fout << pt->getx(d - 1) << std::endl;
			}
			count++;

			fout.close();
		}


	}


	}
	//vector<Polytope*> plts = dels2polytopes(num_parts);

	Free_Point* p1 = new Free_Point({ 0., 0., 20. });

	Free_Point* p2 = new Free_Point({ 0, 0, -65. });
	Free_Point* p3 = new Free_Point({ 0., 0., 1.5 });
	Free_Point* p4 = new Free_Point({ 100., 100., 100. });
	Free_Point* p5 = new Free_Point({ -100., -100., -100. });
	list<Free_Point> frpts = { *p1,*p2,*p3,*p4,*p5 };
	vector<Polytope> plts;
	//Eps_Graph_nD g(3);
	Eps_Graph_nD grid(3, frpts, plts, 10.0);

	Free_Point* q = new Free_Point({ 0., 0., -20. });

	return 0;
}
*/
int main() {
	std::cout.precision(3);
	std::cout << std::fixed;
	std::string config("C:\\Users\\HWI\\Desktop\\StarLab_1017\\postechDNN\\dnn\\NearestNeighbor\\EpsGraphnD\\data_generation\\config.ini");
	// std::string config("./config.ini");
	// std::string config("config.ini");

	ifstream readFile;
	readFile.open(config);

	int d;
	double u_bound;
	int halfplane_constraint;
	int num_parts;
	// int dis_pts;
	int num_subspace;
	int dis_pts;
	int dis_pts_cell;
	
	// 파일 입출력
	if (readFile.is_open()) {
		// string dum1, dum2, str;
		// readFile >> dum1 >> dum2 >> s;
		
		string dum, str;
		// dimension
		readFile >> dum >> str;
		d = stoi(str);

		readFile >> dum >> str;
		u_bound = stod(str);

		readFile >> dum >> str;
		halfplane_constraint = stoi(str);

		readFile >> dum >> str;
		num_parts = stoi(str);

		readFile >> dum >> str;
		num_subspace = stoi(str);

		readFile >> dum >> str;
		dis_pts = stoi(str);

		readFile >> dum >> str;
		dis_pts_cell = stoi(str);
	}
	// 
	else {
		std::cout << "Enter the dimension: ";
		std::cin >> d;

		// defines a bounding box (cube) around the origin
		std::cout << std::endl;
		std::cout << "Maximum value for each coordinate: ";
		std::cin >> u_bound;

		// determines if the halfplanes are generated along the axes (grid)
		std::cout << std::endl;
		std::cout << "Halfplane constraint (0: Griddy, 1: Arbitrary)" << std::endl;
		std::cout << "Select: ";
		std::cin >> halfplane_constraint;

		std::cout << std::endl;
		std::cout << "Number of 'maximum' convex subparts: ";
		std::cin >> num_parts;

		// std::cout << "Number of subspaces along the " << i + 1 << "-th axis: ";
		std::cout << std::endl;
		std::cout << "Number of subspaces along each axis: ";
		std::cin >> num_subspace;

		std::cout << "Number of points to disperse on each halfplane: ";
		std::cin >> dis_pts;

		std::cout << "Number of points to locate inside each cell: ";
		std::cin >> dis_pts_cell;
	}
	
	// ------------------------------------------------------------------------------------------------------------------------
	// dimension d

	// ------------------------------------------------------------------------------------------------------------------------
	// generate a bounding box

	// rectangular bounding box
	std::vector<std::pair<double, double>> bbx;

	for (int i = 0; i < d; i++) bbx.push_back(std::make_pair(-u_bound, u_bound));
	// double upper_bound = 10.0; // maximum value for each coordinate
	// double lower_bound = -upper_bound; // minimum value for each coordinate

	switch (halfplane_constraint) {
	case 0: {
		// for each axis

		std::cout << std::endl;
		vector<int> num_subspaces_over_axes;
		for (int i = 0; i < d; i++) {
			num_subspaces_over_axes.push_back(num_subspace);
		}
		// generate grid (cells)
		// set neighbors inside generate_grid( )
		auto cells = generate_grid(bbx, num_subspaces_over_axes); // 
		activate_cells(cells, num_parts);

		// 각 hp에 흩뿌리고 나서
		// 임의의 active한 pair 사이에서 발생하는 건가? 그럼 각 cell에 대해서 neighbor 다 뒤져서 restricted_halfplane을 가져와야 하나?
		disperse_pts_between_active_cells(bbx, num_subspaces_over_axes, cells, dis_pts);

		for (int i = 0; i + 1 < cells.size(); i++) {
			if (cells[i]->active) {
				for (auto nb : cells[i]->neighbors) {
					if (nb.first->active &&
						cells[i]->total_index < nb.first->total_index) {
						for (int j = 0; j < nb.second->on_points.size(); j++) {
							if (PRINT_POINT_FLAG) print_pt(nb.second->on_points[j], j);
						}
					}
				}
			}
		}

		for (int i = 0; i < cells.size(); i++) {
			generate_points_on_bb(cells[i], dis_pts_cell);
			for (int j = 0; j < cells[i]->in_points.size(); j++) {
				if (PRINT_POINT_FLAG) print_pt(cells[i]->in_points[j], j);
			}
		}

		std::string dir("C:\\qhull\\bin\\A\\");
		// user-defined directory end

		int count = 0;

		for (int i = 0; i < cells.size(); i++) {
			if (!cells[i]->active) continue;

			vector<Point*> pts = cells[i]->in_points;
			for (auto nb : cells[i]->neighbors) {
				if (!nb.first->active) continue;

				pts.insert(pts.end(), nb.second->on_points.begin(), nb.second->on_points.end());
			}

			std::string res = "result";
			if (count < 10) {
				res += "00";
			}
			else if (count < 100) {
				res += "0";
			}
			res += std::to_string(count) + ".txt";

			std::ofstream fout(dir + res);

			// first line contains the dimension
			fout << d << std::endl;
			// second line contains the number of input points
			fout << pts.size() << std::endl;
			// remaining lines contain point coordinates
			for (auto pt : pts) {
				for (int j = 0; j < d - 1; j++) {
					fout << pt->getx(j) << " ";
				}
				fout << pt->getx(d - 1) << std::endl;
			}
			count++;

			fout.close();
		}
	}
	}

	/*
	std::cout << "enter any word: ";
	string strrr;
	std::cin >> strrr;
	*/
	// vector<Polytope*> plts_p = dels2polytopes(num_parts);

	/*
	vector<Polytope> plts;

	for (vector<Polytope*>::iterator it = plts_p.begin(); it != plts_p.end(); it++) {
		plts.push_back(**it);
	}

	vector<Polytope> plts;
	Free_Point* p1 = new Free_Point({ 0., 20. });

	Free_Point* p2 = new Free_Point({ 0, -65. });
	Free_Point* p3 = new Free_Point({ 0., 1.5 });
	Free_Point* p4 = new Free_Point({ 100., 100. });
	Free_Point* p5 = new Free_Point({ -100., -100. });
	list<Free_Point> frpts = { *p1,*p2,*p3,*p4,*p5 };
	//Eps_Graph_nD g(3);
	//cout << "Number of Polytopes: " << plts.size() << endl;

	Eps_Graph_nD grid(2, frpts, plts, 10.0);
	
	Free_Point* q = new Free_Point({ 0., -20. });
	grid.add_freepts(q);
	grid.print_free_point();
	
	grid.print_kNN(*q, 2);
	grid.Dijkstra(*q);
	*/

	return 0;
	
	
}


/*
int main() {
	qhull2polytope();
}
*/

vector<Polytope*> dels2polytopes(int num_topes) {
	vector<Polytope*> ret;

	// std::string dir("C:\\qhull\\bin\\AAA");

	std::cout << "Enter 'A': ";
	int dummy; std::cin >> dummy;

	/*
	// std::cout << "Enter 'A': ";
	// int dummy; std::cin >> dummy;
	string s = "새 폴더";

	// 있으면 넘기기

	while() {
	}

	std::string dir;
	if (iter_num == 0) {
		dir = "C:\\qhull\\bin\\AAA" + s;
	}
	else {
		dir = "C:\\qhull\\bin\\AAA" + s +  " (" + iter_num + ")";
	}

	std::cout << "Enter 'A': ";
	int dummy; std::cin >> dummy;
	*/

	std::string index;
	std::vector<string> filenames;
	 for (int count = 0; count < num_topes; count++) {
	 
		/*
		std::cout << "Enter the input file index: ";
		std::cin >> index; filenames.push_back(index);
		*/

		std::string res = "";
		if (count < 10) {
			res += "00";
		}
		else if (count < 100) {
			res += "0";
		}
		res += std::to_string(count);//  + ".txt";
		filenames.push_back(res);
	}

	for (auto index : filenames) {
		std::ifstream fin;
		
		// AAA에서
		string str = "C:\\qhull\\bin\\AAA" + index + ".txt";
		fin.open(str);

		string s;
		getline(fin, s);
		int dim = stoi(s);
		getline(fin, s);
		int num_pts = stoi(s);

		std::vector<Point*> pts;
		std::vector<double> vec(num_pts, 0.0);
		double val;

		for (int i = 0; i < num_pts; i++) {
			Point* pt = new Point(dim);
			for (int j = 0; j < dim; j++) {
				fin >> val;  
				pt->setx(j, val);
			}
			pts.push_back(pt);
		}

		fin.close();

		Polytope* tope = new Polytope();
		tope->set_vertices(pts);

		std::vector<simplex> sims;

		// std::ifstream fin2(dir + "CH" + to_string(index));
		string str2 = "C:\\qhull\\bin\\AAAA" + index + ".txt";
		std::ifstream fin2;
		
		fin.open(str2);

		getline(fin, s);
		int size = stoi(s);
		for (int index = 0; index < size; index++) {
			fin >> dummy;
			vector<Point*> vec;
			for (int index2 = 0; index2 < dim + 1; index2++) {
				fin >> s; 
				vec.push_back(pts[stoi(s)]);
			}
			auto sim = new simplex(dim, vec);
			sims.push_back(*sim);
		}

		tope->set_simplices(sims);
		ret.push_back(tope);
	}

	return ret;
}

bool isNumeric(std::string const& str)
{
	auto it = str.begin();
	while (it != str.end() && std::isdigit(*it)) {
		it++;
	}
	return !str.empty() && it == str.end();
}


// ------------------------------------------------------------------------------------------------------------------------
// halfplane 생성 함수들
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

// center(주관적 원점)을 지나는 halfplane 하나를 만들어내기
// 어차피 비례하므로, 여기서는 수를 bound할 필요는 없음.
halfplane* gen_hp_thru_p(Point* center) {
	halfplane* hp = new halfplane;
	hp->d = center->n;

	std::uniform_real_distribution<double> uni_dist(0.0, 1.0);

	for (int i = 0; i < hp->d - 1; i++) {
		hp->vals[i] = uni_dist(gen);
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

std::vector<halfplane*> axis_slice(int u_bnd, int dim, int my_dim, int num_cuts) {
	return {};
}

// ------------------------------------------------------------------------------------------------------------------------
// 점집합 생성 함수들


// void generate_points_on_bb(std::vector<std::pair<double, double>> bbx, int dis_pts, int max_iter) {
void generate_points_on_bb(cell* c, int dis_pts, int max_iter) {
	int dim = c->index.size();
	double real_max = DBL_MIN;
	for (const auto val : c->bbx) {
		double temp = max(abs(val.first), abs(val.second));
		if (temp > real_max) real_max = temp;
	}

	std::uniform_real_distribution<double> uni_dist(-real_max, real_max); // uniform distribution
	int iter = 0;
	while (iter < max_iter && dis_pts > c->in_points.size()) {
		iter++;

		Point* pt = new Point(dim);
		for (int i = 0; i < dim; i++) {
			pt->setx(i, uni_dist(gen));
		}

		// strict inequality 필요
		for (int i = 0; i < dim; i++) {
			double x = pt->getx(i);	
			if (x <= c->bbx[i].first || x >= c->bbx[i].second) continue;
		}
		
		c->in_points.push_back(pt);
	}
}

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
// 조건에 맞아떨어질 때까지 계속 랜덤으로 점을 뽑아서 점집합을 반환하는 함수
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

// halfplane 위에 점 생성. bounding box도 아닌데 왜 bb?
void generate_points_on_bb(std::vector<std::pair<double, double>> bbx, halfplane* H, int dim, int con_pts, int max_iter) {
	auto& ret = H->on_points;
	// std::vector<Point*> ret;

	int u_bound = bbx[0].second;
	int num = 0; int iter = 0;

	while (num < con_pts && iter < max_iter) {
		iter++;

		// 앞 d-1개의 값을 랜덤하게 고름
		// u_bound / 5 와 같은 식으로, 범위를 조금 줄이기?
		// 위의 점을 잡은 다음 평면 위에서 움직여서 점 찍기?
		std::uniform_real_distribution<double> uni_dist(-u_bound, u_bound); // uniform distribution
		std::vector<double> vec;

		for (int i = 0 ; i < dim-1; i++) {
			vec.push_back(uni_dist(gen));
		}

		// halfplane 정보 이용하여 마지막 1개 값을 계산
		double val = H->vals[dim];
		for (int i = 0; i < dim - 1; i++) {
			val += H->vals[i] * vec[i];
		}
		val = -val;
		val /= H->vals[dim-1];

		// 계산된 값 역시 bounding box 안에 있다면 해당 점을 ret에 추가, num 증가
		if (abs(val) < abs(u_bound)) {
			vec.push_back(val);
			Point* pt = new Point(vec);

			ret.push_back(pt);
			num++;
		}
	}
}

// for grid
void generate_points_on_restricted_hp(std::vector<std::pair<double, double>> bbx, halfplane* H, vector<int> grid_size, vector<int> index, int con_pts, int max_iter) {
	// 대응되는 point set이 비어있지 않다면(이미 한번 불렸다면) 바로 종료
	if (!H->on_points.empty()) return;
	
	int dim = index.size();
	
	// compute restriction
	// vector<pair<double, double>> restriction;
	for (int i = 0; i < index.size(); i++) {
		if (index[i] == -1) {
			H->restriction[i] = make_pair(H->vals[dim] - EPS_DG, H->vals[dim] + EPS_DG);
			//for (int j = 0; j < dim; j++) {
			//	if (abs(H->vals[j]) > EPS_DG) 
			//	H->restriction[i] = make_pair(-H->vals[dim] - EPS_DG, -H->vals[dim] + EPS_DG);
			//}
		}
		else {
			H->restriction[i] = make_pair(bbx[i].first + (bbx[i].second - bbx[i].first) * index[i] / grid_size[i], 
				bbx[i].first + (bbx[i].second - bbx[i].first) * (index[i] + 1) / grid_size[i]);
		}
	}
	
	auto& ret = H->on_points;

	int u_bound = bbx[0].second;
	int num = 0; int iter = 0;

	while (num < con_pts && iter < max_iter) {
		iter++;

		// 앞 d-1개의 값을 랜덤하게 고름
		// u_bound / 5 와 같은 식으로, 범위를 조금 줄이기?
		// 위의 점을 잡은 다음 평면 위에서 움직여서 점 찍기?
		std::uniform_real_distribution<double> uni_dist(-u_bound, u_bound); // uniform distribution
		std::vector<double> vec;

		for (int i = 0; i < dim; i++) {
			if (index[i] == -1) vec.push_back(H->vals[dim]);
			else vec.push_back(uni_dist(gen));
		}

		for (int j = 0; j < H->restriction.size(); j++) { //  auto pr : restriction) {
			if (H->restriction[j].first > vec[j] || H->restriction[j].second < vec[j]) continue;
		}
		Point* pt = new Point(vec);

		ret.push_back(pt);
		num++;
	}
}

// ------------------------------------------------------------------------------------------------------------------------
// 점집합 분리 함수들
// bounding box의 정보는 필요 없음
// halfplane 하나 가지고, 3개의 subset으로 나눔
// 0, 양수, 음수
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

// ------------------------------------------------------------------------------------------------------------------------
// print 함수들

// halfplane 출력 함수
void print_halfplane(halfplane* H, int index) {
	int dim = H->vals.size() - 1;
	// std::cout << dim << "-dimensional halfplane" << " \#" << index << " generated: " << "(";
	std::cout << dim << "-dimensional halfplane" << " \#" << index << " (";
	for (int i = 0; i < dim; i++) {
		std::cout << H->vals[i] << ", ";
	}
	std::cout << H->vals[dim] << ")" << std::endl;
}

// point 출력 함수
void print_pt(Point* p, int index) {
	int dim = p->getxs().size();
	std::cout << dim << "-dimensional point" << " \#" << index << " (";
	for (int i = 0; i < dim-1; i++) {
		std::cout << p->getx(i) << ", ";
	}
	std::cout << p->getx(dim-1) << ")" << std::endl;
}

// ------------------------------------------------------------------------------------------------------------------------
// 점 변경 함수

// 주어진 점이 bounding box 안에 들어가도록 scaling
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

// ------------------------------------------------------------------------------------------------------------------------
// grid 관련 함수
vector<cell*> generate_grid(std::vector<std::pair<double, double>> bbx, vector<int> num_subspaces_over_axes) {
	int dim = num_subspaces_over_axes.size();
	vector<cell*> cells;
	int total_num_cells = 1;
	for (auto num : num_subspaces_over_axes) total_num_cells *= num;

	vector<int> denominators = compute_denominators(num_subspaces_over_axes);

	// generate all cells
	for (int i = 0; i < total_num_cells; i++) {
		auto c = new cell(i);
		c->index = num2index(num_subspaces_over_axes, denominators, i);
		for (int j = 0; j < c->index.size(); j++) { //  auto val : bbx) {
			c->bbx.push_back(make_pair(bbx[j].first + (bbx[j].second - bbx[j].first) * c->index[j] / num_subspaces_over_axes[j],
				bbx[j].first + (bbx[j].second - bbx[j].first) * (c->index[j] + 1) / num_subspaces_over_axes[j]));
		}
		cells.push_back(c);
	}

	// set neighbor infos (at most 2d)
	for (int i = 0; i < total_num_cells; i++) {
		auto c = cells[i];
		auto index = cells[i]->index;
		for (int j = 0; j < dim; j++) {
			auto index_copy1 = index; index_copy1[j]++;
			auto index_copy2 = index; index_copy2[j]--;
			vector<vector<int>> index_copies = { index_copy1, index_copy2 };
			
			for (auto index_copy : index_copies) {
				int new_num = index2num(num_subspaces_over_axes, denominators, index_copy);
				if (new_num != -1 && i < new_num) {
					auto H = new halfplane(dim);
					for (int j = 0; j < dim; j++) {
						// 부등호가 있으므로 한쪽 방으로만 cell 만들어짐
						if (c->index[j] != cells[new_num]->index[j]) {
							// 해당하는 halfplane을 만듬
							H->vals[j] = 1.0;
							H->vals[dim] = -(bbx[j].first + (bbx[j].second - bbx[j].first) * max(c->index[j], cells[new_num]->index[j]) / num_subspaces_over_axes[j]);
							break;
						}
					}
					c->neighbors.push_back(make_pair(cells[new_num], H));
					cells[new_num]->neighbors.push_back(make_pair(c, H));
				}
			}
		}
	}

	return cells;
}

vector<int> compute_denominators(vector<int> num_subspaces_over_axes) {
	// of dim d-1
	vector<int> denominators;
	int denominator = 1;
	for (int i = num_subspaces_over_axes.size() - 1; i > 0; i--) {
		denominator *= num_subspaces_over_axes[i];
		denominators.push_back(denominator);
	}
	reverse(denominators.begin(), denominators.end());

	return denominators;
}

vector<int> num2index(vector<int> num_subspaces_over_axes, vector<int> denominators, int num) {
	vector<int> index;

	// copy i to compute index
	for (int j = 0; j < denominators.size(); j++) {
		if (j == denominators.size() - 1) {
			index.push_back(num / denominators[j]);
			index.push_back(num % denominators[j]);
		}
		else {
			index.push_back(num / denominators[j]);
			num %= denominators[j];
		}
	}

	return index;
}

int index2num(vector<int> num_subspaces_over_axes, vector<int> multipliers, vector<int> index) {
	int dim = num_subspaces_over_axes.size();

	// index 벗어난 경우는 -1 리턴
	for (int i = 0; i < index.size(); i++) {
		if (index[i] < 0 || index[i] >= num_subspaces_over_axes[i]) return -1;
	}

	int num = 0;
	for (int i = 0; i < dim - 1; i++) {
		num += index[i] * multipliers[i];
	}
	num += index[dim - 1];

	return num;
}

void activate_cells(vector<cell*> cells, int num_parts) {
	for (int i = 0; i < num_parts; i++) cells[i]->active = true;
}

void disperse_pts_between_active_cells(std::vector<std::pair<double, double>> bbx, vector<int> num_subspaces_over_axes, vector<cell*> cells, int num) {
	int dim = num_subspaces_over_axes.size();

	for (int i = 0; i < cells.size(); i++) {
		auto& c = cells[i];
		for (auto& nb : c->neighbors) {
			// c->index 대신에 nb->index 넣어도 결과는 같음
			// 두 번 불릴 텐데, 그 중에 한 번만 실행될 것이라 문제는 없음
			if (c->active && nb.first->active) {

				auto copy_index = c->index;
				for (int j = 0; j < copy_index.size(); j++) {
					if (c->index[j] != nb.first->index[j]) copy_index[j] = -1;
				}

				if (find(copy_index.begin(), copy_index.end(), -1) == copy_index.end()) continue;
				generate_points_on_restricted_hp(bbx, nb.second, num_subspaces_over_axes, copy_index, num);
			}
		}
	}
}

/*


	//case 1: {
	//	std::uniform_real_distribution<double> uni_dist(-u_bound, u_bound); // uniform distribution
	//	break;
	//	}
	//}

	//// ------------------------------------------------------------------------------------------------------------------------
	//// generate halfplanes

	//// for now, only 1 halfplane
	//// std::cout << "Number of halfplanes to generate: ";
	//// int num_h;
	//// std::cin >> num_h;

	//std::cout << std::endl;
	//int num_h = num_parts - 1;

	//std::cout << "Method for halfplane generation (0. Random, 1. Centered)" << std::endl;
	//std::cout << "Select: ";
	//int gen_method; std::cin >> gen_method;

	//// std::vector<HP>; // halfplanes
	//std::vector<halfplane*> halfplanes; // halfplane

	//switch (gen_method) {
	//	// generate halfplanes at random
	//	case 0:
	//		for (int j = 0; j < num_h; j++) {
	//			auto H = gen_hp_random(d, j);
	//			halfplanes.push_back(H);
	//			print_halfplane(H, j);
	//			std::cout << "Halfplane \#" << j << " generated" << std::endl;
	//		}
	//		break;

	//	// generate halfplanes, each centered at a point
	//	case 1:
	//		for (int j = 0; j < num_h; j++) {
	//			std::cout << "Dimension: " << d << std::endl;
	//			std::cout << "- Enter Point\#" << j << "Info -" << std::endl;

	//			Point* pt = new Point;
	//			for (int i = 0; i < d; i++) {
	//				std::cout << "Enter " << i << "-th value: ";
	//				double val; std::cin >> val;
	//				pt->setx(i, val);
	//			}
	//			auto H = gen_hp_thru_p(pt);
	//			halfplanes.push_back(H);
	//			print_halfplane(H, j);
	//			std::cout << "Halfplane \#" << j << " generated" << std::endl;
	//		}
	//		break;
	//}

	//// ------------------------------------------------------------------------------------------------------------------------
	//// disperse certain amount of points on the halfplanes

	//// con_pts stands for convex points
	//std::cout << std::endl;
	//std::cout << "Disperse points onto halfplanes (0. General, 1. Each)" << std::endl;
	//std::cout << "Select: ";
	//int num_method; std::cin >> num_method;
	//switch (num_method) {
	//case 0:
	//	std::cout << "Number of points: ";
	//	int dis_pts; std::cin >> dis_pts;
	//	for (int i = 0; i < num_h; i++) {
	//		auto H = halfplanes[i];
	//		generate_points_on_bb(bbx, halfplanes[i], d, dis_pts);
	//		for (int j = 0; j < H->on_points.size(); j++) {
	//			print_pt(H->on_points[j], j);
	//		}
	//	}
	//	break;
	//case 1:
	//	for (int i = 0; i < num_h; i++) {
	//		auto H = halfplanes[i];

	//		std::cout << "Number of points for halfplane \#";
	//		int dis_pts; std::cin >> dis_pts;
	//		generate_points_on_bb(bbx, halfplanes[i], d, dis_pts);
	//		for (int j = 0; j < H->on_points.size(); j++) {
	//			print_pt(H->on_points[j], j);
	//		}
	//	}
	//	break;
	//}

	//std::vector<std::vector<Point*>> pos_parts;
	//std::vector<std::vector<Point*>> neg_parts;

	//// point sets on both sides, union them with the dispersed ones
	//std::cout << std::endl;
	//for (int i = 0; i < num_h; i++) {
	//	auto H = halfplanes[i];

	//	std::cout << "Number of points on the positive side: ";
	//	int pos_num; std::cin >> pos_num;
	//	auto pos_pts = gen_pts(u_bound, H, true, pos_num);
	//	pos_pts.insert(pos_pts.end(), H->on_points.begin(), H->on_points.end());
	//	for (int j = 0; j < pos_pts.size(); j++) {
	//		print_pt(pos_pts[j], j);
	//	}

	//	std::cout << std::endl;
	//	std::cout << "Number of points on the negative side: ";
	//	int neg_num; std::cin >> neg_num;
	//	auto neg_pts = gen_pts(u_bound, H, false, neg_num);
	//	neg_pts.insert(neg_pts.end(), H->on_points.begin(), H->on_points.end());
	//	for (int j = 0; j < neg_pts.size(); j++) {
	//		print_pt(neg_pts[j], j);
	//	}

	//	pos_parts.push_back(pos_pts);
	//	neg_parts.push_back(neg_pts);
	//}

	//// user-defined directory start
	//std::string dir("C:\\Users\\HWI\\Desktop\\qhull\\bin\\");
	//// std::string dir("C:\\Users\\hwikim\\Desktop\\ALLTAG\\2023\\202307\\20230719\\qhull-2020.2\\bin\\");
	//// std::string dir("C:\\Users\\HWI\\Desktop\\ALLTAG\\2023\\202307\\20230703\\postechDNN\\dnn\\NearestNeighbor\\EpsGraphnD\\data_generation");
	//// user-defined directory end

	//// std::string myst("pts.txt");
	//// std::ifstream fin(dir + myst);
	//int count = 0;

	//pos_parts.insert(pos_parts.end(), neg_parts.begin(), neg_parts.end());
	////for (int i = 0; i < num_parts; i++) {
	////
	////	auto pos_pts = pos_parts[i];
	////	auto neg_pts = neg_parts[i];
	//for (int i = 0; i < pos_parts.size(); i++) {
	//	auto pts = pos_parts[i];

	//	std::string res = "results.out";
	//	if (count < 10) {
	//		res += "00";
	//	}
	//	else if (count < 100) {
	//		res += "0";
	//	}
	//	res += std::to_string(count) + ".txt";

	//	std::ofstream fout(dir + res);

	//	// first line contains the dimension
	//	fout << d << std::endl;
	//	// second line contains the number of input points
	//	fout << pts.size() << std::endl;
	//	// remaining lines contain point coordinates
	//	for (auto pt : pts) {
	//		for (int j = 0; j < d-1; j++) {
	//			fout << pt->getx(j) << " ";
	//		}
	//		fout << pt->getx(d-1) << std::endl;
	//	}
	//	count++;

	//	fout.close();
	//}

	// std::string res; // connectivity graph
*/