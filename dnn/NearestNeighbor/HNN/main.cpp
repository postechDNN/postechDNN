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

#define WAIT()	getchar();
#define END(x)	WAIT(); return(x);

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
	FILE* file_p = NULL;
	fopen_s(&file_p, argv[1], "r");

	if (!file_p) {
		printf("Invalid points_file: %s\n", argv[1]);
		END(1);
	}

	int input_size = 0;
	fscanf_s(file_p, "%d", &input_size);

	Point* inputs = new Point[input_size];
	for (int i = 0; i < input_size; i++) {
		fscanf_s(file_p, "%lf %lf", &inputs[i].coords[X], &inputs[i].coords[Y]);
		printf("%g %g\n", inputs[i].coords[X], inputs[i].coords[Y]);
	}

	fclose(file_p);

	// build compressed quadtree
	CompressedQuadtree<Point> tree(DIM, inputs, input_size);

	if (argc == 2) {
		printf("no queries_file\n");
		END(-2);
	}

	// read query points
	FILE* file_q = NULL;
	fopen_s(&file_q, argv[2], "r");

	if (!file_q) {
		printf("Invalid queries_file: %s\n", argv[2]);
		END(2);
	}

	int k = 0;
	int query_size = 0;
	double epsilon = EPSILON;

	fscanf_s(file_q, "%d %d", &query_size, &k);

	Point* queries = new Point[query_size];
	for (int i = 0; i < query_size; i++)
		fscanf_s(file_q, "%lf %lf", &queries[i].coords[X], &queries[i].coords[Y]);

	fclose(file_q);

	// query
	for (int i = 0; i < query_size; i++) {
		list<pair<Point*, double>> query = tree.knn(k, queries[i], epsilon);
		printf("\nquery #%d: (%g, %g)\n", i+1, queries[i].coords[X], queries[i].coords[Y]);

		for (list<pair<Point*, double>>::iterator i = query.begin(); i != query.end(); i++)
			printf("(%g, %g)\n", (*i->first)[X], (*i->first)[Y]);
	}

	printf("done.\n");
	END(0);
}