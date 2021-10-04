#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "dots.h"
#include "propagation.h"
#include "steiner_pts.h"

using namespace std;

PolyDomain BuildPolyDomain(string FileName) {
	
	ifstream readFile;

	readFile.open(FileName + ".node");
	// one dummy element for indexing 
	vector<Point> pts = {};

	if (readFile.is_open()) {
		string str1;
		getline(readFile, str1);

		while (!readFile.eof())
		{
			double num1;
			stringstream stream1;
			vector<double> pt_info = {};

			getline(readFile, str1);

			if (str1[0] == '#') {
				break;
			}

			stream1.str(str1);
			while (stream1 >> num1) {
				pt_info.push_back(num1);
			}

			int Point_index = int(pt_info[0]);
			pts.push_back(Point(pt_info[1], pt_info[2], pt_info[3], Point_index));
		}
		readFile.close();
	}

	readFile.open(FileName + ".edge");
	vector<Segment> sgs = {};

	if (readFile.is_open()) {
		string str2;
		getline(readFile, str2);

		while (!readFile.eof())
		{
			int num2;
			stringstream stream2;
			vector<int> sg_info = {};

			getline(readFile, str2);

			if (str2[0] == '#') {
				break;
			}

			stream2.str(str2);
			while (stream2 >> num2) {
				sg_info.push_back(num2);
			}

			sort(sg_info.begin()+1, sg_info.end()-1);

			for (int i = 1; i < 3; i++) {
				pts[sg_info[i]].add_iegs(sg_info[0]);
			}

			sgs.push_back(Segment(Point2Vec(pts[sg_info[1]]), Point2Vec(pts[sg_info[2]]), {}, 
						  sg_info[1], sg_info[2], sg_info[0]));
		}
		readFile.close();
	}

	readFile.open(FileName + ".face");
	vector<Tri> fcs = {};

	if (readFile.is_open()) {
		string str3;
		getline(readFile, str3);

		while (!readFile.eof())
		{
			int num3;
			stringstream stream3;
			vector<int> fc_info = {};

			getline(readFile, str3);

			if (str3[0] == '#') {
				break;
			}

			stream3.str(str3);
			while (stream3 >> num3) {
				fc_info.push_back(num3);
			}

			sort(fc_info.begin()+1, fc_info.end()-1);

			for (int i = 1; i < 4; i++) {
				pts[fc_info[i]].add_ifcs(fc_info[0]);
			}

			for (vector<Segment>::iterator it = sgs.begin(); it != sgs.end(); ++it) {
				vector<int> Svec = {it->geta_ind(), it->getb_ind() };
				
				if (std::includes(fc_info.begin()+1, fc_info.end()-1, Svec.begin(), Svec.end())) {
					it->add_fc(fc_info[0]);
				}
			}

			fcs.push_back(Tri(pts[fc_info[1]], pts[fc_info[2]], pts[fc_info[3]], fc_info[1], fc_info[2], fc_info[3]));
		}
		readFile.close();
	}

	readFile.open(FileName + ".ele");
	vector<Tetra> tetras = {};

	if (readFile.is_open()) {
		string str4;
		getline(readFile, str4);

		int check = 0;

		while (!readFile.eof())
		{
			int num4;
			stringstream stream4;
			vector<int> tet_info = {};
			
			getline(readFile, str4);

			if (str4[0] == '#') {
				break;
			}

			stream4.str(str4);
			while (stream4 >> num4) {
				tet_info.push_back(num4);
			}

			sort(tet_info.begin()+1, tet_info.end());

			for (int i = 1; i < 5; i++) {
				pts[tet_info[i]].add_itets(tet_info[0]);
			}

			int ordered_sgs[6] = {0, };

			for (vector<Segment>::iterator it = sgs.begin(); it != sgs.end(); ++it) {
				vector<int> Svec = { it->geta_ind(), it->getb_ind() };

				if (std::includes(tet_info.begin()+1, tet_info.end(), Svec.begin(), Svec.end())) {
					it->add_tet(tet_info[0]);
				}

				if (Svec[0] == tet_info[1] && Svec[1] == tet_info[2]) { ordered_sgs[0] = it->getind(); }
				if (Svec[0] == tet_info[1] && Svec[1] == tet_info[3]) { ordered_sgs[1] = it->getind(); }
				if (Svec[0] == tet_info[1] && Svec[1] == tet_info[4]) { ordered_sgs[2] = it->getind(); }
				if (Svec[0] == tet_info[2] && Svec[1] == tet_info[3]) { ordered_sgs[3] = it->getind(); }
				if (Svec[0] == tet_info[2] && Svec[1] == tet_info[4]) { ordered_sgs[4] = it->getind(); }
				if (Svec[0] == tet_info[3] && Svec[1] == tet_info[4]) { ordered_sgs[5] = it->getind(); }
			}

			for (vector<Tri>::iterator it = fcs.begin(); it != fcs.end(); ++it) {
				vector<int> Fvec = { it->geta(), it->getb(), it->getc() };

				if (std::includes(tet_info.begin()+1, tet_info.end(), Fvec.begin(), Fvec.end())) {
					it->add_tet(tet_info[0]);
				}
			}

			vector<int> _egs = {};

			for (unsigned int i = 0; i < sgs.size(); i++) {
				vector<int> comp = { sgs[i].geta_ind(), sgs[i].getb_ind() };

				if (std::includes(tet_info.begin()+1, tet_info.end(), comp.begin(), comp.end())) {
					_egs.push_back(i);
				}
			}

			vector<int> _fcs = {};

			for (unsigned int i = 0; i < fcs.size(); i++) {
				vector<int> comp = { fcs[i].geta(), fcs[i].getb(), fcs[i].getc() };

				if (std::includes(tet_info.begin()+1, tet_info.end(), comp.begin(), comp.end())) {
					_fcs.push_back(i);
				}
			}

			vector<int> _nds_num = {tet_info[1], tet_info[2], tet_info[3], tet_info[4]};

			tetras.push_back(Tetra(pts[tet_info[1]], pts[tet_info[2]], pts[tet_info[3]], pts[tet_info[4]], 
								   _egs, _fcs, tet_info[0], _nds_num, ordered_sgs));
		}
		readFile.close();
	}

	readFile.open(FileName + ".neigh");

	if (readFile.is_open()) {
		string str5;
		getline(readFile, str5);

		int iter_num = 0;

		while (!readFile.eof())
		{
			int num5;
			stringstream stream5;
			vector<int> tet_info2 = {};

			getline(readFile, str5);

			if (str5[0] == '#') {
				break;
			}

			stream5.str(str5);
			while (stream5 >> num5) {
				tet_info2.push_back(num5);
			}
			
			vector<int>::iterator it;
			for (it = tet_info2.begin() + 1; it != tet_info2.end(); it++) {
				if (*it != -1) {
					tetras[tet_info2[0]].add_itets(*it);
				}
			}
		}
		readFile.close();
	}

	return PolyDomain(pts, sgs, fcs, tetras);
}

/*
int main(){
	string FileName;
	cout << "Enter tetgen file name: ";
	cin >> FileName;
	PolyDomain D = BuildPolyDomain(FileName);
	vector<Segment> segs = MarkPoints(D);

	return 0;
}
*/


// tetgen 자료형과 클래스 간 대응 관계
// node(node) - Point
// face - Tri
// ele(tetrahedron) - Tetra
// edge - Segment

// in .edge files, endpoints are indices in the corresponding .node file.