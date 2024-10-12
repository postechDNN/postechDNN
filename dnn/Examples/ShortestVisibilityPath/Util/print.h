#pragma once
#include <vector>
#include "Edge.h"
#include "Point.h"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#include <GLFW/glfw3.h>
int print_result(vector<Point> point_list, vector<Edge>diagonal_list, vector<Edge> outer_diagonal_list, int * bigT, int v_num) {

	GLFWwindow *window;

	// Initialize the library
	if (!glfwInit())
	{
		return -1;
	}

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	glViewport(int(0.0f), int(0.0f), SCREEN_WIDTH, SCREEN_HEIGHT); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
	glClearColor(255, 255, 255, 0.0);
	glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
	glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1); // essentially set coordinate system
	glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
	glLoadIdentity(); // same as above comment

	int mul, temp;
	auto max_y = max_element(point_list.begin(), point_list.end(), [](Point &a, Point &b) {return a.get_y() < b.get_y(); });
	auto min_y = max_element(point_list.begin(), point_list.end(), [](Point &a, Point &b) {return a.get_y() > b.get_y(); });
	auto max_x = max_element(point_list.begin(), point_list.end(), [](Point &a, Point &b) {return a.get_x() < b.get_x(); });
	auto min_x = max_element(point_list.begin(), point_list.end(), [](Point &a, Point &b) {return a.get_x() > b.get_x(); });

	int x_margin = int(-1 * (min_x->get_x()) + 5);
	int y_margin = int(-1 * (min_y->get_y()) + 5);

	mul = (SCREEN_HEIGHT - 30) / int((max_y->get_y() + y_margin));
	if ((temp = (SCREEN_WIDTH - 30) / (int(max_x->get_x()) + x_margin)) < mul)
		mul = temp;

	GLfloat polygonVertices[100];
	for (int i = 0; i < v_num; i++) {
		polygonVertices[3 * i] = mul * (float(point_list[i].get_x()) + x_margin);
		polygonVertices[3 * i + 1] = mul * (float(point_list[i].get_y()) + y_margin);
		polygonVertices[3 * i + 2] = 0;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // polygon drawing mode (GL_POINT, GL_LINE, GL_FILL)


	GLfloat bigTriangle[9];
	for (int i = 0; i < 3; i++) {
		bigTriangle[3 * i] = mul * (float(point_list[bigT[i]].get_x()) + x_margin);
		bigTriangle[3 * i + 1] = mul * (float(point_list[bigT[i]].get_y()) + y_margin);
		bigTriangle[3 * i + 2] = 0;
	}
	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// render OpenGL here
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor3f(1, float(0.7137), float(0.7568));
		glLineWidth(10);
		glVertexPointer(3, GL_FLOAT, 0, polygonVertices);
		glDrawArrays(GL_POLYGON, 0, v_num);
		glDisableClientState(GL_VERTEX_ARRAY);

		glEnableClientState(GL_VERTEX_ARRAY);
		glColor3f(1, 1, float(0.7568));
		glLineWidth(10);
		glVertexPointer(3, GL_FLOAT, 0, bigTriangle);
		glDrawArrays(GL_POLYGON, 0, 3);
		glDisableClientState(GL_VERTEX_ARRAY);

		glEnable(GL_LINE_SMOOTH);
		glLineWidth(1);
		glColor3f(float(0.6), float(0.6), float(0.6));
		glLineStipple(1, 0x00FF);
		glEnableClientState(GL_VERTEX_ARRAY);
		for (int i = 0; i < diagonal_list.size(); i++) {
			int origin = diagonal_list[i].get_origin();
			int dest = diagonal_list[i].get_dest();
			GLdouble lineVertices[] = { mul*(point_list[origin].get_x() + x_margin),mul*(point_list[origin].get_y() + y_margin) , 0,
				mul*(point_list[dest].get_x() + x_margin),mul*(point_list[dest].get_y() + y_margin) , 0, };
			glVertexPointer(3, GL_FLOAT, 0, lineVertices);
			glDrawArrays(GL_LINES, 0, 2);
		}
		for (int i = 0; i < outer_diagonal_list.size(); i++) {
			int origin = outer_diagonal_list[i].get_origin();
			int dest = outer_diagonal_list[i].get_dest();
			GLdouble lineVertices[] = { mul*(point_list[origin].get_x() + x_margin),mul*(point_list[origin].get_y() + y_margin) , 0,
				mul*(point_list[dest].get_x() + x_margin),mul*(point_list[dest].get_y() + y_margin) , 0, };
			glVertexPointer(3, GL_FLOAT, 0, lineVertices);
			glDrawArrays(GL_LINES, 0, 2);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glPopAttrib();
		glDisable(GL_LINE_SMOOTH);



		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
/*void print_point_list(vector<Point>& point_list) {
	cout << "-------------Print list--------------" << endl;
	for (vector<Point>::iterator iter = point_list.begin(); iter != point_list.end(); iter++) {
		cout << "( X, Y ) = ( " << iter->get_x() << ", " << iter->get_y() << ")" << endl;
	}
	cout << "-------------------------------------" << endl;
}

void print_diagonal_list() {
	cout << "-------------Diagonal List --------------" << endl;
	for (vector<Edge>::iterator iter = diagonal_list.begin(); iter != diagonal_list.end(); iter++) {
		cout << "( X, Y ) = ( " << iter->get_origin() << ", " << iter->get_dest() << ")" << endl;
	}
	cout << "-------------------------------------" << endl;

}*/