
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
	CButton m_check_vertex;
	CButton m_check_edge;
	afx_msg void OnBnClickedCheckVertex();
	afx_msg void OnBnClickedCheckEdge();
	CButton m_button_add;
	afx_msg void OnCbnSelchangeComboFunc();
	afx_msg void OnBnClickedButtonAdd();
	CEdit m_edit_qr1;
	CEdit m_edit_qr2;
	afx_msg void OnBnClickedButtonView();
	CButton m_check_path;
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
	CEdit m_edit_query_tx;
	CEdit m_edit_query_ty;
	CEdit m_edit_result_length;
};
