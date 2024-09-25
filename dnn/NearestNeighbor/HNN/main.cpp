#include "compressed_quadtree.h"
#include <GL/glfw3.h>
#include <GL/freeglut.h>
#include <chrono>

using namespace std;
using namespace chrono;

typedef double type_point;

// #define DEBUG	1

#define DIM		2	// dimension
#define X		0
#define Y		1

enum InputMod { defualt, Distance, Insert, Delete, Query };
InputMod NowMod = defualt;
int MenuCentorX = 700;
int MenuCentorY = 500;
int MenuH = 100;
int MenuW = 100;
int MenuScaleX = 50;
int MenuScaleY = 50;

struct Point {
	type_point coords[DIM];

	Point() {
		coords[X] = 0.0;
		coords[Y] = 0.0;
	};

	Point(double x, double y) {
		coords[X] = x;
		coords[Y] = y;
	}

	Point(const Point& other) {
		coords[X] = other.coords[X];
		coords[Y] = other.coords[Y];
	}

	type_point operator[](size_t idx) const { return coords[idx]; }
	type_point& operator[](size_t idx) { return coords[idx]; }
};

std::vector<Point> cond;
std::vector<Point> InputPoints;
int k_value = 3;
Point QueryPoint(-1, -1);
std::vector<int> NNPoints;
CompressedQuadtree<Point>* myd;

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
/*
points_file
	- first line: input_size
	- the others: inputs_coords_x inputs_coords_y

queries_file
	- first line: query_size k
	- the others: queries_coords_x queries_coords_y
*/
/*
int main(int argc, char** argv) {


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
*/

void reshape(int w, int h) {

	glViewport(0, 0, w, h);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600);
}

void display() {
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//gluOrtho2D(min_x, max_x, min_y, max_y);


	glLineWidth(8);
	glPointSize(5.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glEnable(GL_POINT_SMOOTH);




	//boundary for distance function
	glBegin(GL_LINE_LOOP);
	glVertex2d(MenuCentorX + MenuW, MenuCentorY + MenuH);
	glVertex2d(MenuCentorX + MenuW, MenuCentorY - MenuH);
	glVertex2d(MenuCentorX - MenuW, MenuCentorY - MenuH);
	glVertex2d(MenuCentorX - MenuW, MenuCentorY + MenuH);
	glEnd();
	//
	glBegin(GL_POINTS);
	glVertex2d(MenuCentorX, MenuCentorY);
	glEnd();
	//


	int ModPosStartX = 630;
	int ModPosStartY = 370;
	//print about mod
	glRasterPos2d(ModPosStartX, ModPosStartY);
	const char* modst = "defualt";
	switch (NowMod) {
	case Distance:
		modst = "DIST";
		break;
	case Insert:
		modst = "INSERT";
		break;
	case Query:
		modst = "Query";
		break;
	}
	int ModLen = strlen(modst);
	for (unsigned int i = 0; i < ModLen; i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, modst[i]);
	//about number of k

	char theK[4] = "k=3";
	glRasterPos2d(ModPosStartX + 120, ModPosStartY);
	theK[2] = k_value + '0';
	for (unsigned int i = 0; i < 3; i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, theK[i]);


	if (NowMod == Distance) {
		if (!cond.empty()) {
			glBegin(GL_POINTS);
			glVertex2d(MenuCentorX, MenuCentorY);
			glEnd();
		}


		glBegin(GL_LINES);
		for (unsigned int i = 0; i + 1 < cond.size(); i++) {
			glVertex2d(MenuCentorX + MenuScaleX * cond[i].coords[0], MenuCentorY + MenuScaleY * cond[i].coords[1]);
			glVertex2d(MenuCentorX + MenuScaleX * cond[i + 1].coords[0], MenuCentorY + MenuScaleY * cond[i + 1].coords[1]);
		}
		glEnd();
	}
	else {
		glBegin(GL_LINE_LOOP);
		glEnd();
	}

	//print input point
	for (unsigned int i = 0; i < InputPoints.size(); i++) {

		if (NowMod == Query && QueryPoint.coords[0] > 0 && QueryPoint.coords[1] > 0 && NNPoints[i] == 1) {
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		else glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2d(InputPoints[i].coords[0], InputPoints[i].coords[1]);
		glEnd();
	}

	glColor3f(0.0f, 0.0f, 0.0f);
	if (NowMod == Query && QueryPoint.coords[0] > 0 && QueryPoint.coords[1] > 0) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2d(QueryPoint.coords[0], QueryPoint.coords[1]);
		glEnd();
	}
	glColor3f(0.0f, 0.0f, 0.0f);
	/*
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2d(point_list[0].get_x(), point_list[0].get_y());
	glEnd();

	glColor3f(0.5f, 0.7f, 0.30f);
	glBegin(GL_POINTS);

	glEnd();

	glLineWidth(3);//every diagonal
	glColor3f(float(0.6), float(0.6), float(0.6));


	glColor3f(1.0f, 0.0f, 1.0f);

	glColor3d(0, 0.47, 0.43);



	glColor3f(0.0f, 1.0f, 1.0f);
	*/

	glutSwapBuffers();
	return;

}

void AddPoint(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			if (NowMod == Distance) {
				y = 600 - y;
				if (MenuCentorX - MenuW <= x && x <= MenuCentorX + MenuW && MenuCentorY - MenuH <= y && y <= MenuCentorY + MenuH) {
					cond.push_back(Point(((double)(x - MenuCentorX)) / MenuScaleX, ((double)(y - MenuCentorY)) / MenuScaleY));
				}
			}
			else if (NowMod == Insert) {
				InputPoints.push_back(Point(x, 600 - y));
			}
			else if (NowMod == Query) {
				QueryPoint = Point(x, 600 - y);

			}

		}

	}

}


void ModChange(unsigned char key, int x, int y) {
	if (NowMod == Distance) {
		delete myd;
		myd = new CompressedQuadtree<Point>(DIM, &cond[0], cond.size());

		switch (key) {
		case 'i':
		case 'I':
			NowMod = Insert;
			break;
		case 'q':
		case 'Q':
			QueryPoint = Point(-1, -1);
			NowMod = Query;
			break;
		case 'd':
		case 'D':
			NowMod = defualt;
			break;
		}

	}
	else {
		switch (key) {
		case 'i':
		case 'I':
			NowMod = Insert;
			break;
		case 'q':
		case 'Q':
			QueryPoint = Point(-1, -1);
			NowMod = Query;
			break;
		case 'd':
		case 'D':
			if (!cond.empty()) cond.clear();
			NowMod = Distance;
			break;
		}
	}

	if (key >= '1' && key <= '9') {
		k_value = key - '0';
	}

	glutPostRedisplay();
}


int main(int argc, char** argv) {
	cond.push_back(Point(1, 1));
	cond.push_back(Point(1, -1));
	cond.push_back(Point(-1, -1));
	cond.push_back(Point(-1, 1));
	myd = new CompressedQuadtree<Point>(DIM, &cond[0], cond.size());

	glutInit(&argc, argv);
	glutInitWindowPosition(100, 0);
	glutInitWindowSize(800, 600);//창 크기 설정
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Heuristic Nearest neighbor");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(AddPoint);
	glutKeyboardFunc(ModChange);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();

	return 0;

}