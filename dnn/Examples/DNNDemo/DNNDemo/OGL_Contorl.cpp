// OGL_Contorl.cpp: 구현 파일
//

#include "pch.h"
#include "DNNDemo.h"
#include "OGL_Contorl.h"

#include <GL/gl.h> 
#include <GL/glu.h>
#include <random>

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
	this->m_mouse_drag = false;
	

	this->view[0][0] = 1.1;
	this->view[0][1] = 1.1;
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

void OGL_Contorl::readDCEL(CString path) {
	this->DDS.object2D.readDCEL(path, 2);
	Invalidate();
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

	if (this->mode == 2) {
		this->DDS.object2D.getNorm(normTrans, normMul, 2);

		::glPushMatrix();
		::glColor3f(0.0f, 1.0f, 0.0f);
		glColor3d(1.0, 0.0, 0.0);

		if (this->DDS.object2D.getDrawFaces()) {
			srand(time(NULL));
			for (int i = 0; i < this->DDS.object2D.getFacesNum(); i++) {
				if (this->DDS.object2D.getFace(i).isInner() == false) continue;

				::glColor3f((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);

				glBegin(GL_POLYGON);
				for (int j = 0; j < this->DDS.object2D.getFace(i).getSize(); j++) {
					OGL_Point p = this->DDS.object2D.getFace(i).getPoint(j);
					glVertex2d((p.getX()- normTrans[0])/normMul[0], (p.getY()-normTrans[1])/normMul[1]);
				}
				glEnd();
			}
		}

		if (this->DDS.object2D.getDrawEdges()) {
			::glColor3f(1.0f, 1.0f, 1.0f);
			glLineWidth(3.0f);
			for (int i = 0; i < this->DDS.object2D.getEdgesNum(); i++) {
				OGL_Point sp = this->DDS.object2D.getEdge(i).getStartP();
				OGL_Point ep = this->DDS.object2D.getEdge(i).getEndP();
				glBegin(GL_LINES);
				glVertex2d((sp.getX() - normTrans[0]) / normMul[0], (sp.getY() - normTrans[1]) / normMul[1]);
				glVertex2d((ep.getX() - normTrans[0]) / normMul[0], (ep.getY() - normTrans[1]) / normMul[1]);
				glEnd();
			}
		}

		if (this->DDS.object2D.getDrawVertices()) {
			::glColor3f(0.0f, 1.0f, 0.0f);
			glPointSize(3.0f);
			glBegin(GL_POINTS);
			for (int i = 0; i < this->DDS.object2D.getVerticsNum(); i++) {
				OGL_Point p = this->DDS.object2D.getVertex(i).getPos();
				glVertex2d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1]);
			}
			glEnd();
		}

		::glPopMatrix();
		::glFinish();
		if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc())) {}
		
	}
	else {
		this->DDS.object3D.getNorm(normTrans, normMul, 3);
		::glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ORTHO, ORTHO, -ORTHO, ORTHO, -ORTHO, ORTHO);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(this->view[0][0], this->view[0][1], this->view[0][2],
			this->view[1][0], this->view[1][1], this->view[1][2], 
			this->view[2][0], this->view[2][1], this->view[2][2]);

		if (this->DDS.object3D.getDrawFaces()) {
			glColor3d(1.0, 1.0, 1.0);
			
			for (int i = 0; i < this->DDS.object3D.getFacesNum(); i++) {
				glBegin(GL_POLYGON);
				for (int j = 0; j < this->DDS.object3D.getFace(i).getSize(); j++) {
					OGL_Point p = this->DDS.object3D.getFace(i).getPoint(j);
					glVertex3d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1], (p.getZ() - normTrans[2]) / normMul[2]);
				}
				glEnd();
			}

		}
		if (this->DDS.object3D.getDrawEdges()) {
			glColor3d(1.0, 1.0, 1.0);
			glLineWidth(1.0f);
			for (int i = 0; i < this->DDS.object3D.getEdgesNum(); i++) {
				glBegin(GL_LINES);
				OGL_Point sp = this->DDS.object3D.getEdge(i).getStartP();
				OGL_Point ep = this->DDS.object3D.getEdge(i).getEndP();
				glVertex3d((sp.getX() - normTrans[0]) / normMul[0], (sp.getY() - normTrans[1]) / normMul[1], (sp.getZ() - normTrans[2]) / normMul[2]);
				glVertex3d((ep.getX() - normTrans[0]) / normMul[0], (ep.getY() - normTrans[1]) / normMul[1], (ep.getZ() - normTrans[2]) / normMul[2]);
				glEnd();
			}
		}
		if (this->DDS.object3D.getDrawVertices()) {
			glColor3d(0.0f, 1.0f, 1.0f);
			glPointSize(1.0f);
			glBegin(GL_POINTS);
			for (int i = 0; i < this->DDS.object3D.getVerticsNum(); i++) {
				OGL_Point p = this->DDS.object3D.getVertex(i).getPos();
				glVertex3d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1], (p.getZ() - normTrans[2]) / normMul[2]);
			}
			glEnd();
		}
		::glPopMatrix();
		::glFinish();
		if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc())) {}
	}
}


void OGL_Contorl::OnMove(int x, int y)
{
	CStatic::OnMove(x, y);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Invalidate(TRUE);
	UpdateWindow();
}

void OGL_Contorl::setDrawObject(int m, OBJECT o, bool b) {
	if (m == 2) {
		switch (o) {
		case 0:
			this->DDS.object2D.setDrawVertices(b);
			this->DDS.object3D.setDrawVertices(b);
			break;
		case 1:
			this->DDS.object2D.setDrawEdges(b);
			this->DDS.object3D.setDrawEdges(b);
			break;
		case 2:
			this->DDS.object2D.setDrawFaces(b);
			this->DDS.object3D.setDrawFaces(b);
			break;
		default:
			break;
		}
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

void OGL_Contorl::moveFront() {
	double move[3];
	for (int i = 0; i < 3; i++) {
		move[i] = this->view[1][i] - this->view[0][i];
	}
	for (int i = 0; i < 3; i++) {
		this->view[0][i] += move[i] * MOVING_SPEED;
		this->view[1][i] += move[i] * MOVING_SPEED;
	}
}
void OGL_Contorl::moveBack() {
	double move[3];
	for (int i = 0; i < 3; i++) {
		move[i] = this->view[1][i] - this->view[0][i];
	}
	for (int i = 0; i < 3; i++) {
		this->view[0][i] -= move[i] * MOVING_SPEED;
		this->view[1][i] -= move[i] * MOVING_SPEED;
	}
}
void OGL_Contorl::moveLeft() {
	if (!this->isVectorUpdate) {
		this->updateVectors();
		isVectorUpdate = true;
	}
	for (int i = 0; i < 3; i++) {
		this->view[0][i] += this->vectors[1][i] * MOVING_SPEED;
		this->view[1][i] += this->vectors[1][i] * MOVING_SPEED;
	}
}
void OGL_Contorl::moveRight() {
	if (!this->isVectorUpdate) {
		this->updateVectors();
		isVectorUpdate = true;
	}
	for (int i = 0; i < 3; i++) {
		this->view[0][i] += this->vectors[2][i] * MOVING_SPEED;
		this->view[1][i] += this->vectors[2][i] * MOVING_SPEED;
	}
}

void OGL_Contorl::rotateCamera(int mx, int my) {
	this->isVectorUpdate = false;

	int dx = this->mouse[0] - mx;
	int dy = this->mouse[1] - my;
	double vb[3]; // front vector

	// compute vb
	for (int i = 0; i < 3; i++) {
		vb[i] = this->view[1][i] - this->view[0][i];
	}

	double cosZ = vb[2] / std::sqrt(std::pow(vb[1], 2) + std::pow(vb[2], 2));
	double rotateAZ = acos(cosZ);
	if (cosZ > 0) {
		rotateAZ = (M_PI / 2) - rotateAZ;
	}
	else {
		rotateAZ = rotateAZ - (M_PI / 2);
	}
	
	double temp[3];
	temp[0] = vb[0];
	temp[1] = std::cos(rotateAZ) * vb[1] - std::sin(rotateAZ) * vb[2];
	temp[2] = std::sin(rotateAZ) * vb[1] + std::cos(rotateAZ) * vb[2];
	for (int i = 0; i < 3; i++) vb[i] = temp[i];

	double cosY = vb[1] / std::sqrt(std::pow(vb[0], 2) + std::pow(vb[1], 2));
	double rotateAY = acos(cosZ);
	if (cosZ > 0) {
		rotateAY = (M_PI / 2) - rotateAY;
	}
	else {
		rotateAY = rotateAY - (M_PI / 2);
	}

	temp[2] = vb[2];
	temp[0] = std::cos(rotateAY) * vb[0] - std::sin(rotateAY) * vb[1];
	temp[1] = std::sin(rotateAY) * vb[0] + std::cos(rotateAY) * vb[1];
	for (int i = 0; i < 3; i++) vb[i] = temp[i];


	double angleZ, angleY;
	angleZ = (dx * M_PI / 180 * ROTATING_SPEED);
	angleY = (dy * M_PI / 180 * ROTATING_SPEED);

	temp[1] = vb[1];
	temp[0] = std::cos(angleY) * vb[0] + std::sin(angleY) * vb[2];
	temp[2] = -std::sin(angleY) * vb[0] + std::cos(angleY) * vb[2];
	for (int i = 0; i < 3; i++) vb[i] = temp[i];

	temp[0] = vb[0];
	temp[1] = std::cos(angleZ) * vb[1] - std::sin(angleZ) * vb[2];
	temp[2] = std::sin(angleZ) * vb[1] + std::cos(angleZ) * vb[2];
	for (int i = 0; i < 3; i++) vb[i] = temp[i];

	temp[2] = vb[2];
	temp[0] = std::cos(-rotateAY) * vb[0] - std::sin(-rotateAY) * vb[1];
	temp[1] = std::sin(-rotateAY) * vb[0] + std::cos(-rotateAY) * vb[1];
	for (int i = 0; i < 3; i++) vb[i] = temp[i];

	temp[0] = vb[0];
	temp[1] = std::cos(-rotateAZ) * vb[1] - std::sin(-rotateAZ) * vb[2];
	temp[2] = std::sin(-rotateAZ) * vb[1] + std::cos(-rotateAZ) * vb[2];
	for (int i = 0; i < 3; i++) vb[i] = temp[i];

	for (int i = 0; i < 3; i++) {
		this->view[1][i] = vb[i] + this->view[0][i];
	}
}

void OGL_Contorl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CStatic::OnLButtonDown(nFlags, point);
	this->mouse[0] = point.x;
	this->mouse[1] = point.y;
	this->m_mouse_drag = true;
}

void OGL_Contorl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (this->m_mouse_drag) {
		this->rotateCamera(point.x, point.y);
		this->mouse[0] = point.x;
		this->mouse[1] = point.y;
	}
	CStatic::OnMouseMove(nFlags, point);
}


void OGL_Contorl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CStatic::OnLButtonUp(nFlags, point);

	this->m_mouse_drag = false;
}

// EPSGraph function

void OGL_Contorl::read3Deps(CString path) {
	this->DDS.read3Deps(path);
<<<<<<< HEAD
	this->DDS.object3D.updateNorm(3);
=======
>>>>>>> 01192a494847673d5d9a34ebcee6939e60a4fac4
	Invalidate();
}
