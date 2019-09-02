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
#include <sstream>
#include "hourglass_operation.h"

#define NULL_HELPER -1
#define PI 3.1415926535897931
using namespace std;

int read_file(string filePath);
void print_result(int argc, char **argv);
void display();
void reshape(int w, int h);
vector<Point> test_points;
vector<int> sequence_diagonal;
int d_size;
Hourglass final_hour;
PointS point_state;

Hourglass test_hourglass;

////////////////////
vector<Point> polygon_boundary;
////////////////////

Hourglass find_shortest_path(vector<Point> test_points);

int w_h=800, w_w=800;

void construct_hourglasses() {
	for (int i = 0; i < d_size; i++) {
		for (int j = 0; j < d_size; j++) {
			if (s_graph[i][j] == connected) {
				construct_hourglass(i, j);
			}
		}
	}
}
bool findPath(SNode * root, vector<SNode *> &path, SNode * k)
{
	// base case 
	if (root == NULL) return false;
	if (root == k)
		return true;

	// Store this node in path vector. The node will be removed if 
	// not in path from root to k 
	path.push_back(root);

	// See if the k is same as root's key 

	// Check if k is found in left or right sub-tree 
	if ((root->get_left_children() && findPath(root->get_left_children(), path, k)) ||
		(root->get_right_children() && findPath(root->get_right_children(), path, k)))
		return true;

	// If not present in subtree rooted with root, remove root from 
	// path[] and return false 
	path.pop_back();
	return false;
}
//find triangle path
bool findPath(Triangle * root, vector<Triangle *> &path, Triangle * k)
{
	// base case 
	if (root == NULL) return false;
	path.push_back(root);
	if (root == k)
		return true;
	bool * check = root->get_dual_check_children();
	int * ad_tr = root->get_adjacent_triangles();
	// Check if k is found in left or right sub-tree 
	for (int i = 0; i < 3; i++) {
		if (check[i]&&findPath(&t_list[ad_tr[i]],path,  k))
			return true;
	}
	path.pop_back();
	return false;
}
Triangle * find_common_triangle(Triangle *t1, Triangle *t2) {
	vector<Triangle *> path1, path2;
	if (!findPath(&t_list[t_head], path1, t1) || !findPath(&t_list[t_head], path2, t2))
		return NULL;

	int i;
	for (i = 0; i < (int)path1.size() && i < (int)path2.size(); i++)
		if (path1[i] != path2[i])
			break;
	return path1[i - 1];
}
SNode * find_common_ancestor(int _t1, int _t2) {//input: triangle where the test_points are located
	// to store paths to n1 and n2 from the root 
	vector<SNode *> path1, path2;

	// Find paths from root to n1 and root to n1. If either n1 or n2 
	// is not present, return -1 
	Triangle t1 = t_list[_t1], t2 = t_list[_t2];
	SNode* s1 = t1.get_node(), * s2 = t2.get_node();
	if (!findPath(s_head, path1, s1) || !findPath(s_head, path2, s2))
		return NULL;

	/* Compare the paths to get the first different value */
	int i;
	for (i = 0; i < (int)path1.size() && i < (int)path2.size(); i++)
		if (path1[i] != path2[i])
			break;
	SNode * common_ancestor = path1[i - 1];

	Triangle * common_low_triangle = find_common_triangle(&t_list[_t1], &t_list[_t2]);
	bool check = false;
	bool inclusive;
	std::vector<SNode*>::reverse_iterator rit;
	vector<Triangle *> dummy;
	for (rit = path2.rbegin(); rit != path2.rend(); rit++) {
		if (!check) {
			inclusive = true;
			int * t = edge_triangle[(*rit)->get_diagonal()];
			for (int i = 0; i < 2; i++) {
				if (t[i] != -1) {
					//Triangle &target = t_list[t[i]];
					bool b1 = findPath(&t_list[t[i]],dummy, &t_list[_t1]);
					bool b2 = findPath(&t_list[t[i]],dummy,&t_list[_t2]);
					if (((&t_list[t[i]])!=(&t_list[_t1]))&&((&t_list[t[i]]) != (&t_list[_t2]))&&(!b1 || b2) && (b1 || !b2) && (common_low_triangle != &t_list[t[i]]))
						inclusive = false;
				}
			}
			if(inclusive) sequence_diagonal.push_back((*rit)->get_diagonal());
			if (common_ancestor == (*rit)) {
				check = true;
			}
		}
	}

	vector<SNode*>::iterator it;
	check = false;
	for (it = path1.begin(); it != path1.end(); it++) {
		if (common_ancestor == *it) {
			check++;
		}
		else if (check) {
			inclusive = true;
			int * t = edge_triangle[(*it)->get_diagonal()];
			for (int i = 0; i < 2; i++) {
				if (t[i] != -1) {
					bool b1 = findPath(&t_list[t[i]], dummy, &t_list[_t1]);
					bool b2 = findPath(&t_list[t[i]], dummy, &t_list[_t2]);
					if (((&t_list[t[i]]) != (&t_list[_t1])) && ((&t_list[t[i]]) != (&t_list[_t2])) && (!b1 || b2) && (b1 || !b2) && (common_low_triangle != &t_list[t[i]]))
						inclusive = false;
				}
			}
			if(inclusive) sequence_diagonal.push_back((*it)->get_diagonal());
		}
	}
	
	return common_ancestor;
}
void free_data() {
	for (int i = 0; i < v_num; i++) {
		delete(edge_finder[i]);
	}
	delete(edge_finder);

	for (int i = 0; i < (int)(diagonal_list.size()); i++) {
		delete(edge_triangle[i]);
		delete(s_graph[i]);
	}
	delete(s_graph);
	delete(edge_triangle);

	delete_snodes(s_head);

	delete(t_list);
}

vector<Point> input_polygon;
void white_page()
{
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 800);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(5);
	glPointSize(8.0f);
	//glEnable(GL_POINT_SMOOTH);

	glBegin(GL_POINTS);
	glColor3f(1.0, float(0.7137), float(0.7568));
	for (int i = 0; i < input_polygon.size(); i++)
	{
		glVertex3f(input_polygon[i].get_x(), input_polygon[i].get_y(), 0.0);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, float(0.7137), float(0.7568));
	for (int i = 0; i < input_polygon.size(); i++)
	{
		glVertex3f(input_polygon[i].get_x(), input_polygon[i].get_y(), 0.0);
	}
	glEnd();


	glutSwapBuffers();
}
void add_polygon_points(int button, int state, int x, int y)
{
	float fx = x;
	float fy = 800-y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Point new_point(fx, fy);
		input_polygon.push_back(new_point);
		glFlush();
	}

	glutPostRedisplay();
}
void end_of_polygon(int key, int x, int y)
{
	if (key == GLUT_KEY_DOWN) //delete all the points
		input_polygon.clear();
	else if (key == GLUT_KEY_LEFT) //delete a single point
	{
		if (!input_polygon.empty())
			input_polygon.pop_back();
	}
	else if (key == GLUT_KEY_UP)//save!
	{
		string filePath = "input/new_input_please_save_separately.txt";

		ofstream writeFile(filePath.data());
		if (writeFile.is_open())
		{
			writeFile << input_polygon.size() << endl;
			for (int i = 0; i < input_polygon.size(); i++)
			{
				writeFile << input_polygon[i].get_x() << " " << input_polygon[i].get_y() << endl;
			}
		}

		exit(10);
	}

	glutPostRedisplay();
}
void add_input_file(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 0);
	glutInitWindowSize(800, 800);//창 크기 설정
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Create New Input Polygon");
	glutReshapeFunc(reshape);
	
	glutDisplayFunc(white_page);
	glutMouseFunc(add_polygon_points);
	glutSpecialFunc(end_of_polygon);
	//glutKeyboardFunc(clear_test_points);
	glutPostRedisplay();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();
	
	return;
}
void preprocess_polygon()
{
	vector<int> polygon = vector<int>(point_list.size());

	iota(polygon.begin(), polygon.end(), 0);
	polygon_list.push_back(polygon);
	make_big_triangle();

	for (int i = 0; i < (int)point_list.size(); i++) {
		outer_edge_list.push_back(Edge(i, (i + 1) % point_list.size()));
	}
	bool  inside = true;
	vector<Edge> new_d_list(find_monotone_polygons(polygon_list));//divides P into smaller polygons(not necessarily triangles) -> 아직 test point 안 잡음
	diagonal_list.insert(diagonal_list.end(), new_d_list.begin(), new_d_list.end());
	new_d_list = find_monotone_polygons(outer_polygon_list);
	outer_diagonal_list.insert(outer_diagonal_list.end(), new_d_list.begin(), new_d_list.end());

	new_d_list = triangulate_monotone_polygons(polygon_list);
	diagonal_list.insert(diagonal_list.end(), new_d_list.begin(), new_d_list.end());
	new_d_list = triangulate_monotone_polygons(outer_polygon_list);
	outer_diagonal_list.insert(outer_diagonal_list.end(), new_d_list.begin(), new_d_list.end());

	d_size = diagonal_list.size();
	t_num = int(polygon_list.size());
	dual_tree(v_num);

	construct_hourglasses();

	diagonal_list = vector<Edge>(diagonal_list.begin(), diagonal_list.begin() + d_size);
	
}
int main(int argc, char **argv) {
	polygon_list = vector<vector<int>>();
	diagonal_list = vector<Edge>();
	outer_diagonal_list = vector<Edge>();
	selected_triangle = vector<int>();
	outer_edge_list = vector<Edge>();
	sequence_diagonal = vector<int>();
	null_edge_list = vector<Edge *>();
	init_hourglass_val();

	int menu;
	do {
		printf("Type '1' to add input polygon or '2' to test the input files\n");
		scanf("%d", &menu);
		if(menu==1)//add input files
			add_input_file(argc, argv);
		else if (menu==2)// find shortest path in polygon
		{
			printf("you have chosen to test out the program. Which input file would you like to test on? Type in the integer number only\n");
			int number = -1;
			scanf("%d", &number);
			string filename;
			stringstream s;

			s << number;

			filename = s.str();

			filename = "input/input" + filename + ".txt";
			if (read_file(filename) != -1) {

				preprocess_polygon();
				point_state = PointS();
			
				while (point_state.step());
				print_result(argc, argv);

				return 0;
			}
			system("cls");
			menu = 3;
		}
		else
		{
			printf("Type in a valid request\n");
		}
	} while (menu != 1 && menu != 2);
	
}

Hourglass find_shortest_path(vector<Point> test_points) //input : two test points , returns final hourglass(string) representing shortest path of the two points
{
	selected_triangle = vector<int>();
	sequence_diagonal = vector<int>();
	for (int i = 0; i < (int)test_points.size(); i++) {
		int found_triangle = point_state.find_triangle(test_points[i]);
		selected_triangle.push_back(found_triangle);
	}
	Hourglass final_hourglass;
	if (selected_triangle[0] == selected_triangle[1]) {
		final_hourglass.set_string(new String(point_list.size() - 1, point_list.size() - 2));
		final_hourglass.set_first_edge(Edge(point_list.size() - 1));
		final_hourglass.set_second_edge(Edge(point_list.size() - 2));
		glutPostRedisplay();
		return final_hourglass;
	}
	SNode * common_ancestor = find_common_ancestor(selected_triangle[0], selected_triangle[1]);
	Hourglass origin, dest;

	origin = construct_hourglass_point(point_list.size() - 1, sequence_diagonal.front());
	dest = construct_hourglass_point(point_list.size() - 2, sequence_diagonal.back());
	if (sequence_diagonal.size() == 1) {
		final_hourglass = concatenate_hourglasses(origin, dest);
	}
	else if (sequence_diagonal.size() >= 2) {
		int h_num = s_graph[sequence_diagonal[0]][sequence_diagonal[1]];
		h_num = (h_num == -1) ? s_graph[sequence_diagonal[1]][sequence_diagonal[0]] : h_num;
		final_hourglass = hourglass_list[h_num];

		Hourglass temp;
		for (int i = 2; i < (int)sequence_diagonal.size(); i++) {
			int d_1 = sequence_diagonal[i - 1];
			int d_2 = sequence_diagonal[i];
			int h_num = s_graph[d_1][d_2];
			h_num = (h_num == -1) ? s_graph[d_2][d_1] : h_num;
			temp = hourglass_list[h_num];
			final_hourglass = concatenate_hourglasses(temp, final_hourglass);
		}
		final_hourglass = concatenate_hourglasses(origin, final_hourglass);
		final_hourglass = concatenate_hourglasses(final_hourglass, dest);
	}
	for (int i = 0; i<2; i++)
		diagonal_list.pop_back();

	return final_hourglass;
}
void add_test_point(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		
		if (button == GLUT_LEFT_BUTTON) {
			cout << x << "," << y << endl;
			Point p(-1, x*(max_x - min_x) / w_w + min_x, (w_h - y)*(max_y - min_y) / w_h + min_y);
			cout << "p : " << p.get_x() << "," << p.get_y() << endl;
			int test_tri = point_state.find_triangle(p);
			cout << "t_num" << test_tri << endl;

			if (test_tri < (int)polygon_list.size() && (int)test_points.size()<2) {
				test_points.push_back(p);
				point_list.push_back(p); 
			}

			if (test_points.size() == 2)
				final_hour = find_shortest_path(test_points); // RETURNS SINGLE FINAL HOURGLASS FOR THE TWO POINTS IN THE INPUT VECTOR

			glutPostRedisplay();
		}
	}

}
void clear_test_points() {
	for (int i = 0; i < (int)test_points.size(); i++)
		point_list.pop_back();
	test_points = vector<Point>();
	selected_triangle = vector<int>();
	sequence_diagonal = vector<int>();
}
void clear_test_points(unsigned char key, int x, int y) {
	switch (key) {
	case ' ':
		clear_test_points();
		break;
	}
	glutPostRedisplay();
} 
void print_result(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitWindowPosition(100, 0);
	glutInitWindowSize(800, 800);//창 크기 설정
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Shortest Path in a simple Polygon");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(add_test_point);
	glutKeyboardFunc(clear_test_points);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();
	free_data();
	return;
}
void reshape(int w, int h) {
	w_w = w;
	w_h = h;
	glViewport(0, 0, w, h);
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);
}
void display_point(Point p) {
	glBegin(GL_POINTS); //starts drawing of points
	glVertex2d(p.get_x(), p.get_y());
	glEnd();
	return;
}
void display_point(int p) {
	glBegin(GL_POINTS); //starts drawing of points
	glVertex2d(point_list[p].get_x(), point_list[p].get_y());
	glEnd();
	return;
}
void display_edge(int p1, int p2) {
	glBegin(GL_LINES);
	glVertex2d(point_list[p1].get_x(), point_list[p1].get_y());
	glVertex2d(point_list[p2].get_x(), point_list[p2].get_y());
	glEnd();
	return;
}
void display_edge(Edge e) {
	if(e.get_origin()!= -1)
		display_edge(e.get_origin(), e.get_dest());
	return;
}
void display_chain(Chain * chain) {
	if (chain == NULL) return;
	vector<int> p0_list = chain->get_point_list();
	for (int p = 0; p < (int)p0_list.size(); p++) {
		if (p == 0) {
			display_point(p0_list[p]);
		}
		else {
			display_edge(p0_list[p - 1], p0_list[p]);
		}
	}
	return;
}
void display_string(String * s) {
	display_chain(s->get_chain());
	/*int c_num = s->get_children_number();
	if (c_num == 0) {
		display_chain(s->get_chain());
	}
	else {
		if (c_num > 0) {
			display_string(s->get_left_string());
		}
		if (c_num > 1) {
			display_string(s->get_middle_string());
		}
		if (c_num > 2) {
			display_string(s->get_right_string());
		}
	}*/
	return;
}
void display() {

	max_y = max_element(point_list.begin(), point_list.end() - 3 - test_points.size(), [](Point &a, Point &b) {return a.get_y() < b.get_y(); })->get_y();
	min_y = max_element(point_list.begin(), point_list.end() - 3 - test_points.size(), [](Point &a, Point &b) {return a.get_y() > b.get_y(); })->get_y();
	max_x = max_element(point_list.begin(), point_list.end() - 3 - test_points.size(), [](Point &a, Point &b) {return a.get_x() < b.get_x(); })->get_x();
	min_x = max_element(point_list.begin(), point_list.end() - 3 - test_points.size(), [](Point &a, Point &b) {return a.get_x() > b.get_x(); })->get_x();

	MAX_x = max_x;

	glLoadIdentity();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(min_x, max_x, min_y, max_y);

	
	glLineWidth(8);
	glPointSize(5.0f);
	glEnable(GL_POINT_SMOOTH);

	
	glColor3f(1, float(0.7137), float(0.7568)); 

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < v_num; i++)
	{
		glVertex2d(point_list[i].get_x(), point_list[i].get_y());
	}
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2d(point_list[0].get_x(), point_list[0].get_y());
	glEnd();

	glColor3f(0.5f, 0.7f, 0.30f);
	glBegin(GL_POINTS);
	for (int i = 1; i < v_num; i++)
	{
		glVertex2d(point_list[i].get_x(), point_list[i].get_y());
	}
	glEnd();

	glLineWidth(3);//every diagonal
	glColor3f(float(0.6), float(0.6), float(0.6));
	for (int i = 0; i < (int)(diagonal_list.size()); i++) {;
		display_edge(diagonal_list[i]);
	}
	
	glColor3f(1.0f, 0.0f, 1.0f);
	for (int i = 0; i < (int)sequence_diagonal.size(); i++) {
		display_edge(diagonal_list[sequence_diagonal[i]]);
	}
	
	glColor3d(0, 0.47, 0.43);
	
	for (int t = 0; t <(int)test_points.size(); t++) {
		display_point(test_points[t]);
	}
	if (test_points.size() >= 2) {
		glColor3f(0.0f, 1.0f, 0.0f);
		Chain ** first_chain = final_hour.get_first_chain();
		for (int i = 0; i < 2; i++) {
			if (first_chain[i] == NULL) continue;
			display_chain(first_chain[i]);
		}

		String * s = final_hour.get_string();
		if (s != NULL) display_string(s);

		Chain ** second_chain = final_hour.get_second_chain();
		for (int i = 0; i < 2; i++) {
			if (second_chain[i] == NULL) continue;
			display_chain(second_chain[i]);
		}

		glColor3f(0.0f, 1.0f, 1.0f);
		Edge * e_list = final_hour.get_edge_list();
		for (int i = 0; i < 2; i++) {
			display_edge(e_list[i]);
		}
	}
	
	glutSwapBuffers();
	return;

	/*glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 3; i++)
	glVertex2d(point_list[bigT[i]].get_x(), point_list[bigT[i]].get_y());
	glEnd();
	*/

	/*
	for (int i = 0; i < (int)outer_diagonal_list.size(); i++) {
	int origin = outer_diagonal_list[i].get_origin();
	int dest = outer_diagonal_list[i].get_dest();
	glBegin(GL_LINES);
	glVertex2d(point_list[origin].get_x(), point_list[origin].get_y());
	glVertex2d(point_list[dest].get_x(), point_list[dest].get_y());
	glEnd();
	}*/

	/*glEnable(GL_POINT_SMOOTH);
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
	*/

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
			polygon_boundary.push_back(p);
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

