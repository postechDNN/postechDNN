#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "dots.h"
#include "propagation.h"

using namespace std;

PolyDomain BuildPolyDomain(string FileName) {
	
	ifstream readFile;

	readFile.open(FileName + ".node");
	// one dummy element for indexing 
	vector<Point> pts = {Point(0, 0, 0)};

	if (readFile.is_open()) {
		string str1;
		getline(readFile, str1);

		while (!readFile.eof())
		{
			double num1;
			stringstream stream1;
			vector<double> pt_info = {};

			getline(readFile, str1);
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
	vector<Segment> edges;

	if (readFile.is_open()) {
		string str2;
		getline(readFile, str2);

		while (!readFile.eof())
		{
			int num2;
			stringstream stream2;
			vector<int> sg_info = {};

			getline(readFile, str2);
			stream2.str(str2);
			while (stream2 >> num2) {
				sg_info.push_back(num2);
			}

			MyVec V1 = Point2Vec(pts[sg_info[1]]), V2 = Point2Vec(pts[sg_info[2]]);
			edges.push_back(Segment(V1, V2, {}));
		}
		readFile.close();
	}

	readFile.open(FileName + ".face");
	vector<Tri> faces;

	if (readFile.is_open()) {
		string str3;
		getline(readFile, str3);

		while (!readFile.eof())
		{
			int num3;
			stringstream stream3;
			vector<int> fc_info = {};

			getline(readFile, str3);
			stream3.str(str3);
			while (stream3 >> num3) {
				fc_info.push_back(num3);
			}

			faces.push_back(Tri(pts[fc_info[1]], pts[fc_info[2]], pts[fc_info[3]]));
		}
		readFile.close();
	}

	readFile.open(FileName + ".ele");
	vector<Tetra> tetras;

	if (readFile.is_open()) {
		string str4;
		getline(readFile, str4);

		while (!readFile.eof())
		{
			int num4;
			stringstream stream4;
			vector<int> tet_info = {};

			getline(readFile, str4);
			stream4.str(str4);
			while (stream4 >> num4) {
				tet_info.push_back(num4);
			}

			tetras.push_back(Tetra(pts[tet_info[1]], pts[tet_info[2]], pts[tet_info[3]], pts[tet_info[4]]));
		}
		readFile.close();
	}

	return PolyDomain(pts, edges, faces, tetras);
}

int main()
{
	string FileName;
	cout << "Enter tetgen file name: ";
	cin >> FileName;
	PolyDomain D = BuildPolyDomain(FileName);
	return 0;
}

// tetgen 자료형과 클래스 간 대응 관계
// node(node) - Point
// face - Tri
// ele(tetrahedron) - Tetra
// edge - Segment

// in .edge files, endpoints are indices in the corresponding .node file.