
// EpsGraphNd_DemoDlg.h: 헤더 파일
//

#pragma once


// CEpsGraphNdDemoDlg 대화 상자
class CEpsGraphNdDemoDlg : public CDialogEx
{
// 생성입니다.
public:
	CEpsGraphNdDemoDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EPSGRAPHND_DEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit dimension;
	CEdit maxValue;
	CEdit halfplaneConstraints;
	CEdit maximumConvexSubparts;
	CEdit subspacesAlongEachAxis;
	CEdit pointsWithinEachCell;
	CEdit pointsOnEachHalfplane;
	CEdit domains;
	CEdit K;
	CEdit x;
	CEdit y;
	CEdit queryTime;

	CEdit epsilon;
	// CEdit queryTime;

	// afx_msg void OnBnClickedButton3();
	CButton ok2;
	// afx_msg void OnEnChangedimension();
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedOpenConfigFile();
	afx_msg void OnBnClickedGenerate();
	afx_msg void OnBnClickedReadTetrahedralizedDomain();
	afx_msg void OnBnClickedButtonSetDirectory();
	afx_msg void OnBnClickedsetdirectorypolytope();
	afx_msg void OnBnClickedfreepointgenerate();
	
	CEdit IDC_log;
	// CEdit IDC_log2;

	afx_msg void OnBnClickedLocateRandomly();
	afx_msg void OnBnClickedsetepsilon();
	afx_msg void OnBnClickedMakeEpsilonGraph();
	afx_msg void OnBnClickedSetdimension();
};

void addEmptyLine();