#include "helperOpen.h"

bool helperOpen() {
	ShellExecute(NULL, _T("open"), _T("helper.txt"), NULL, NULL, SW_SHOW);
	return true;
}