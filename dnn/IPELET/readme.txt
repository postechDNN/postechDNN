
* Windows���� ������ ���

0. ipe*.h ���ϵ��� mingw�� ����־����. (�� mingw��ƾ���)
1. ���� ��ɾ�� *.o���� ����
 > g++ -c -O2 -DWIN32 -fno-exceptions -fno-rtti -Iinclude "���ϸ�".cpp
2. ������ *.o������ �̿��Ͽ� ���̺귯��(*.dll) ����
 > g++ -shared -o "���ϸ�".dll "���ϸ�".o -L"ipe���" -lipe
3. ������ *.dll���ϰ� *.lua ������ ipelets������ �Ű��ش�.


* ��Ÿ class ������ ipelib����Ʈ ����
http://ipe.otfried.org/manual/index.html
