#include "EpsGraph3D.h"
#include "Point.h"
#include "Polytope.h"
#include <list>
#include <vector>
#include <fstream>
#include <random>
#include <iomanip>
#include <iostream>
#include <string>


using namespace std;

int main() {
	double max_coor = 25.0;
	int qr_num = 1000;
	int fr_num = 100000;
	int object_num = 75;
	int k_var[5] = { 10, 50, 100, 500, 1000 };
	ifstream file("query_test4.txt");
	int testcase;
	file >> testcase;
	for (int i = 0; i < testcase; i++) {
		std::list<Free_Point> frpts = {};
		std::vector<Polytope> plts = {};
		std::vector<Free_Point> qrpts = {};
		for (int j = 0; j < object_num; j++) {
			int num_face;
			file >> num_face;
			vector<Face*> temp_face = {};
			for (int k = 0; k < num_face; k++) {
				vector<Point*> temp_point = {};
				double x, y, z;
				for (int l = 0; l < 3; l++) {
					file >> x >> y >> z;
					Point* one_point = new Point(x, y, z);
					temp_point.push_back(one_point);
				}
				Face* one_face = new Face(temp_point);
				temp_face.push_back(one_face);
			}
			Polytope temp_pol;
			temp_pol.setpolytope(temp_face);
			plts.push_back(temp_pol);
		}
		string temp_name; string qr_name;
		int fr_count = 0;
		switch (i) {
		case 0: {temp_name = "fr_pt_4_1.txt"; qr_name = "qr_pt_4_1.txt"; break; }
		case 1: {temp_name = "fr_pt_4_2.txt"; qr_name = "qr_pt_4_2.txt"; break; }
		case 2: {temp_name = "fr_pt_4_3.txt"; qr_name = "qr_pt_4_3.txt"; break; }
		case 3: {temp_name = "fr_pt_4_4.txt"; qr_name = "qr_pt_4_4.txt"; break; }
		case 4: {temp_name = "fr_pt_4_5.txt"; qr_name = "qr_pt_4_5.txt"; break; }
		case 5: {temp_name = "fr_pt_4_6.txt"; qr_name = "qr_pt_4_6.txt"; break; }
		case 6: {temp_name = "fr_pt_4_7.txt"; qr_name = "qr_pt_4_7.txt"; break; }
		case 7: {temp_name = "fr_pt_4_8.txt"; qr_name = "qr_pt_4_8.txt"; break; }
		case 8: {temp_name = "fr_pt_4_9.txt"; qr_name = "qr_pt_4_9.txt"; break; }
		case 9: {temp_name = "fr_pt_4_10.txt"; qr_name = "qr_pt_4_10.txt"; break; }
		/*case 0: {temp_name = "query_pt_4_1.txt"; break; }
		case 1: {temp_name = "query_pt_4_2.txt"; break; }
		case 2: {temp_name = "query_pt_4_3.txt"; break; }
		case 3: {temp_name = "query_pt_4_4.txt"; break; }
		case 4: {temp_name = "query_pt_4_5.txt"; break; }
		case 5: {temp_name = "query_pt_4_6.txt"; break; }
		case 6: {temp_name = "query_pt_4_7.txt"; break; }
		case 7: {temp_name = "query_pt_4_8.txt"; break; }
		case 8: {temp_name = "query_pt_4_9.txt"; break; }
		case 9: {temp_name = "query_pt_4_10.txt"; break; }*/
		}
		ofstream file_fr(temp_name);
		ofstream file_query(qr_name);
		cout << i << endl;
		random_device rd;
		default_random_engine eng(rd());
		while (fr_count < fr_num) {
			
			uniform_real_distribution<double> gen(0, 1);
			double x = max_coor * gen(eng);
			double y = max_coor * gen(eng);
			double z = max_coor * gen(eng);
			bool check = false;
			Free_Point t = { x, y, z };
			for (auto pol : plts) {
				if (pol.isIn(&t)) {
					check = true;
					break;
				}
			}
			if (check) { continue; }
			file_fr << x << " " << y << " " << z << endl;
			fr_count++;
		}
		fr_count = 0;
		cout << i << endl;
		while (fr_count < qr_num) {
			uniform_real_distribution<double> gen(0, 1);
			double x = max_coor * gen(eng);
			double y = max_coor * gen(eng);
			double z = max_coor * gen(eng);
			bool check = false;
			Free_Point t = { x, y, z };
			for (auto pol : plts) {
				if (pol.isIn(&t)) {
					check = true;
					break;
				}
			}
			if (check) { continue; }
			file_query << x << " " << y << " " << z << endl;
			fr_count++;
		}
		file_fr.close();
		file_query.close();
		/*Eps_Graph_3D grid(frpts, plts, 1);
		for (int i = 0; i < 5; i++) {
			for (auto qr : qrpts) {
				grid.print_kNN(qr, k_var[i]);
			}
		}
		*/
	}
	file.close();
	return 0;
}
