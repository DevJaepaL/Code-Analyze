// �� ��������� include �ϴ� ������ �ߺ� �������� �ʱ� ���� Guard
#ifndef _TETRIS_VIEW_H
#define _TETRIS_VIEW_H

#include "TetrisManager.h" // �� ������� �ʿ��� ������� �ҷ�����

// TetrisView ����ü�� �����մϴ�. ����δ� tetrisManager�� �ֽ��ϴ�.
typedef struct _tetrisView{
	TetrisManager tetrisManager;
}TetrisView;

// TetrisView.c ���� �ʿ��� �Լ� ��� �Դϴ�.
void TetrisView_StartGame(TetrisView* tetrisView);
void TetrisView_ProcessGame(TetrisView* tetrisView, int processType, int direction);
void TetrisView_EndGame(TetrisView* tetrisView);

#endif