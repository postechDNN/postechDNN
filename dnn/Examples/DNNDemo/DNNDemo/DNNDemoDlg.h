
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
	afx_msg void OnBnClickedButtonOk();
	CButton m_check_vertex;
	CButton m_check_edge;
	CButton m_check_face;
	afx_msg void OnBnClickedCheckVertex();
	afx_msg void OnBnClickedCheckEdge();
	afx_msg void OnBnClickedCheckFace();
};
