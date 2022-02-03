# One click Ipelet dll maker

## Requirements

1. Install IPE (http://ipe.otfried.org) and mingw (match 32bit or 64bit)

2. Set environment variable for mingw

3. Add all ipe*.h files in Ipe/include to include folder of mingw  
(If mingw is installed at <strong>mingw</strong>, "<strong>mingw</strong>&#92;lib&#92;gcc&#92;<strong>Package</strong>-mingw32&#92;<strong>Version</strong>&#92;include" is possible folder)


## Customize

1. Make exe file by 'build' in 'release' (VS, match 32bit or 64bit)
2. Move exe file into target directory (Make directory in Ipe directory)
3. If you want to use "Make". using "shell[0].lpParameters = _T("/c make");"


## File I/O
Since administrator authority, using "ShellExecute(NULL, _T("runas"), _T("..\\bin\\ipe.exe"), NULL, NULL, SW_SHOW);"
instead of "DWORD exe = ::ShellExecuteEx(&shell[3]);" in last else statement
