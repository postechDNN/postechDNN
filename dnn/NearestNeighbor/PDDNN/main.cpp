#include "PDGraph.h"
#include <GL/glfw3.h>
#include <GL/freeglut.h>

enum InputMod { defualt, Distance, Insert, Delete, Query };
InputMod NowMod = defualt;
std::vector<Point*> *cond;
std::vector<Point> InputPoints;

int k_value = 3;
Point QueryPoint(-1, -1);
std::vector<int> NNPoints;

PolygonalDomain *pd;
PDgraph graph;
int MenuCentorX = 700;
int MenuCentorY = 500;
int MenuH = 100;
int MenuW = 100;
int MenuScaleX = 50;
int MenuScaleY = 50;


void reshape(int w, int h) {
	//w_w = w;
	//w_h = h;
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
		if (!cond->empty()) {
			glBegin(GL_POINTS);
			glVertex2d(MenuCentorX, MenuCentorY);
			glEnd();
		}


		glBegin(GL_LINES);
		for (unsigned int i = 0; i + 1 < cond->size(); i++) {
			glVertex2d(MenuCentorX + MenuScaleX * (*cond)[i]->getx(), MenuCentorY + MenuScaleY * (*cond)[i]->gety());
			glVertex2d(MenuCentorX + MenuScaleX * (*cond)[i+1]->getx(), MenuCentorY + MenuScaleY * (*cond)[i+1]->gety());
		}
		glEnd();
	}
	else {
		glBegin(GL_LINE_LOOP);
		for (unsigned int i = 0; i < graph.getNodes()->size(); i++)
			glVertex2d(MenuCentorX + MenuScaleX * graph.getNodes()->at(i)->getPoint()->getx(), MenuCentorY + MenuScaleY * graph.getNodes()->at(i)->getPoint()->gety());
		glEnd();
	}

	//print input point
	for (unsigned int i = 0; i < InputPoints.size(); i++) {

		if (NowMod == Query && QueryPoint.getx() > 0 && QueryPoint.gety() > 0 && NNPoints[i] == 1) {
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		else glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2d(InputPoints[i].getx(), InputPoints[i].gety());
		glEnd();
	}

	glColor3f(0.0f, 0.0f, 0.0f);
	if (NowMod == Query && QueryPoint.getx() > 0 && QueryPoint.gety() > 0) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2d(QueryPoint.getx(), QueryPoint.gety());
		glEnd();
	}
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSwapBuffers();
	return;

}


void AddPoint(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			if (NowMod == Distance) {
				y = 600 - y;
				if (MenuCentorX - MenuW <= x && x <= MenuCentorX + MenuW && MenuCentorY - MenuH <= y && y <= MenuCentorY + MenuH) {
					Point p = Point(((double)(x - MenuCentorX)) / MenuScaleX, ((double)(y - MenuCentorY)) / MenuScaleY);
					(*cond).push_back(&p);
				}
			}
			else if (NowMod == Insert) {
				InputPoints.push_back(Point(x, 600 - y));
			}
			else if (NowMod == Query) {
				QueryPoint = Point(x, 600 - y);
				graph.knn(&QueryPoint, k_value);
				

			}

		}

	}

}


void ModChange(unsigned char key, int x, int y) {
	if (NowMod == Distance) {
		graph = PDgraph(pd,cond);

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
			if (!cond->empty()) cond->clear();
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
	Point p1(1, 1);
	Point p2(2, 2);
	Point p3(3, 4);

	PDNode pn1(&p1);
	PDNode pn2(&p2);
	PDNode pn3(&p3);


	pn1.addNode(&pn2);
	pn1.addNode(&pn3);


	graph.getNodes()->push_back(&pn1);
	graph.getNodes()->push_back(&pn2);
	graph.getNodes()->push_back(&pn3);

	graph.knn(&QueryPoint, k_value);

	glutInit(&argc, argv);
	glutInitWindowPosition(100, 0);
	glutInitWindowSize(800, 600);//창 크기 설정
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Dynamic Distance Nearest neighbor");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(AddPoint);
	glutKeyboardFunc(ModChange);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();

	return 0;

}
