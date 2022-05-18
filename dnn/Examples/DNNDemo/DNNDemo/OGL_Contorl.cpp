// OGL_Contorl.cpp: 구현 파일
//

#include "pch.h"
#include "DNNDemo.h"
#include "OGL_Contorl.h"

#include <GL/gl.h> 
#include <GL/glu.h>
#include <fstream>
#include <map>
#include <string>
#include <random>

#pragma comment(lib, "glu32.lib" ) 
#pragma comment(lib, "opengl32.lib" )

// OGL_Contorl

IMPLEMENT_DYNAMIC(OGL_Contorl, CStatic)

OGL_Contorl::OGL_Contorl(){
	this->mode = 2;
}

OGL_Contorl::~OGL_Contorl(){

}

void OGL_Contorl::setMode(int m) {
	this->mode = m;
}

void OGL_Contorl::readDCEL(CString path) {
	std::ifstream file(path);
	int vn, fn, en;
	file >> vn >> fn >> en;
	
	std::string key;
	std::map<std::string,int> vMap;
	for (int i = 0; i < vn; i++) {
		file >> key;
		vMap[key] = i;
		this->object2D.addVertex();
	}

	std::map<std::string, int> fMap;
	for (int i = 0; i < fn; i++) {
		file >> key;
		fMap[key] = i;
		this->object2D.addFace();
	}

	std::map<std::string, int> eMap;
	for (int i = 0; i < en; i++) {
		file >> key;
		eMap[key] = i;
		this->object2D.addEdge();
	}

	double x, y;
	std::string temp;
	for (int i = 0; i < vn; i++) {
		file >> key >> x >> y >> temp;
		 this->object2D.getVertex(vMap[key]).setPos(x, y);
	}

	std::string se;
	int in;
	for (int i = 0; i < fn; i++) {
		file >> key >> se >> in >> temp;
		if (se == "NULL") {
			this->object2D.getFace(fMap[key]).setInner(false);
		}
		else {
			this->object2D.getFace(fMap[key]).setInner(true);
		}
		if (in != 0) {
			for (int j = 0; j < in - 1; j++) {
				file >> temp;
			}
		}
	}
	
	std::string vkey, fkey, ne;
	for (int i = 0; i < en; i++) {
		file >> key >> vkey >> temp >> fkey >> ne >> temp;
		Point p = this->object2D.getVertex(vMap[vkey]).getPos();
		this->object2D.getFace(fMap[fkey]).addPoint(p);
		this->object2D.getEdge(eMap[key]).setEndP(p);
		this->object2D.getEdge(eMap[ne]).setStartP(p);
	}
	this->object2D.updateNorm(2);
	Invalidate();
}

BEGIN_MESSAGE_MAP(OGL_Contorl, CStatic)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_MOVE()
END_MESSAGE_MAP()



// OGL_Contorl 메시지 처리기

BOOL OGL_Contorl::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		 1 ,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24 ,
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
	//double normTrans;
	//double normMul;

	// 사각형 테스트
	if (this->mode == 2) {
		this->object2D.getNorm(normTrans, normMul, 2);
		//this->object2D.getNorm(normTrans, normMul);

		::glPushMatrix();
		::glColor3f(0.0f, 1.0f, 0.0f);
		glColor3d(1.0, 0.0, 0.0);

		if (this->object2D.getDrawFaces()) {
			srand(time(NULL));
			for (int i = 0; i < this->object2D.getFacesNum(); i++) {
				if (this->object2D.getFace(i).isInner() == false) continue;

				::glColor3f((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);

				glBegin(GL_POLYGON);
				for (int j = 0; j < this->object2D.getFace(i).getSize(); j++) {
					Point p = this->object2D.getFace(i).getPoint(j);
					glVertex2d((p.getX()- normTrans[0])/normMul[0], (p.getY()-normTrans[1])/normMul[1]);
					//glVertex2d((p.getX() - normTrans) / normMul, (p.getY() - normTrans) / normMul);
				}
				glEnd();
			}
		}

		if (this->object2D.getDrawEdges()) {
			::glColor3f(1.0f, 1.0f, 1.0f);
			glLineWidth(3.0f);
			for (int i = 0; i < this->object2D.getEdgesNum(); i++) {
				Point sp = this->object2D.getEdge(i).getStartP();
				Point ep = this->object2D.getEdge(i).getEndP();
				glBegin(GL_LINES);
				glVertex2d((sp.getX() - normTrans[0]) / normMul[0], (sp.getY() - normTrans[1]) / normMul[1]);
				glVertex2d((ep.getX() - normTrans[0]) / normMul[0], (ep.getY() - normTrans[1]) / normMul[1]);
				//glVertex2d((sp.getX() - normTrans) / normMul, (sp.getY() - normTrans) / normMul);
				//glVertex2d((ep.getX() - normTrans) / normMul, (ep.getY() - normTrans) / normMul);
				glEnd();
			}
		}

		if (this->object2D.getDrawVertices()) {
			::glColor3f(0.0f, 1.0f, 0.0f);
			glPointSize(3.0f);
			glBegin(GL_POINTS);
			for (int i = 0; i < this->object2D.getVerticsNum(); i++) {
				Point p = this->object2D.getVertex(i).getPos();
				glVertex2d((p.getX() - normTrans[0]) / normMul[0], (p.getY() - normTrans[1]) / normMul[1]);
				//glVertex2d((p.getX() - normTrans) / normMul, (p.getY() - normTrans) / normMul);
			}
			glEnd();
		}

		::glPopMatrix();
		::glFinish();
		if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc())) {}
		
	}
	else {
		::glPushMatrix();
		::glColor3f(0.0f, 1.0f, 0.0f);
		glColor3d(1.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
		glVertex2d(-0.9, -0.9);
		glVertex2d(0.9, -0.9);
		glVertex2d(0.9, 0.9);
		glVertex2d(-0.9, 0.9);
		glEnd();

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
			this->object2D.setDrawVertices(b);
			break;
		case 1:
			this->object2D.setDrawEdges(b);
			break;
		case 2:
			this->object2D.setDrawFaces(b);
			break;
		default:
			break;
		}
	}
}