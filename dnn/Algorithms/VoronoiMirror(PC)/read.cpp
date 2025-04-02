#include "voronoi_camera.h"

void read_data(vector <sites> &s, vector <vertices> &v)
{

	unsigned int no_of_sites, no_of_vertices;
	int temp;
	ifstream file1, file2;
	char inputFilename1[] = "./res/in.txt";
	char inputFilename2[] = "./res/sites_coordinates.txt";

	file1.open(inputFilename1, ios::in);
	if (!file1)
	{
		cerr << "Can't open input file " << inputFilename1 << endl;
		getchar();
		exit(1);
	}

	file2.open(inputFilename2, ios::in);
	if (!file2)
	{
		cerr << "Can't open input file " << inputFilename2 << endl;
		getchar();
		exit(1);
	}

	cout << "\n\nReading from files ";
	cout << "\n\nBuilding initial voronoi structure........" << endl;
	file1 >> no_of_vertices;

	vertices temp1;

	for (unsigned int i = 0; i < no_of_vertices; i++)
	{
		file1 >> temp1.x_cor;
		file1 >> temp1.y_cor;
		v.push_back(temp1);
	}

	file1 >> no_of_sites;

	sites temp2;
	unsigned int no;
	for (unsigned int i = 0; i < no_of_sites; i++)
	{
		file2 >> temp2.x_cor;
		file2 >> temp2.y_cor;
		file1 >> no;
		for (unsigned int j = 0; j < no; j++) { file1 >> temp; temp2.list.push_back(temp); }
		for (unsigned int j = 0; j < no; j++) { file1 >> temp; temp2.near_site.push_back(temp); }
		temp2.active = -1;
		temp2.allowed = 0;
		temp2.create_count = 0;
		temp2.move_count = 0;
		temp2.disform_x = 0;
		temp2.disform_y = 0;
		s.push_back(temp2);
		temp2.list.clear();
		temp2.near_site.clear();
	}

	file1.close();
	file2.close();
	cout << "\n\nSuccess building initial voronoi structure!" << endl;
}
