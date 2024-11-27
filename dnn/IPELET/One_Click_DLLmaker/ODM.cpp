#include <Windows.h>
#include <tchar.h>
#include <thread>
#include <string>

int main() {
	SHELLEXECUTEINFO shell[4];
	size_t cn;

	// DLL 파일 이름 수정해서 사용
	std::string dllName = "USERIPE.dll";

	for (int i = 0; i < 4; i++) {
		::ZeroMemory(&shell[i], sizeof(SHELLEXECUTEINFO));
		shell[i].lpFile = _T("cmd.exe");
		shell[i].cbSize = sizeof(SHELLEXECUTEINFO);
		shell[i].nShow = SW_HIDE;
		shell[i].fMask = SEE_MASK_NOCLOSEPROCESS;
		shell[i].lpVerb = _T("open");
		shell[i].lpDirectory = _T("NULL");
	}
	
	// Makefile 이용하는 경우 주석 코드 사용 
	// shell[0].lpParameters = _T("/c make 2> log.txt"); 
	shell[0].lpParameters = _T("/c g++ -c -O2 -DWIN32 -fno-exceptions -fno-rtti -Iinclude *.cpp 2> log.txt");
	
	// DLL 파일 만드는 코드
	std::string ss = "/c g++ -shared -o " + dllName + " *.o -L\"..\\bin\" -lipe";
	const char* str1 = ss.c_str();
	wchar_t* wstr1 = new wchar_t[strlen(str1)];
	mbstowcs_s(&cn, wstr1, strlen(str1) + 1, str1, strlen(str1) + 1);
	shell[1].lpParameters = wstr1;
	
	// DLL 파일 옮기는 코드
	ss = "/c move " + dllName + " ..\\ipelets";
	const char* str2 = ss.c_str();
	wchar_t* wstr2 = new wchar_t[strlen(str2)];
	mbstowcs_s(&cn, wstr2, strlen(str2) + 1, str2, strlen(str2) + 1);
	shell[2].lpParameters = wstr2;
	
	// IPE 자동 실행 코드
	shell[3].lpFile = _T("explorer.exe");
	shell[3].lpParameters = _T("..\\bin\\ipe.exe");

	for (int i = 0; i < 3; i++) {
		DWORD exe = ::ShellExecuteEx(&shell[i]);
		if (shell[i].hProcess != NULL) {
			::WaitForSingleObject(shell[i].hProcess, INFINITE);
		}
	}
	
	// 컴파일 에러 발생시 로그 실행
	std::fstream readFile;
	readFile.open("log.txt");
	char arr[1];
	readFile.getline(arr, 1);
	if (!readFile.eof()) {
		ShellExecute(NULL, _T("open"), _T("log.txt"), NULL, NULL, SW_SHOW);
	}

	// DLL파일 이동 실패시 메시지 박스 출력
	else if (checkDLL.good()) {
		checkDLL.close();
		MessageBox(GetFocus(), _T("DLL파일 이동에 실패했습니다. IPE를 종료해주세요."), _T("확인"), MB_OK);
	}

	// 컴파일 성공시 IPE 실행
	else {
		DWORD exe = ::ShellExecuteEx(&shell[3]);
		//ShellExecute(NULL, _T("runas"), _T("..\\bin\\ipe.exe"), NULL, NULL, SW_SHOW);
	}

	return 0;
}