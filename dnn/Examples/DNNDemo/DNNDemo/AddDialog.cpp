// AddDialog.cpp: 구현 파일
//

#include "pch.h"
#include "DNNDemo.h"
#include "AddDialog.h"
#include "afxdialogex.h"


// AddDialog 대화 상자

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


// AddDialog 메시지 처리기


void AddDialog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
		//this->m_static_key.SetDlgItemTextW(_T("KNN"));

	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
