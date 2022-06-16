/*	ifndef 전처리기를 이용하여 
	헤더파일을 중복하지 않기 위해 선언해줍니다.*/
#ifndef _BOARD_H
#define _BOARD_H

#include "Block.h" // 필요한 헤더파일 불러오기.

	/*	이름이 GameStatus인 열거형 상수(enum)를 정의합니다. */
enum GameStatus{
	PLAYING, END
};

#define BOARD_ROW_SIZE 20 // 테트리스 판의 가로 행을 매크로 상수(define)으로 정의 합니다.
#define BOARD_COL_SIZE 14 // 테트리스 판의 세로 열을 매크로 상수(define)으로 정의 합니다.

/*	구조체 TetrisManger를 정의 해줍니다.
	구조체 안에 멤버로는 2차원 배열인 board(테트리스의 게임판)와
	구조체 block(현재 블럭, 다음 블럭),
	int형 deletedLineCount(삭제한 줄 개수),
	int형 speedLevel(블럭이 떨어지는 속도의 레벨)이 있습니다. */
typedef struct _tetrisManager{
	char board[BOARD_ROW_SIZE][BOARD_COL_SIZE];
	Block block;
	int deletedLineCount;
	int speedLevel;
}TetrisManager;

//TetrisManager.c 에서 사용할 함수들의 선언입니다. 
void TetrisManager_Init(TetrisManager* tetrisManager, int speedLevel); 
int TetrisManager_CheckValidPosition(TetrisManager* tetrisManager, int direction);
void TetrisManager_ChangeBoardByDirection(TetrisManager* tetrisManager, int direction);
void TetrisManager_ChangeBoardByAuto(TetrisManager* tetrisManager);
void TetrisManager_ProcessDirectDown(TetrisManager* tetrisManager);
void TetrisManager_ProcessDeletingLines(TetrisManager* tetrisManager);
int TetrisManager_IsReachedToBottom(TetrisManager* tetrisManager);
int TetrisManager_ProcessReachedCase(TetrisManager* tetrisManager);
void TetrisManager_Sleep(TetrisManager* tetrisManager);
void TetrisManager_Print(TetrisManager* tetrisManager);

#endif