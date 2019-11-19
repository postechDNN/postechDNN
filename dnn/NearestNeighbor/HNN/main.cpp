#include "compressed_quadtree.h"

using namespace std;

typedef double type_point;

// #define DEBUG	1

#define DIM		2	// dimension
#define X		0
#define Y		1

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

#define WAIT()	getchar();
#define END(x)	WAIT(); exit(x);

Point* readPoints(char* path, int& size, int& k) {
	FILE* file = NULL;
	fopen_s(&file, path, "r");

	if (!file) {
		printf("Invalid file: %s\n", path);
		END(1);
	}

	if (k < 0)
		fscanf_s(file, "%d", &size);
	else
		fscanf_s(file, "%d %d", &size, &k);

	Point* points = new Point[size];
	for (int i = 0; i < size; i++)
		fscanf_s(file, "%lf %lf", &points[i].coords[X], &points[i].coords[Y]);

#ifdef DEBUG
	printf("%s:\n", path);
	for (int i = 0; i < size; i++)
		printf("%g %g\n", points[i].coords[X], points[i].coords[Y]);
	printf("\n");
#endif // DEBUG

	fclose(file);
	return points;
}

#define EPSILON		0.1

int main(int argc, char** argv) {
	/*
	points_file
		- first line: input_size
		- the others: inputs_coords_x inputs_coords_y

	queries_file
		- first line: query_size k
		- the others: queries_coords_x queries_coords_y
	*/

	// input params exception
	if (argc < 2) {
		printf("Usage: executive_file points_file [queries_file]\n");
		END(-1);
	}

	// read input points
	int k = -1;
	int input_size = 0;
	Point* inputs = readPoints(argv[1], input_size, k);

	// build compressed quadtree
	CompressedQuadtree<Point> tree(DIM, inputs, input_size);

	if (argc == 2) {
		printf("no queries_file\n");
		END(-2);
	}

	// read query points
	k = 0;
	int query_size = 0;
	Point* queries = readPoints(argv[2], query_size, k);

	double epsilon = EPSILON;

	// query
	for (int i = 0; i < query_size; i++) {
		list<pair<Point*, double>> query = tree.knn(k, queries[i], epsilon);
		printf("query #%d: (%g, %g)\n", i + 1, queries[i].coords[X], queries[i].coords[Y]);

		for (list<pair<Point*, double>>::iterator i = query.begin(); i != query.end(); i++)
			printf("(%g, %g)\n", (*i->first)[X], (*i->first)[Y]);
		printf("\n");
	}

	printf("done.\n");
	END(0);
}