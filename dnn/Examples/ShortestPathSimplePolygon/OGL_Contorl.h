#pragma once


// OGL_Contorl

#include "../ShortestPathSimplePolygon_DS.h"

enum OBJECT { VERTEX, EDGE, FACE, PATH };

class OGL_Contorl : public CStatic
{
	DECLARE_DYNAMIC(OGL_Contorl)

public:
	OGL_Contorl();
	virtual ~OGL_Contorl();
	void setMode(int m);

	void setCamera(int tx, int ty, int width, int height);

	void addVertex(int x, int y);

	bool readPolygon(std::string fileName);
	int addQueryPoint(double x, double y);

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

	void setDrawObject(int m, OBJECT o, bool b);
	void updateVectors(); // update up vector and 

	DNN_DS DDS;

	double view[3][3]; // opengl view eye[3] center[3] up[3]
	double vectors[3][3]; // up[3] left[3] right[3]
	int mouse[2];
	bool isChange;
	bool isMove;

private:
	int mode;
	bool isVectorUpdate;
};


