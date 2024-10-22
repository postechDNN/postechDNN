#pragma once


// AddDialog 대화 상자

enum CONTROL_MODE { EADD, EDELETE, EQUERY, EVIEW };

class AddDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AddDialog)

public:
	AddDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	AddDialog(CONTROL_MODE CM, int dimension, CWnd* pParent = nullptr);
	virtual ~AddDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CONTROL_MODE control_mode;
	int dimension;
	double coordinate[2];
	CEdit m_edit_x;
	CEdit m_edit_y;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
