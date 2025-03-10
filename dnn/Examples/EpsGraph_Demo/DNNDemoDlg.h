﻿
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
	CButton m_button_render;
	afx_msg void OnBnClickedButtonRender();
	CButton m_button_add;
	CButton m_button_delete;
	CButton m_check_f1;
	CButton m_check_f2;
	afx_msg void OnCbnSelchangeComboFunc();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedCheckF1();
	afx_msg void OnBnClickedCheckF2();
	afx_msg void OnBnClickedCheckNoo1();
	afx_msg void OnBnClickedCheckNoo2();
	CButton m_check_noo1;
	CButton m_check_noo2;
	CStatic m_static_q1;
	CButton m_button_query;
	afx_msg void OnBnClickedButtonQuery();
	CEdit m_edit_q1;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_edit_qr1;
	CEdit m_edit_qr2;
	CEdit m_edit_qr3;
	CButton m_button_view;
	CEdit m_edit_vx;
	CEdit m_edit_vy;
	CEdit m_edit_vz;
	afx_msg void OnBnClickedButtonView();
	CButton m_check_path;
	afx_msg void OnBnClickedCheckPath();
};
