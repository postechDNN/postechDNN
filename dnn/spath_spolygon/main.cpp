#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "NODE.h"
#include "Edge.h"
#include "Point.h"
#include "Tree.h"
#include "VertexID.h"
#include "PointS.h"
#include "SNode.h"
#include "polygon_operation.h"
#include "polygon_decomposition.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#define NULL_HELPER -1
#define PI 3.1415926535897931
using namespace std;

int read_file(string filePath);
void print_result(int argc, char **argv);
void display();
void reshape(int w, int h);
vector<Point> test_points;

int main(int argc, char **argv) {

	// initialization
	polygon_list = vector<vector<int>>();
	diagonal_list = vector<Edge>();
	outer_diagonal_list = vector<Edge>();
	selected_triangle = vector<int>();
	outer_edge_list = vector<Edge>();

	if (read_file("input/input3.txt") == -1) return 0;

	vector<int> polygon = vector<int>(point_list.size());
	iota(polygon.begin(), polygon.end(), 0);
	polygon_list.push_back(polygon);
	make_big_triangle();

	for (int i = 0; i < (int)point_list.size(); i++) {
		outer_edge_list.push_back(Edge(i, (i + 1) % point_list.size()));
	}

	bool  inside = true;
	vector<Edge> new_d_list(find_monotone_polygons(polygon_list));
	diagonal_list.insert(diagonal_list.end(), new_d_list.begin(), new_d_list.end());
	new_d_list = find_monotone_polygons(outer_polygon_list);
	outer_diagonal_list.insert(outer_diagonal_list.end(), new_d_list.begin(), new_d_list.end());

	new_d_list = triangulate_monotone_polygons(polygon_list);
	diagonal_list.insert(diagonal_list.end(), new_d_list.begin(), new_d_list.end());
	new_d_list = triangulate_monotone_polygons(outer_polygon_list);
	outer_diagonal_list.insert(outer_diagonal_list.end(), new_d_list.begin(), new_d_list.end());

	t_num = int(polygon_list.size());
	dual_tree(v_num);


	PointS point_state;
	while (point_state.step());

	test_points.push_back(Point(-1, 5, 1));
	test_points.push_back(Point(-1, 9, 5));
	for (int i = 0; i < (int)test_points.size(); i++) {
		int found_triangle = point_state.find_triangle(test_points[i]);
		selected_triangle.push_back(found_triangle);
	}


	print_result(argc, argv);

	return 0;
}
void print_result(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);//창 크기 설정
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Shortest Path in a simple Polygon");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
	
	return;
}
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);
}
void display() {
	
	auto max_y = max_element(point_list.begin(), point_list.end()-3, [](Point &a, Point &b) {return a.get_y() < b.get_y(); });
	auto min_y = max_element(point_list.begin(), point_list.end()-3, [](Point &a, Point &b) {return a.get_y() > b.get_y(); });
	auto max_x = max_element(point_list.begin(), point_list.end()-3, [](Point &a, Point &b) {return a.get_x() < b.get_x(); });
	auto min_x = max_element(point_list.begin(), point_list.end()-3, [](Point &a, Point &b) {return a.get_x() > b.get_x(); });

	glLoadIdentity();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(min_x->get_x(), max_x->get_x(),min_y->get_y(),max_y->get_y());

	glColor3f(1, float(0.7137), float(0.7568));
	glLineWidth(10);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < v_num; i++)
		glVertex2d(point_list[i].get_x(), point_list[i].get_y());
	glEnd();
	

	/*glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 3; i++)
		glVertex2d(point_list[bigT[i]].get_x(), point_list[bigT[i]].get_y());
	glEnd();
	*/
	glLineWidth(3);
	glColor3f(float(0.6), float(0.6), float(0.6));
	for (int i = 0; i < (int)diagonal_list.size(); i++) {
		int origin = diagonal_list[i].get_origin();
		int dest = diagonal_list[i].get_dest();
		glBegin(GL_LINES);
			glVertex2d(point_list[origin].get_x(), point_list[origin].get_y());
			glVertex2d(point_list[dest].get_x(), point_list[dest].get_y());
		glEnd();
	}
	/*
	for (int i = 0; i < (int)outer_diagonal_list.size(); i++) {
		int origin = outer_diagonal_list[i].get_origin();
		int dest = outer_diagonal_list[i].get_dest();
		glBegin(GL_LINES);
			glVertex2d(point_list[origin].get_x(), point_list[origin].get_y());
			glVertex2d(point_list[dest].get_x(), point_list[dest].get_y());
		glEnd();
	}*/

	for (int t = 0; t <int(selected_triangle.size()); t++) {
		glPointSize(8.0f);

		glEnable(GL_POINT_SMOOTH);
		glColor3d(0, 0.47, 0.43);
		glBegin(GL_POINTS); //starts drawing of points
		glVertex2d(test_points[t].get_x(), test_points[t].get_y());
		glEnd();
		
		glColor3d(0, 0, 1);
		if (selected_triangle[t] == -1) {
			continue;
		}
		else if (selected_triangle[t] >= t_num) {
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 3; i++) {
				Point p = point_list[outer_polygon_list[selected_triangle[t] - t_num][i]];
				glVertex2d(p.get_x(), p.get_y());
			}
			glEnd();
		} 
		else {
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 3; i++) {
				Point p = point_list[polygon_list[selected_triangle[t]][i]];
				glVertex2d(p.get_x(), p.get_y());
			}
			glEnd();
		}
		
		glPointSize(8.0f);
		
		glEnable(GL_POINT_SMOOTH);
		glColor3d(0, 0.47, 0.43);
		glBegin(GL_POINTS); //starts drawing of points
			glVertex2d(test_points[t].get_x(), test_points[t].get_y());
		glEnd();
	}
	glutSwapBuffers();
	return;
}
int read_file(const string filePath) {
	ifstream openFile(filePath.data());
	if (openFile.is_open()) {

		openFile >> v_num;
		point_list = vector<Point>();
		point_list.reserve(v_num);

		point_type x, y;
		int id = 0;
		while (openFile >> x >> y) {
			Point p = Point(id, x, y);
			point_list.push_back(p);
			id++;
		}
		openFile.close();
	}
	else {
		cout << "No such file" << endl;
		return -1;
	}
	return 1;
}

