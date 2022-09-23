#pragma once


// OGL_Contorl

#include "DNN_DS.h"

enum OBJECT { VERTEX, EDGE, FACE };

class OGL_Contorl : public CStatic
{
	DECLARE_DYNAMIC(OGL_Contorl)

public:
	OGL_Contorl();
	virtual ~OGL_Contorl();
	void setMode(int m);
	void readDCEL(CString path);
	void read3Deps(CString path);

	void moveFront();
	void moveBack();
	void moveLeft();
	void moveRight();

	void rotateCamera(int mx, int my);

protected:
	HGLRC m_hRC;
	CDC* m_pDC;

	BOOL SetupPixelFormat();
	BOOL InitGLContext();
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void setDrawObject(int m, OBJECT o, bool b);
	void updateVectors(); // update up vector and 

	DNN_DS DDS;

	double view[3][3]; // opengl view eye[3] center[3] up[3]
	double vectors[3][3]; // up[3] left[3] right[3]
	int mouse[2];
	bool isChange;

private:
	int mode;
	bool m_mouse_drag;
	bool isVectorUpdate;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


