
// DNNDemoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "DNNDemo.h"
#include "DNNDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDNNDemoDlg 대화 상자



CDNNDemoDlg::CDNNDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DNNDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDNNDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPENGL, m_picture_opengl);
	DDX_Control(pDX, IDC_COMBO_FUNC, m_combo_func);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_edit_filename);
	DDX_Control(pDX, IDC_CHECK_VERTEX, m_check_vertex);
	DDX_Control(pDX, IDC_CHECK_EDGE, m_check_edge);
	DDX_Control(pDX, IDC_CHECK_FACE, m_check_face);
}

BEGIN_MESSAGE_MAP(CDNNDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CDNNDemoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CDNNDemoDlg::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDNNDemoDlg::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_CHECK_VERTEX, &CDNNDemoDlg::OnBnClickedCheckVertex)
	ON_BN_CLICKED(IDC_CHECK_EDGE, &CDNNDemoDlg::OnBnClickedCheckEdge)
	ON_BN_CLICKED(IDC_CHECK_FACE, &CDNNDemoDlg::OnBnClickedCheckFace)
END_MESSAGE_MAP()


// CDNNDemoDlg 메시지 처리기

BOOL CDNNDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// Func Combo 초기화 코드
	m_combo_func.AddString(_T("DCEL"));
	m_combo_func.AddString(_T("추가메뉴"));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDNNDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDNNDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDNNDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDNNDemoDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}




void CDNNDemoDlg::OnBnClickedButtonFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");

	static TCHAR BASED_CODE szFilter[] = _T("in 파일(*.IN) | *.IN;*.in |모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.IN"), _T("in"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()){
		CString pathName = dlg.GetPathName();
		MessageBox(pathName);
		m_edit_filename.SetWindowTextW(pathName);
	}
}

void CDNNDemoDlg::OnBnClickedButtonOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int menu = m_combo_func.GetCurSel();
	CString path;
	m_edit_filename.GetWindowTextW(path);
	switch (menu) {
	case 0:
		m_picture_opengl.readDCEL(path);
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnBnClickedCheckVertex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->m_picture_opengl.setDrawObject(2, VERTEX, m_check_vertex.GetCheck());
}


void CDNNDemoDlg::OnBnClickedCheckEdge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->m_picture_opengl.setDrawObject(2, EDGE, m_check_edge.GetCheck());
}


void CDNNDemoDlg::OnBnClickedCheckFace()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->m_picture_opengl.setDrawObject(2, FACE, m_check_face.GetCheck());
}
