#pragma once


// AddDialog ��ȭ ����

enum CONTROL_MODE { EADD, EDELETE, EQUERY, EVIEW };

class AddDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AddDialog)

public:
	AddDialog(CWnd* pParent = nullptr);   // ǥ�� �������Դϴ�.
	AddDialog(CONTROL_MODE CM, int dimension, CWnd* pParent = nullptr);
	virtual ~AddDialog();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CONTROL_MODE control_mode;
	int dimension;
	CString key;
	double coordinate[3];
	CEdit m_edit_key;
	CEdit m_edit_x;
	CEdit m_edit_y;
	CEdit m_edit_z;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CStatic m_static_key;
};
