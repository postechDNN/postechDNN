
// DNNDemo.h: PROJECT_NAME 애플리케이션에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CDNNDemoApp:
// 이 클래스의 구현에 대해서는 DNNDemo.cpp을(를) 참조하세요.
//

class CDNNDemoApp : public CWinApp
{
public:
	CDNNDemoApp();

	// 재정의입니다.
public:
	virtual BOOL InitInstance();

	// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CDNNDemoApp theApp;
