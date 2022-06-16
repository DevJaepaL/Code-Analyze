// 게임 메인 코드 부분 //
#include <stdio.h>
#include <conio.h>
#include "Constant.h"
#include "TetrisView.h"

int main(int argc, char* argv[]){
	TetrisView tetrisView; // tetrisView 변수 선언(구조체 TetrisView를 통해)
	TetrisView_StartGame(&tetrisView); // 함수 호출로 게임 시작.
	int processType; // DIRECTION || DIRECT_DOWN || AUTO 를 저장할 변수 선언
	int direction; // 방향을 저장할 변수 선언
	while (True){ // 특정 조건을 만족하지 않는 이상 무한반복 하도록 while문 사용.
		processType = AUTO; // 입력값이 없으면 자동적으로 블럭이 자동적으로 움직이기.
		direction = DOWN; //			''''			아래 방향으로 내려가게 하기.
		if (_kbhit()){ // _khbit() 함수 호출하여 사용자의 키 입력에 버퍼가 있는지 체크.
			int key = _getch(); // 함수를 key라는 변수에 넣기.
			if (_kbhit()){ // 만약 키 입력이 발견되면
				key = _getch(); // _getch() 함수 호출하여 사용자의 키값을 불러옵니다.
				switch (key){ // 분기를 나누고 키값에 따라 방향이 바뀌도록 설정합니다.

				/*	방향키는 입력 버퍼에 2개의 값이 연속으로 들어간다고 합니다.
					첫번째는 0xE0(=224), 그리고 방향키에 대한 정수 값,
					두가지 값이 들어가기 때문에 두 개의 입력버퍼로부터
					값을 가져와서 확인해야합니다. 즉슨,
					17번줄에서 _kbhit() 함수의 호출 뒤, 반환 값이 0 아니면
					상 하 좌 우 방향키중 하나가 입력됬다는 말이기 때문에
					switch-case 문에서 입력값에 따른 분기마다
					processType 과 direction 값을 설정해줍니다.*/
				case UP_KEY_CODE: 
					processType = DIRECTION;
					direction = UP;
					break;
				case LEFT_KEY_CODE:
					processType = DIRECTION;
					direction = LEFT;
					break;
				case RIGHT_KEY_CODE:
					processType = DIRECTION;
					direction = RIGHT;
					break;
				case DOWN_KEY_CODE:
					processType = DIRECTION;
					direction = DOWN;
					break;
				}
			}
			else{ // 버퍼에 키 값이 하나만 있다면 스페이스바의 값인지 확인 후
				  // procesType에 DIRECT_DOWN을 저장해줍니다.
				switch (key){
				case SPACE_BAR_KEY_CODE:
					processType = DIRECT_DOWN;
				}
			}
		}
		TetrisView_ProcessGame(&tetrisView, processType, direction);
		/*	사용자로부터 입력 받은 키의 분류 처리가 끝났을 경우
			TetrisView_ProcessGame()함수를 호출해 테트리스 게임을 진행시켜줍니다.
			매개변수인 tetrisView,processType,direction을 넘겨줍니다. */
	}
	return 0; // 프로그램 종료
}