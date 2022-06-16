// 헤더 파일을 중복 포함되지 않게 하기 위한 Guard.
#ifndef _CONSTANT_H
#define _CONSTANT_H

/*  ======================================
	Boolean 이라는 열거형 상수를 정의합니다.
	False는 0, True는 1로 지정합니다.
	====================================== */
enum Boolean{
	False, True
};

/*  ======================================
	Direction 이라는 열거형 상수를 정의합니다.
	이는, 방향을 의미합니다. UP은 0, RIGHT는 1
	DOWN는 2, LEFT는 3으로 정의 됩니다.
	====================================== */
enum Direction{
	UP, RIGHT, DOWN, LEFT
};

/*	======================================
	ProcessType 이라는 열거형 상수를 정의합니다.
	이는, 게임 진행 종류를 뜻하는데,
	DIRECTION(=0)은 사용자가 방향키를
	눌렀을 경우를 뜻하고,
	DIRECT_DOWN(=1)은 사용자가 스페이스바를
	눌러서 블럭을 바로 밑으로 내려야 하는 경우를 뜻하고,
	AUTO(=2)는 지정해 놓은 속도에 따라 블럭이 자동으로
	밑으로 한칸 내려오는 경우를 뜻합니다.
	====================================== */
enum ProcessType{
	DIRECTION, DIRECT_DOWN, AUTO
};

/*  ======================================
	KeyCode 이라는 열거형 상수를 정의합니다.
	이것은 사용자로부터 입력받은 키보드 값들을
	정수 값으로 표현합니다.
	72,75 등등은 아스키 코드값을 의미합니다.
	UP_KEY_CODE는 화살표 위 방향키
	LEFT_KEY_CODE는 화살표 왼쪽 방향,
	RIGHT_KEY_CODE는 화살표 오른쪽 방향키
	DOWN_KEY CODE는 화살표 아래쪽 방향키
	SPACE_BAR_CODE는 스페이스바 키 값을 나타냅니다.
	====================================== */
enum KeyCode{
	UP_KEY_CODE = 72, LEFT_KEY_CODE = 75, 
	RIGHT_KEY_CODE = 77, DOWN_KEY_CODE = 80, 
	SPACE_BAR_KEY_CODE = 32
};

/*  ======================================
	BoardType 이라는 열거형 상수를 지정합니다.
	이는, 테트리스 판의 각 위치에 대한 종류를 뜻합니다.
	EMPTY는 아무것도 없는 비어있는 상태,
	MOVING_BLOCK은 움직이는 현재 블럭,
	FIXED_BLOCK은 더 이상 움직이지 못하여 굳어진 블럭,
	LEFT_WALL,RIGHT_WALL,BOTTOM_WALL,TOP_WALL은
	각각 왼쪽,오른쪽,아래쪽,위쪽 벽을 의미합니다.
	이와 마찬가지로 EDGE 부분은
	각 왼쪽 위, 오른쪽 위, 왼쪽 아래, 오른쪽 아래 모서리를
	의미합니다.
	====================================== */
enum BoardType{
	EMPTY,
	MOVING_BLOCK, FIXED_BLOCK,
	LEFT_WALL, RIGHT_WALL, BOTTOM_WALL, TOP_WALL,
	LEFT_TOP_EDGE, RIGHT_TOP_EDGE, LEFT_BOTTOM_EDGE, RIGHT_BOTTOM_EDGE
};

// 테트리스에서 사용되는 속도 레벨의 최대 레벨과
// 최소 레벨을 매크로 상수로 지정합니다.
#define MAX_SPEED_LEVEL 10
#define MIN_SPEED_LEVEL 1

#endif