// OGL_Contorl.cpp: 구현 파일
//

#include "pch.h"
#include "PDDNN.h"
#include "OGL_Contorl.h"

#include <GL/gl.h> 
#include <GL/glu.h>
#include <random>
#include <utility>

#define _USE_MATH_DEFINES
#include <math.h> 

#pragma comment(lib, "glu32.lib" ) 
#pragma comment(lib, "opengl32.lib" )

#define PADDING 0.1
#define ORTHO 2.5
#define VIEW 1.1

#define MOVING_SPEED 0.1
#define ROTATING_SPEED 0.01



// OGL_Contorl

IMPLEMENT_DYNAMIC(OGL_Contorl, CStatic)

OGL_Contorl::OGL_Contorl(){
	
	// Initialize
	this->mode = 2;
	this->isMove = false;

	//this->view[0][0] = 1.1;
	//this->view[0][1] = 1.1;
	//this->view[0][2] = 1.1;
	this->view[0][0] = 0;
	this->view[0][1] = 0;
	this->view[0][2] = 1.1;
	this->view[1][0] = 0;
	this->view[1][1] = 0;
	this->view[1][2] = 0;
	this->view[2][0] = 0;
	this->view[2][1] = 0;
	this->view[2][2] = 1.0;

	// compute up vector
	this->updateVectors();
	this->isVectorUpdate = true;

}

OGL_Contorl::~OGL_Contorl(){

}

void OGL_Contorl::setMode(int m) {
	this->mode = m;
}


BEGIN_MESSAGE_MAP(OGL_Contorl, CStatic)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_MOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// OGL_Contorl 메시지 처리기

BOOL OGL_Contorl::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		 1 ,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32 ,
		 0 , 0 , 0 , 0 , 0 , 0 ,
		 0 ,
		 0 ,
		 0 ,
		 0 ,   0 , 0 , 0 ,
		 16 ,
		 0 ,
		 0 ,
		PFD_MAIN_PLANE,
		0 ,
		 0 , 0 , 0
	};

	int pixelformat;
	if (0 == (pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd))) {
		return FALSE;
	}

	if (FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd)) {
		return FALSE;
	}

	return TRUE;
}


BOOL OGL_Contorl::InitGLContext()
{
	m_pDC = new CClientDC(this);

	if (NULL == m_pDC) {
		return FALSE;
	}
	if (!SetupPixelFormat()) return FALSE;
	if (0 == (m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc()))) {
		return FALSE;
	}
	if (FALSE == ::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC)) {
		return FALSE;
	}

	return TRUE;
}


void OGL_Contorl::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	LONG style = GetWindowLong(this->m_hWnd, GWL_STYLE);
	style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	SetWindowLong(this->m_hWnd, GWL_STYLE, style);


	InitGLContext();
	CStatic::PreSubclassWindow();
}


void OGL_Contorl::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (FALSE == ::wglMakeCurrent(NULL, NULL)) {
		// 필요한 경우 오류 처리
	}

	if (FALSE == ::wglDeleteContext(m_hRC)) {
		// 필요에 따라 오류 처리
	}

	if (m_pDC) delete m_pDC;
}


void OGL_Contorl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	double normTrans[3];
	double normMul[3];

	this->PDDS.object2D.getNorm(normTrans, normMul, 2);

	::glPushMatrix();
	::glColor3f(0.0f, 1.0f, 0.0f);
	glColor3d(1.0, 0.0, 0.0);

	/*
	if (this->PDDS.object2D.getDrawFaces()) {
		::glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_POLYGON);
		glPolygonMode(GL_FRONT,GL_FILL);
		for (int j = 0; j < inputPolygon.size(); j++) {
			OGL_Point p(inputPolygon[j].first, inputPolygon[j].second);
			glVertex2d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1]);
		}
		glEnd();
	}
	*/

	// Obstacle temp
	if (this->PDDS.object2D.getDrawObject(EDGE)) {
		::glColor3f(0.5f, 0.5f, 0.5f);
		glLineWidth(3.0f);
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < this->PDDS.getObstacleTempSize(); j++) {
			auto pt = this->PDDS.getObstacleTempVertex(j);
			OGL_Point p(pt.first, pt.second);
			glVertex2d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1]);
		}
		glEnd();
		
	}
	if (this->PDDS.object2D.getDrawObject(VERTEX)) {
		::glColor3f(0.0f, 0.0f, 0.5f);
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		for (int j = 0; j < this->PDDS.getObstacleTempSize(); j++) {
			auto pt = this->PDDS.getObstacleTempVertex(j);
			OGL_Point p(pt.first, pt.second);
			glVertex2d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1]);
		}
		glEnd();
	}

	// Polygonal domain edges
	if (this->PDDS.object2D.getDrawObject(EDGE)) {
		::glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(3.0f);
		for (int i = 0; i < this->PDDS.getObstaclesNum(); i++) {
			glBegin(GL_LINE_LOOP);
			for (int j = 0; j < this->PDDS.getObstacleSize(i); j++) {
				auto pt = this->PDDS.getObstacleVertex(i, j);
				OGL_Point p(pt.first, pt.second);
				glVertex2d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1]);
			}
			glEnd();
		}
	}

	// Polygonal domain vertices
	if (this->PDDS.object2D.getDrawObject(VERTEX)) {
		::glColor3f(0.0f, 0.0f, 1.0f);
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		for (int i = 0; i < this->PDDS.getObstaclesNum(); i++) {
			for (int j = 0; j < this->PDDS.getObstacleSize(i); j++) {
				auto pt = this->PDDS.getObstacleVertex(i, j);
				OGL_Point p(pt.first, pt.second);
				glVertex2d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1]);
			}
		}
		glEnd();
	}

	// input points
	if (this->PDDS.object2D.getDrawObject(POINTSET)) {
		::glColor3f(1.0f, 1.0f, 1.0f);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		for (int i = 0; i < this->PDDS.getInputPointsSize(); i++) {
			auto pt = this->PDDS.getInputPoint(i);
			OGL_Point p(pt.first, pt.second);
			glVertex2d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1]);
		}
		glEnd();
	}
	// query result
	if (this->PDDS.object2D.getDrawObject(QUERY)) {
		::glColor3f(0.0f, 1.0f, 0.0f);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		for (int i = 0; i < this->PDDS.getQueryResultSize(); i++) {
			auto pt = this->PDDS.getQueryResultCoord(i);
			OGL_Point p(pt.first, pt.second);
			glVertex2d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1]);
		}
		glEnd();
	}
	// query point
	if (this->PDDS.object2D.getDrawObject(QUERY)) {
		if (this->PDDS.isQueryInserted()) {
			auto pt = this->PDDS.getQueryPoint();
			::glColor3f(1.0f, 0.0f, 0.0f);
			glPointSize(5.0f);
			glBegin(GL_POINTS);
			OGL_Point p(pt.first, pt.second);
			glVertex2d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1]);		
			glEnd();
		}
	}

	::glPopMatrix();
	::glFinish();
	if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc())) {}

		
}

void OGL_Contorl::setDrawObject(int m, OBJECT o, bool b) {
	if (m == 2) {
		this->PDDS.object2D.setDrawObject(o, b);
	}
	else if (m == 3) {
		//this->PDDS.object3D.setDrawObject(o, b);
	}
}

void OGL_Contorl::updateVectors() {
	double va[3], vb[3]; // up vector, front vector

	// compute vb
	for (int i = 0; i < 3; i++) {
		vb[i] = this->view[1][i] - this->view[0][i];
	}

	// compute up vector
	double cosZ = vb[2] / std::sqrt(std::pow(vb[1],2)+ std::pow(vb[2], 2));
	double rotateAZ = acos(cosZ);
	if (cosZ > 0) {
		rotateAZ = (M_PI / 2) - rotateAZ;
	}
	else {
		rotateAZ = rotateAZ - (M_PI / 2);
	}

	va[0] = 0;
	va[1] = -std::sin(-rotateAZ);
	va[2] = std::cos(-rotateAZ);

	// cross product (compute left and right vectores)
	this->vectors[1][0] = va[1] * vb[2] - va[2] * vb[1];
	this->vectors[1][1] = va[2] * vb[0] - va[0] * vb[2];
	this->vectors[1][2] = va[0] * vb[1] - va[1] * vb[0];

	this->vectors[2][0] = vb[1] * va[2] - vb[2] * va[1];
	this->vectors[2][1] = vb[2] * va[0] - vb[0] * va[2];
	this->vectors[2][2] = vb[0] * va[1] - vb[1] * va[0];
	// normalize;
	double norm = std::sqrt(std::pow(this->vectors[1][0], 2) + std::pow(this->vectors[1][1], 2) + std::pow(this->vectors[1][2], 2));
	for (int i = 0; i < 3; i++) {
		this->vectors[1][i] /= norm;
	}
	norm = std::sqrt(std::pow(this->vectors[2][0], 2) + std::pow(this->vectors[2][1], 2) + std::pow(this->vectors[2][2], 2));
	for (int i = 0; i < 3; i++) {
		this->vectors[2][i] /= norm;
	}
	for (int i = 0; i < 3; i++) {
		this->vectors[0][i] = va[i];
	}
}

void OGL_Contorl::setCamera(int tx, int ty, int width, int height) {
	this->PDDS.object2D.setNorm(tx, ty, width, height);
}

void OGL_Contorl::addVertexPolygon(double x, double y) {
	this->PDDS.insertObstacleVertex(x,y);
}

int OGL_Contorl::addPoint(double x, double y) {
	return this->PDDS.insertInputPoint(x, y);
}

int OGL_Contorl::addPointAlign(double x, double y) {
	int ax, ay;
	double normTrans[3];
	double normMul[3];
	this->PDDS.object2D.getNorm(normTrans, normMul, 2);


	ax = x * normMul[0];
	ax = ax + normTrans[0];
	ay = y * normMul[1];
	ay = ay + normTrans[1];

	return this->PDDS.insertInputPoint(ax, ay);
}

int OGL_Contorl::dynamicAddPoint(double x, double y) {
	return this->PDDS.dynamicInsertInputPoint(x, y);
}

int OGL_Contorl::dynamicAddPointAlign(double x, double y) {
	int ax, ay;
	double normTrans[3];
	double normMul[3];
	this->PDDS.object2D.getNorm(normTrans, normMul, 2);


	ax = x * normMul[0];
	ax = ax + normTrans[0];
	ay = y * normMul[1];
	ay = ay + normTrans[1];

	return this->PDDS.dynamicInsertInputPoint(ax, ay);
}

bool OGL_Contorl::readInputData(std::string fileName) {
	int readResult = this->PDDS.readInputData(fileName);
	if (readResult == 0) {
		return false;
	}
	else {
		double maxx = std::numeric_limits<double>::lowest();
		double minx = -maxx;
		double maxy = std::numeric_limits<double>::lowest();
		double miny = -maxy;

		for (int i = 0; i < this->PDDS.getObstaclesNum(); i++) {
			for (int j = 0; j < this->PDDS.getObstacleSize(i); j++) {
				auto pt = this->PDDS.getObstacleVertex(i, j);
				if (minx > pt.first) minx = pt.first;
				if (maxx < pt.first) maxx = pt.first;
				if (miny > pt.second) miny = pt.second;
				if (maxy < pt.second) maxy = pt.second;
			}
		}

		for (int i = 0; i < this->PDDS.getInputPointsSize(); i++) {
			auto pt = this->PDDS.getInputPoint(i);
			if (minx > pt.first) minx = pt.first;
			if (maxx < pt.first) maxx = pt.first;
			if (miny > pt.second) miny = pt.second;
			if (maxy < pt.second) maxy = pt.second;
		}

		int tx = (maxx + minx) / 2;

		int width = (maxx - minx) / 2;

		width = width * 1.1;
		int ty = (maxy + miny) / 2;
		int height = (maxy - miny) / 2;
		height = height * 1.1;
		this->PDDS.object2D.setNorm(tx, ty, width, height);

		Invalidate(TRUE);
		UpdateWindow();
		return true;
	}
}

int OGL_Contorl::addQueryPoint(double x, double y) {
	return this->PDDS.insertQueryPoint(x, y);
}


int OGL_Contorl::addQueryPointAlign(double x, double y) {
	int ax, ay;
	double normTrans[3];
	double normMul[3];
	this->PDDS.object2D.getNorm(normTrans, normMul, 2);


	ax = x * normMul[0];
	ax = ax + normTrans[0];
	ay = y * normMul[1];
	ay = ay + normTrans[1];

	return this->PDDS.insertQueryPoint(ax, ay);
}

