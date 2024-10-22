#pragma once


// OGL_Contorl

#include "Object.h"

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

	void setDrawObject(int m, OBJECT o, bool b);

private:
	int mode;
	Object object2D;
	Object object3D;
};


