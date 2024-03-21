
* Windows에서 컴파일 방법

0. ipe*.h 파일들이 mingw에 들어있어야함. (즉 mingw깔아야함)
1. 다음 명령어로 *.o파일 생성
 > g++ -c -O2 -DWIN32 -fno-exceptions -fno-rtti -Iinclude "파일명".cpp
2. 생성된 *.o파일을 이용하여 라이브러리(*.dll) 생성
 > g++ -shared -o "파일명".dll "파일명".o -L"ipe경로" -lipe
3. 생성된 *.dll파일과 *.lua 파일을 ipelets폴더로 옮겨준다.


* 기타 class 설명은 ipelib사이트 참조
http://ipe.otfried.org/manual/index.html
