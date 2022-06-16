// 이 헤더파일을 include 하는 곳에서 중복 포함하지 않기 위한 Guard
#ifndef _TETRIS_VIEW_H
#define _TETRIS_VIEW_H

#include "TetrisManager.h" // 이 헤더에서 필요한 헤더파일 불러오기

// TetrisView 구조체를 정의합니다. 멤버로는 tetrisManager가 있습니다.
typedef struct _tetrisView{
	TetrisManager tetrisManager;
}TetrisView;

// TetrisView.c 에서 필요한 함수 목록 입니다.
void TetrisView_StartGame(TetrisView* tetrisView);
void TetrisView_ProcessGame(TetrisView* tetrisView, int processType, int direction);
void TetrisView_EndGame(TetrisView* tetrisView);

#endif