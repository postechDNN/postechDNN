#include "compressed_quadtree.h"
#include <chrono>

using namespace std;
using namespace chrono;

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

#define EPSILON		0.05

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
	int path_len = strlen(argv[2]) + 2;
	char* path = (char*)calloc(path_len, sizeof(char));
	path[0] = 'a';
	strcpy_s(path + 1, path_len, argv[2]);

	FILE* output = NULL;
	fopen_s(&output, path, "w");
	fprintf(output, "%d\n", query_size);

	char* path_time = (char*)calloc(path_len, sizeof(char));
	path_time[0] = 'q';
	strcpy_s(path_time + 1, path_len, argv[2]);

	FILE* output_time = NULL;
	fopen_s(&output_time, path_time, "w");

	system_clock::time_point start, end;
	duration<double> sec;
	printf("q:%d\n", query_size);

	for (int i = 0; i < query_size; i++) {
		start = system_clock::now();
		list<pair<Point*, double>> query = tree.knn(k, queries[i], epsilon);
		end = system_clock::now();
		sec = end - start;
		fprintf(output_time, "%g\n", sec);

		// printf("query #%d: (%g, %g)\n", i + 1, queries[i].coords[X], queries[i].coords[Y]);

		for (list<pair<Point*, double>>::iterator i = query.begin(); i != query.end(); i++) {
			// printf("(%g, %g)\n", (*i->first)[X], (*i->first)[Y]);
			fprintf(output, "%g %g\n", (*i->first)[X], (*i->first)[Y]);
		}
		// printf("\n");
	}
	fclose(output);
	fclose(output_time);

	printf("done.\n");
	END(0);
}