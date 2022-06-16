// TetrisManager.c���� �ʿ��� ������ϵ��� �ε� �մϴ�.
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "TetrisManager.h"
#include "Util.h"
#include "Constant.h"

// �ʱ� ��Ʈ������ ���� �ӵ��� ��ũ�� ���(define)�� �����մϴ�.
#define INITIAL_SPEED 500

// �ӵ� ������ �ϳ��� �����Ҽ��� �þ offset�� ��ũ�� ���(define)�� �����մϴ�.
#define SPEED_LEVEL_OFFSET 100

/*	������ 3���� �������� �Ǹ�, �ӵ� ������ �����ϱ� ���� ��ũ�� ���(define)�� ���� �մϴ�.
	(��Ʈ���� �÷��̽� 3���� �������� �Ǹ�, �ӵ� ���� +1) */
#define LEVELP_UP_CONDITION 3

//	������ ���� ���¸� ����� ��ġ�� ��ǥ���� ��ũ�� ���(define)�� �����մϴ�.
#define STATUS_POSITION_X_TO_PRINT 40
#define STATUS_POSITION_Y_TO_PRINT 6

// TetrisManager.c ���� �ҽ����Ͽ����� ���� �Լ����� ����(static)�Դϴ�.
static void _TetrisManager_ClearBoard(TetrisManager* tetrisManager);
static void _TetrisManager_ChangeBoardByStatus(TetrisManager* tetrisManager, int status);
static void _TetrisManager_UpSpeedLevel(TetrisManager* tetrisManager);
static void _TetrisManager_SearchLineIndexesToDelete(TetrisManager* tetrisManager, int* indexes, int* count);
static void _TetrisManager_DeleteLines(TetrisManager* tetrisManager, int* indexes, int count);

/* =============================================================
	TetrisManager_Init() �Լ� �κ��Դϴ�.
	�Ű������� tetrisManager �� speedLevel(����ڰ� �Է��� �ӵ� ����)��
	���� �˴ϴ�. �� �Լ��� ����� Tetris�� ���۵� �� �ҷ��� �Լ��̸�
	tetrisManager�� ��� �������� �ʱ�ȭ ���ִ� ����� �մϴ�.
	============================================================= */
void TetrisManager_Init(TetrisManager* tetrisManager, int speedLevel){
	Block block; // ����ü Block���� block ����
	block.current = -1; /*	block.current ���� - 1�� �ʱ�ȭ �մϴ�.
							�� ������ , ���� ���� ���� ������ ���� �����̱� �����Դϴ�. */
	
		/*	memset �Լ��� ?
		** �޸��� ���� ���ϴ� ũ�⸸ŭ Ư�� ������ ������ �� �ִ� �Լ��Դϴ�.
		   memset �Լ��� ȣ���Ͽ�, tetrisManager�� board�� 0���� �ʱ�ȭ �մϴ�. **
		���⼭ "->" �� �������� �򰥸����� �ִµ�, 
		����ü �����Ͱ� ����� �����Ѵٴ� �ǹ� �Դϴ�. 
		��, tetrisManager -> board��, tetrisManager(����ü ������)�� board(���)��
		����Ų�ٴ� �ǹ̶�� �� �� �ֽ��ϴ�. */
	memset(tetrisManager->board, 0, sizeof(char)* BOARD_ROW_SIZE * BOARD_COL_SIZE);

	/*	_TetrisManager_ClearBoard() �Լ��� ȣ����, tetrisManager��
	board�� �ʱ� ���¸� �����մϴ�. (�ܺ� ���� ����) */
	_TetrisManager_ClearBoard(tetrisManager);

	// block_Make()�Լ��� ȣ����, ���� ������ tetrisManager -> block�� �������ݴϴ�.
	tetrisManager->block = Block_Make(True, block);

	/*	tetrisManager->deletedLineCount �� 0����,
		speedLevel�� ����ڷκ��� �Է¹��� speedLevel �� �ʱ�ȭ ���ݴϴ�. */
	tetrisManager->deletedLineCount = 0;
	tetrisManager->speedLevel = speedLevel;
}

/*	=============================================================
	TetrisManager_CheckValidPosition() �Լ��� �κ� �Դϴ�.
	�Ű������� ����ü �����ͷ� tetrisManager��, direction(����)�� ���� �޽��ϴ�.
	�� �Լ��� ���޹��� �������� block�� �ӽ÷� �̵� || ȸ�� ����,
	�� �� ���°� ��ȿ�� ���θ� EMPTY ������ ��ȯ ���ݴϴ�.
	=============================================================	*/
int TetrisManager_CheckValidPosition(TetrisManager* tetrisManager, int direction){
	
	/*	tetrisManager->block �� direction�� �Ű������� �ѱ��,
		Block_Move()�� ȣ���Ͽ� ���� block ���� ����ü ���� temp�� �ӽ� �����մϴ�. */
	Block temp = Block_Move(tetrisManager->block, direction);
	int i;

	/*	temp�� 4���� �迭 index�� x,y ��ǥ���� ���� ���� ��,
		�� ��ǥ�� �ش��ϴ� tetrisManager -> board �� EMPTY�� �ƴϰ�,
		MOVING_BLOCK (���� �����̴� ��)�� �ƴ� ���, �� ���¸� �״�� ��ȯ(��ȿ���� ����)�ϰ�
		��� �� �ΰ����� ��쿡�� �ش�Ǹ�, EMPTY�� ��ȯ�� �ݴϴ�.(��ȿ�ϴٴ� �ǹ�)*/
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
	TetrisManager_ChangeBoardByDirection()�Լ��� �κ� �Դϴ�.
	�Ű������� ����ü ������ ���� tetrisManager�� int�� ���� direction(����)�� ���޹ް�,
	�� �Լ��� ������ ���⿡ �ش��ϴ� ó���� ���ؼ� ���� �̵� �Ǵ� ȸ�� ��Ű��
	�׿� ���� ��Ʈ������ ���� ���� ���� �����Ű�� �۾��� ���ݴϴ�.
	=============================================================	*/
void TetrisManager_ChangeBoardByDirection(TetrisManager* tetrisManager, int direction){
	/*	ȸ���ÿ� ���� �ε��� ��ȿġ ���� ���°� �Ǿ��� ����
		�� ĭ �з� �����鼭 ȸ���� �����ؾ� �ϱ� ������,
		�̸� ���� �ӽ� ��ȿ���� üũ ��� temp������ �������ݴϴ�. */
	int tempDirection = DOWN;
	int tempCheckResult = EMPTY;

	/*	�۾��� �ϱ� ��, ���� _TetrisManager_ClearBoard() �Լ��� ȣ����,
	��Ʈ������ ���� ���¸� ������ �ݽô�. */
	_TetrisManager_ClearBoard(tetrisManager);
	

	/*	TetrisManager() �Լ��� ȣ����, �ش� direction�� �ش��ϴ�
		���� ���� ��ġ�� ��ȿ������ ���ϱ� ���� int�� ���� checkResult�� ������ �ݴϴ�. */
	int checkResult = TetrisManager_CheckValidPosition(tetrisManager, direction);
	
	/*	if ���� �̿��� , ���� �� ���� ��ġ�� ��ȿ(����ִ�)�ϴٸ�, Block_Move() �Լ��� ȣ���Ͽ�
		������ ���� direction���� �̵� Ȥ�� ȸ���մϴ�. */
	if (checkResult == EMPTY){
		tetrisManager->block = Block_Move(tetrisManager->block, direction);
	}

	/*	checkResult�� ��ȿ���� ���� ����� ó�� �κ��Դϴ�.
		ȸ���� ������ ����,������,�Ʒ��������� �̵��� �����̸� �ȵǳ�,
		ȸ���� ��쿡�� ���� �������� ȸ���� �ϸ� �� ĭ �з������� ��� ����� �մϴ�.

		���� checkResult�� ������� ����,
		TOP_WALL �� ��� tempDirection = DOWN,
		RIGHT_WALL �� ��� tempDirection = LEFT,
		LEFT_WALL �� ��� tempDirection = RIGHT �� �����ϰ�

		Block_Move() �Լ��� tempDirection�� �ѱ��
		block�� ���� �ݴ� �������� �з� ������ ���ݴϴ�.
		�� ���� ������ direction ���� �ѱ�� Block_Move()�� ȣ���� ���� ȸ�� �����ݴϴ�.*/
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

		/*	checkResult ���� ��ȿ���� ���� ���������, ȸ���� �ƴ� ����� ó���Դϴ�.
		������ �������ε� ������ ���̳� �̹� ��ġ�� ���� ���� ���� ���,
		�Ǵ� �����ε� ���� ���̳� �̹� ��ġ�� ���� ���ʿ��� ���� ���� ���
		�ش� �������� �̵��� ���ϰ� �ϳ�,
		������ ���� �ð� ���� �Ŀ� �ڵ����� ��������� �ϱ� ������,

		TetrisManager_Sleep() �Լ��� ȣ����, ���������� ��
		Block_Move() �Լ��� ȣ���� DOWN���� ������ �Ѱܼ� ���� �ڵ�����
		��ĭ �����ݴϴ�.
			
		������ ǥ���ϸ� �̵� ���ϰ� �Ǿ��� �� ������ �� ��ġ�� �ӹ��� ���� �ʵ���
		�����ϱ� ���� ����Դϴ�. */
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
	/* _TetrisManager_ChangeBoardByStatus() �Լ��� ȣ���ϰ�
		MOVIG_BLOCK�� �Ѱܼ� tetrisManager�� board���� ���� �ٲ� ���� ��ġ��
		�缳�� ���ݴϴ�. */
	_TetrisManager_ChangeBoardByStatus(tetrisManager, MOVING_BLOCK);
}

/*	=============================================================
	TetrisManager_ChangeBoardByAuto() �Լ��� �κ��Դϴ�.
	�� �Լ��� ������ �Ű������� tetrisManager�� ���޹ް�,
	�ڵ����� ���� �� ĭ �������� �ϴ� ������ �մϴ�.
	============================================================= */
void TetrisManager_ChangeBoardByAuto(TetrisManager* tetrisManager){
	
	/* TetrisManager_ChangeBoardByDirection() �Լ���
	DOWN �Ű������� �Ѱ� �� ĭ ������ ���� �������� �մϴ�. */
	TetrisManager_ChangeBoardByDirection(tetrisManager, DOWN);
}

/*	=============================================================
	TetrisManager_ProcessDirectDown() �Լ� �κ��Դϴ�.
	�Ű������� ����ü ������ ������ tetrisManager�� �޾�,
	�����̽��ٸ� ������ ��� ���� �ѹ��� ������ ����� �մϴ�.
	=============================================================	*/
void TetrisManager_ProcessDirectDown(TetrisManager* tetrisManager){

	/*	TetrisManager_IsReachedToBottom()�Լ��� �ҷ� ���� ������ �̵��� �� �ִ�����
		üũ �� �� �̵� �����ϸ�, ����ؼ� �������� ChangeBoardByDirection() �Լ���
		ȣ���Ͽ� �Ű������� DOWN�� �Ѱ��� �� ������ ���� ��ĭ �� �����ݴϴ�.
		�ݺ����� Ȱ���� ���� ���� �� ���� ������ �����ִ� ����� �� �Դϴ�. */
	while (!TetrisManager_IsReachedToBottom(tetrisManager)){
		TetrisManager_ChangeBoardByDirection(tetrisManager, DOWN);
	}
}

/*	=====================================================
	TetrisManager_ProcessDeletingLines() �Լ� �κ��Դϴ�.
	�Ű������� tetrisManager�� ���� �� ������ ������ ã�� ���� ���� ��
	��Ʈ���� ���� ���Ӱ� �������ִ� ������ ���ݴϴ� .
	===================================================== */
void TetrisManager_ProcessDeletingLines(TetrisManager* tetrisManager){

	// ������ ������ ��ġ�� ������ �迭 �� ������ �������ݴϴ�.
	int indexes[BOARD_ROW_SIZE];
	int count;
	int i;

	/*	�Լ��� ȣ���Ͽ�, ������ ������ ��ġ�� ã�� ���� �迭�� �����ϰ�
		count�� �� ������ �� ������ �������ݴϴ�. */
	_TetrisManager_SearchLineIndexesToDelete(tetrisManager, indexes, &count);

	/*	���� ���� ������ ���� �ִٸ�, DeleteLines()�Լ��� ȣ���Ͽ�
	�ش��ϴ� ��ġ�� ���ε��� �������ݴϴ�. ���� ������ ���� ������ 3�� ����� ���
	UpSpeedLevel() �Լ��� ȣ���Ͽ� ������ �ӵ� ������ 1 ���� �����ݴϴ�. */
	if (count > 0){
		_TetrisManager_DeleteLines(tetrisManager, indexes, count);
		for (i = tetrisManager->speedLevel; i <= tetrisManager->deletedLineCount / LEVELP_UP_CONDITION; i++){
			_TetrisManager_UpSpeedLevel(tetrisManager);
		}
	}
}

/*	=============================================================
	TetrisManager_IsReachedToBottom() �Լ� �κ��Դϴ�.
	�� �Լ��� �����, ���� �̵����� ���� ������ �̵��� �� �ִ��� üũ�Ͽ�
	�� ����� True OR False �� ��ȯ���ִ� ����� �մϴ�.
	=============================================================	*/
int TetrisManager_IsReachedToBottom(TetrisManager* tetrisManager){
	int i;
	/*	for���� ���� ���� �̵����� block�� 2���� �迭( 4���� ���� )�� ��ǥ�� �޾�
	�� ��ǥ�� �ٷ� ���� EMPTY�� �ƴϰ� MOVING_BLOCK�� �ƴ�, �� �� �ΰ��� ������
	���� ��� True(=1)�� ���� ��ȯ�� �ݴϴ�.*/
	for (i = 0; i < POSITIONS_SIZE; i++){
		int x = Block_GetPositions(tetrisManager->block)[i].x;
		int y = Block_GetPositions(tetrisManager->block)[i].y;
		if (tetrisManager->board[x + 1][y] != EMPTY && tetrisManager->board[x + 1][y] != MOVING_BLOCK){
			return True;
		}
	}
	return False; // �ƴ� ��� False(=0)�� ��ȯ���ݴϴ�. 
}

/*	=============================================================
	TetrisManager_ProcessReacehdCase() �Լ��� �κ��Դϴ�.
	�� �Լ��� �����, ���� �ؿ� ����� ���(���̳� ������ �浹)
	���� �̵����� ���� ������Ű��, �� ���� ȣ���ϴ� ����� ���ݴϴ�.
	============================================================= */
int TetrisManager_ProcessReachedCase(TetrisManager* tetrisManager){

	/*	���� ChageBoardByStatus() �Լ��� ȣ���Ͽ� FIXED_BLOCK �Ű������� �Ѱ�
		���� ���� �ش� �ϴ� ��ġ�� ��Ʈ�����ǿ��� FIXED_BLOCK���� �������ݴϴ�. */
	_TetrisManager_ChangeBoardByStatus(tetrisManager, FIXED_BLOCK);

	/*	Block_Make()�Լ��� ȣ���Ͽ� �� ���� �������ݴϴ�.
		(�Ű� ������ False�� �ѱ� ������ ���� ó�� �Ҹ��� ���� �ƴ�
		2��° ���� ������ �Ҹ��� ���̶�� ���� �������ֱ� ���� �� �Դϴ�.) */
	tetrisManager->block = Block_Make(False, tetrisManager->block);

	/*	IsReachedToBottom()�Լ��� ȣ���� ������ ���� ������ �� �� �ִ����� ���� ���θ�
		üũ�� �ݴϴ�. �� �� ���� ��� END(���� ����) ���� ��ȯ�ϰ�
		�� �� ���� ��쿣 PLAYING(���� ����) ���� ��ȯ���ݴϴ�. */
	if (TetrisManager_IsReachedToBottom(tetrisManager)){
		return END;
	}
	else{
		return PLAYING;
	}
}

/*	=============================================================
	TetrisManager_Sleep() �Լ� �κ��Դϴ�.
	�� �Լ��� ����� �Ű������� tetrisManager�� ���� �ް�
	�ӵ� ������ ���� ���α׷��� �����ӵ��� ������ ���ִ� ������ ���ݴϴ�.
	============================================================= */
void TetrisManager_Sleep(TetrisManager* tetrisManager){
	int i;
	/*	�������� milliSecond�� INITIAL_SPEED(=500)�� �־��ݴϴ�.
		���⼭ �ڷ��� DWORD��, unsigned long�� �ڷ����� typedef�� DWORD ��� ������
		�ڷ����ε�, 4����Ʈ�� �ڷ����̶�� �� �� �ֽ��ϴ�.

		DWORD�� WORD �ڷ������� �Ļ��� ���ε�,
		WORD�� ���� CPU�� �ѹ��� ó�� �� �� �ִ� ���� �� �մϴ�.
		WORD = 16��Ʈ�� DWORD = 32��Ʈ �� 4����Ʈ�� �ڷ����̶�� �� �� �ֽ��ϴ�. */
	DWORD milliSecond = INITIAL_SPEED;
	/*	For���� ���� �ּ� �ӵ� �������� ���� �ӵ� �������� ����,
		if������ �б⸦ ����� �ִ� �ӵ� ���� / 2 ������ millisSecond����
		SPEED_LEVEL_OFFSET(=500)��ŭ ���ְ� , �� ���Ŀ�
		milliSecond ���� (SPEED_LEVEL_OFFSET / 5)�� ����ŭ ���ݴϴ�. */
	for (i = MIN_SPEED_LEVEL; i < tetrisManager->speedLevel; i++){
		if (i < MAX_SPEED_LEVEL / 2){
			milliSecond -= SPEED_LEVEL_OFFSET;
		}
		else{
			milliSecond -= (SPEED_LEVEL_OFFSET / 5);
		}
	}
	/*	�� �� Sleep() �Լ��� ȣ���� �Ű�������
		milliSecond�� �޾��ְ� ���� �������� �ӵ��� �ٿ��ݴϴ�. */
	Sleep(milliSecond);
}

/*=============================================================
TetrisManager_Print() �Լ� �κ��Դϴ�.
�� �Լ��� ������ ��Ʈ���� ���� ��� �� ���� ���¸� ����ϴ� ������ ���ݴϴ�.
=============================================================== */
void TetrisManager_Print(TetrisManager* tetrisManager){
	int i;
	int j;
	
	/*	CursorUtil_GotoXY() �Լ��� �ҷ��� ��, Ŀ���� (0,0)��ǥ ��ġ�� �̵� ��Ų ��
		�ݺ������� tetrisManager -> board �迭�� �ݺ��ϸ�
		boardType�� ���� �ش��ϴ� ���ڿ����� ������ݴϴ�. */
	CursorUtil_GotoXY(0, 0);
	for (i = 0; i < BOARD_ROW_SIZE; i++){
		for (j = 0; j < BOARD_COL_SIZE; j++){
			switch (tetrisManager->board[i][j]){
			case LEFT_TOP_EDGE:
				printf("��");
				break;
			case RIGHT_TOP_EDGE:
				printf("��");
				break;
			case LEFT_BOTTOM_EDGE:
				printf("��");
				break;
			case RIGHT_BOTTOM_EDGE:
				printf("��");
				break;
			case EMPTY:
				printf("  ");
				break;
			case MOVING_BLOCK:
				printf("��");
				break;
			case FIXED_BLOCK:
				printf("��");
				break;
			case LEFT_WALL:
			case RIGHT_WALL:
				printf("��");
				break;
			case TOP_WALL:
			case BOTTOM_WALL:
				printf("��");
				break;
			}
		}
		printf("\n");
	}

	/*	��Ʈ������ ���� �ӵ������� ���ݱ��� ������ �� �� ������ ����ϰ�
		Block_PrintNext() �Լ��� ȣ���Ͽ� ������ ���� ���� ������� ��
		CursorUtil_Hide() �Լ��� �ҷ��� Ŀ���� �����ݴϴ�. */
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
	TetrisManager_ClearBoard()�Լ� �κ��Դϴ�.
	�� �Լ��� ������, �Ű������� tetrisManager�� ���� �ް�
	��Ʈ���� ���� �ʱ�ȭ ������ ���ݴϴ�.
	============================================================= */
static void _TetrisManager_ClearBoard(TetrisManager* tetrisManager){
	int i;
	int j;

	// For������ ��Ʈ���� ���� ���ʺ��� ������ ���� �缳�����ݴϴ�.
	for (i = 0; i < BOARD_ROW_SIZE; i++){
		tetrisManager->board[i][0] = LEFT_WALL;
		tetrisManager->board[i][BOARD_COL_SIZE - 1] = RIGHT_WALL;
	}
	// For������ ��Ʈ������ �� ���� �Ʒ� ���� �缳�����ݴϴ�.
	for (i = 0; i < BOARD_COL_SIZE; i++){
		tetrisManager->board[0][i] = TOP_WALL;
		tetrisManager->board[BOARD_ROW_SIZE - 1][i] = BOTTOM_WALL;
	}
	// For������ ��Ʈ���� ���� ���ʿ��� ������ ���� ������ ������ �� �������� �����մϴ�.
	for (i = 1; i < BOARD_ROW_SIZE - 1; i++){
		for (j = 1; j < BOARD_COL_SIZE - 1; j++){
			if (tetrisManager->board[i][j] != FIXED_BLOCK){
				tetrisManager->board[i][j] = EMPTY;
			}
		}
	}

	// ��Ʈ�������� �𼭸��� ���� �κ��� �缳�� ���ݴϴ�.
	tetrisManager->board[0][0] = LEFT_TOP_EDGE;
	tetrisManager->board[0][BOARD_COL_SIZE - 1] = RIGHT_TOP_EDGE;
	tetrisManager->board[BOARD_ROW_SIZE - 1][0] = LEFT_BOTTOM_EDGE;
	tetrisManager->board[BOARD_ROW_SIZE - 1][BOARD_COL_SIZE - 1] = RIGHT_BOTTOM_EDGE;
}

/*	=============================================================
	TetrisManager_ChangeBoardByStatus()�Լ� �κ��Դϴ�.
	�Ű������� tetrisManager(����ü ����), status(int�� ����)�� ���޹ް�
	�ش� status�� ���� ��ġ�� ��Ʈ�����ǿ��� �����ϴ� ������ ���ݴϴ�.
	�Ű����� status�� ��Ʈ���� �ǿ��� ���� ��ġ�� �ش��ϴ� ��ǥ�� ������ ���¸� ���մϴ�.
	============================================================= */
static void _TetrisManager_ChangeBoardByStatus(TetrisManager* tetrisManager, int status){
	int i;
	// For������ block�� 4�� ��ǥ���� ���� �Է¹��� status�� �����մϴ�.
	for (i = 0; i < POSITIONS_SIZE; i++){
		int x = Block_GetPositions(tetrisManager->block)[i].x;
		int y = Block_GetPositions(tetrisManager->block)[i].y;
		tetrisManager->board[x][y] = status;
	}
}

/*	=============================================================
	TetrisManager_UpSpeedLevel() �Լ� �κ��Դϴ�.
	�Ű������� tetrisManager�� �ް�, �ӵ������� 1 ���� ��ŵ�ϴ�.
	============================================================= */
static void _TetrisManager_UpSpeedLevel(TetrisManager* tetrisManager){
	// ���� �ӵ������� �ִ� �ӷ� ����(=10)���� ���� ��쿡�� 1 ���� ��ŵ�ϴ�.
	if (tetrisManager->speedLevel < MAX_SPEED_LEVEL){
		tetrisManager->speedLevel++;
	}
}

/*	=============================================================
	TetrisManager_SearchLineIndexesToDelete() �Լ� �κ��Դϴ�.
	�Ű� ������ �� 3�� ���� �޴µ�, tetrisManager��
	indexes( ������ ������ ��ġ���� ���� �迭), count(������ ���� ����)
	�� ���޹޾Ƽ� ���� ��Ʈ���� �ǿ��� ������ ������ ��ġ��
	ã�� ������ �մϴ�.
	============================================================= */
static void _TetrisManager_SearchLineIndexesToDelete(TetrisManager* tetrisManager, int* indexes, int* count){
	int i;
	int j;
	int toDelete;
	// indexes �迭�� memset()�Լ��� ���Ͽ� -1�� ���� �ʱ�ȭ ���ݴϴ�.
	memset(indexes, -1, sizeof(int)* (BOARD_ROW_SIZE - 2));
	*count = 0; // �� ������ count�� ���� �մϴ�.

	/*	��Ʈ���� ���� ���� ���鼭 ����(BOARD_ROW_SIZE)�� ����
	��� FIXED ����� ��츦 ã���ϴ�. �� �� �ش� ��ġ��
	indexes�� �������ְ�, �� �� ���� count�� ���� 1�� �����մϴ�.*/
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
	TetrisManager_DeleteLines() �Լ��� ���Ǻ� �Դϴ�.
	�Ű������� tetrisManager�� indexes, count�� �޾Ƽ�
	������ ������ ���������� �����ִ� ������ ���ִ� �Լ��Դϴ�.
	============================================================= */
static void _TetrisManager_DeleteLines(TetrisManager* tetrisManager, int* indexes, int count){
	int i;
	int j;
	int k = BOARD_ROW_SIZE - 2;
	int toDelete;
	// ���� ������ ���� �ӽú����� �����, 
	// �ӽ÷� ������ ��Ʈ�������� �������ݴϴ�.
	char temp[BOARD_ROW_SIZE][BOARD_COL_SIZE] = { EMPTY };
	
	/*	For������ ��Ʈ�������� ���鼭 indexes�� �ش��ϴ� ����
		������(if��) ��� ���� �ӽú����� �־��ݴϴ�. */
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
	/*	�ӽú���(temp)�� �ִ� ���������� �ٽ� �����ǿ� �־��ݴϴ�. */
	for (i = 1; i < BOARD_ROW_SIZE - 1; i++){
		for (j = 1; j < BOARD_COL_SIZE - 1; j++){
			tetrisManager->board[i][j] = temp[i][j];
		}
	}
	// tetrinsManager �� deletedLineCount ����, count�� ����ŭ ������ŵ�ϴ�.
	tetrisManager->deletedLineCount += count;
}