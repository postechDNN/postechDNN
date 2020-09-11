
// epsGraphDlg.h: 헤더 파일
//

#pragma once

#include "../../../NearestNeighbor/EpsGraph/eps_graph.h"
// CepsGraphDlg 대화 상자
class CepsGraphDlg : public CDialogEx
{
// 생성입니다.
public:
	CepsGraphDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EPSGRAPH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedmake();
	afx_msg void OnBnClickedquery();
	afx_msg void OnBnClickedaddpoint();
	afx_msg void OnBnClickeddeletepoint();
	afx_msg void OnBnClickedaddpolygon();
	afx_msg void OnBnClickeddeletepolygon();
	afx_msg void OnBnClickedquerypoint();
	afx_msg void OnBnClickedquerypolygon();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOpen();
};

vector<Point> generatePolygon(double ctrX, double ctrY, double aveRadius, double irregularity, double spikeyness, int numVerts);
double __clip(double x, double min, double max);

