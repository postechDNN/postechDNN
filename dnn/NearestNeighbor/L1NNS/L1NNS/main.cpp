#include "RectangularDomain.h"
#include<vector>
#include<algorithm>
#include<queue>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h>

#define IDLE 0
#define MAKERECT 1
#define INSERTION 0
#define QUERY 1
#define HIGH 2147483746
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
PPP previn;
int width = 1024;
int height = 768;
vector<Rect> input;
double rad = 0.5;
int printcount;
int processing;
RectangularDomain D;

bool med(double x,double y,double l,double r,double t,double b)
{
	return l <= x && x <= r && b <= y && y <= t;
}

bool intersect(Rect A, Rect B) 
{
	double l = B.getl();
	double r = B.getr();
	double t = B.gett();
	double b = B.getb();
	printf("in intersect %lf %f %lf %lf\n", l, r, t, b);
	if (med(A.getl(), A.getb(), l, r, t, b)) printf("t"); else printf("f");
	if (med(A.getl(), A.gett(), l, r, t, b)) printf("t"); else printf("f");
	if (med(A.getr(), A.getb(), l, r, t, b)) printf("t"); else printf("f");
	if (med(A.getr(), A.gett(), l, r, t, b)) printf("t"); else printf("f");
	printf("\n");
	return med(A.getl(), A.getb(), l, r, t, b) || med(A.getl(), A.gett(), l, r, t, b) || med(A.getr(), A.getb(), l, r, t, b) || med(A.getr(), A.gett(), l, r, t, b);
}

void Printf() 
{
	for (; printcount < input.size(); printcount++)
		printf("Rectangles %lf, %lf, %lf, %lf\n", input[printcount].getl(), input[printcount].getr(), input[printcount].getb(), input[printcount].gett());
}

void DrawRect()
{
//	if (mouse_cond == MAKERECT)
//	{
//		glEnable(GL_LINE_STIPPLE);
		//glLineStipple(1, 0x0101); // 점선의 모양 결정
		// 0x0101 은 16진수 숫자로써 점선의 패턴을 만들어내는 코드 이다
//		glBegin(GL_LINE_LOOP);
//		glVertex2f(lb.x, lb.y);
//		glVertex2f(rt.x, lb.y);
//		glVertex2f(rt.x, rt.y);
//		glVertex2f(lb.x, rt.y);
//		glEnd();
//	}
	if (mouse_cond == IDLE)
	{
		//		glDisable(GL_LINE_STIPPLE);
	};
}

void DisplayWindow()
{
//	glClear(GL_COLOR_BUFFER_BIT);

	DrawRect();
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

		if(-width / 2 <= endpoint.x && endpoint.x <= -width / 2 + 50 && -height / 2 <= endpoint.y && endpoint.y <= -height / 2 + 50)
		{
			processing = QUERY;
		//	printf("rudolfh, %lf %lf\n", endpoint.x,endpoint.y);
			D = RectangularDomain(input);
		}

		else if (-width / 2 + 100 <= endpoint.x && endpoint.x <= -width / 2 + 150 && -height / 2 <= endpoint.y && endpoint.y <= -height / 2 + 50)
		{
			processing = INSERTION;
			if (previ.x != HIGH)
			{
				glColor3f(1.0, 1.0, 1.0);
				glPointSize(3.0);
				glBegin(GL_POINTS);
				glVertex2f(previ.x, previ.y);
				glEnd();
				glColor3f(0.0, 0.0, 0.0);
				glPointSize(3.0);
				glBegin(GL_POINTS);
				glVertex2f(previn.x, previn.y);
				glEnd();
			}
		}

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
							printf("ttt");
							break;
						}
					}
					if (possible)
					{
						glColor3f(0.0, 0.0, 0.0);
						glPointSize(3.0);
						glBegin(GL_POINTS);
						glVertex2f(lb.x, lb.y);
						glEnd();
						input.push_back(X);
					}
				}
				else if(processing == QUERY)
				{
					bool possible = true;
					for (int i = 0; i < input.size(); i++)
					{
						if (intersect(X, input[i]) || intersect(input[i], X))
						{
							possible = false;
							printf("ttt");
							break;
						}
					}
					if (possible)
					{
						if (previ.x != HIGH)
						{
							glColor3f(1.0, 1.0, 1.0);
							glPointSize(3.0);
							glBegin(GL_POINTS);
							glVertex2f(previ.x, previ.y);
							glEnd();
							glColor3f(0.0, 0.0, 0.0);
							glPointSize(3.0);
							glBegin(GL_POINTS);
							glVertex2f(previn.x, previn.y);
							glEnd();
						}
						glColor3f(0.0, 0.0, 1.0);
						glPointSize(3.0);
						glBegin(GL_POINTS);
						glVertex2f(lb.x, lb.y);
						glEnd();
						Point *nearest = D.NNS(Point(lb.x, lb.y)).p;
						glColor3f(1.0, 0.0, 1.0);
						glPointSize(3.0);
						glBegin(GL_POINTS);
						glVertex2f(nearest->getx(), nearest->gety());
						glEnd();
						previ.x = lb.x;
						previ.y = lb.y;
						previn.x = nearest->getx();
						previn.y = nearest->gety();
					}
				}
			}
			else if (processing == INSERTION && lb.x < rt.x && lb.y < rt.y)
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

void Reshape(int w, int h) 
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, w, h);
	glLoadIdentity();
	gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
}

void MouseDrag(int x,int y) 
{
	if (mouse_cond == MAKERECT)
	{
		endpoint.x = x - width / 2;
		endpoint.y = height / 2 - y;
	}
	ClassifyCorner();
	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("L1NNS in RectDomain");
	gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	processing = INSERTION;
	previ.x = previ.y = HIGH;

	glColor3f(1.0, 0.0, 0.0);
	glRectf(-width / 2, -height / 2, -width / 2 + 50, -height / 2 + 50);
	glColor3f(0.0, 1.0, 0.0);
	glRectf(-width / 2 + 100, -height / 2, -width / 2 + 150, -height / 2 + 50);

	glutDisplayFunc(DisplayWindow);
//	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseDrag);
	glutMainLoop();
}

/*
int main()
{
	//MAKE RECTDomain
	RectangularDomain RD;

	//query

	//Point *nearest = RD.NNS(Point(-3.3, 13.6)).p;

	//printf("%lf %lf\n",nearest->getx(),nearest->gety());


	//query
	Point *nearest = RD.NNS(Point(351.678,424.72)).p;
	printf("%lf %lf\n",nearest->getx(),nearest->gety());

	nearest = RD.NNS(Point(152.921,175.87)).p;
	printf("%lf %lf\n", nearest->getx(), nearest->gety());

	nearest = RD.NNS(Point(414.16, 146.245)).p;
	printf("%lf %lf\n", nearest->getx(), nearest->gety());

	system("pause");
}
;*/