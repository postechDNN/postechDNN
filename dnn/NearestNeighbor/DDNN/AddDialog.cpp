// AddDialog.cpp: 구현 파일
//

#include "pch.h"
#include "DDNN.h"
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
	DDX_Control(pDX, IDC_EDIT_X, m_edit_x);
	DDX_Control(pDX, IDC_EDIT_Y, m_edit_y);
}


BEGIN_MESSAGE_MAP(AddDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &AddDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// AddDialog 메시지 처리기


void AddDialog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	this->m_edit_x.GetWindowTextW(temp);
	this->coordinate[0] = _wtof(temp);
	this->m_edit_y.GetWindowTextW(temp);
	this->coordinate[1] = _wtof(temp);
	
	CDialogEx::OnOK();
}


BOOL AddDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->SetWindowTextW(_T("Query Control"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
