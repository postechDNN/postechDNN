#include "eps_graph.h"

using namespace std;

void test(Eps_Graph G, Free_Point p) {
	for (int i = 4; i < 5; i++) {
		cout << '-' << endl;
		for (auto nb : G.kNN(p, i)) {
			cout << nb.x << ' ' << nb.y << endl;
		}
	}
}

int main() {

	list<Free_Point> _fr_pts;
	_fr_pts.push_back(Free_Point(0.2, 4.8)); _fr_pts.push_back(Free_Point(1.5, 3.5));
	_fr_pts.push_back(Free_Point(2.5, 0.5)); _fr_pts.push_back(Free_Point(3.5, 1.5));

	vector<Point> vers; // polygon vertices
	vers.push_back(Point(0.5, 4.5)); vers.push_back(Point(0.5, 1.5));
	vers.push_back(Point(2.5, 1.5)); vers.push_back(Point(2.5, 4.5));

	vector<Polygon> figures;
	figures.push_back(Polygon{ vers, 0 });

	Eps_Graph G = Eps_Graph(_fr_pts, figures, 0.5); // polygon ¾øÀÌ

	// test(G, _fr_pts.front());

	vector<Free_Point> added_pts;
	added_pts.push_back(Free_Point(1.95, 0.55)); added_pts.push_back(Free_Point(3.35, 1.65));
	added_pts.push_back(Free_Point(3.95, 4.55));

	G.add_freepts(added_pts);

	// test(G, _fr_pts.front());

	vector<Point> second_pol;
	second_pol.push_back(Point(3.9, 4.8)); second_pol.push_back(Point(3.98, 4.61)); 
	second_pol.push_back(Point(3.97, 4.23)); second_pol.push_back(Point(3.6, 4.4));

	G.add_pol(Polygon{second_pol, 1});
	
	// test(G, _fr_pts.front());

	list<Free_Point>::iterator iter = _fr_pts.begin();
	advance(iter, 2);
	// test(G, *iter);

	// test(G, Free_Point(2.5, 0.5));

	G.delete_pol(0);
	// test(G, _fr_pts.front());
	// test(G, *iter);

	G.delete_freept(4);

	/*
	for (int i = 0; i < 10; i++) {
		test(G, *iter);
	}
	*/

	for (int i = 0; i < 10; i++) {
		test(G, _fr_pts.front());
	}
	


	return 0;
}

