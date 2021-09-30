

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include<vector>
#include <cmath>       /* for cos(), sin(), and sqrt() */
#include <GL/glut.h>    /* OpenGL Utility Toolkit header */

#include "dPoint.h"
#include "dEdge.h"
#include "dFace.h"
#include "dDCEL.h"
#include "dUtil.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265
#endif
static GLfloat lightColor[] = { 0.8, 1.0, 0.8, 1.0 }; /* green-tinted */
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };  /* Infinite light location. */

dDCEL* obj;

GLfloat angle = -150;   /* in degrees */
GLfloat angle2 = 30;   /* in degrees */
int moving, startx, starty;


void Gen_Tetrahedron() {
	dDCEL *Tetrahedron = new dDCEL;
	vector<dPoint*>* pt = new vector<dPoint*>;
	pt->push_back(new dPoint(1, 1, 1));
	pt->push_back(new dPoint(-1, 1, -1));
	pt->push_back(new dPoint(1, -1, -1)); 
	pt->push_back(new dPoint(-1, -1, 1));

	vector<dEdge*> *ed = new vector<dEdge*>;
	ed->push_back(new dEdge(pt->at(0), pt->at(1)));
	ed->push_back(new dEdge(pt->at(1), pt->at(2)));
	ed->push_back(new dEdge(pt->at(0), pt->at(2)));
	ed->push_back(new dEdge(pt->at(0), pt->at(3)));
	ed->push_back(new dEdge(pt->at(1), pt->at(3)));
	ed->push_back(new dEdge(pt->at(2), pt->at(3)));

	vector<dFace*> *fc = new vector<dFace*>;

	vector<dEdge*>* tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(0), ed->at(1), ed->at(2) });
	fc->push_back(new dFace(3));
	fc->at(0)->setInnerEdges(tmp);

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(0), ed->at(3), ed->at(4) });
	fc->push_back(new dFace(3));
	fc->at(1)->setInnerEdges(tmp);

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(1), ed->at(4), ed->at(5) });
	fc->push_back(new dFace(3));
	fc->at(2)->setInnerEdges(tmp);

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(2), ed->at(3), ed->at(5) });
	fc->push_back(new dFace(3));
	fc->at(3)->setInnerEdges(tmp);

	Tetrahedron->set_points(pt);
	Tetrahedron->set_edges(ed);
	Tetrahedron->set_faces(fc);
	obj = Tetrahedron;
};

void Gen_Dodecahedron() {
	double phi = (1 + sqrt(5.0)) / 2;
	double c = 2 - phi;
	double b = 1 / phi;
	dDCEL *Dodeca = new dDCEL;
	vector<dPoint*>* pt = new vector<dPoint*>;
	pt->push_back(new dPoint(c, 0, 1));		//0
	pt->push_back(new dPoint(-c, 0, 1));	//1
	pt->push_back(new dPoint(c, 0, -1));	//2
	pt->push_back(new dPoint(-c, 0, -1));	//3

	pt->push_back(new dPoint(0, 1, c));		//4
	pt->push_back(new dPoint(0, -1, c));	//5
	pt->push_back(new dPoint(0, 1, -c));	//6
	pt->push_back(new dPoint(0, -1, -c));	//7

	pt->push_back(new dPoint(1, c, 0));		//8
	pt->push_back(new dPoint(-1, c, 0));	//9
	pt->push_back(new dPoint(1, -c, 0));	//10
	pt->push_back(new dPoint(-1, -c, 0));	//11

	pt->push_back(new dPoint(b, b, b));		//12
	pt->push_back(new dPoint(-b, b, b));	//13
	pt->push_back(new dPoint(b, -b, b));	//14
	pt->push_back(new dPoint(b, b, -b));	//15
	pt->push_back(new dPoint(-b, -b, b));	//16
	pt->push_back(new dPoint(-b, b, -b));	//17
	pt->push_back(new dPoint(b, -b, -b));	//18
	pt->push_back(new dPoint(-b, -b, -b));	//19

	vector<dEdge*> *ed = new vector<dEdge*>;
	vector<dFace*> *fc = new vector<dFace*>;
	vector<dEdge*>* tmp = new vector<dEdge*>;

	//f1(0,1,2,3,4)
	ed->push_back(new dEdge(pt->at(0), pt->at(1)));	//0
	ed->push_back(new dEdge(pt->at(1), pt->at(13)));//1
	ed->push_back(new dEdge(pt->at(13), pt->at(4)));//2
	ed->push_back(new dEdge(pt->at(4), pt->at(12)));//3
	ed->push_back(new dEdge(pt->at(12), pt->at(0)));//4
	
	tmp->assign({ ed->at(0), ed->at(1), ed->at(2) , ed->at(3), ed->at(4) });
	fc->push_back(new dFace(3));
	fc->at(0)->setInnerEdges(tmp);

	//f2(0,5,6,7,8)
													//0 (1->0)
	ed->push_back(new dEdge(pt->at(0), pt->at(14)));//5
	ed->push_back(new dEdge(pt->at(14), pt->at(5)));//6
	ed->push_back(new dEdge(pt->at(5), pt->at(16)));//7
	ed->push_back(new dEdge(pt->at(16), pt->at(1)));//8

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(0), ed->at(5), ed->at(6) , ed->at(7), ed->at(8) });
	fc->push_back(new dFace(3));
	fc->at(1)->setInnerEdges(tmp);

	//f3(9,10,11,12,13)
	ed->push_back(new dEdge(pt->at(2), pt->at(3)));	//9
	ed->push_back(new dEdge(pt->at(3), pt->at(19)));//10
	ed->push_back(new dEdge(pt->at(19), pt->at(7)));//11
	ed->push_back(new dEdge(pt->at(7), pt->at(18)));//12
	ed->push_back(new dEdge(pt->at(18), pt->at(2)));//13

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(9), ed->at(10), ed->at(11) , ed->at(12), ed->at(13) });
	fc->push_back(new dFace(3));
	fc->at(2)->setInnerEdges(tmp);

	//f4(9,14,15,16,17)
													//9 (3->2)
	ed->push_back(new dEdge(pt->at(2), pt->at(15)));//14
	ed->push_back(new dEdge(pt->at(15), pt->at(6)));//15
	ed->push_back(new dEdge(pt->at(6), pt->at(17)));//16
	ed->push_back(new dEdge(pt->at(17), pt->at(3)));//17

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(9), ed->at(14), ed->at(15) , ed->at(16), ed->at(17) });
	fc->push_back(new dFace(3));
	fc->at(3)->setInnerEdges(tmp);

	//f5(18,3,19,20,15)
	ed->push_back(new dEdge(pt->at(6), pt->at(4)));	//18
													//3 (4->12)
	ed->push_back(new dEdge(pt->at(12), pt->at(8)));//19
	ed->push_back(new dEdge(pt->at(8), pt->at(15)));//20
													//15 (15->6)

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(18), ed->at(3), ed->at(19) , ed->at(20), ed->at(15) });
	fc->push_back(new dFace(3));
	fc->at(4)->setInnerEdges(tmp);

	//f6(18,16,21,22,2)		[4,6,17,9,13]
													//18 (4->6)
													//16 (6->17)
	ed->push_back(new dEdge(pt->at(17), pt->at(9)));//21
	ed->push_back(new dEdge(pt->at(9), pt->at(13)));//22
													//2 (13->4)

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(18), ed->at(16), ed->at(21) , ed->at(22), ed->at(2) });
	fc->push_back(new dFace(3));
	fc->at(5)->setInnerEdges(tmp);

	//f7(23,7,24,25,11)	[7,5,16,11,19]
	ed->push_back(new dEdge(pt->at(7), pt->at(5)));	//23
													//7 (5->16)
	ed->push_back(new dEdge(pt->at(16), pt->at(11)));//24
	ed->push_back(new dEdge(pt->at(11), pt->at(19)));//25
													//11 (19->7)

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(23), ed->at(7), ed->at(24) , ed->at(25), ed->at(11) });
	fc->push_back(new dFace(3));
	fc->at(6)->setInnerEdges(tmp);

	//f8(23,12,26,27,6) [5,7,18,10,14]
													//23 (5->7)
													//12 (7->18)
	ed->push_back(new dEdge(pt->at(18), pt->at(10)));//26
	ed->push_back(new dEdge(pt->at(10), pt->at(14)));//27
													//6 (14->5)

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(23), ed->at(12), ed->at(26) , ed->at(27), ed->at(6) });
	fc->push_back(new dFace(3));
	fc->at(7)->setInnerEdges(tmp);

	//f9(28,27,5,4,19) [8,10,14,0,12]
	ed->push_back(new dEdge(pt->at(8), pt->at(10)));//28
													//27 (10->14)
													//5 (14->0)
													//4 (0->12)
													//19 (12->8)

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(28), ed->at(27), ed->at(5) , ed->at(4), ed->at(19) });
	fc->push_back(new dFace(3));
	fc->at(8)->setInnerEdges(tmp);

	//f10(28,20,14,13,26) [10,8,15,2,18]
													//28 (10->8)
													//20 (8->15)
													//14 (15->2)
													//13 (2->18)
													//26 (18->10)

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(28), ed->at(20), ed->at(14) , ed->at(13), ed->at(26) });
	fc->push_back(new dFace(3));
	fc->at(9)->setInnerEdges(tmp);

	//f11(29,25,10,17,21) [9,11,19,3,17]
	ed->push_back(new dEdge(pt->at(9), pt->at(11)));//29
													//25 (11->19)
													//10 (19->3)
													//17 (3->17)
													//21 (17->9)

	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(29), ed->at(25), ed->at(10) , ed->at(17), ed->at(21) });
	fc->push_back(new dFace(3));
	fc->at(10)->setInnerEdges(tmp);

	//f12(29,22,1,8,24) [11,9,13,1,16]
	tmp = new vector<dEdge*>;
	tmp->assign({ ed->at(29), ed->at(22), ed->at(1) , ed->at(8), ed->at(24) });
	fc->push_back(new dFace(3));
	fc->at(11)->setInnerEdges(tmp);


	Dodeca->set_points(pt);
	Dodeca->set_edges(ed);
	Dodeca->set_faces(fc);
	if (obj != NULL) delete obj;
	obj = Dodeca;
};

bool is_same_pt(dPoint* p1, dPoint* p2) {
	int d1 = p1->getd();
	int d2 = p2->getd();
	if (d1 != d2) return false;
	for (int i = 0; i < d1; i++) {
		if (abs(p1->getc(i) - p2->getc(i)) > ERR) return false;
	}
	return true;
}

/* ARGSUSED2 */
static void
mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			moving = 0;
		}
	}
}

/* ARGSUSED1 */
static void
motion(int x, int y)
{
	if (moving) {
		angle = angle + (x - startx);
		angle2 = angle2 + (y - starty);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}
}

void drawobj(bool fill=false)
{
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glLineWidth(3);
	glColor3f(1, 1, 0);

	if (fill) {
		glDisable(GL_CULL_FACE);
		auto fc = obj->get_faces();
		for (auto f: *fc) {
			glBegin(GL_POLYGON);
			
			auto eds = f->getInnerEdges();
			auto tmp = eds->at(0)->get_s();

			dPoint *r[3];
			r[0] = tmp;
			if(is_same_pt(r[0],eds->at(1)->get_s())) r[1]=eds->at(1)->get_t();
			else r[1] = eds->at(1)->get_s();
			if (is_same_pt(r[1], eds->at(2)->get_s())) r[2] = eds->at(2)->get_t();
			else r[2] = eds->at(2)->get_s();
			dPoint normal = normal3d(r[0],r[1],r[2]);
			glNormal3d((GLdouble)normal.getx(), (GLdouble)normal.gety(), (GLdouble)normal.getz());
			//glVertex3d((GLdouble)tmp->getx(), (GLdouble)tmp->gety(), (GLdouble)tmp->getz());
			for (auto ed : *eds) {
				if (is_same_pt(ed->get_s(),tmp))
					tmp = ed->get_t();
				else tmp = ed->get_s();
				glVertex3d((GLdouble)tmp->getx(), (GLdouble)tmp->gety(), (GLdouble)tmp->getz());
			}
			glEnd();
		}
	}
	else {
		auto eds = obj->get_edges();
		for (auto e : *eds) {
			glBegin(GL_LINES);
			glVertex3d((GLdouble)e->get_s()->getx(), (GLdouble)e->get_s()->gety(), (GLdouble)e->get_s()->getz());
			glVertex3d((GLdouble)e->get_t()->getx(), (GLdouble)e->get_t()->gety(), (GLdouble)e->get_t()->getz());
			glEnd();
		}
	}
}

void redraw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
	drawobj(true);
	glutSwapBuffers();
}

void init()
{
	/* Use depth buffering for hidden surface elimination. */
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	gluPerspective( /* field of view in degree */ 40.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 2.0, /* Z far */ 10.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(4.0, 4.0, 4.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);      /* up is in postivie Y direction */

	 /* Adjust cube position to be asthetic angle. */
	//glTranslatef(0.0, 0.0, -1.0);
	//glRotatef(60, 1.0, 0.0, 0.0);
	//glRotatef(-20, 0.0, 0.0, 1.0);
	glClearColor(1, 1, 1, 1);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

int main(int argc, char **argv)
{
	//Gen_Tetrahedron();
	Gen_Dodecahedron();
	glutInit(&argc, argv);
	glutInitWindowSize(680, 680);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Drawing object in dDCEL (d=3)");
	glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	init();

	glutMainLoop();
	return 0;
}