
// epsGraphDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "epsGraph.h"
#include "epsGraphDlg.h"
#include "afxdialogex.h"
#include "../../../NearestNeighbor/EpsGraph/eps_graph.h"
#include <random>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <list>
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
Eps_Graph* eps_graph = nullptr;

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


// CepsGraphDlg 대화 상자



CepsGraphDlg::CepsGraphDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EPSGRAPH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CepsGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CepsGraphDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CepsGraphDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CepsGraphDlg::OnBnClickedCancel)
	//ON_EN_CHANGE(IDC_EDIT1, &CepsGraphDlg::OnEnChangeEdit1)
	//ON_EN_CHANGE(IDC_EDIT2, &CepsGraphDlg::OnEnChangeEdit2)
	//ON_BN_CLICKED(IDC_BUTTON1, &CepsGraphDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_make, &CepsGraphDlg::OnBnClickedmake)
	ON_BN_CLICKED(IDC_query, &CepsGraphDlg::OnBnClickedquery)
	ON_BN_CLICKED(IDC_add_point, &CepsGraphDlg::OnBnClickedaddpoint)
	ON_BN_CLICKED(IDC_delete_point, &CepsGraphDlg::OnBnClickeddeletepoint)
	ON_BN_CLICKED(IDC_add_polygon, &CepsGraphDlg::OnBnClickedaddpolygon)
	ON_BN_CLICKED(IDC_delete_polygon, &CepsGraphDlg::OnBnClickeddeletepolygon)
	ON_BN_CLICKED(IDC_query_point, &CepsGraphDlg::OnBnClickedquerypoint)
	ON_BN_CLICKED(IDC_query_polygon, &CepsGraphDlg::OnBnClickedquerypolygon)
	ON_BN_CLICKED(IDC_Open, &CepsGraphDlg::OnBnClickedOpen)
END_MESSAGE_MAP()


// CepsGraphDlg 메시지 처리기

BOOL CepsGraphDlg::OnInitDialog()
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CepsGraphDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CepsGraphDlg::OnPaint()
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
HCURSOR CepsGraphDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CepsGraphDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CepsGraphDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CepsGraphDlg::OnBnClickedmake()
{
	double epsilon;
	int n_points, n_polygons;
	CString buff;
	GetDlgItemText(IDC_Epsilon, buff);
	epsilon = _wtof(buff);

	n_points = GetDlgItemInt(IDC_Points);
	n_polygons = GetDlgItemInt(IDC_Polygons);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	double MIN_X = 0.0, MAX_X = 1000.0;
	double MIN_Y = 0.0, MAX_Y = 1000.0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis_x(MIN_X,MAX_X);
	std::uniform_real_distribution<double> dis_y(MIN_Y,MAX_Y);

	list<Free_Point> pts;

	for (int i = 0; i < n_points;i++) {
		double x = dis_x(gen), y = dis_y(gen);


		pts.push_back(Free_Point(x, y));
	}

	vector<_Polygon> polygons;
	//TO_DO : Make random simple polygon
	
	if (eps_graph) {
		delete eps_graph;
		eps_graph = nullptr;
	}
	/*
	vector <Point> vers; // polygon vertices
	vers.push_back(Point(0.5, 4.5)); vers.push_back(Point(0.5, 1.5));
	vers.push_back(Point(2.5, 1.5)); vers.push_back(Point(2.5, 4.5));
	polygons.push_back(_Polygon(vers));
	*/

	int m = int(sqrt(n_polygons)) + 1;
	double grid_width = (MAX_X - MIN_X) / m;
	double grid_height = (MAX_Y - MIN_Y) / m;
	std::uniform_real_distribution<double> dis_unit(0, 1);
	int MAX_NUM_VERT = 10;
	std::uniform_int_distribution<int> dis_numVerts(3,MAX_NUM_VERT);
	for (int i = 0; i < n_polygons;i++) {
		int row = i / m;
		int col = i % m;
		double ctrX = row * grid_width + grid_width / 2;
		double ctrY = col * grid_height + grid_height / 2;
		double aveRadius = min(grid_height, grid_width) / 4;
		double irregurality = dis_unit(gen);
		double spikeness = dis_unit(gen);
		int numVerts = dis_numVerts(gen);
		
		vector <Point> vers = generatePolygon(ctrX, ctrY, aveRadius, irregurality, spikeness, numVerts);
		std::reverse(vers.begin(), vers.end());
		polygons.push_back(_Polygon(vers,i));
	}

	clock_t startTime = clock();
	eps_graph = new Eps_Graph(pts, polygons, epsilon);
	clock_t endTime = clock();
	int construction_time = endTime - startTime;
	CString log, tmp;
	log.Format(L"Epsilon(= %lf) Graph is made successfully.\r\n", epsilon);
	tmp.Format(L"# of points = %d, # of polygons = %d\r\n", n_points, n_polygons);
	log = log + tmp;
	tmp.Format(L"Construction time : %d ms", construction_time);
	log = log + tmp;
	SetDlgItemText(IDC_log, log);
}


void CepsGraphDlg::OnBnClickedquery()
{
	double x, y;
	CString buff;
	GetDlgItemText(IDC_x, buff);
	x = _wtof(buff);
	GetDlgItemText(IDC_y, buff);
	y = _wtof(buff);
	int k = GetDlgItemInt(IDC_k);
	CString log;
	if (k > eps_graph->fr_pts.size()) {
		log.Format(L"K(%d) should be smaller than or equal to the number of points(%d).",k,eps_graph->fr_pts.size());
		SetDlgItemText(IDC_log, log);
		return;
	}
	

	Free_Point query_point(x, y);
	vector<Free_Point> results;
	clock_t startTime = clock();
	if (eps_graph) results = eps_graph->kNN(query_point, k);
	clock_t endTime = clock();

	if (results.size() < k) {
		log.Format(L"The query point (%lf, %lf) is inside some polygon.\r\n",x,y);
		SetDlgItemText(IDC_log, log);
		return;
	}

	int q_time = endTime - startTime;
	CString result;
	for (int i = 0; i < k;i++) {
		CString temp;
		double dist = sqrt((results[i].x - x) * (results[i].x - x) + (results[i].y - y) * (results[i].y - y));
		Grid_Point anchor_result = eps_graph->query_anchor(results[i]);
		Grid_Point anchor_query = eps_graph->query_anchor(query_point);
		double dist_result = sqrt((anchor_result.x - results[i].x) * (anchor_result.x - results[i].x) + (anchor_result.y - results[i].y) * (anchor_result.y - results[i].y));
		double dist_query = sqrt((anchor_query.x - query_point.x) * (anchor_query.x - query_point.x) + (anchor_query.y - query_point.y) * (anchor_query.y - query_point.y));
		temp.Format(L"%lf %lf d : %lf\r\n", results[i].x, results[i].y, eps_graph->NN_dist[i] * eps_graph->eps + dist_query + dist_result);
		result = result + temp;
	}
	SetDlgItemText(IDC_query_result, result);
	CString query_time;
	query_time.Format(L"%d ms",q_time);
	SetDlgItemText(IDC_query_time, query_time);


	log.Format(L"%d-NN Query is done.",k);
	SetDlgItemText(IDC_log, log);
}

void CepsGraphDlg::OnBnClickedquerypoint()
{
	int idx = GetDlgItemInt(IDC_index_point);
	CString log;
	if (idx < 0 || idx >= eps_graph->fr_pts.size()) {
		log.Format(L"Index Range Error!\r\nTotal number of points = %d", eps_graph->fr_pts.size());
		SetDlgItemText(IDC_log, log);
		return;
	}
	clock_t startTime = clock();
	Free_Point p = eps_graph->get_free_point(idx);
	clock_t endTime = clock();
	int tick = endTime - startTime;

	log.Format(L"%d index Point Query is done.", idx);
	SetDlgItemText(IDC_log, log);
	CString tmp;
	tmp.Format(L"%d ms", tick);
	SetDlgItemText(IDC_query_time,tmp);
	tmp.Format(L"%lf %lf", p.x, p.y);
	SetDlgItemText(IDC_query_result, tmp);
}


void CepsGraphDlg::OnBnClickedaddpoint()
{
	double x, y;
	CString buff;
	GetDlgItemText(IDC_x2, buff);
	x = _wtof(buff);
	GetDlgItemText(IDC_y2, buff);
	y = _wtof(buff);

	//TO DO : add point
	vector<Free_Point> added_pts = {Free_Point(x,y)};
	eps_graph->add_freepts(added_pts);
	CString log;
	log.Format(L"(%lf, %lf) Point is added.\r\n# of points = %d",x,y,eps_graph->fr_pts.size());
	SetDlgItemText(IDC_log, log);
}


void CepsGraphDlg::OnBnClickeddeletepoint()
{
	int idx = GetDlgItemInt(IDC_index_point);
	CString log;
	if (idx < 0 || idx >= eps_graph->fr_pts.size()) {
		log.Format(L"Index Range Error!\r\nTotal number of points = %d", eps_graph->fr_pts.size());
		SetDlgItemText(IDC_log, log);
		return;
	}

	//TO DO : delete point
	Free_Point p = eps_graph->get_free_point(idx);
	eps_graph->delete_freept(idx);
	log.Format(L"idx : %d - (%lf, %lf) Point is deleted.\r\n# of points = %d",idx,p.x ,p.y, eps_graph->fr_pts.size());
	SetDlgItemText(IDC_log, log);
}

BOOL CDialogEx::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
		if (GetDlgItem(IDC_vertices) == GetFocus()) {
			CEdit* edit = (CEdit*)GetDlgItem(IDC_vertices);
			int nLen = edit->GetWindowTextLength();
			edit->SetSel(nLen, nLen);
			edit->ReplaceSel(_T("\r\n"));
		}
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CepsGraphDlg::OnBnClickedaddpolygon()
{
	CString buff;
	CString token;
	GetDlgItemText(IDC_vertices, buff);
	int iStart=0;
	int i = 0;
	double x= 0.0, y= 0.0;
	vector<Point> vertices;
	while ((token = buff.Tokenize(_T(" \n"), iStart)) != "") {
		if (i % 2 == 0) 
			x = _wtof(token);
		else {
			y = _wtof(token);
			vertices.push_back(Point(x, y));
		}
		i++;
	}
	//TO DO : Add Polygon
	int idx = eps_graph->pols.size();
	eps_graph->add_pol(_Polygon(vertices,idx));
	CString log;
	CString tmp;
	log.Format(L"%d th Polygon is added.\r\n# of polygons = %d\r\nVertices\r\n",idx+1,idx+1);
	for (auto i : vertices) {
		tmp.Format(L"(%lf, %lf)\r\n", i.x, i.y);
		log = log + tmp;
	}
	SetDlgItemText(IDC_log, log);
}


void CepsGraphDlg::OnBnClickeddeletepolygon()
{
	int idx = GetDlgItemInt(IDC_index_polygon);
	CString log,tmp;
	if (idx < 0 || idx >= eps_graph->fr_pts.size()) {
		log.Format(L"Index Range Error!\r\nTotal number of polygons = %d", eps_graph->pols.size());
		SetDlgItemText(IDC_log, log);
		return;
	}

	//TO DO : Delete Polygon
	_Polygon poly = eps_graph->get_polygon(idx);
	eps_graph->delete_pol(idx);
	log.Format(L"idx : %d - Polygon is deleted.\r\n# of polygons = %d\r\nVertices\r\n", idx, eps_graph->pols.size());
	for (auto i : poly.vers) {
		tmp.Format(L"(%lf, %lf)\r\n", i.x, i.y);
		log = log + tmp;
	}
	SetDlgItemText(IDC_log, log);
}



void CepsGraphDlg::OnBnClickedquerypolygon()
{
	int idx = GetDlgItemInt(IDC_index_polygon);

	CString log;
	if (idx < 0 || idx >= eps_graph->pols.size()) {
		log.Format(L"Index Range Error!\r\nTotal number of polygons = %d", eps_graph->pols.size());
		SetDlgItemText(IDC_log, log);
		return;
	}
	clock_t startTime = clock();
	_Polygon poly = eps_graph->get_polygon(idx);
	clock_t endTime = clock();
	int tick = endTime - startTime;

	log.Format(L"%d index Polygon Query is done.", idx);
	SetDlgItemText(IDC_log, log);
	CString tmp;
	tmp.Format(L"%d ms", tick);
	SetDlgItemText(IDC_query_time, tmp);
	CString result;
	for (auto i : poly.vers) {
		tmp.Format(L"%lf %lf\r\n", i.x, i.y);
		result = result + tmp;
	}
	SetDlgItemText(IDC_query_result, result);

}

double __clip(double x, double min, double max) {
	if (min > max) return x;
	else if (x < min) return min;
	else if (x > max) return max;
	else return x;
}

vector<Point> generatePolygon(double ctrX, double ctrY, double aveRadius, double irregularity, double spikeyness, int numVerts) {
	double PI = 3.1415926535897;
	irregularity = __clip(irregularity, 0,1) * 2 * PI / numVerts;
	spikeyness = __clip(spikeyness, 0, 1) * aveRadius;

	vector<double> angleSteps;
	double lower = (2 * PI / numVerts) - irregularity;
	double upper = (2 * PI / numVerts) + irregularity;
	double sum = 0;
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int i = 0; i < numVerts;i++) {
		std::uniform_real_distribution<double> dis(lower,upper);
		double tmp = dis(gen);
		angleSteps.push_back(tmp);
		sum += tmp;
	}

	double k = sum / 2 / PI;
	for (int i = 0; i < numVerts;i++)
		angleSteps[i] /= k;
	
	vector<Point> points;
	std::uniform_real_distribution<double> dis(0, 2*PI);
	double angle = dis(gen);
	for (int i = 0; i < numVerts;i++) {
		std::normal_distribution<double> dis(aveRadius, spikeyness);
		double r_i = __clip(dis(gen), 0, 2 * aveRadius);
		double x = ctrX + r_i * cos(angle);
		double y = ctrY + r_i * sin(angle);
		points.push_back(Free_Point(x, y));

		angle = angle + angleSteps[i];
	}
	return points;
}


void CepsGraphDlg::OnBnClickedOpen()
{
	static TCHAR BASED_CODE szFilter[] = _T("텍스트 파일(*.TXT) | *.TXT;|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.txt"), _T("text"), OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		MessageBox(pathName);
		std::ifstream input_file(pathName);
		
		int n_pts, n_polys;
		std::list<Free_Point> pts;
		std::vector<_Polygon> polys;
		input_file >> n_pts >> n_polys;
		for (int i = 0;i < n_pts;i++) {
			double x, y;
			input_file >> x >> y;
			pts.push_back(Free_Point(x, y));
		}
		for (int i = 0; i < n_polys;i++) {
			double x, y;
			int n_vertices;
			input_file >> n_vertices;
			std::vector<Point> vertices;
			for (int j = 0; j < n_vertices;j++) {
				input_file >> x >> y;
				vertices.push_back(Point(x, y));
			}
			polys.push_back(_Polygon(vertices, i));
		}
		double epsilon = 10;

		CString buff;
		GetDlgItemText(IDC_Epsilon, buff);
		if(buff.IsEmpty() == false)
			epsilon = _wtof(buff);

		int K = 1;
		
		clock_t startTime = clock();
		eps_graph = new Eps_Graph(pts, polys, epsilon);
		clock_t endTime = clock();
		int construction_time = endTime - startTime;
		CString log, tmp;
		log.Format(L"Epsilon(= %lf) Graph is open successfully.\r\n", epsilon);
		tmp.Format(L"# of points = %d, # of polygons = %d\r\n", n_pts, n_polys);
		log = log + tmp;
		tmp.Format(L"Construction time : %d ms", construction_time);
		log = log + tmp;
		SetDlgItemText(IDC_log, log);

		input_file.close();

	}
}
