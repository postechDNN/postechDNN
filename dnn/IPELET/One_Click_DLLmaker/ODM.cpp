#include <Windows.h>
#include <tchar.h>
#include <thread>
#include <string>

int main() {
	SHELLEXECUTEINFO shell[4];
	size_t cn;

	// DLL ���� �̸� �����ؼ� ���
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
	
	// Makefile �̿��ϴ� ��� �ּ� �ڵ� ��� 
	// shell[0].lpParameters = _T("/c make 2> log.txt"); 
	shell[0].lpParameters = _T("/c g++ -c -O2 -DWIN32 -fno-exceptions -fno-rtti -Iinclude *.cpp 2> log.txt");
	
	// DLL ���� ����� �ڵ�
	std::string ss = "/c g++ -shared -o " + dllName + " *.o -L\"..\\bin\" -lipe";
	const char* str1 = ss.c_str();
	wchar_t* wstr1 = new wchar_t[strlen(str1)];
	mbstowcs_s(&cn, wstr1, strlen(str1) + 1, str1, strlen(str1) + 1);
	shell[1].lpParameters = wstr1;
	
	// DLL ���� �ű�� �ڵ�
	ss = "/c move " + dllName + " ..\\ipelets";
	const char* str2 = ss.c_str();
	wchar_t* wstr2 = new wchar_t[strlen(str2)];
	mbstowcs_s(&cn, wstr2, strlen(str2) + 1, str2, strlen(str2) + 1);
	shell[2].lpParameters = wstr2;
	
	// IPE �ڵ� ���� �ڵ�
	shell[3].lpFile = _T("explorer.exe");
	shell[3].lpParameters = _T("..\\bin\\ipe.exe");

	for (int i = 0; i < 3; i++) {
		DWORD exe = ::ShellExecuteEx(&shell[i]);
		if (shell[i].hProcess != NULL) {
			::WaitForSingleObject(shell[i].hProcess, INFINITE);
		}
	}
	
	// ������ ���� �߻��� �α� ����
	std::fstream readFile;
	readFile.open("log.txt");
	char arr[1];
	readFile.getline(arr, 1);
	if (!readFile.eof()) {
		ShellExecute(NULL, _T("open"), _T("log.txt"), NULL, NULL, SW_SHOW);
	}

	// DLL���� �̵� ���н� �޽��� �ڽ� ���
	else if (checkDLL.good()) {
		checkDLL.close();
		MessageBox(GetFocus(), _T("DLL���� �̵��� �����߽��ϴ�. IPE�� �������ּ���."), _T("Ȯ��"), MB_OK);
	}

	// ������ ������ IPE ����
	else {
		DWORD exe = ::ShellExecuteEx(&shell[3]);
		//ShellExecute(NULL, _T("runas"), _T("..\\bin\\ipe.exe"), NULL, NULL, SW_SHOW);
	}

	return 0;
}