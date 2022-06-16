// TetrisManager.c에서 필요한 헤더파일들을 로드 합니다.
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "TetrisManager.h"
#include "Util.h"
#include "Constant.h"

// 초기 테트리스의 시작 속도를 매크로 상수(define)로 정의합니다.
#define INITIAL_SPEED 500

// 속도 레벨이 하나씩 증가할수록 늘어날 offset을 매크로 상수(define)로 정의합니다.
#define SPEED_LEVEL_OFFSET 100

/*	라인이 3줄이 지워지게 되면, 속도 레벨이 증가하기 위한 매크로 상수(define)를 정의 합니다.
	(테트리스 플레이시 3줄이 지워지게 되면, 속도 레벨 +1) */
#define LEVELP_UP_CONDITION 3

//	게임의 현재 상태를 출력할 위치의 좌표값을 매크로 상수(define)로 정의합니다.
#define STATUS_POSITION_X_TO_PRINT 40
#define STATUS_POSITION_Y_TO_PRINT 6

// TetrisManager.c 내부 소스파일에서만 사용될 함수들의 선언(static)입니다.
static void _TetrisManager_ClearBoard(TetrisManager* tetrisManager);
static void _TetrisManager_ChangeBoardByStatus(TetrisManager* tetrisManager, int status);
static void _TetrisManager_UpSpeedLevel(TetrisManager* tetrisManager);
static void _TetrisManager_SearchLineIndexesToDelete(TetrisManager* tetrisManager, int* indexes, int* count);
static void _TetrisManager_DeleteLines(TetrisManager* tetrisManager, int* indexes, int count);

/* =============================================================
	TetrisManager_Init() 함수 부분입니다.
	매개변수로 tetrisManager 와 speedLevel(사용자가 입력한 속도 레벨)이
	전달 됩니다. 이 함수의 기능은 Tetris가 시작될 시 불려질 함수이며
	tetrisManager의 멤버 변수들을 초기화 해주는 기능을 합니다.
	============================================================= */
void TetrisManager_Init(TetrisManager* tetrisManager, int speedLevel){
	Block block; // 구조체 Block으로 block 생성
	block.current = -1; /*	block.current 값을 - 1로 초기화 합니다.
							그 이유는 , 아직 현재 블럭을 설정을 안한 상태이기 때문입니다. */
	
		/*	memset 함수란 ?
		** 메모리의 값을 원하는 크기만큼 특정 값으로 세팅할 수 있는 함수입니다.
		   memset 함수를 호출하여, tetrisManager의 board를 0으로 초기화 합니다. **
		여기서 "->" 가 무엇인지 헷갈릴수도 있는데, 
		구조체 포인터가 멤버를 접근한다는 의미 입니다. 
		즉, tetrisManager -> board는, tetrisManager(구조체 포인터)가 board(멤버)를
		가리킨다는 의미라고 볼 수 있습니다. */
	memset(tetrisManager->board, 0, sizeof(char)* BOARD_ROW_SIZE * BOARD_COL_SIZE);

	/*	_TetrisManager_ClearBoard() 함수를 호출해, tetrisManager의
	board를 초기 상태를 설정합니다. (외부 벽과 같은) */
	_TetrisManager_ClearBoard(tetrisManager);

	// block_Make()함수를 호출해, 블럭을 생성해 tetrisManager -> block에 저장해줍니다.
	tetrisManager->block = Block_Make(True, block);

	/*	tetrisManager->deletedLineCount 를 0으로,
		speedLevel를 사용자로부터 입력받은 speedLevel 로 초기화 해줍니다. */
	tetrisManager->deletedLineCount = 0;
	tetrisManager->speedLevel = speedLevel;
}

/*	=============================================================
	TetrisManager_CheckValidPosition() 함수의 부분 입니다.
	매개변수로 구조체 포인터로 tetrisManager와, direction(방향)을 전달 받습니다.
	이 함수는 전달받은 방향으로 block을 임시로 이동 || 회전 시켜,
	그 블럭 상태가 유효한 여부를 EMPTY 값으로 반환 해줍니다.
	=============================================================	*/
int TetrisManager_CheckValidPosition(TetrisManager* tetrisManager, int direction){
	
	/*	tetrisManager->block 과 direction을 매개변수로 넘기며,
		Block_Move()를 호출하여 얻은 block 값을 구조체 변수 temp에 임시 저장합니다. */
	Block temp = Block_Move(tetrisManager->block, direction);
	int i;

	/*	temp의 4개의 배열 index에 x,y 좌표값을 각각 얻어온 후,
		그 좌표에 해당하는 tetrisManager -> board 가 EMPTY가 아니고,
		MOVING_BLOCK (현재 움직이는 블럭)도 아닐 경우, 그 상태를 그대로 반환(유효하지 않음)하고
		모두 이 두가지인 경우에만 해당되면, EMPTY를 반환해 줍니다.(유효하다는 의미)*/
	for (i = 0; i < POSITIONS_SIZE; i++){
		int x = Block_GetPositions(temp)[i].x;
		int y = Block_GetPositions(temp)[i].y;
		if (tetrisManager->board[x][y] != EMPTY && tetrisManager->board[x][y] != MOVING_BLOCK){
			return tetrisManager->board[x][y];
		}
	}
	return EMPTY;
}

/*	=============================================================
	TetrisManager_ChangeBoardByDirection()함수의 부분 입니다.
	매개변수인 구조체 포인터 변수 tetrisManager와 int형 변수 direction(방향)을 전달받고,
	이 함수의 역할은 방향에 해당하는 처리에 대해서 블럭을 이동 또는 회전 시키고
	그에 따른 테트리스의 판의 상태 또한 변경시키는 작업을 해줍니다.
	=============================================================	*/
void TetrisManager_ChangeBoardByDirection(TetrisManager* tetrisManager, int direction){
	/*	회전시에 벽에 부딪혀 유효치 않은 상태가 되었을 때도
		한 칸 밀려 나오면서 회전은 가능해야 하기 때문에,
		이를 위한 임시 유효상태 체크 결과 temp변수를 선언해줍니다. */
	int tempDirection = DOWN;
	int tempCheckResult = EMPTY;

	/*	작업을 하기 전, 먼저 _TetrisManager_ClearBoard() 함수를 호출해,
	테트리스의 판의 상태를 정리해 줍시다. */
	_TetrisManager_ClearBoard(tetrisManager);
	

	/*	TetrisManager() 함수를 호출해, 해당 direction에 해당하는
		블럭의 다음 위치가 유효한지를 구하기 위해 int형 변수 checkResult에 저장해 줍니다. */
	int checkResult = TetrisManager_CheckValidPosition(tetrisManager, direction);
	
	/*	if 문을 이용해 , 만약 그 다음 위치가 유효(비어있다)하다면, Block_Move() 함수를 호출하여
		실제로 블럭을 direction으로 이동 혹은 회전합니다. */
	if (checkResult == EMPTY){
		tetrisManager->block = Block_Move(tetrisManager->block, direction);
	}

	/*	checkResult가 유효하지 않을 경우의 처리 부분입니다.
		회전을 제외한 왼쪽,오른쪽,아래쪽으로의 이동은 움직이면 안되나,
		회전의 경우에는 벽을 만나더라도 회전을 하며 한 칸 밀려나오게 출력 해줘야 합니다.

		따라서 checkResult의 결과값에 따라,
		TOP_WALL 일 경우 tempDirection = DOWN,
		RIGHT_WALL 일 경우 tempDirection = LEFT,
		LEFT_WALL 일 경우 tempDirection = RIGHT 로 지정하고

		Block_Move() 함수에 tempDirection을 넘기며
		block을 벽의 반대 방향으로 밀려 나오게 해줍니다.
		그 다음 실제로 direction 값을 넘기며 Block_Move()를 호출해 블럭을 회전 시켜줍니다.*/
	else{
		if (direction == UP){
			switch (checkResult){
			case TOP_WALL:
				tempDirection = DOWN;
				tempCheckResult = TOP_WALL;
				break;
			case RIGHT_WALL:
				tempDirection = LEFT;
				tempCheckResult = RIGHT_WALL;
				break;
			case LEFT_WALL:
				tempDirection = RIGHT;
				tempCheckResult = LEFT_WALL;
				break;
			}
			do{
				tetrisManager->block = Block_Move(tetrisManager->block, tempDirection);
			} while (TetrisManager_CheckValidPosition(tetrisManager, direction) == tempCheckResult);
			tetrisManager->block = Block_Move(tetrisManager->block, direction);
		}

		/*	checkResult 값이 유효하지 않은 경우이지만, 회전이 아닌 경우의 처리입니다.
		방향은 오른쪽인데 오른쪽 벽이나 이미 설치된 블럭이 막고 있을 경우,
		또는 왼쪽인데 왼쪽 벽이나 이미 설치된 블럭이 왼쪽에서 막고 있을 경우
		해당 방향으로 이동은 못하게 하나,
		밑으로 일정 시간 지난 후에 자동으로 내려와줘야 하기 때문에,

		TetrisManager_Sleep() 함수를 호출해, 지연시켜준 후
		Block_Move() 함수를 호출해 DOWN으로 방향을 넘겨서 블럭을 자동으로
		한칸 내려줍니다.
			
		간단히 표현하면 이동 못하게 되었을 때 가만히 그 위치에 머물러 있지 않도록
		방지하기 위한 기능입니다. */
		else{
			if (direction == RIGHT && checkResult == RIGHT_WALL ||
				direction == LEFT && checkResult == LEFT_WALL || 
				direction == RIGHT && checkResult == FIXED_BLOCK ||
				direction == LEFT && checkResult == FIXED_BLOCK){
				TetrisManager_Sleep(tetrisManager);
				tetrisManager->block = Block_Move(tetrisManager->block, DOWN);
			}
		}
	}
	/* _TetrisManager_ChangeBoardByStatus() 함수를 호출하고
		MOVIG_BLOCK을 넘겨서 tetrisManager의 board에서 새로 바뀐 블럭의 위치를
		재설정 해줍니다. */
	_TetrisManager_ChangeBoardByStatus(tetrisManager, MOVING_BLOCK);
}

/*	=============================================================
	TetrisManager_ChangeBoardByAuto() 함수의 부분입니다.
	이 함수의 역할은 매개변수로 tetrisManager를 전달받고,
	자동으로 블럭을 한 칸 내려가게 하는 역할을 합니다.
	============================================================= */
void TetrisManager_ChangeBoardByAuto(TetrisManager* tetrisManager){
	
	/* TetrisManager_ChangeBoardByDirection() 함수에
	DOWN 매개변수를 넘겨 한 칸 밑으로 블럭을 내려가게 합니다. */
	TetrisManager_ChangeBoardByDirection(tetrisManager, DOWN);
}

/*	=============================================================
	TetrisManager_ProcessDirectDown() 함수 부분입니다.
	매개변수로 구조체 포인터 변수인 tetrisManager를 받아,
	스페이스바를 눌렀을 경우 블럭을 한번에 내리는 기능을 합니다.
	=============================================================	*/
void TetrisManager_ProcessDirectDown(TetrisManager* tetrisManager){

	/*	TetrisManager_IsReachedToBottom()함수를 불러 블럭이 밑으로 이동할 수 있는지를
		체크 한 후 이동 가능하면, 계속해서 내려가고 ChangeBoardByDirection() 함수를
		호출하여 매개변수로 DOWN을 넘겨준 후 밑으로 블럭을 한칸 씩 내려줍니다.
		반복문을 활용해 블럭을 내릴 수 있을 때까지 내려주는 기능인 것 입니다. */
	while (!TetrisManager_IsReachedToBottom(tetrisManager)){
		TetrisManager_ChangeBoardByDirection(tetrisManager, DOWN);
	}
}

/*	=====================================================
	TetrisManager_ProcessDeletingLines() 함수 부분입니다.
	매개변수로 tetrisManager를 받은 후 삭제할 라인을 찾고 삭제 해준 뒤
	테트리스 판을 새롭게 갱신해주는 역할을 해줍니다 .
	===================================================== */
void TetrisManager_ProcessDeletingLines(TetrisManager* tetrisManager){

	// 삭제할 라인의 위치를 저장할 배열 및 개수를 선언해줍니다.
	int indexes[BOARD_ROW_SIZE];
	int count;
	int i;

	/*	함수를 호출하여, 삭제할 라인의 위치를 찾아 변수 배열에 저장하고
		count에 그 라인의 총 개수를 저장해줍니다. */
	_TetrisManager_SearchLineIndexesToDelete(tetrisManager, indexes, &count);

	/*	만약 삭제 가능한 줄이 있다면, DeleteLines()함수를 호출하여
	해당하는 위치의 라인들을 삭제해줍니다. 또한 삭제한 줄의 개수가 3의 배수일 경우
	UpSpeedLevel() 함수를 호출하여 게임의 속도 레벨을 1 증가 시켜줍니다. */
	if (count > 0){
		_TetrisManager_DeleteLines(tetrisManager, indexes, count);
		for (i = tetrisManager->speedLevel; i <= tetrisManager->deletedLineCount / LEVELP_UP_CONDITION; i++){
			_TetrisManager_UpSpeedLevel(tetrisManager);
		}
	}
}

/*	=============================================================
	TetrisManager_IsReachedToBottom() 함수 부분입니다.
	이 함수의 기능은, 현재 이동중인 블럭이 밑으로 이동할 수 있는지 체크하여
	그 결과를 True OR False 로 반환해주는 기능을 합니다.
	=============================================================	*/
int TetrisManager_IsReachedToBottom(TetrisManager* tetrisManager){
	int i;
	/*	for문을 통해 현재 이동중인 block의 2차원 배열( 4개의 상자 )의 좌표를 받아
	그 좌표의 바로 밑이 EMPTY가 아니고 MOVING_BLOCK이 아닌, 즉 이 두개의 조건이
	참일 경우 True(=1)의 값을 반환해 줍니다.*/
	for (i = 0; i < POSITIONS_SIZE; i++){
		int x = Block_GetPositions(tetrisManager->block)[i].x;
		int y = Block_GetPositions(tetrisManager->block)[i].y;
		if (tetrisManager->board[x + 1][y] != EMPTY && tetrisManager->board[x + 1][y] != MOVING_BLOCK){
			return True;
		}
	}
	return False; // 아닐 경우 False(=0)를 반환해줍니다. 
}

/*	=============================================================
	TetrisManager_ProcessReacehdCase() 함수의 부분입니다.
	이 함수의 기능은, 블럭이 밑에 닿았을 경우(벽이나 블럭과의 충돌)
	현재 이동중인 블럭을 고정시키고, 새 블럭을 호출하는 기능을 해줍니다.
	============================================================= */
int TetrisManager_ProcessReachedCase(TetrisManager* tetrisManager){

	/*	먼저 ChageBoardByStatus() 함수를 호출하여 FIXED_BLOCK 매개변수를 넘겨
		현재 블럭에 해당 하는 위치를 테트리스판에서 FIXED_BLOCK으로 변경해줍니다. */
	_TetrisManager_ChangeBoardByStatus(tetrisManager, FIXED_BLOCK);

	/*	Block_Make()함수를 호출하여 새 블럭을 생성해줍니다.
		(매개 변수로 False를 넘긴 이유는 블럭이 처음 불리는 것이 아닌
		2번째 이후 블럭부터 불리는 것이라는 것을 구분해주기 위한 것 입니다.) */
	tetrisManager->block = Block_Make(False, tetrisManager->block);

	/*	IsReachedToBottom()함수를 호출해 생성한 블럭이 밑으로 갈 수 있는지에 대한 여부를
		체크해 줍니다. 갈 수 없을 경우 END(게임 종료) 값을 반환하고
		갈 수 있을 경우엔 PLAYING(게임 진행) 값을 반환해줍니다. */
	if (TetrisManager_IsReachedToBottom(tetrisManager)){
		return END;
	}
	else{
		return PLAYING;
	}
}

/*	=============================================================
	TetrisManager_Sleep() 함수 부분입니다.
	이 함수의 기능은 매개변수로 tetrisManager를 전달 받고
	속도 레벨에 따라 프로그램의 지연속도를 느리게 해주는 역할을 해줍니다.
	============================================================= */
void TetrisManager_Sleep(TetrisManager* tetrisManager){
	int i;
	/*	지역변수 milliSecond에 INITIAL_SPEED(=500)을 넣어줍니다.
		여기서 자료형 DWORD란, unsigned long의 자료형을 typedef로 DWORD 라고 선언한
		자료형인데, 4바이트의 자료형이라고 볼 수 있습니다.

		DWORD는 WORD 자료형에서 파생된 것인데,
		WORD라 함은 CPU가 한번에 처리 할 수 있는 양을 뜻 합니다.
		WORD = 16비트고 DWORD = 32비트 즉 4바이트의 자료형이라고 볼 수 있습니다. */
	DWORD milliSecond = INITIAL_SPEED;
	/*	For문을 통해 최소 속도 레벨부터 현재 속도 레벨까지 돌고,
		if문으로 분기를 만들고 최대 속도 레벨 / 2 까지는 millisSecond에서
		SPEED_LEVEL_OFFSET(=500)만큼 빼주고 , 그 이후엔
		milliSecond 에서 (SPEED_LEVEL_OFFSET / 5)의 값만큼 빼줍니다. */
	for (i = MIN_SPEED_LEVEL; i < tetrisManager->speedLevel; i++){
		if (i < MAX_SPEED_LEVEL / 2){
			milliSecond -= SPEED_LEVEL_OFFSET;
		}
		else{
			milliSecond -= (SPEED_LEVEL_OFFSET / 5);
		}
	}
	/*	그 후 Sleep() 함수를 호출해 매개변수로
		milliSecond를 받아주고 블럭이 내려오는 속도를 줄여줍니다. */
	Sleep(milliSecond);
}

/*=============================================================
TetrisManager_Print() 함수 부분입니다.
이 함수의 역할은 테트리스 판의 출력 및 게임 상태를 출력하는 역할을 해줍니다.
=============================================================== */
void TetrisManager_Print(TetrisManager* tetrisManager){
	int i;
	int j;
	
	/*	CursorUtil_GotoXY() 함수를 불러온 후, 커서를 (0,0)좌표 위치로 이동 시킨 후
		반복문으로 tetrisManager -> board 배열을 반복하며
		boardType에 따라 해당하는 문자열들을 출력해줍니다. */
	CursorUtil_GotoXY(0, 0);
	for (i = 0; i < BOARD_ROW_SIZE; i++){
		for (j = 0; j < BOARD_COL_SIZE; j++){
			switch (tetrisManager->board[i][j]){
			case LEFT_TOP_EDGE:
				printf("┎");
				break;
			case RIGHT_TOP_EDGE:
				printf("┒");
				break;
			case LEFT_BOTTOM_EDGE:
				printf("┖");
				break;
			case RIGHT_BOTTOM_EDGE:
				printf("┚");
				break;
			case EMPTY:
				printf("  ");
				break;
			case MOVING_BLOCK:
				printf("■");
				break;
			case FIXED_BLOCK:
				printf("▧");
				break;
			case LEFT_WALL:
			case RIGHT_WALL:
				printf("｜");
				break;
			case TOP_WALL:
			case BOTTOM_WALL:
				printf("―");
				break;
			}
		}
		printf("\n");
	}

	/*	테트리스의 현재 속도레벨과 지금까지 삭제한 총 줄 개수를 출력하고
		Block_PrintNext() 함수를 호출하여 다음에 나올 블럭을 출력해준 뒤
		CursorUtil_Hide() 함수를 불러와 커서를 숨겨줍니다. */
	CursorUtil_GotoXY(STATUS_POSITION_X_TO_PRINT, STATUS_POSITION_Y_TO_PRINT);
	printf("***** Tetris *****\n");
	CursorUtil_GotoXY(STATUS_POSITION_X_TO_PRINT, STATUS_POSITION_Y_TO_PRINT + 1);
	printf("Current speed level : %d level\n", tetrisManager->speedLevel);
	CursorUtil_GotoXY(STATUS_POSITION_X_TO_PRINT, STATUS_POSITION_Y_TO_PRINT + 2);
	printf("Deleted lines : %d lines", tetrisManager->deletedLineCount);
	Block_PrintNext(tetrisManager->block, STATUS_POSITION_X_TO_PRINT, STATUS_POSITION_Y_TO_PRINT + 4);
	CursorUtil_Hide();
}

/*	=============================================================
	TetrisManager_ClearBoard()함수 부분입니다.
	이 함수의 역할은, 매개변수로 tetrisManager를 전달 받고
	테트리스 판의 초기화 역할을 해줍니다.
	============================================================= */
static void _TetrisManager_ClearBoard(TetrisManager* tetrisManager){
	int i;
	int j;

	// For문으로 테트리스 판의 왼쪽벽과 오른쪽 벽을 재설정해줍니다.
	for (i = 0; i < BOARD_ROW_SIZE; i++){
		tetrisManager->board[i][0] = LEFT_WALL;
		tetrisManager->board[i][BOARD_COL_SIZE - 1] = RIGHT_WALL;
	}
	// For문으로 테트리스의 윗 벽과 아래 벽을 재설정해줍니다.
	for (i = 0; i < BOARD_COL_SIZE; i++){
		tetrisManager->board[0][i] = TOP_WALL;
		tetrisManager->board[BOARD_ROW_SIZE - 1][i] = BOTTOM_WALL;
	}
	// For문으로 테트리스 판의 안쪽에서 굳어진 블럭을 제외한 공간을 빈 공간으로 설정합니다.
	for (i = 1; i < BOARD_ROW_SIZE - 1; i++){
		for (j = 1; j < BOARD_COL_SIZE - 1; j++){
			if (tetrisManager->board[i][j] != FIXED_BLOCK){
				tetrisManager->board[i][j] = EMPTY;
			}
		}
	}

	// 테트리스판의 모서리의 대한 부분을 재설정 해줍니다.
	tetrisManager->board[0][0] = LEFT_TOP_EDGE;
	tetrisManager->board[0][BOARD_COL_SIZE - 1] = RIGHT_TOP_EDGE;
	tetrisManager->board[BOARD_ROW_SIZE - 1][0] = LEFT_BOTTOM_EDGE;
	tetrisManager->board[BOARD_ROW_SIZE - 1][BOARD_COL_SIZE - 1] = RIGHT_BOTTOM_EDGE;
}

/*	=============================================================
	TetrisManager_ChangeBoardByStatus()함수 부분입니다.
	매개변수인 tetrisManager(구조체 변수), status(int형 변수)를 전달받고
	해당 status로 블럭의 위치를 테트리스판에서 변경하는 역할을 해줍니다.
	매개변수 status는 테트리스 판에서 블럭의 위치에 해당하는 좌표를 변경할 상태를 뜻합니다.
	============================================================= */
static void _TetrisManager_ChangeBoardByStatus(TetrisManager* tetrisManager, int status){
	int i;
	// For문으로 block의 4개 좌표들을 돌며 입력받은 status로 변경합니다.
	for (i = 0; i < POSITIONS_SIZE; i++){
		int x = Block_GetPositions(tetrisManager->block)[i].x;
		int y = Block_GetPositions(tetrisManager->block)[i].y;
		tetrisManager->board[x][y] = status;
	}
}

/*	=============================================================
	TetrisManager_UpSpeedLevel() 함수 부분입니다.
	매개변수로 tetrisManager를 받고, 속도레벨을 1 증가 시킵니다.
	============================================================= */
static void _TetrisManager_UpSpeedLevel(TetrisManager* tetrisManager){
	// 현재 속도레벨이 최대 속로 레벨(=10)보다 작은 경우에만 1 증가 시킵니다.
	if (tetrisManager->speedLevel < MAX_SPEED_LEVEL){
		tetrisManager->speedLevel++;
	}
}

/*	=============================================================
	TetrisManager_SearchLineIndexesToDelete() 함수 부분입니다.
	매개 변수를 총 3개 전달 받는데, tetrisManager와
	indexes( 삭제할 라인의 위치들을 담을 배열), count(삭제할 라인 개수)
	를 전달받아서 현재 테트리스 판에서 삭제할 라인의 위치를
	찾는 역할을 합니다.
	============================================================= */
static void _TetrisManager_SearchLineIndexesToDelete(TetrisManager* tetrisManager, int* indexes, int* count){
	int i;
	int j;
	int toDelete;
	// indexes 배열에 memset()함수를 통하여 -1로 전부 초기화 해줍니다.
	memset(indexes, -1, sizeof(int)* (BOARD_ROW_SIZE - 2));
	*count = 0; // 그 값들을 count에 저장 합니다.

	/*	테트리스 판을 전부 돌면서 가로(BOARD_ROW_SIZE)한 줄이
	모두 FIXED 블록인 경우를 찾습니다. 그 후 해당 위치를
	indexes에 저장해주고, 그 때 마다 count의 값을 1씩 증가합니다.*/
	for (i = 1; i < BOARD_ROW_SIZE - 1; i++){
		toDelete = True;
		for (j = 1; j < BOARD_COL_SIZE - 1; j++){
			if (tetrisManager->board[i][j] != FIXED_BLOCK){
				toDelete = False;
				break;
			}
		}
		if (toDelete){
			indexes[(*count)++] = i;
		}
	}
}

/*	=============================================================
	TetrisManager_DeleteLines() 함수의 정의부 입니다.
	매개변수로 tetrisManager와 indexes, count를 받아서
	삭제할 라인을 직접적으로 지워주는 역할을 해주는 함수입니다.
	============================================================= */
static void _TetrisManager_DeleteLines(TetrisManager* tetrisManager, int* indexes, int count){
	int i;
	int j;
	int k = BOARD_ROW_SIZE - 2;
	int toDelete;
	// 라인 삭제를 위해 임시변수를 만들어, 
	// 임시로 삭제된 테트리스판을 저장해줍니다.
	char temp[BOARD_ROW_SIZE][BOARD_COL_SIZE] = { EMPTY };
	
	/*	For문으로 테트리스판을 돌면서 indexes에 해당하는 줄을
		제외한(if문) 모든 줄을 임시변수에 넣어줍니다. */
	for (i = BOARD_ROW_SIZE - 2; i > 0; i--){
		toDelete = False;
		for (j = 0; j < BOARD_COL_SIZE; j++){
			if (i == indexes[j]){
				toDelete = True;
				break;
			}
		}
		if (!toDelete){
			for (j = 0; j < BOARD_COL_SIZE; j++){
				temp[k][j] = tetrisManager->board[i][j];
			}
			k--;
		}
	}
	/*	임시변수(temp)에 있던 기존값들을 다시 기존판에 넣어줍니다. */
	for (i = 1; i < BOARD_ROW_SIZE - 1; i++){
		for (j = 1; j < BOARD_COL_SIZE - 1; j++){
			tetrisManager->board[i][j] = temp[i][j];
		}
	}
	// tetrinsManager 의 deletedLineCount 값을, count의 값만큼 증가시킵니다.
	tetrisManager->deletedLineCount += count;
}