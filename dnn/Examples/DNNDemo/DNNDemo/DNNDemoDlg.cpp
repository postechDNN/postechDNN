
// DNNDemoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "DNNDemo.h"
#include "DNNDemoDlg.h"
#include "AddDialog.h"
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
	DDX_Control(pDX, IDC_BUTTON_RENDER, m_button_render);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_button_add);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_button_delete);
	DDX_Control(pDX, IDC_CHECK_F1, m_check_f1);
	DDX_Control(pDX, IDC_CHECK_F2, m_check_f2);
	DDX_Control(pDX, IDC_CHECK_NOO1, m_check_noo1);
	DDX_Control(pDX, IDC_CHECK_NOO2, m_check_noo2);
	DDX_Control(pDX, IDC_STATIC_Q1, m_static_q1);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_button_query);
	DDX_Control(pDX, IDC_EDIT_Q1, m_edit_q1);
	DDX_Control(pDX, IDC_EDIT_QR1, m_edit_qr1);
	DDX_Control(pDX, IDC_EDIT_QR2, m_edit_qr2);
	DDX_Control(pDX, IDC_EDIT_QR3, m_edit_qr3);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_button_view);
	DDX_Control(pDX, IDC_EDIT_VX, m_edit_vx);
	DDX_Control(pDX, IDC_EDIT_VY, m_edit_vy);
	DDX_Control(pDX, IDC_EDIT_VZ, m_edit_vz);
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
	ON_BN_CLICKED(IDC_BUTTON_RENDER, &CDNNDemoDlg::OnBnClickedButtonRender)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNC, &CDNNDemoDlg::OnCbnSelchangeComboFunc)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDNNDemoDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDNNDemoDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_CHECK_F1, &CDNNDemoDlg::OnBnClickedCheckF1)
	ON_BN_CLICKED(IDC_CHECK_F2, &CDNNDemoDlg::OnBnClickedCheckF2)
	ON_BN_CLICKED(IDC_CHECK_NOO1, &CDNNDemoDlg::OnBnClickedCheckNoo1)
	ON_BN_CLICKED(IDC_CHECK_NOO2, &CDNNDemoDlg::OnBnClickedCheckNoo2)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CDNNDemoDlg::OnBnClickedButtonQuery)
	ON_WM_KEYDOWN()
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

	// Initialize Func Combo
	m_combo_func.AddString(_T("Read DCEL"));
	m_combo_func.AddString(_T("3D nearest neighbor search"));
	//m_combo_func.AddString(_T("추가메뉴"));

	// Initialize Check boxes
	m_check_vertex.SetCheck(true);
	m_check_edge.SetCheck(true);
	m_check_face.SetCheck(true);
	m_check_f1.SetCheck(true);
	m_check_noo1.SetCheck(true);

	// Initialize rendering object
	this->m_picture_opengl.setDrawObject(2, VERTEX, m_check_vertex.GetCheck());
	this->m_picture_opengl.setDrawObject(3, VERTEX, m_check_vertex.GetCheck());
	this->m_picture_opengl.setDrawObject(2, EDGE, m_check_edge.GetCheck());
	this->m_picture_opengl.setDrawObject(3, EDGE, m_check_edge.GetCheck());
	this->m_picture_opengl.setDrawObject(2, FACE, m_check_face.GetCheck());
	this->m_picture_opengl.setDrawObject(3, FACE, m_check_face.GetCheck());

	//this->EnableWindow(true);

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

	static TCHAR BASED_CODE szFilter[] = _T("입력 파일(*.in, *.txt) | *.IN;*.in;*.TXT;*.txt |모든파일(*.*)|*.*||");

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
	case 0: // Read DCEL
		m_picture_opengl.readDCEL(path);
		break;
	case 1: // 3D nearest neighbor
		m_picture_opengl.read3Deps(path);
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnBnClickedCheckVertex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = this->m_combo_func.GetCurSel();
	switch (menu) {
	case 0:
		this->m_picture_opengl.setDrawObject(2, VERTEX, m_check_vertex.GetCheck());
		break;
	case 1:
		this->m_picture_opengl.setDrawObject(3, VERTEX, m_check_vertex.GetCheck());
		break;
	default:
		break;
	}
	
}


void CDNNDemoDlg::OnBnClickedCheckEdge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = this->m_combo_func.GetCurSel();
	switch (menu) {
	case 0:
		this->m_picture_opengl.setDrawObject(2, EDGE, m_check_edge.GetCheck());
		break;
	case 1:
		this->m_picture_opengl.setDrawObject(3, EDGE, m_check_edge.GetCheck());
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnBnClickedCheckFace()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = this->m_combo_func.GetCurSel();
	switch (menu) {
	case 0:
		this->m_picture_opengl.setDrawObject(2, FACE, m_check_face.GetCheck());
		break;
	case 1:
		this->m_picture_opengl.setDrawObject(3, FACE, m_check_face.GetCheck());
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnBnClickedButtonRender()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Invalidate(TRUE);
	UpdateWindow();
}


void CDNNDemoDlg::OnCbnSelchangeComboFunc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = this->m_combo_func.GetCurSel();
	switch (menu) {
	case 0: // Read DCEL
		this->m_picture_opengl.setMode(2);
		this->m_check_vertex.SetWindowTextW(_T("Vertex"));
		this->m_check_edge.SetWindowTextW(_T("Edge"));
		this->m_check_face.SetWindowTextW(_T("Face"));
		this->m_check_f1.EnableWindow(false);
		this->m_check_f2.EnableWindow(false);
		this->m_check_noo1.EnableWindow(false);
		this->m_check_noo2.EnableWindow(false);
		this->m_button_add.EnableWindow(false);
		this->m_button_delete.EnableWindow(false);
		this->m_edit_q1.EnableWindow(false);
		this->m_static_q1.SetWindowTextW(_T("Query value"));
		this->m_button_query.EnableWindow(false);
		break;
	case 1: // 3D nearest neighbor
		this->m_picture_opengl.setMode(3);
		this->m_check_vertex.SetWindowTextW(_T("Free point"));
		this->m_check_edge.SetWindowTextW(_T("Grid"));
		this->m_check_face.SetWindowTextW(_T("Polytope"));
		this->m_check_f1.EnableWindow(true);
		this->m_check_f2.EnableWindow(true);
		this->m_check_noo1.EnableWindow(true);
		this->m_check_noo2.EnableWindow(true);
		this->m_button_add.EnableWindow(true);
		this->m_button_delete.EnableWindow(true);
		this->m_edit_q1.EnableWindow(true);
		this->m_static_q1.SetWindowTextW(_T("KNN"));
		this->m_button_query.EnableWindow(true);
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = this->m_combo_func.GetCurSel();
	switch (menu) {
	case 1: // 3D nearest neighbor
		if (m_check_f1.GetCheck()) { // Add free point
			if (m_check_noo1.GetCheck()) { // Add by manual
				AddDialog dlg(EADD,3);
				if (IDOK == dlg.DoModal()) {
					CString keyValue = dlg.key;
					double coordinate[3];
					for (int i = 0; i < 3; i++) coordinate[i] = dlg.coordinate[i];
					m_picture_opengl.DDS.add_fr(coordinate);
					// keyValue와 좌표로 삽입을 수행하는 함수 호출
					Invalidate(TRUE);
					UpdateWindow();
				}
			}
			else if (m_check_noo2.GetCheck()) { // Add by file
				static TCHAR BASED_CODE szFilter[] = _T("입력 파일(*.in, *.txt) | *.IN;*.in;*.TXT;*.txt |모든파일(*.*)|*.*||");
				CFileDialog dlg(TRUE, _T("*.IN"), _T("in"), OFN_HIDEREADONLY, szFilter);
				if (IDOK == dlg.DoModal()) {
					CString pathName = dlg.GetPathName();
					MessageBox(pathName);
					m_picture_opengl.DDS.add_fr(pathName);
					// 아래 readDCEL처럼 add by file 함수 호출
					// m_picture_opengl.readDCEL(path);
					Invalidate(TRUE);
				}
			}
		} 
		else if (m_check_f2.GetCheck()) { // Add polytope (Only add by file)
			if (m_check_noo1.GetCheck()) {
				MessageBox(_T("Polytope can only be inserted by file input"));
			}
			else {
				static TCHAR BASED_CODE szFilter[] = _T("입력 파일(*.in, *.txt) | *.IN;*.in;*.TXT;*.txt |모든파일(*.*)|*.*||");
				CFileDialog dlg(TRUE, _T("*.IN"), _T("in"), OFN_HIDEREADONLY, szFilter);
				if (IDOK == dlg.DoModal()) {
					CString pathName = dlg.GetPathName();
					MessageBox(pathName);
					m_picture_opengl.DDS.add_poly(pathName);
					// 아래 readDCEL처럼 add by file 함수 호출
					// m_picture_opengl.readDCEL(path);
					Invalidate(TRUE);
				}
			}
		} 
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnBnClickedButtonDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = this->m_combo_func.GetCurSel();
	switch (menu) {
	case 1: // 3D nearest neighbor
		if (m_check_f1.GetCheck()) { // delete free point
			if (m_check_noo2.GetCheck()) {
				MessageBox(_T("Free point can only be deleted manually"));
			}
			else {
				AddDialog dlg(EDELETE, 0);
				if (IDOK == dlg.DoModal()) {
					CString keyValue = dlg.key;
					int del_key = _ttoi(keyValue);
					m_picture_opengl.DDS.del_fr(del_key);
					// keyValue로 삭제를 수행하는 함수 호출
				}
			}
		} 
		else if (m_check_f2.GetCheck()) { // delete polytope
			if (m_check_noo2.GetCheck()) {
				MessageBox(_T("Polytope can only be deleted manually"));
			}
			else {
				AddDialog dlg(EDELETE, 0);
				if (IDOK == dlg.DoModal()) {
					CString keyValue = dlg.key;
					int del_key = _ttoi(keyValue);
					m_picture_opengl.DDS.del_poly(del_key);
					// keyValue로 삭제를 수행하는 함수 호출
				}
			}
		}
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnBnClickedCheckF1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_check_f1.GetCheck()) {
		m_check_f2.SetCheck(false);
		//m_check_noo1.EnableWindow(true);
	}
	else {
		m_check_f2.SetCheck(true);
		//m_check_noo1.EnableWindow(false);
	} 

}


void CDNNDemoDlg::OnBnClickedCheckF2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_check_f2.GetCheck()) { 
		m_check_f1.SetCheck(false);
		//m_check_noo2.SetCheck(true);
		//m_check_noo1.SetCheck(false);
		//m_check_noo1.EnableWindow(false);
	} 
	else {
		m_check_f1.SetCheck(true);
		//m_check_noo1.EnableWindow(true);
	}
}


void CDNNDemoDlg::OnBnClickedCheckNoo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_check_noo1.GetCheck()) m_check_noo2.SetCheck(false);
	else m_check_noo2.SetCheck(true);
}


void CDNNDemoDlg::OnBnClickedCheckNoo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_check_noo1.IsWindowEnabled() == true) {
		if (m_check_noo2.GetCheck()) m_check_noo1.SetCheck(false);
		else m_check_noo1.SetCheck(true);
	}
	else {
		m_check_noo2.SetCheck(true);
	}
}


void CDNNDemoDlg::OnBnClickedButtonQuery()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = this->m_combo_func.GetCurSel();
	switch (menu) {
	case 1:
	{
		AddDialog dlg(EQUERY, 3);
		if (IDOK == dlg.DoModal()) {
			
			int knn = _ttoi(dlg.key);
			double coordinate[3];
			for (int i = 0; i < 3; i++) coordinate[i] = dlg.coordinate[i];
			m_picture_opengl.DDS.set_knn(coordinate, knn);
			// k값과 좌표로 query를 수행하는 함수 호출

			// Print result
			// Query data size
			
			this->m_edit_q1.SetWindowTextW(dlg.key);
			CString temp;
			temp.Format(_T("%d"), m_picture_opengl.DDS.get_physical_memory() + m_picture_opengl.DDS.get_virtual_memory());
			m_edit_qr1.SetWindowTextW(temp);
			// Query time
			temp.Format(_T("%d"), m_picture_opengl.DDS.get_execution_time());
			m_edit_qr2.SetWindowTextW(temp);
			// Query accuracy
			//temp.Format(_T("%f"), m_picture_opengl.DDS.get_accuracy());
			//m_edit_qr3.SetWindowTextW(temp);
		}
	}
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar) {
	case 0x57: // 'w'
		this->m_picture_opengl.moveFront();
		Invalidate(TRUE);
		UpdateWindow();
		break;
	case 0x53: // 's'
		this->m_picture_opengl.moveBack();
		Invalidate(TRUE);
		UpdateWindow();
		break;
	case 0x41: // 'a'
		this->m_picture_opengl.moveLeft();
		Invalidate(TRUE);
		UpdateWindow();
		break;
	case 0x44: // 'd'
		this->m_picture_opengl.moveRight();
		Invalidate(TRUE);
		UpdateWindow();
		break;
	default:
		break;
	}

}

BOOL CDNNDemoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN)
	{
		::SendMessage(this->GetSafeHwnd(), pMsg->message, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}

	if (pMsg->message == WM_LBUTTONDOWN)
	{	
		CRect rect;
		this->m_picture_opengl.GetWindowRect(rect);
		if (rect.PtInRect(pMsg->pt)) {
			::SendMessage(this->m_picture_opengl.GetSafeHwnd(), pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}
	if (pMsg->message == WM_MOUSEMOVE)
	{
		CRect rect;
		this->m_picture_opengl.GetWindowRect(rect);
		if (rect.PtInRect(pMsg->pt)) {
			::SendMessage(this->m_picture_opengl.GetSafeHwnd(), pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}
	if (pMsg->message == WM_LBUTTONUP)
	{
		CRect rect;
		this->m_picture_opengl.GetWindowRect(rect);
		if (rect.PtInRect(pMsg->pt)) {
			::SendMessage(this->m_picture_opengl.GetSafeHwnd(), pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}
