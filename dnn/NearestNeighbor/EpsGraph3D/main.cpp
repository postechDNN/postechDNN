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
#include "chrono"
#include <math.h>


using namespace std;

int main() {
	double max_coor = 15.0;
	int qr_num = 1000;
	int fr_num = 10000;
	int clu_num = 100;
	int object_num = 10;
	int k_var[5] = { 10, 50, 100, 500, 1000 };
	ifstream file("error_test.txt");
	//ofstream file_fr("fr_pt_error.txt");
	//ofstream file_qr("qr_pt_error.txt");
	ifstream file_fr("fr_pt_error.txt");
	ifstream file_qr("qr_pt_error.txt");
	ofstream error_data("error_data2.txt");
	int testcase;
	double total_time = 0.0;
	file >> testcase;
	random_device rd; 
	std::mt19937 gen(rd());
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
		//string temp_name; string qr_name;
		/*switch (i) {
		case 0: {temp_name = "fr_pt_1_1.txt"; qr_name = "qr_pt_1_1.txt"; break; }
		case 1: {temp_name = "fr_pt_1_2.txt"; qr_name = "qr_pt_1_2.txt"; break; }
		case 2: {temp_name = "fr_pt_1_3.txt"; qr_name = "qr_pt_1_3.txt"; break; }
		case 3: {temp_name = "fr_pt_1_4.txt"; qr_name = "qr_pt_1_4.txt"; break; }
		case 4: {temp_name = "fr_pt_1_5.txt"; qr_name = "qr_pt_1_5.txt"; break; }
		case 5: {temp_name = "fr_pt_1_6.txt"; qr_name = "qr_pt_1_6.txt"; break; }
		case 6: {temp_name = "fr_pt_1_7.txt"; qr_name = "qr_pt_1_7.txt"; break; }
		case 7: {temp_name = "fr_pt_1_8.txt"; qr_name = "qr_pt_1_8.txt"; break; }
		case 8: {temp_name = "fr_pt_1_9.txt"; qr_name = "qr_pt_1_9.txt"; break; }
		case 9: {temp_name = "fr_pt_1_10.txt"; qr_name = "qr_pt_1_10.txt"; break; }
		}*/	
		/*
		file_fr << 0.0 << " " << 0.0 << " " << 0.0 << endl;
		file_fr << max_coor << " " << max_coor << " " << max_coor << endl;
		int fr_count = 2;
		while (fr_count < fr_num) {
			uniform_real_distribution<> dis(0.0, 1.0);
			double x = max_coor * dis(gen); double y = max_coor * dis(gen); double z = max_coor * dis(gen);
			Point p = { x,y,z };
			bool check = true;
			for (auto pol : plts) {
				if (pol.isIn(&p)) {
					check = false;
					break;
				}
			}
			if (check) {
				file_fr << x << " " << y << " " << z << endl;
				fr_count++;
			}
		}
		int qr_count = 0;
		while (qr_count < qr_num) {
			uniform_real_distribution<> dis(0.0, 1.0);
			double x = max_coor*dis(gen); double y = max_coor * dis(gen); double z = max_coor * dis(gen);
			Point p = { x,y,z };
			bool check = true;
			for (auto pol : plts) {
				if (pol.isIn(&p)) {
					check = false;
					break;
				}
			}
			if (check) { 
				file_qr << x << " " << y << " " << z << endl;
				qr_count++;
			}
		}
		cout << i << endl; */
		
		for (int fr_count = 0; fr_count < fr_num; fr_count++) {
			double x, y, z;
			file_fr >> x >> y >> z;
			Free_Point one_point = {x, y ,z};
			frpts.push_back(one_point);
		}
		for (int qr_count = 0; qr_count < qr_num; qr_count++) {
			double x, y, z;
			file_qr >> x >> y >> z;
			Free_Point one_point = { x, y ,z };
			qrpts.push_back(one_point);
		}
		double exe_time;
		if (i >= 25) { continue; }
		Eps_Graph_3D grid(frpts, plts, 1);
		for (int l = 0; l < 5; l++) {
			for (auto qr : qrpts) {
				auto start = chrono::high_resolution_clock::now();
				grid.kNN(qr, k_var[l]);
				auto stop = chrono::high_resolution_clock::now();
				auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
				exe_time = duration.count();
			}
		}
	}
	file_fr.close();
	file_qr.close();
	file.close();
	return 0;
}
