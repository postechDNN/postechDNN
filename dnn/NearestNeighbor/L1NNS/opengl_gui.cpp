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
#define HIGH 2147483647
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
vector<Point_Distance> nearest;
int width = 1024;
int height = 768;
vector<Rect> input;
double rad = 0.5;
int printcount;
int processing;
unique_ptr<RectangularDomain> D;
int K = 1;
int pcnt;
int printtoggle = 0;

bool med(double x, double y, double l, double r, double t, double b)
{
	return l <= x && x <= r && b <= y && y <= t;
}

bool intersect(Rect A, Rect B)
{
	double l = B.getl();
	double r = B.getr();
	double t = B.gett();
	double b = B.getb();
	return med(A.getl(), A.getb(), l, r, t, b) || med(A.getl(), A.gett(), l, r, t, b) || med(A.getr(), A.getb(), l, r, t, b) || med(A.getr(), A.gett(), l, r, t, b);
}

void Printf()
{
	for (; printcount < input.size(); printcount++)
		printf("Rectangles %lf, %lf, %lf, %lf\n", input[printcount].getl(), input[printcount].getr(), input[printcount].getb(), input[printcount].gett());
}

void DisplayWindow()
{
	//	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	//	glutSwapBuffers();
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

		if (-width / 2 <= endpoint.x && endpoint.x <= -width / 2 + 50 && -height / 2 <= endpoint.y && endpoint.y <= -height / 2 + 50)
		{
			processing = QUERY;
			/*if (printtoggle == 1)
			{
				Cgraph Tempo = D.getcarrier(2);
				int nnn = Tempo.edgecnt();
				glColor3f(1.0, 1.0, 1.0);
				for (int i = 0; i < nnn; i++)
				{
					glBegin(GL_LINE_LOOP);
					glVertex2f(Tempo.getedge(i)->gets()->getx(), Tempo.getedge(i)->gets()->gety());
					glVertex2f(Tempo.getedge(i)->gett()->getx(), Tempo.getedge(i)->gett()->gety());
					glEnd();
				}
			}*/
			//	printf("rudolfh, %lf %lf\n", endpoint.x,endpoint.y);
			D.reset(new RectangularDomain(input));
			/*Cgraph Tempo = D.getcarrier(2);
			int nnn = Tempo.edgecnt();
			glColor3f(1.0, 0.0, 0.0);
			for (int i = 0; i < nnn; i++)
			{
				glBegin(GL_LINE_LOOP);
				glVertex2f(Tempo.getedge(i)->gets()->getx(), Tempo.getedge(i)->gets()->gety());
				glVertex2f(Tempo.getedge(i)->gett()->getx(), Tempo.getedge(i)->gett()->gety());
				glEnd();
			}
			printtoggle = 1;*/
		}

		else if (-width / 2 + 100 <= endpoint.x && endpoint.x <= -width / 2 + 150 && -height / 2 <= endpoint.y && endpoint.y <= -height / 2 + 50)
		{
			processing = INSERTION;
			if (previ.x != HIGH)
			{
				glColor3f(1.0, 1.0, 1.0);
				glPointSize(5.0);
				glBegin(GL_POINTS);
				glVertex2f(previ.x, previ.y);
				glEnd();
				glColor3f(0.0, 0.0, 0.0);
				glPointSize(5.0);
				glBegin(GL_POINTS);
				int i = previn.size() - 1;
				for (; i >= 0; i--)
				{
					glVertex2f(previn[i].x, previn[i].y);
					previn.pop_back();
				}
				glEnd();
			}
		}

		else if (-width / 2 + 200 <= endpoint.x && endpoint.x <= -width / 2 + 250 && -height / 2 <= endpoint.y && endpoint.y <= -height / 2 + 50)
		{
			processing = DELETION;
			if (previ.x != HIGH)
			{
				glColor3f(1.0, 1.0, 1.0);
				glPointSize(5.0);
				glBegin(GL_POINTS);
				glVertex2f(previ.x, previ.y);
				glEnd();
				glColor3f(0.0, 0.0, 0.0);
				glPointSize(5.0);
				glBegin(GL_POINTS);
				int i = previn.size() - 1;
				for (; i >= 0; i--)
				{
					glVertex2f(previn[i].x, previn[i].y);
					previn.pop_back();
				}
				glEnd();
			}
			//On Construction
		}

		else if (width / 2 - 50 <= endpoint.x && endpoint.x <= width / 2 && -height / 2 <= endpoint.y && endpoint.y <= -height / 2 + 50)
			exit(0);

		else
		{
			Rect X(lb.x, rt.x, rt.y, lb.y);
			//	printf("X = %lf %f %lf %lf\n", X.getl(), X.getr(), X.getb(), X.gett());

			if (lb.x == rt.x && lb.y == rt.y)
			{
				//	printf("yep");
				if (processing == INSERTION)
				{
					bool possible = true;
					for (int i = 0; i < input.size(); i++)
					{
						if (intersect(X, input[i]) || intersect(input[i], X))
						{
							possible = false;
							break;
						}
					}
					if (possible)
					{
						glColor3f(0.0, 0.0, 0.0);
						glPointSize(5.0);
						glBegin(GL_POINTS);
						glVertex2f(lb.x, lb.y);
						glEnd();
						input.push_back(X);
					}
				}
				else if (processing == QUERY)
				{
					int checkcnt = 0;
					for (int i = 0; i < input.size(); i++) {
						if (input[i].getl() == input[i].getr() && input[i].getb() == input[i].gett())
							checkcnt++;
					}
					if (checkcnt != 0) {
						bool possible = true;
						for (int i = 0; i < input.size(); i++)
						{
							if (intersect(X, input[i]) || intersect(input[i], X))
							{
								possible = false;
								break;
							}
						}
						if (possible)
						{
							if (previ.x != HIGH)
							{
								glColor3f(1.0, 1.0, 1.0);
								glPointSize(5.0);
								glBegin(GL_POINTS);
								glVertex2f(previ.x, previ.y);
								glEnd();
								glColor3f(0.0, 0.0, 0.0);
								glPointSize(5.0);
								glBegin(GL_POINTS);
								int i = previn.size() - 1;
								for (; i >= 0; i--)
								{
									glVertex2f(previn[i].x, previn[i].y);
									previn.pop_back();
								}
								glEnd();
							}
							glColor3f(0.0, 0.0, 1.0);
							glPointSize(5.0);
							glBegin(GL_POINTS);
							glVertex2f(lb.x, lb.y);
							glEnd();
							nearest = D->kNNS(Point(lb.x, lb.y), K);
							glColor3f(1.0, 0.0, 1.0);
							glPointSize(5.0);
							glBegin(GL_POINTS);
							for (int i = 0; i < nearest.size(); i++)
							{
								glVertex2f(nearest[i].p->getx(), nearest[i].p->gety());
								previn.push_back({ nearest[i].p->getx(),nearest[i].p->gety() });
							}
							glEnd();
							previ.x = lb.x;
							previ.y = lb.y;
						}
					}
				}
				else if (processing == DELETION)
				{
					if (input.size() != 0) {
						bool possible = true;
						int index = 0;
						for (int i = 0; i < input.size(); i++)
						{
							if (intersect(X, input[i]) || intersect(input[i], X))
							{
								possible = false;
								index = i;
								break;
							}
						}
						if (possible)
						{
							index = -1;
							double minvar = HIGH;
							for (int i = 0; i < input.size(); i++)
							{
								if (input[i].getl() == input[i].getr() && input[i].getb() == input[i].gett())
								{
									if (fabs(lb.x - input[i].getl()) + fabs(lb.y - input[i].getb()) < minvar)
									{
										minvar = fabs(lb.x - input[i].getl()) + fabs(lb.y - input[i].getb());
										index = i;
									}
								}
							}
							glColor3f(1.0, 1.0, 1.0);
							glPointSize(5.0);
							glBegin(GL_POINTS);
							glVertex2f(input[index].getl(), input[index].getb());
							glEnd();
							input.erase(input.begin() + index);
						}
						else
						{
							glColor3f(1.0, 1.0, 1.0);
							glRectf(input[index].getl(), input[index].getb(), input[index].getr(), input[index].gett());
							input.erase(input.begin() + index);
						}
					}
				}
			}
			else if (processing == INSERTION && lb.x + 5 < rt.x && lb.y + 5 < rt.y)
			{
				bool possible = true;
				for (int i = 0; i < input.size(); i++)
				{
					if (intersect(X, input[i]) || intersect(input[i], X))
					{
						possible = false;
						break;
					}
				}
				if (possible)
				{
					glColor3f(0.5, 0.5, 0.5);
					glRectf(lb.x, lb.y, rt.x, rt.y);
					input.push_back(X);
				}
			}
			//		Printf();
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

void Menu(int value)
{
	switch (value) {
	case 1:
		K = 1;
		break;
	case 2:
		K = 2;
		break;
	case 3:
		K = 3;
		break;
	case 4:
		K = -1;
		break;
	}
	glutPostRedisplay();
}

void L1NNS(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("L1NNS in RectDomain");
	gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2);


	GLint kNNMenu = glutCreateMenu(Menu);
	glutAddMenuEntry("2-NNS", 2);
	glutAddMenuEntry("3-NNS", 3);
	glutAddMenuEntry("Farthest", 4);

	glutCreateMenu(Menu);
	glutAddMenuEntry("NNS", 1);

	glutAddSubMenu("k-NNS", kNNMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);



	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	processing = INSERTION;
	previ.x = previ.y = HIGH;

	glColor3f(1.0, 0.0, 0.0);
	glRectf(-width / 2, -height / 2, -width / 2 + 50, -height / 2 + 50);
	glColor3f(0.0, 1.0, 0.0);
	glRectf(-width / 2 + 100, -height / 2, -width / 2 + 150, -height / 2 + 50);
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-width / 2 + 200, -height / 2, -width / 2 + 250, -height / 2 + 50);
	glColor3f(0.0, 0.0, 0.0);
	glRectf(width / 2 - 50, -height / 2, width / 2, -height / 2 + 50);

	glutDisplayFunc(DisplayWindow);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseDrag);
	glutMainLoop();
}


int main(int argc, char** argv)
{
	L1NNS(argc, argv);
	return 0;
}