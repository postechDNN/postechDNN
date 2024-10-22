
// DNNDemoDlg.h: 헤더 파일
//

#pragma once

#include "OGL_Contorl.h"

// CDNNDemoDlg 대화 상자
class CDNNDemoDlg : public CDialogEx
{
// 생성입니다.
public:
	CDNNDemoDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	void CDNNDemoDlg::printLogEditBox();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DNNDEMO_DIALOG };
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
	OGL_Contorl m_picture_opengl;
	CComboBox m_combo_func;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonFile();
	CEdit m_edit_filename;
	CButton m_check_vertex;
	CButton m_check_edge;
	CButton m_check_pointset;
	CButton m_check_query;
	afx_msg void OnBnClickedCheckVertex();
	afx_msg void OnBnClickedCheckEdge();
	afx_msg void OnBnClickedCheckPointset();
	afx_msg void OnBnClickedCheckQuery();
	CButton m_button_add;
	afx_msg void OnCbnSelchangeComboFunc();
	afx_msg void OnBnClickedButtonAdd();
	//CButton m_check_path;
	afx_msg void OnBnClickedCheckPath();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CButton m_button_save;
	CButton m_button_delete;
	afx_msg void OnClickedButtonSave();
	afx_msg void OnClickedButtonDelete();
	CButton m_check_query1;
	CButton m_check_query2;
	afx_msg void OnBnClickedCheckQuery1();
	afx_msg void OnBnClickedCheckQuery2();
	CEdit m_edit_query_sx;
	CEdit m_edit_query_sy;
	CButton m_check_drawing_polygon;
	CButton m_check_drawing_point;
	CButton m_check_drawing_query;
	afx_msg void OnBnClickedCheckDrawingPolygon();
	afx_msg void OnBnClickedCheckDrawingPoint();
	afx_msg void OnBnClickedCheckDrawingQuery();
	CEdit m_edit_result;
	CEdit m_edit_knn;
	CButton m_button_knn;
	afx_msg void OnBnClickedButtonKnn();
	CButton m_button_file;
};
