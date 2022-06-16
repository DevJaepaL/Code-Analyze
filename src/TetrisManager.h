/*	ifndef ��ó���⸦ �̿��Ͽ� 
	��������� �ߺ����� �ʱ� ���� �������ݴϴ�.*/
#ifndef _BOARD_H
#define _BOARD_H

#include "Block.h" // �ʿ��� ������� �ҷ�����.

	/*	�̸��� GameStatus�� ������ ���(enum)�� �����մϴ�. */
enum GameStatus{
	PLAYING, END
};

#define BOARD_ROW_SIZE 20 // ��Ʈ���� ���� ���� ���� ��ũ�� ���(define)���� ���� �մϴ�.
#define BOARD_COL_SIZE 14 // ��Ʈ���� ���� ���� ���� ��ũ�� ���(define)���� ���� �մϴ�.

/*	����ü TetrisManger�� ���� ���ݴϴ�.
	����ü �ȿ� ����δ� 2���� �迭�� board(��Ʈ������ ������)��
	����ü block(���� ��, ���� ��),
	int�� deletedLineCount(������ �� ����),
	int�� speedLevel(���� �������� �ӵ��� ����)�� �ֽ��ϴ�. */
typedef struct _tetrisManager{
	char board[BOARD_ROW_SIZE][BOARD_COL_SIZE];
	Block block;
	int deletedLineCount;
	int speedLevel;
}TetrisManager;

//TetrisManager.c ���� ����� �Լ����� �����Դϴ�. 
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