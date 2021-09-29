

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

bool is_same_pt(dPoint* p1, dPoint* p2) {
	int d1 = p1->getd();
	int d2 = p2->getd();
	if (d1 != d2) return false;
	for (int i = 0; i < d1; i++) {
		if (p1->getc(i) != p2->getc(i)) return false;
	}
	return true;
}
void drawobj(bool fill=false)
{
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glLineWidth(3);
	glColor3f(1, 1, 0);

	if (fill) {
		auto fc = obj->get_faces();
		for (auto f: *fc) {
			glBegin(GL_POLYGON);

			auto eds = f->getInnerEdges();
			auto tmp = eds->at(0)->get_s();
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
	drawobj();
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
	gluLookAt(0.0, 4.0, 4.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0);      /* up is in postivie Y direction */

	 /* Adjust cube position to be asthetic angle. */
	//glTranslatef(0.0, 0.0, -1.0);
	//glRotatef(60, 1.0, 0.0, 0.0);
	//glRotatef(-20, 0.0, 0.0, 1.0);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

int main(int argc, char **argv)
{
	Gen_Tetrahedron();
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