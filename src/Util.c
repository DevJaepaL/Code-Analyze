/*	�ҽ��ڵ忡 �ʿ��� ��������� include �մϴ�.
	�����ξ��� Util.h ����� �ҷ��ɴϴ�.*/
#include <Windows.h>
#include "Util.h"

/*	=====================================
	CursorUtil_GotoXY �Լ��� ���Ǻ��Դϴ�.
	�Ű����� int �ڷ����� x�� y�� �޾Ƽ�,
	��� Ŀ���� x,y ��ġ�� �̵� ��ŵ�ϴ�.
	===================================== */
void CursorUtil_GotoXY(int x, int y){
	COORD pos = { x, y }; // x�� y�� ������ COORD ����ü�� ����, �̸� �Ű� ������ �ѱ�ϴ�. 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	/*	�� ��, windows.h ���̺귯���� ����ִ�
		SetConsoleCursorPosition �Լ��� �ҷ��� ��� Ŀ���� �̵� ��ŵ�ϴ�. */
}

/*	=====================================
	CursorUtil_Hide  �Լ��� ���Ǻ� �Դϴ�.
	�� �Լ��� Ŀ���� ����� ������ �մϴ�.
	===================================== */
void CursorUtil_Hide(){
	CONSOLE_CURSOR_INFO cursorInfo; // CONSOLE_CURSOR_INFO ����ü�� ����.
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE; // ����� bVisible�� FALSE�� ����.
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	// �̸� SetConsoleCursorInfo �Լ��� �Ѱܼ� Ŀ���� �����ݴϴ�.
}