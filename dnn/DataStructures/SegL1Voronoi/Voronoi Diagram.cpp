#include <cfloat>
#include <cmath>
#include <fstream>
#include "Data Structure.h"
using namespace std;

//////////////////////////////////
/*Voronoi Diagram*/

int main() {
	
	vector < Site *> site_vector;
	//vector<Vertexptr> vertex_vector;
	//vector<Edgeptr> edge_vector;
	vector<Vertex *> vertex_vector;
	vector<Edge *> edge_vector;
	//site_vector.push_back(new Site(70.0, 70.0, 100.0, 100.0));
	//site_vector.push_back(new Site(80.0, 140.0, 140.0, 140.0));
	//site_vector.push_back(new Site(170.0, 90.0, 170.0, 160.0));
	//site_vector.push_back(new Site(110.0, 200.0, 160.0, 250.0));
	site_vector.push_back(new Site(90.0, 170.0, 90.0, 280.0));
	site_vector.push_back(new Site(100.0, 100.0, 150.0, 150.0));
	site_vector.push_back(new Site(150.0, 240.0, 200.0, 190.0));
	/*site_vector.push_back(new Site(230.0, 270.0, 280.0, 270.0));
	site_vector.push_back(new Site(180.0, 90.0, 250.0, 90.0));
	site_vector.push_back(new Site(250.0, 140.0, 300.0, 190.0));*/

	Voronoi voronoi_diagram = Voronoi(site_vector, vertex_vector, edge_vector);
	voronoi_diagram.create();

	/*ofstream voronoi("voronoi_diagram.txt");

	voronoi << site_vector.size() << endl;
	for (s_it = site_vector.begin(); s_it != site_vector.end(); s_it++) {
		voronoi << (*s_it)->get_l_endpoint().first << " " << (*s_it)->get_l_endpoint().second << " "
			<< (*s_it)->get_r_endpoint().first << " " << (*s_it)->get_r_endpoint().second << endl;
	}

	voronoi << vertex_vector.size() << endl;
	for (v_it = vertex_vector.begin(); v_it != vertex_vector.end(); v_it++) {
		voronoi << (*v_it)->get_vertex().first << " " << (*v_it)->get_vertex().second << endl;
	}
	
	voronoi << edge_vector.size();
	for (e_it = edge_vector.begin(); e_it != edge_vector.end(); e_it++) {

	}
	*/

	return 0;
}