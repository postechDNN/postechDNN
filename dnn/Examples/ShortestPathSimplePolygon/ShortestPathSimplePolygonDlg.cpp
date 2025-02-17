
// DNNDemoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ShortestPathSimplePolygon.h"
#include "ShortestPathSimplePolygonDlg.h"
#include "AddDialog.h"
#include "afxdialogex.h"

#include <vector>
#include <utility>
#include <string>

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

void CDNNDemoDlg::clearEditBox() {
	this->m_edit_query_sx.SetWindowTextW(_T(""));
	this->m_edit_query_sy.SetWindowTextW(_T(""));
	this->m_edit_query_tx.SetWindowTextW(_T(""));
	this->m_edit_query_ty.SetWindowTextW(_T(""));
	this->m_edit_result_length.SetWindowTextW(_T(""));
	this->m_edit_result_log.SetWindowTextW(_T(""));
}

void CDNNDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPENGL, m_picture_opengl);
	DDX_Control(pDX, IDC_COMBO_FUNC, m_combo_func);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_edit_filename);
	DDX_Control(pDX, IDC_CHECK_VERTEX, m_check_vertex);
	DDX_Control(pDX, IDC_CHECK_EDGE, m_check_edge);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_button_add);
	DDX_Control(pDX, IDC_CHECK_PATH, m_check_path);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_button_save);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_button_delete);
	DDX_Control(pDX, IDC_CHECK_QUERY1, m_check_query1);
	DDX_Control(pDX, IDC_CHECK_QUERY2, m_check_query2);
	DDX_Control(pDX, IDC_EDIT_QUERY_SX, m_edit_query_sx);
	DDX_Control(pDX, IDC_EDIT_QUERY_SY, m_edit_query_sy);
	DDX_Control(pDX, IDC_EDIT_QUERY_TX, m_edit_query_tx);
	DDX_Control(pDX, IDC_EDIT_QUERY_TY, m_edit_query_ty);
	DDX_Control(pDX, IDC_EDIT_RESULT_LENGTH, m_edit_result_length);
	DDX_Control(pDX, IDC_EDIT_RESULT_LOG, m_edit_result_log);
	DDX_Control(pDX, IDC_BUTTON_FILE, m_button_file);
}

BEGIN_MESSAGE_MAP(CDNNDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CDNNDemoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CDNNDemoDlg::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_CHECK_VERTEX, &CDNNDemoDlg::OnBnClickedCheckVertex)
	ON_BN_CLICKED(IDC_CHECK_EDGE, &CDNNDemoDlg::OnBnClickedCheckEdge)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNC, &CDNNDemoDlg::OnCbnSelchangeComboFunc)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDNNDemoDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_CHECK_PATH, &CDNNDemoDlg::OnBnClickedCheckPath)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDNNDemoDlg::OnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDNNDemoDlg::OnClickedButtonDelete)
	ON_BN_CLICKED(IDC_CHECK_QUERY1, &CDNNDemoDlg::OnBnClickedCheckQuery1)
	ON_BN_CLICKED(IDC_CHECK_QUERY2, &CDNNDemoDlg::OnBnClickedCheckQuery2)
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
	m_combo_func.AddString(_T("1. Add input file (Make polygon)"));
	m_combo_func.AddString(_T("2. Test the input file (Shortest path query)"));
	//m_combo_func.AddString(_T("추가메뉴"));

	// Initialize Check boxes
	m_check_vertex.SetCheck(true);
	m_check_edge.SetCheck(true);
	m_check_path.SetCheck(true);
	m_check_query1.SetCheck(true);

	// Initialize rendering object
	this->m_picture_opengl.setDrawObject(2, VERTEX, m_check_vertex.GetCheck());
	this->m_picture_opengl.setDrawObject(2, EDGE, m_check_edge.GetCheck());
	this->m_picture_opengl.setDrawObject(2, PATH, m_check_path.GetCheck());
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
		this->m_picture_opengl.DDS.spsp.clearData();
		this->clearEditBox();
		CString pathName = dlg.GetPathName();
		MessageBox(pathName);
		m_edit_filename.SetWindowTextW(pathName);
		if (this->m_picture_opengl.readPolygon(std::string(CT2CA(pathName))) == false) {
			MessageBox(_T("File Read Failure"), _T("Error"), MB_ICONHAND);
		}
		Invalidate(TRUE);
		UpdateWindow();	
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
		this->m_picture_opengl.setDrawObject(2, VERTEX, m_check_vertex.GetCheck());
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
		this->m_picture_opengl.setDrawObject(2, EDGE, m_check_edge.GetCheck());
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnCbnSelchangeComboFunc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = this->m_combo_func.GetCurSel();
	switch (menu) {
	case 0: // Add polygon
		this->m_picture_opengl.setMode(2);
		this->m_button_file.EnableWindow(false);
		this->m_check_path.EnableWindow(false);
		this->m_edit_filename.EnableWindow(false);
		this->m_check_query1.SetCheck(true);
		this->m_check_query2.SetCheck(false);
		this->m_check_query2.EnableWindow(false);
		this->m_button_add.EnableWindow(false);

		//Set opengl camera
		RECT rect;
		GetDlgItem(IDC_OPENGL)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		{
			int width = (rect.right - rect.left) / 2;
			int height = (rect.bottom - rect.top) / 2;
			int tx = (rect.right + rect.left) / 2;
			int ty = (rect.bottom + rect.top) / 2;
			m_picture_opengl.setCamera(tx, ty, width, height);
		}

		this->m_picture_opengl.DDS.spsp.clearData();
		this->clearEditBox();
		Invalidate(TRUE);
		UpdateWindow();
		break;
	case 1: // Query
		this->m_picture_opengl.setMode(2);
		this->m_button_file.EnableWindow(true);
		this->m_check_path.EnableWindow(true);
		this->m_check_query2.EnableWindow(true);
		this->m_button_add.EnableWindow(false);

		this->m_picture_opengl.DDS.spsp.clearData();
		this->clearEditBox();
		Invalidate(TRUE);
		UpdateWindow();
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnBnClickedButtonAdd()
{	
	if (this->m_picture_opengl.DDS.spsp.getNumPolygonVertices() < 3) {
		MessageBox(_T("Invalid polygon input."), _T("Error"), MB_ICONHAND);
	}
	else {
		this->m_picture_opengl.DDS.spsp.clearTestPoint();
		this->clearEditBox();
		AddDialog dlg(EADD, 3);
		if (IDOK == dlg.DoModal()) {
			bool source = this->m_picture_opengl.DDS.spsp.addQueryPoint(dlg.coordinate[0], dlg.coordinate[1]);
			bool target = this->m_picture_opengl.DDS.spsp.addQueryPoint(dlg.coordinate[2], dlg.coordinate[3]);
			if (source == false && target == false) {
				MessageBox(_T("Invalid source point and target point input."), _T("Error"), MB_ICONHAND);
			}
			else if (source == false) {
				MessageBox(_T("Invalid source point input."), _T("Error"), MB_ICONHAND);
			}
			else if (target == false) {
				MessageBox(_T("Invalid target point input."), _T("Error"), MB_ICONHAND);
			}
			else {
				CString temp;
				temp.Format(_T("%lf"), dlg.coordinate[0]);
				this->m_edit_query_sx.SetWindowTextW(temp);
				temp.Format(_T("%lf"), dlg.coordinate[1]);
				this->m_edit_query_sy.SetWindowTextW(temp);
				temp.Format(_T("%lf"), dlg.coordinate[2]);
				this->m_edit_query_tx.SetWindowTextW(temp);
				temp.Format(_T("%lf"), dlg.coordinate[3]);
				this->m_edit_query_ty.SetWindowTextW(temp);
				temp.Format(_T("%lf"), this->m_picture_opengl.DDS.spsp.getPathLength());
				this->m_edit_result_length.SetWindowTextW(temp);

				
				if (!this->m_picture_opengl.DDS.spsp.Strings.empty()) {
					CString result_log;
					auto path_points = this->m_picture_opengl.DDS.spsp.Strings.front();
					for (size_t i = 0; i < path_points.size(); i++) {
						CString temp1, temp2;
						temp1.Format(_T("%lf"), path_points[i].first);
						temp2.Format(_T("%lf"), path_points[i].second);
						result_log += temp1 + _T(' ') + temp2 + _T("\r\n");
					}
					this->m_edit_result_log.SetWindowTextW(result_log);
				}
				
			}
			Invalidate(TRUE);
			UpdateWindow();
		}
	}
}

void CDNNDemoDlg::OnBnClickedCheckPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = this->m_combo_func.GetCurSel();
	switch (menu) {
	case 0:
		this->m_picture_opengl.setDrawObject(2, PATH, m_check_path.GetCheck());
		break;
	case 1:
		this->m_picture_opengl.setDrawObject(2, PATH, m_check_path.GetCheck());
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (this->m_check_query1.GetCheck()) {
		int menu = this->m_combo_func.GetCurSel();
		RECT rect;
		GetDlgItem(IDC_OPENGL)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		switch (menu) {
		case 0:
			if (rect.left < point.x && point.x < rect.right &&
				rect.top < point.y && point.y < rect.bottom) {
				int x = point.x;
				int ygap = point.y - rect.top;
				int y = rect.bottom - ygap;
				m_picture_opengl.addVertex(x, y);
				Invalidate(TRUE);
				UpdateWindow();
			}

			break;
		case 1:
			if (this->m_picture_opengl.DDS.spsp.getNumPolygonVertices() < 3) {
				MessageBox(_T("Invalid polygon input."), _T("Error"), MB_ICONHAND);
			}
			else {
				if (rect.left < point.x && point.x < rect.right &&
					rect.top < point.y && point.y < rect.bottom) {
					double width = (rect.right - rect.left) / 2;
					double tx = (rect.right + rect.left) / 2;
					double x = (double(point.x) - tx) / width;

					double height = (rect.bottom - rect.top) / 2;
					double ty = (rect.bottom + rect.top) / 2;
					int ygap = point.y - rect.top;
					double y = rect.bottom - ygap;
					y = (y - ty) / height;

					if (this->m_picture_opengl.DDS.spsp.getNumQueryPoint() == 2) {
						this->m_picture_opengl.DDS.spsp.clearTestPoint();
						this->clearEditBox();
					}

					if (m_picture_opengl.addQueryPoint(x, y)) {
						std::vector<std::pair<double, double>> query_points = m_picture_opengl.DDS.spsp.getQueryPoints();
						if (query_points.size() == 1) {
							CString temp;
							temp.Format(_T("%lf"), query_points[0].first);
							this->m_edit_query_sx.SetWindowTextW(temp);
							temp.Format(_T("%lf"), query_points[0].second);
							this->m_edit_query_sy.SetWindowTextW(temp);
						}
						else if (query_points.size() == 2) {
							CString temp;
							temp.Format(_T("%lf"), query_points[1].first);
							this->m_edit_query_tx.SetWindowTextW(temp);
							temp.Format(_T("%lf"), query_points[1].second);
							this->m_edit_query_ty.SetWindowTextW(temp);

							temp.Format(_T("%lf"), this->m_picture_opengl.DDS.spsp.getPathLength());
							this->m_edit_result_length.SetWindowTextW(temp);

							if (!this->m_picture_opengl.DDS.spsp.Strings.empty()) {
								CString result_log;
								auto path_points = this->m_picture_opengl.DDS.spsp.Strings.front();
								for (size_t i = 0; i < path_points.size(); i++) {
									CString temp1, temp2;
									temp1.Format(_T("%lf"), path_points[i].first);
									temp2.Format(_T("%lf"), path_points[i].second);
									result_log += temp1 + _T(' ') + temp2 + _T("\r\n");
								}
								this->m_edit_result_log.SetWindowTextW(result_log);
							}
						}
					}

					else {
						MessageBox(_T("Invalid query input."), _T("Error"), MB_ICONHAND);				
					}
					Invalidate(TRUE);
					UpdateWindow();
				}
			}
			break;
		default:
			break;
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDNNDemoDlg::OnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = m_combo_func.GetCurSel();
	if (menu == 0 && this->m_picture_opengl.DDS.spsp.getNumPolygonVertices() < 3) {
		MessageBox(_T("Invalid polygon input."), _T("Error"), MB_ICONHAND);
	}

	static TCHAR BASED_CODE szFilter[] = _T("입력 파일(*.in, *.txt) | *.IN;*.in;*.TXT;*.txt |모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.IN"), _T("in"), OFN_HIDEREADONLY, szFilter);
	
	if (IDOK == dlg.DoModal()) {
		CString pathName = dlg.GetPathName();
	
		CString temp;
		switch (menu) {
		case 0: // input polygon
			m_picture_opengl.DDS.spsp.printInputPolygon(std::string(CT2CA(pathName)));
			break;
		case 1: // query
			// Query time
			m_picture_opengl.DDS.spsp.printQuery(std::string(CT2CA(pathName)));
			//temp.Format(_T("%d"), m_picture_opengl.DDS.get_execution_time());
			//m_edit_qr2.SetWindowTextW(temp);
			break;
		default:
			break;
		}
	}
}


void CDNNDemoDlg::OnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = m_combo_func.GetCurSel();
	switch (menu) {
	case 0: // input polygon
		this->m_picture_opengl.DDS.spsp.deleteVertex();
		this->clearEditBox();
		Invalidate(TRUE);
		UpdateWindow();
		break;
	case 1: // query
		this->m_picture_opengl.DDS.spsp.clearTestPoint();
		this->clearEditBox();
		Invalidate(TRUE);
		UpdateWindow();
		break;
	default:
		break;
	}
}


void CDNNDemoDlg::OnBnClickedCheckQuery1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int menu = this->m_combo_func.GetCurSel();
	if (menu != 0) {
		if (this->m_check_query1.GetCheck()) {
			this->m_check_query1.SetCheck(true);
			this->m_check_query2.SetCheck(false);
			this->m_picture_opengl.DDS.spsp.clearTestPoint();
			this->clearEditBox();
		}
		else {
			this->m_check_query1.SetCheck(false);
			this->m_check_query2.SetCheck(true);
			this->m_picture_opengl.DDS.spsp.clearTestPoint();
			this->clearEditBox();
		}
	}
	else {
		this->m_check_query1.SetCheck(true);
	}
}


void CDNNDemoDlg::OnBnClickedCheckQuery2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (this->m_check_query2.GetCheck()) {
		this->m_check_query1.SetCheck(false);
		this->m_check_query2.SetCheck(true);
		this->m_button_add.EnableWindow(true);
		this->m_picture_opengl.DDS.spsp.clearTestPoint();
		this->clearEditBox();
	}
	else {
		this->m_check_query1.SetCheck(true);
		this->m_check_query2.SetCheck(false);
		this->m_button_add.EnableWindow(false);
		this->m_picture_opengl.DDS.spsp.clearTestPoint();
		this->clearEditBox();
	}
}

