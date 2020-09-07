#include "RectangularDomain.h"
#include <stdio.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
using namespace std;

#define IDLE 0
#define MAKERECT 1
#define INSERTION 0
#define DELETION 2
#define QUERY 1
#define HIGH 8888577.0
typedef struct ppp {
	double x;
	double y;
}PPP;

int mouse_cond;
PPP startpoint;
PPP endpoint;
PPP lb;
PPP rt;
PPP previ;
vector<PPP> previn;
vector<Point*> nearest;
int width = 1024;
int height = 768;
int processing;
int K = 1;
int pcnt;
int printtoggle = 0;
RectangularDomain *D;

void DisplayWindow() {
	glFlush();
}

void ClassifyCorner(void)
{
	lb.x = min(startpoint.x, endpoint.x);
	lb.y = min(startpoint.y, endpoint.y);
	rt.x = max(startpoint.x, endpoint.x);
	rt.y = max(startpoint.y, endpoint.y);
}

void MouseClick(int button, int state, int x, int y)
{
	if (mouse_cond == IDLE && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouse_cond = MAKERECT;
		startpoint.x = x - width / 2;
		startpoint.y = height / 2 - y;
		endpoint.x = startpoint.x;
		endpoint.y = startpoint.y;
		ClassifyCorner();
	}
	if (mouse_cond == MAKERECT && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		//	printf("state = %d\n", processing);
		mouse_cond = IDLE;
		endpoint.x = x - width / 2;
		endpoint.y = height / 2 - y;
		ClassifyCorner();

		if (-width / 2 <= endpoint.x && endpoint.x <= -width / 2 + 250 && -height / 2 <= endpoint.y && endpoint.y <= -height / 2 + 50)
			processing = QUERY;

		else if (-width / 2 + 258 <= endpoint.x && endpoint.x <= -width / 2 + 508 && -height / 2 <= endpoint.y && endpoint.y <= -height / 2 + 50)
			processing = INSERTION;

		else if (-width / 2 + 516 <= endpoint.x && endpoint.x <= -width / 2 + 766 && -height / 2 <= endpoint.y && endpoint.y <= -height / 2 + 50)
			processing = DELETION;

		else if (-width / 2 + 774 <= endpoint.x && endpoint.x <= -width / 2 + 1024 && -height / 2 <= endpoint.y && endpoint.y <= -height / 2 + 50) {
			delete D;
			exit(0);
		}

		else {
			if (processing == INSERTION && (lb.x + 10 <= rt.x || lb.y + 10 <= rt.y)) {
				vector<Rect*> rinput;
				rinput.push_back(new Rect(lb.x, rt.x, lb.y, rt.y));
				if (D->insertion(rinput) == 1) {
					glColor3f(0.5, 0.5, 0.5);
					glRectf(lb.x, lb.y, rt.x, rt.y);
				}
			}
			else {
				if (processing == INSERTION) {
					vector<Point*> pinput;
					pinput.push_back(new Point(rt.x, rt.y));
					if (D->insertion(pinput) == 1) {
						glColor3f(0.0, 0.0, 0.0);
						glPointSize(5.0);
						glBegin(GL_POINTS);
						glVertex2f(rt.x, rt.y);
						glEnd();
					}
				}
				else if (processing == QUERY) {
					if (D->getdatacnt() != 0) {
						vector<Point*> qres = D->kNNS(new Point(rt.x, rt.y), K);
						if (qres.size() != 0) {
							glColor3f(0.0, 0.0, 1.0);
							glPointSize(5.0);
							glBegin(GL_POINTS);
							glVertex2f(rt.x, rt.y);
							glEnd();
							glColor3f(1.0, 0.0, 1.0);
							glPointSize(5.0);
							glBegin(GL_POINTS);
							for (int i = 0; i < qres.size(); i++)
								glVertex2f(qres[i]->getx(), qres[i]->gety());
							glEnd();
						}
					}
				}
				else if (processing == DELETION) {
					int rcnt = D->getobscnt();
					int pcnt = D->getdatacnt();
					if (rcnt + pcnt > 0) {
						Point* clicked = new Point(rt.x, rt.y);
						double mintop = HIGH;
						int pindex = -1;
						double mintor = HIGH;
						int rindex = -1;
						for (int i = 0; i < rcnt; i++) {
							Rect* ri = D->getobstacle(i);
							if (ri->getld()->distance(clicked) < mintor) {
								mintor = ri->getld()->distance(clicked);
								rindex = i;
							}
							if (ri->getlu()->distance(clicked) < mintor) {
								mintor = ri->getlu()->distance(clicked);
								rindex = i;
							}
							if (ri->getrd()->distance(clicked) < mintor) {
								mintor = ri->getrd()->distance(clicked);
								rindex = i;
							}
							if (ri->getru()->distance(clicked) < mintor) {
								mintor = ri->getru()->distance(clicked);
								rindex = i;
							}
						}
						for (int i = 0; i < rcnt; i++) {
							Point* pi = D->getdat(i);
							if (pi->distance(clicked) < mintop) {
								mintop = pi->distance(clicked);
								pindex = i;
							}
						}
						if (mintop > mintor) {
							vector<Rect*> rinput;
							Rect* temp = D->getobstacle(rindex);
							rinput.push_back(new Rect(temp->getl(),temp->getr(),temp->getd(),temp->getu()));
							glColor3f(1.0, 1.0, 1.0);
							glRectf(temp->getl(), temp->getr(), temp->getd(), temp->getu());
							D->deletion(rinput);
						}
						else {
							vector<Point*> pinput;
							Point* temp = D->getdat(pindex);
							pinput.push_back(new Point(temp->getx(), temp->gety()));
							glColor3f(1.0, 1.0, 1.0);
							glPointSize(5.0);
							glBegin(GL_POINTS);
							glVertex2f(temp->getx(), temp->gety());
							glEnd();
							D->deletion(pinput);
						}
						delete clicked;
					}
				}
			}
		}
	}
	glutPostRedisplay();
	DisplayWindow();
}

void MouseDrag(int x, int y)
{
	if (mouse_cond == MAKERECT)
	{
		endpoint.x = x - width / 2;
		endpoint.y = height / 2 - y;
	}
	ClassifyCorner();
	glutPostRedisplay();
}

void Menu(int value) {
	switch (value) {
	case 1: K = 1; break;
	case 2: K = 2; break;
	case 3: K = 3; break;
	case 4: K = -1; break;
	}
	glutPostRedisplay();
}

void main() {
	D = new RectangularDomain();
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(width, height);
	glutCreateWindow("L1NNS in Rectangular Domain");
	gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2);

	glutCreateMenu(Menu);
	glutAddMenuEntry("NNS", 1);
	glutAddMenuEntry("2-NNS", 2);
	glutAddMenuEntry("3-NNS", 3);
	glutAddMenuEntry("Farthest", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	processing = INSERTION; //change to phase

	glColor3f(1.0, 0.0, 0.0);
	glRectf(-width / 2, -height / 2, -width / 2 + 250, -height / 2 + 50);
	glColor3f(0.0, 1.0, 0.0);
	glRectf(-width / 2 + 258, -height / 2, -width / 2 + 508, -height / 2 + 50);
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-width / 2 + 516, -height / 2, -width / 2 + 766, -height / 2 + 50);
	glColor3f(0.0, 0.0, 0.0);
	glRectf(-width / 2 + 774, -height / 2, -width / 2 + 1024, -height / 2 + 50);

	glutDisplayFunc(DisplayWindow);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseDrag);
	glutMainLoop();
}