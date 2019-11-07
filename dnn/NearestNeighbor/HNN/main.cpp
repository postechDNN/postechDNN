#include "compressed_quadtree.h"

using namespace std;

typedef double type_point;
#define DIM	2	// dimension
#define X	0
#define Y	1

struct Point {
	type_point coords[DIM];

	Point() {
		coords[X] = 0.0;
		coords[Y] = 0.0;
	};

	Point(const Point& other) {
		coords[X] = other.coords[X];
		coords[Y] = other.coords[Y];
	}

	type_point operator[](size_t idx) const { return coords[idx]; }
	type_point& operator[](size_t idx) { return coords[idx]; }
};

#define INPUT_SIZE	10	// the number of input points
#define QUERY_SIZE	2	// the number of query points
#define EPSILON		0.1

int main(int argc, char** argv) {
	// TODO:fileio not stdio as below
	/*
	if (argc < 2) {
		printf("usage: executive_file points_file [queries_file] [epsilon]\n");
		return 1;
	}
	*/

	// read input points
	printf("read input points %d\n", INPUT_SIZE);
	Point inputs[INPUT_SIZE] = {};
	for (int i = 0; i < INPUT_SIZE; i++)
		scanf_s("%lf %lf", &inputs[i].coords[X], &inputs[i].coords[Y]);

	// build compressed quadtree
	CompressedQuadtree<Point> tree(DIM, inputs, INPUT_SIZE);

	// read query points
	printf("read query points %d\n", QUERY_SIZE);
	Point queries[QUERY_SIZE] = {};
	for (int i = 0; i < QUERY_SIZE; i++)
		scanf_s("%lf %lf", &queries[i].coords[X], &queries[i].coords[Y]);

	// read query epsilon
	printf("read query epsilon: ");
	double epsilon = 0.0;
	scanf_s("%lf", &epsilon);
	
	// read k
	printf("read k: ");
	int k = 0;
	scanf_s("%d", &k);

	for (int i = 0; i < QUERY_SIZE; i++) {
		list<pair<Point*, double>> query = tree.knn(k, queries[i], epsilon);
		printf("query %d: (%f, %f)\n", i, queries[i].coords[X], queries[i].coords[Y]);

		for (list<pair<Point*, double>>::iterator i = query.begin(); i != query.end(); i++)
			printf("(%f, %f)\n", (*i->first)[X], (*i->first)[Y]);
	}

	printf("done.\n");
	while (getchar());
}