#include <GL/glfw3.h>
#include <GL/freeglut.h>
#include "DD.h"


enum InputMod {defualt,Distance,Insert,Delete,Query};
InputMod NowMod = defualt;
std::vector<ConvexDistPoint> cond;
std::vector<ConvexDistPoint> InputPoints;

int k_value = 3;
ConvexDistPoint QueryPoint(-1,-1);
std::vector<int> NNPoints;

ConvexDistance* myd;
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
		glVertex2d(MenuCentorX , MenuCentorY );
	glEnd();
	//


	int ModPosStartX = 630;
	int ModPosStartY = 370;
	//print about mod
	glRasterPos2d(ModPosStartX, ModPosStartY);
	const char* modst="defualt";
	switch (NowMod) {
	case Distance :
		modst = "DIST";
		break;
	case Insert :
		modst = "INSERT";
		break;
	case Query :
		modst = "Query";
		break;
	}
	int ModLen = strlen(modst);
	for (unsigned int i = 0; i < ModLen; i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, modst[i]);
	//about number of k

	char theK[4] = "k=3";
	glRasterPos2d(ModPosStartX+120, ModPosStartY);
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
		for (unsigned int i = 0; i +1< cond.size(); i++){
			glVertex2d(MenuCentorX + MenuScaleX * cond[i].pos[0], MenuCentorY + MenuScaleY * cond[i].pos[1]);
			glVertex2d(MenuCentorX + MenuScaleX * cond[i+1].pos[0], MenuCentorY + MenuScaleY * cond[i+1].pos[1]);
		}
		glEnd();
	}
	else {
		glBegin(GL_LINE_LOOP);
		for (unsigned int i = 0; i < myd->distPolygon.size(); i++)
			glVertex2d(MenuCentorX + MenuScaleX * myd->distPolygon[i].pos[0], MenuCentorY + MenuScaleY * myd->distPolygon[i].pos[1]);
		glEnd();
	}

	//print input point
	for(unsigned int i=0;i< InputPoints.size();i++){
		
		if (NowMod == Query && QueryPoint.pos[0] > 0 && QueryPoint.pos[1] > 0 && NNPoints[i]==1) {
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		else glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2d(InputPoints[i].pos[0], InputPoints[i].pos[1]);
		glEnd();
	}

	glColor3f(0.0f, 0.0f, 0.0f);
	if (NowMod == Query && QueryPoint.pos[0] > 0 && QueryPoint.pos[1] > 0) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2d(QueryPoint.pos[0], QueryPoint.pos[1]);
		glEnd();
	}
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSwapBuffers();
	return;

}

void DDNN(ConvexDistPoint Qp) {
	std::vector<std::pair<double, int> > pointlist;
	for (unsigned int i = 0; i < InputPoints.size(); i++)
		pointlist.push_back(std::make_pair(myd->GetDist(InputPoints[i], Qp), i));
	std::sort(pointlist.begin(), pointlist.end());
	NNPoints.resize(pointlist.size());
	for (unsigned int i = 0; i < NNPoints.size(); i++)
		NNPoints[i] = 0;

	for (unsigned int i = 0; i < pointlist.size() && i < k_value; i++) {
		NNPoints[pointlist[i].second] = 1;
	}

}

void AddPoint(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			if (NowMod == Distance) {
				y = 600 - y;
				if (MenuCentorX - MenuW <= x && x <= MenuCentorX + MenuW && MenuCentorY - MenuH <= y && y <= MenuCentorY + MenuH) {
					cond.push_back(ConvexDistPoint(((double)(x - MenuCentorX))/MenuScaleX, ((double)(y - MenuCentorY)) / MenuScaleY));
				}
			}
			else if (NowMod == Insert) {
				InputPoints.push_back(ConvexDistPoint(x, 600-y));
			}
			else if (NowMod == Query) {
				DDNN(ConvexDistPoint(x, 600 - y));
				QueryPoint = ConvexDistPoint(x, 600 - y);

			}

		}

	}
	
}


void ModChange(unsigned char key, int x, int y) {
	if (NowMod == Distance) {
		delete myd;
		myd = new ConvexDistance(cond);

		switch (key) {
			case 'i':
			case 'I':
				NowMod = Insert;
				break;
			case 'q':
			case 'Q':
				QueryPoint = ConvexDistPoint(-1, -1);
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
			QueryPoint = ConvexDistPoint(-1, -1);
			NowMod = Query;
			break;
		case 'd':
		case 'D':
			if(!cond.empty()) cond.clear();
			NowMod = Distance;
			break;
		}
	}

	if (key >= '1' && key <= '9') {
		k_value = key - '0';
	}

	glutPostRedisplay();
}



int main(int argc,char ** argv) {
	
	cond.push_back(ConvexDistPoint(1, 1));
	cond.push_back(ConvexDistPoint(1, -1));
	cond.push_back(ConvexDistPoint(-1, -1));
	cond.push_back(ConvexDistPoint(-1, 1));
	myd=new ConvexDistance(cond);

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

