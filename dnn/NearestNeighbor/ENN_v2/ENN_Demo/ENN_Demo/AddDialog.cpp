// AddDialog.cpp: ���� ����
//

#include "pch.h"
#include "ENN_Demo.h"
#include "AddDialog.h"
#include "afxdialogex.h"


// AddDialog ��ȭ ����

IMPLEMENT_DYNAMIC(AddDialog, CDialogEx)

AddDialog::AddDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_DIALOG, pParent)
{

}

AddDialog::AddDialog(CONTROL_MODE CM, int dimension, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_DIALOG, pParent)
{
	this->control_mode = CM;
	this->dimension = dimension;
}

AddDialog::~AddDialog()
{
}

void AddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_KEY, m_edit_key);
	DDX_Control(pDX, IDC_EDIT_X, m_edit_x);
	DDX_Control(pDX, IDC_EDIT_Y, m_edit_y);
	DDX_Control(pDX, IDC_EDIT_Z, m_edit_z);
	DDX_Control(pDX, IDC_STATIC_KEY, m_static_key);
}


BEGIN_MESSAGE_MAP(AddDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &AddDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// AddDialog �޽��� ó����


void AddDialog::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->m_edit_key.GetWindowTextW(this->key);
	CString temp;
	if (this->dimension > 0) {
		this->m_edit_x.GetWindowTextW(temp);
		this->coordinate[0] = _wtof(temp);
	}
	if (this->dimension > 1) {
		this->m_edit_y.GetWindowTextW(temp);
		this->coordinate[1] = _wtof(temp);
	}
	if (this->dimension > 2) {
		this->m_edit_z.GetWindowTextW(temp);
		this->coordinate[2] = _wtof(temp);
	}
	CDialogEx::OnOK();
}


BOOL AddDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if (this->control_mode == EADD) {
		if (dimension < 3) {
			this->m_edit_z.EnableWindow(false);
			if (dimension < 2) {
				this->m_edit_y.EnableWindow(false);
				if (dimension < 1) {
					this->m_edit_x.EnableWindow(false);
				}
			}
		}
	}
	else if (this->control_mode == EDELETE) {
		this->SetWindowTextW(_T("Delete Control"));
		this->m_edit_x.EnableWindow(false);
		this->m_edit_y.EnableWindow(false);
		this->m_edit_z.EnableWindow(false);
	}
	else if (this->control_mode == EQUERY) {
		this->SetWindowTextW(_T("Query Control"));
		this->SetDlgItemTextW(IDC_STATIC_KEY, _T("KNN"));
	}
	else if (this->control_mode == EVIEW) {
		this->SetWindowTextW(_T("View Control"));
		this->m_edit_key.EnableWindow(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
