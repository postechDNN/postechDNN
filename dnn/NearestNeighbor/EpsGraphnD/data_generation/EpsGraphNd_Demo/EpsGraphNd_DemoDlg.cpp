
// EpsGraphNd_DemoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"

#include "EpsGraphNd_Demo.h"
#include "EpsGraphNd_DemoDlg.h"

#include "afxdialogex.h"
#include "../EpsGraphnD.h"
#include "../data_generation.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <io.h>
#include <string>
#include <ctime>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Eps_Graph* eps_graph = nullptr;
Eps_Graph_nD* epsGraph = nullptr; 
// bool PRINT_POINT_FLAG = false;


// need to fix

// std::string polytopeStorePath = "C:\\qhull\\bin\\A\\";
std::string polytopeStorePath = "C:\\qhull\\bin\\A\\";
// std::string tetraDomainStr = "C:\\qhull\\bin\\A\\";
std::string tetraDomainStr = "C:\\qhull\\bin\\A\\";

int globalDimension = 1;
vector<Polytope> globalPolytopes = {};
list<Free_Point> globalFreePoints = {};
double globalEpsilon = 5.0;

// vector<double> queryPointVals = {};
Point* globalQueryPoint = nullptr;

CString globalLog;
CString globalLogTemp;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

namespace fs = std::filesystem;

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
public:

	afx_msg void OnBnClickedButton_OpenInputFiles();
	// afx_msg void OnBnClickedOk();
	// afx_msg void OnBnClickedCancel();
	// afx_msg void OnEnChangedimension();
	// afx_msg void OnBnClickedGenerate();
	// afx_msg void OnBnClickedOk2();
	// afx_msg void OnBnClickedQuery();
	// afx_msg void OnEnSetfocushalfplaneconstraints();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	// DDX_Text(pDX, IDC_K, )// CEpsGraphNdDemoDlg::K);
	// DDX_Text(pDX, IDC_x, );

	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	// ON_BN_CLICKED(IDC_BUTTON3, &CAboutDlg::OnBnClickedButton_OpenInputFiles)
	//ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	//ON_BN_CLICKED(IDCANCEL, &CAboutDlg::OnBnClickedCancel)

	//ON_EN_CHANGE(IDC_dimension, &CAboutDlg::OnEnChangedimension)
	//ON_BN_CLICKED(IDC_BUTTON4, &CAboutDlg::OnBnClickedGenerate)
	//// ON_BN_CLICKED(IDOK2, &CAboutDlg::OnBnClickedOk2)

	//ON_BN_CLICKED(IDC_BUTTON2, &CAboutDlg::OnBnClickedQuery)
	//ON_EN_SETFOCUS(IDC_halfplaneConstraints, &CAboutDlg::OnEnSetfocushalfplaneconstraints)
	

END_MESSAGE_MAP()


// CEpsGraphNdDemoDlg 대화 상자



CEpsGraphNdDemoDlg::CEpsGraphNdDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EPSGRAPHND_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEpsGraphNdDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_dimension, dimension);
	DDX_Control(pDX, IDC_maxValue, maxValue);
	DDX_Control(pDX, IDC_halfplaneConstraints, halfplaneConstraints);
	DDX_Control(pDX, IDC_maximumConvexSubparts, maximumConvexSubparts);
	DDX_Control(pDX, IDC_subspacesAlongEachAxis, subspacesAlongEachAxis);
	DDX_Control(pDX, IDC_pointsWithinEachCell, pointsWithinEachCell);
	DDX_Control(pDX, IDC_pointsOnEachHalfplane, pointsOnEachHalfplane);
	DDX_Control(pDX, IDC_domains, domains);
	DDX_Control(pDX, IDC_K, K);
	DDX_Control(pDX, IDC_x, x);
	DDX_Control(pDX, IDC_y, y);
	DDX_Control(pDX, IDC_queryTime, queryTime);
	// DDX_Control(pDX, IDOK2, ok2);
	DDX_Control(pDX, IDC_EDIT2, IDC_log);
}

BEGIN_MESSAGE_MAP(CEpsGraphNdDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	// ON_BN_CLICKED(IDC_BUTTON3, &CEpsGraphNdDemoDlg::OnBnClickedButton3)
	// ON_EN_CHANGE(IDC_dimension, &CEpsGraphNdDemoDlg::OnEnChangedimension)
	ON_BN_CLICKED(IDC_BUTTON2, &CEpsGraphNdDemoDlg::OnBnClickedQuery)
	ON_BN_CLICKED(IDC_BUTTON5, &CEpsGraphNdDemoDlg::OnBnClickedOpenConfigFile)
	ON_BN_CLICKED(IDC_BUTTON4, &CEpsGraphNdDemoDlg::OnBnClickedGenerate)
	ON_BN_CLICKED(IDC_BUTTON6, &CEpsGraphNdDemoDlg::OnBnClickedReadTetrahedralizedDomain)
	ON_BN_CLICKED(IDC_BUTTON7, &CEpsGraphNdDemoDlg::OnBnClickedButtonSetDirectory)

	ON_BN_CLICKED(IDC_setDirectoryPolytope, &CEpsGraphNdDemoDlg::OnBnClickedsetdirectorypolytope)

	ON_BN_CLICKED(IDC_freePointGenerate, &CEpsGraphNdDemoDlg::OnBnClickedfreepointgenerate)
	ON_BN_CLICKED(IDC_BUTTON8, &CEpsGraphNdDemoDlg::OnBnClickedLocateRandomly)
	ON_BN_CLICKED(IDC_setEpsilon, &CEpsGraphNdDemoDlg::OnBnClickedsetepsilon)
	ON_BN_CLICKED(IDC_setEpsilon2, &CEpsGraphNdDemoDlg::OnBnClickedMakeEpsilonGraph)
	ON_BN_CLICKED(IDC_SetDimension, &CEpsGraphNdDemoDlg::OnBnClickedSetdimension)
END_MESSAGE_MAP()


// CEpsGraphNdDemoDlg 메시지 처리기

BOOL CEpsGraphNdDemoDlg::OnInitDialog()
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
	GetDlgItem(IDC_dimension)->SetWindowText(_T("4"));
	GetDlgItem(IDC_maxValue)->SetWindowText(_T("10.0"));
	GetDlgItem(IDC_maximumConvexSubparts)->SetWindowText(_T("2"));
	GetDlgItem(IDC_subspacesAlongEachAxis)->SetWindowText(_T("2"));
	GetDlgItem(IDC_pointsOnEachHalfplane)->SetWindowText(_T("3"));
	GetDlgItem(IDC_pointsWithinEachCell)->SetWindowText(_T("4"));
	// GetDlgItem(IDC_BUTTON6)->SetWindowText(_T("2"));
	GetDlgItem(IDC_domains)->SetWindowText(_T("2"));
	GetDlgItem(IDC_freePoints)->SetWindowText(_T("50"));
	GetDlgItem(IDC_K)->SetWindowText(_T("3"));
	GetDlgItem(IDC_epsilon)->SetWindowText(_T("5.0"));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CEpsGraphNdDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEpsGraphNdDemoDlg::OnPaint()
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
HCURSOR CEpsGraphNdDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAboutDlg::OnBnClickedButton_OpenInputFiles()
{
	static TCHAR BASED_CODE szFilter[] = _T("텍스트 파일(*.TXT) | *.TXT;|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.txt"), _T("text"), OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		// std::ifstream input_file(pathName);
	}

	// GetDlgItemText(IDC_Epsilon, buff);


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



// generate (nonconvex) polytope
void CEpsGraphNdDemoDlg::OnBnClickedGenerate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int dimension;
	int maximumConvexSubparts;
	int pointsWithinEachCell;
	double maxValue; CString CmaxValue;
	int subspacesAlongEachAxis;
	int pointsOnEachHalfplane;

	dimension = GetDlgItemInt(IDC_dimension);

	maximumConvexSubparts = GetDlgItemInt(IDC_maximumConvexSubparts);

	pointsWithinEachCell = GetDlgItemInt(IDC_pointsWithinEachCell);

	GetDlgItemText(IDC_maxValue, CmaxValue);
	maxValue = _wtof(CmaxValue);

	subspacesAlongEachAxis = GetDlgItemInt(IDC_subspacesAlongEachAxis);

	pointsOnEachHalfplane = GetDlgItemInt(IDC_pointsOnEachHalfplane);

	// int auto_answer;
	int d = dimension;
	double u_bound = maxValue;
	// int halfplane_constraint = ;
	int num_parts = maximumConvexSubparts;
	// int dis_pts;
	int num_subspace = subspacesAlongEachAxis;
	int dis_pts = pointsOnEachHalfplane;
	int dis_pts_cell = pointsWithinEachCell;


	std::cout.precision(3);
	std::cout << std::fixed;
	// std::string config("C:\\Users\\HWI\\Desktop\\StarLab_1017\\postechDNN\\dnn\\NearestNeighbor\\EpsGraphnD\\data_generation\\config.ini");

	std::vector<std::pair<double, double>> bbx;

	for (int i = 0; i < d; i++) bbx.push_back(std::make_pair(-u_bound, u_bound));

	std::cout << std::endl;
	vector<int> num_subspaces_over_axes;
	for (int i = 0; i < d; i++) {
		num_subspaces_over_axes.push_back(num_subspace);
	}
	// generate grid (cells)
	// set neighbors inside generate_grid( )
	auto cells = generate_grid(bbx, num_subspaces_over_axes); // 
	activate_cells(cells, num_parts);

	// 각 hp에 흩뿌리고 나서
	// 임의의 active한 pair 사이에서 발생하는 건가? 그럼 각 cell에 대해서 neighbor 다 뒤져서 restricted_halfplane을 가져와야 하나?
	disperse_pts_between_active_cells(bbx, num_subspaces_over_axes, cells, dis_pts);

	for (int i = 0; i + 1 < cells.size(); i++) {
		if (cells[i]->active) {
			for (auto nb : cells[i]->neighbors) {
				if (nb.first->active &&
					cells[i]->total_index < nb.first->total_index) {
					for (int j = 0; j < nb.second->on_points.size(); j++) {
						// if (PRINT_POINT_FLAG) print_pt(nb.second->on_points[j], j);
					}
				}
			}
		}
	}

	for (int i = 0; i < cells.size(); i++) {
		generate_points_on_bb(cells[i], dis_pts_cell);
		for (int j = 0; j < cells[i]->in_points.size(); j++) {
			// if (PRINT_POINT_FLAG) print_pt(cells[i]->in_points[j], j);
		}
	}

	std::string dir(polytopeStorePath);
	// std::string dir("C:\\qhull\\bin\\A\\");

	// 원래 폴더에 들어 있던 파일 모두 지우기
	fs::path currentPath = dir;
	fs::remove_all(dir);
	fs::create_directories(currentPath);

	int count = 0;

	for (int i = 0; i < cells.size(); i++) {
		if (!cells[i]->active) continue;

		vector<Point*> pts = cells[i]->in_points;
		for (auto nb : cells[i]->neighbors) {
			if (!nb.first->active) continue;

			pts.insert(pts.end(), nb.second->on_points.begin(), nb.second->on_points.end());
		}

		std::string res = "result";
		if (count < 10) {
			res += "00";
		}
		else if (count < 100) {
			res += "0";
		}
		res += std::to_string(count) + ".txt";

		std::ofstream fout(dir + res);

		// first line contains the dimension
		fout << d << std::endl;
		// second line contains the number of input points
		fout << pts.size() << std::endl;
		// remaining lines contain point coordinates
		for (auto pt : pts) {
			for (int j = 0; j < d - 1; j++) {
				fout << pt->getx(j) << " ";
			}
			fout << pt->getx(d - 1) << std::endl;
		}
		count++;

		fout.close();
	}

	globalLogTemp.Format(L" Generated Polytopes Successfully");
	globalLog += globalLogTemp;
	// CString log;
	// log.Format(L" Generated Polytopes Successfully");
	SetDlgItemText(IDC_log2, globalLog);

}

void CEpsGraphNdDemoDlg::OnBnClickedOpenConfigFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 읽어오기



}

// pathkNN
void CEpsGraphNdDemoDlg::OnBnClickedQuery()
{
	if (globalQueryPoint) {
		epsGraph->queryPoint = globalQueryPoint;

		// Point니까 Free_Point 위치에 넣어도 되나?

		
	}
	else {
		CString buff;
		CString token;
		GetDlgItemText(IDC_vertices, buff);
		int iStart = 0;
		// int i = 0;
		// double x = 0.0, y = 0.0;
		vector<double> vals;

		double temp = 0.0;
		vector<Point> vertices;
		while ((token = buff.Tokenize(_T(" \n"), iStart)) != "") {
			temp = _wtof(token);
			vals.push_back(temp);
			//if (i % 2 == 0)
			//	x = _wtof(token);
			//else {
			//	y = _wtof(token);
			//	vertices.push_back(Point(x, y));
			//}
			//i++;
		}
		Free_Point queryP(vals);
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int K;
	K = GetDlgItemInt(IDC_K);

	auto& p = epsGraph->queryPoint;
	int& k = K;

	vector<Free_Point> nbhd = epsGraph->kNN(p, k + 1);

	//////////////////////////////////////// iostream 사용 시 원래 출력 부분

	//for (auto nb : nbhd) {
	//	bool cond = (nb.getxs() == p->getxs());

	//	// 만약 본인과 같으면 넘어가고
	//	if (cond) continue;

	//	// 아니라면 좌표 모두 출력
	//	for (int j = 0; j < epsGraph->n; j++) {
	//		cout << nb.getx(j) << ' ';
	//	}
	//	cout << endl;
	//}

	//////////////////////////////////////// iostream 사용 시 원래 출력 부분

	CString log;
	CString tmp;
	log.Format(L"-------------------Print KNN-------------------\r\n");
	for (auto nb : nbhd) {
		bool cond = (nb.getxs() == p->getxs());

		// 만약 본인과 같으면 넘어가고
		if (cond) continue;

		// 아니라면 좌표 모두 출력
		for (int j = 0; j < epsGraph->n; j++) {
			if (j == 0) {
				tmp.Format(L"(");
				log = log + tmp;
			}

			tmp.Format(L"%lf ", nb.getx(j));
			log = log + tmp;
			// cout << nb.getx(j) << ' ';

			if (j == epsGraph->n - 1) {
				tmp.Format(L")");
				log = log + tmp;
			}
		}
		tmp.Format(L"\r\n");
		log = log + tmp;
	}
	SetDlgItemText(IDC_log2, log);


	// epsGraph->path_kNN(epsGraph->queryPoint, K);
	//double x; CString Cx;
	//double y; CString Cy;
	//GetDlgItemText(IDC_x, Cx);
	//x = _wtof(Cx);
	//GetDlgItemText(IDC_x, Cy);
	//y = _wtof(Cy);

	//CString log;


	//// Free_Point query_point(x, y);
	//Free_Point query_point({1.0, 2.0});



	//vector<Free_Point> results;
	//clock_t startTime = clock();
	//if (eps_graph) results = eps_graph->kNN(query_point, k);
	//clock_t endTime = clock();
}

// (2) set polytopes
void CEpsGraphNdDemoDlg::OnBnClickedReadTetrahedralizedDomain()
{

	CString log;

	if (tetraDomainStr.empty()) {
		log.Format(L"No valid directory");
		SetDlgItemText(IDC_log2, log);
		return;
	}
	
	// 이미 생성된 domain이 있다면, 초기화하고 다시 생성 
	if (epsGraph) {
		delete epsGraph;
		epsGraph = nullptr;
	}

	fs::path p(tetraDomainStr);

	vector<vector<Polytope*>> topes;

	//std::filesystem::path p(Str);

	fs::recursive_directory_iterator iter(p);

	vector<string> strVec;

	int numFiles = 0;

	// 현재는 내부에 있는 파일 개수만 셈
	for (auto& i = iter; i != fs::end(iter); ++i) {
		numFiles += 1;

		//auto A = i->path();
		//string stringPath = i->path().generic_string();

		//if (stringPath.find("DEL") == string::npos) continue;

		//strVec.push_back(stringPath);
	}

	int numDels = numFiles / 2;

	topes.push_back(dels2polytopes(tetraDomainStr, numDels));

	int dimension = topes[0][0]->getDimension();

	vector<Polytope> plts;
	for (auto tps : topes) {
		for (auto ts : tps) {
			plts.push_back(*ts);
		}
	}

	// 읽어온 plts (polytopes)를 바탕으로 epsilon graph 클래스 생성.

	//*** dimension을 또 가져와야 하나?
	//int dimension;
	//dimension = GetDlgItemInt(IDC_dimension);
	//*** dimension을 또 가져와야 하나?

	//// 현재 epsilon = 5로 되어있는 것을 수정 필요
	//epsGraph = new Eps_Graph_nD(dimension, {}, plts, 5);

	globalPolytopes = plts;

	log.Format(L"Successfully Read Tetrahedralized Domain");
	SetDlgItemText(IDC_log2, log);

	//if (eps_graph) {
	//	delete eps_graph;
	//	eps_graph = nullptr;
	//}
}

// tetrahedralized domain을 읽어올 디렉토리를 선택
void CEpsGraphNdDemoDlg::OnBnClickedButtonSetDirectory()
{
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("Domain이 포함된 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기

	// 경로를 가져와 사용할 경우, Edit Control 에 값 저장
	CString str;
	str.Format(_T("%s"), szBuffer);
	SetDlgItemText(IDC_EDIT2, str);

	std::string Str = std::string(CT2CA(str));

}

// polytope을 random generate한 뒤 저장할 directory를 선택
void CEpsGraphNdDemoDlg::OnBnClickedsetdirectorypolytope()
{
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("Domain이 포함된 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기

	// 경로를 가져와 사용할 경우, Edit Control 에 값 저장
	CString str;
	str.Format(_T("%s"), szBuffer);
	SetDlgItemText(IDC_EDIT2, str);

	polytopeStorePath = std::string(CT2CA(str));
	polytopeStorePath += "\\";
	// std::string Str = std::string(CT2CA(str));
}


// 입력한 수만큼
// generate free points randomly
// 우선은 polytope 이후 순서라고 하자.
// (3) set free points
void CEpsGraphNdDemoDlg::OnBnClickedfreepointgenerate()
{
	clock_t startTime = clock();

	CString log;

	int numFreePoints;
	numFreePoints = GetDlgItemInt(IDC_freePoints);

	int dimension;
	std::vector<Polytope> plts;



	//// 만약 epsGraph가 nullptr, 즉 tetrahedralized domain이 생성되지 않았다면 그대로 종료
	//if (!epsGraph) {
	//	log.Format(L"No tetrahedralized domain, construct epsilon graph only with free points (without polytopes)");
	//	// Epsilon Graph Index Range Error!\r\nTotal number of points = %d", eps_graph->fr_pts.size());
	//	SetDlgItemText(IDC_log2, log);

	//	dimension = GetDlgItemInt(IDC_dimension);
	//	// epsGraph = new Eps_Graph_nD(dimension, {}, {});

	//	// return;
	//}
	//else {
	//	dimension = epsGraph->n;
	//	plts = epsGraph->pols;

	//}

	dimension = globalDimension;
	plts = globalPolytopes;

	// set bounding box - (TODO) 나중에 class 생성자로 만들어야 함
	std::vector<std::pair<double, double>> bbx;

	double maxValue;
	CString CmaxValue;
	GetDlgItemText(IDC_maxValue, CmaxValue);
	maxValue = _wtof(CmaxValue);

	double u_bound = maxValue;

	// int u_bound = 50; //*** need to fix!

	for (int i = 0; i < dimension; i++) bbx.push_back(std::make_pair(-u_bound, u_bound));
	// set bounding box - (TODO) 나중에 class 생성자로 만들어야 함

	int num_pt = 0;
	vector<Point*> pts;

	// 일단은 주어진 polytope 바깥에 있도록
	while (num_pt < numFreePoints) {

		// 세번째 parameter는 그냥, 한 번에 몇 개의 점씩 만들 건지임
		// 즉 지금은, 점을 하나씩 만드는 것
		vector<Point*> ps = generate_point_sites(bbx, dimension, 1);

		// vector<Point*> ps = generate_point_sites(bbx, 2, 1, 0);

		for (auto tp : plts) {
			// 점이 input polytope 안에 있는 경우는 무시
			// (즉 원하는 수만큼의 점을 free space에서 얻기 위해서) - 이 코드는 삭제해도 되지 않나?
			if (tp.isIn(ps[0])) continue;
		}
		pts.push_back(ps[0]);
		num_pt++;
	}

	list<Free_Point> frpts(pts.begin(), pts.end());

	globalFreePoints = frpts;

	// epsGraph->fr_pts = frpts;

	clock_t endTime = clock();

	int construction_time = endTime - startTime;
	globalLog += _T("Construction time : %d ms", construction_time);

	globalLog += _T("Generated Free Points Successfully\r\n");


	// log.Format(L"Generated Free Points Successfully\r\n");
	// log += globalLogTemp;
	SetDlgItemText(IDC_log2, globalLog);
}


// query point 하나만 만들기
// 따라서 EpsGraphNd 클래스에 멤버 변수로 추가해야 함.
// query point를 random으로 generate (하나의 점)
void CEpsGraphNdDemoDlg::OnBnClickedLocateRandomly()
{
	clock_t startTime = clock();

	UpdateData(false);
	UpdateData(true);

	CString log;

	int dimension;

	if (!epsGraph) {
		log.Format(L"No tetrahedralized domain"); 
		// Epsilon Graph Index Range Error!\r\nTotal number of points = %d", eps_graph->fr_pts.size());
		SetDlgItemText(IDC_log2, log);

		return;
	}
	else {
		dimension = epsGraph->n;
	}
	 
	// set bounding box - (TODO) 나중에 class 생성자로 만들어야 함
	std::vector<std::pair<double, double>> bbx;

	// int u_bound = 50; //*** need to fix!

	double maxValue;
	CString CmaxValue;
	GetDlgItemText(IDC_maxValue, CmaxValue);
	maxValue = _wtof(CmaxValue);

	double u_bound = maxValue;

	for (int i = 0; i < dimension; i++) bbx.push_back(std::make_pair(-u_bound, u_bound));
	// set bounding box - (TODO) 나중에 class 생성자로 만들어야 함

	vector<Point*> ps;
	ps = generate_point_sites(bbx, dimension, 1);
	// epsGraph->queryPoint = ps[0];
	globalQueryPoint = ps[0];

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	log.Format(L"Randomly Located Query Point Successfully");
	SetDlgItemText(IDC_log2, log);
}


void CEpsGraphNdDemoDlg::OnBnClickedsetepsilon()
{
	clock_t startTime = clock();

	// CString log;

	CString Cepsilon;
	GetDlgItemText(IDC_epsilon, Cepsilon);
	// epsGraph->eps = _wtof(Cepsilon);
	globalEpsilon = _wtof(Cepsilon);

	globalLog.Format(L"Set epsilon Successfully");
	SetDlgItemText(IDC_log2, globalLog);

	// 만약 epsilon graph가 아직 생성되지 않았다면
	// 로그를 출력하고 바로 종료
	//if (!epsGraph) {
	//	// log.Format(L"Randomly Located Query Point Successfully");
	//	// SetDlgItemText(IDC_log2, log);
	//}
	//// epsilon graph가 생성되어 있다면 epsilon 값을 업데이트
	//else {
	//	CString Cepsilon;
	//	GetDlgItemText(IDC_epsilon, Cepsilon);
	//	// epsGraph->eps = _wtof(Cepsilon);
	//	globalEpsilon = _wtof(Cepsilon);
	//}
}


void CEpsGraphNdDemoDlg::OnBnClickedMakeEpsilonGraph()
{
	clock_t startTime = clock();

	//// 만약 epsilon graph가 있으면 log 출력 후 종료
	//if (epsGraph) {
	//	
	//	globalLog.Format(L"Randomly Located Query Point Successfully");
	//	SetDlgItemText(IDC_log2, globalLog);
	//}

	//// 없다면 새로 생성
	//else {
	//	int dimension;
	//	dimension = GetDlgItemInt(IDC_dimension);
	//
	//	epsGraph = new Eps_Graph_nD(dimension, globalFreePoints, globalPolytopes, globalEpsilon);
	//}

	epsGraph = new Eps_Graph_nD(globalDimension, globalFreePoints, globalPolytopes, globalEpsilon);

	globalLog.Format(L"Constructed Epsilon Graph Successfully");
	globalLog += _T("\r\n\r\n");
	SetDlgItemText(IDC_log2, globalLog);
}

// (1) set dimension
void CEpsGraphNdDemoDlg::OnBnClickedSetdimension()
{
	clock_t startTime = clock();

	globalDimension = GetDlgItemInt(IDC_dimension);

	globalLogTemp.Format(L"Set dimension Successfully");
	globalLog += globalLogTemp; 

	globalLog+= _T("\r\n\r\n");
	// globalLog += _T("abcde");

	//globalLogTemp.Format(L"\r\n");
	//globalLog += globalLogTemp;

	//addEmptyLine();
	SetDlgItemText(IDC_log2, globalLog);
}

void addEmptyLine() {
	globalLogTemp.Format(L"\r\n");
	globalLog += globalLogTemp;
}