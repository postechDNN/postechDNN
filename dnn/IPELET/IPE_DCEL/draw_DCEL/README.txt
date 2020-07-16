DRAW DCEL FOR IPELET 

1. DCEL 폴더의 상대적 위치를 가지고 include 하기 때문에, github의 DNN 폴더를 그대로 이용하여야 한다.

2. draw_DCEL 폴더의 Makefile 파일을 텍스트로 열고 해당부분에 ipe.exe의 위치를 넣는다.
 $(CC) -shared -o $(TARGET) $(OBJECTS) -L"ipe.exe 위치" -lipe
예시) $(CC) -shared -o $(TARGET) $(OBJECTS) -LC:/ipe-7.2.17/bin -lipe

3. draw_DCEL 폴더에서 cmd 창을 열고 "make" 명령어 입력

4. 생성된 draw_DCEL.dll 과 해당폴더의 draw_DCEL.lua 파일을 ipelets 폴더에 옮겨놓는다.
예시) C:\ipe-7.2.17\ipelets 폴더에 dll 파일과 lua 파일을 옮겨놓는다.

5. ipelet을 실행 후 상단에 ipelets > draw_DCEL을 클릭.
선택창에서 DCEL이 저장된 text 파일을 고르면 그림으로 그려진다.

주의) dcel text file의 절대 주소안에 한글이 들어가면 오류 발생. 오직 영어로된 path안에 둘 것.