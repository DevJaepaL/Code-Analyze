/*	소스코드에 필요한 헤더파일을 include 합니다.
	만들어두었던 Util.h 헤더를 불러옵니다.*/
#include <Windows.h>
#include "Util.h"

/*	=====================================
	CursorUtil_GotoXY 함수의 정의부입니다.
	매개변수 int 자료형인 x와 y를 받아서,
	출력 커서를 x,y 위치로 이동 시킵니다.
	===================================== */
void CursorUtil_GotoXY(int x, int y){
	COORD pos = { x, y }; // x와 y를 가지고 COORD 구조체를 생성, 이를 매개 변수로 넘깁니다. 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	/*	그 후, windows.h 라이브러리에 들어있는
		SetConsoleCursorPosition 함수를 불러와 출력 커서를 이동 시킵니다. */
}

/*	=====================================
	CursorUtil_Hide  함수의 정의부 입니다.
	이 함수는 커서를 숨기는 역할을 합니다.
	===================================== */
void CursorUtil_Hide(){
	CONSOLE_CURSOR_INFO cursorInfo; // CONSOLE_CURSOR_INFO 구조체를 선언.
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE; // 멤버인 bVisible에 FALSE를 저장.
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	// 이를 SetConsoleCursorInfo 함수에 넘겨서 커서를 숨겨줍니다.
}