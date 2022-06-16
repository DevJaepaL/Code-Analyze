// 필요한 헤더파일 불러오기
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "TetrisView.h"
#include "TetrisManager.h"
#include "Constant.h"

#pragma comment(lib, "winmm.lib") // 소리 및 음악 재생을 위해 winmm.lib 라이브러리를 로드합니다.

// 터미널 실행시 음악재생을 위해 음악 파일명을 매크로 상수로 지정합니다.
#define TETRIS_BACKGROUND_MUSIC_FILE_NAME "..\\res\\tetris_background_music.wav"

/*	======================================
	TetrisView_StartGame() 함수 부분입니다.
	매개 변수로 구조체 멤버인 tetrisView를 받아
	테트리스 게임을 시작하기 위한 기초 설정을 하고
	게임을 시작 해주는 역할을 합니다.
	====================================== */
void TetrisView_StartGame(TetrisView* tetrisView){
	int speedLevel;
	PlaySound(TEXT(TETRIS_BACKGROUND_MUSIC_FILE_NAME), NULL, SND_ASYNC | SND_LOOP);
	// PlaySound() 라는 함수를 호출하여 배경음악을 재생합니다.
	printf("* * * * * Tetris * * * * * \n Select Speed level (1 ~ 10) : "); // 게임 시작시 출력 문구
	scanf_s("%d", &speedLevel); // 사용자로부터 스피드 레벨 설정 받기
	
	// speedLevel의 범위가 유효하지 않을 경우 임의로 MIN_SPEED_LEVEL(=1)로 설정합니다.
	if (speedLevel > MAX_SPEED_LEVEL || speedLevel < MIN_SPEED_LEVEL){
		speedLevel = MIN_SPEED_LEVEL;
	}
	TetrisManager_Init(&tetrisView->tetrisManager, speedLevel);
	// TetrisManager_Init()함수를 호출해 tetrisManager 변수를 초기화합니다.

	TetrisManager_Print(&tetrisView->tetrisManager);
	// TetrisManager_print()함수를 호출해 테트리스 판 , 블럭 , 게임 상태를 출력해줍니다.
}

/*	======================================
	TetrisView_ProcessGame() 함수 부분입니다.
	매개변수로	tetrisManager,
				processType(사용자의 입력값을 구분하는 변수),
				direction(방향) 을 전달받아서
	실제로 게임을 진행해주는 기능입니다.
	====================================== */
void TetrisView_ProcessGame(TetrisView* tetrisView, int processType, int direction){
	/*	62 - 70 라인
		분기점을 여러개 나누고 processType을 확인하여
		DIRECTION(방향키를 누른 경우)
		DIRECT_DOWN(스페이스 바를 누른 경우)
		AUTO(자동으로 내려오는 경우)
		이 3가지의 분기로 게임을 진행합니다.

		processType == DIRECT일 경우,
			TetrisManager_ChangeBoardByDirection() 함수를 호출해
			블럭의 방향을 업데이트 하고 화면을 갱신해줍니다.
		processType == DIRECT_DOWN일 경우,
			TetrisManager_ProcesDirectDown() 함수를 호출해
			현재 블럭이 내려갈 수 있을 때 까지 내리고, 화면을 갱신해줍니다.
		processType == AUTO일 경우,
			TetrisManager_ChangeBoardByAuto() 함수를 호출해
				한 칸 밑으로 블럭을 내린 뒤 화면을 갱신합니다. */
	if (processType == DIRECTION){
		TetrisManager_ChangeBoardByDirection(&tetrisView->tetrisManager, direction);
	}
	else if (processType == DIRECT_DOWN){
		TetrisManager_ProcessDirectDown(&tetrisView->tetrisManager);
	}
	else if (processType == AUTO){
		TetrisManager_ChangeBoardByAuto(&tetrisView->tetrisManager);
	}

	/*	TetrisManager_IsReachedToBottom()함수를 통해,
		현재 블럭이 바닥에 닿았는지 여부를 체크 합니다.
		만약 블럭이 바닥에 닿았다면, 사용자가 좌우키를 눌렀는지
		확인 후 그 방향이 비었는지 확인 합니다.

		왼쪽이나, 오른쪽이 비었는지 확인하기 위해
		TetrisManager_CheckValidPosition() 함수를 호출하여
		체크 해줍니다. 만약 이 두가지중 하나라도 조건이 성립되면
		TetrisManager_ChangeBoardByDirection() 함수를 호출해
		사용자가 원하는 방향으로 블럭을 이동할 수 있도록 해줍니다. */
	if (TetrisManager_IsReachedToBottom(&tetrisView->tetrisManager)){
		//if you are going to move the block which has bottom wall or bottom fixed block, permit the block to move the direction
		if (processType == DIRECTION && direction == LEFT && (TetrisManager_CheckValidPosition(&tetrisView->tetrisManager, LEFT) == EMPTY) || 
			processType == DIRECTION && direction == RIGHT && (TetrisManager_CheckValidPosition(&tetrisView->tetrisManager, RIGHT) == EMPTY)){
			TetrisManager_ChangeBoardByDirection(&tetrisView->tetrisManager, direction);
		}
		if (TetrisManager_ProcessReachedCase(&tetrisView->tetrisManager) == END){
			TetrisView_EndGame(tetrisView);
		}
	}
	// ...ProcessDeletingLines() 함수를 호출해 블럭들이 한줄일 경우 지워줍니다.
	TetrisManager_ProcessDeletingLines(&tetrisView->tetrisManager);
	TetrisManager_Print(&tetrisView->tetrisManager);

	/* processType이 AUTO이면 ...Sleep()함수를 호출해
		입력한 speedLevel에 따라 일정시간동안 게임을 지연시켜줍니다.*/
	if (processType == AUTO){
		TetrisManager_Sleep(&tetrisView->tetrisManager);
	}
}

/*	======================================
	TetrisView_EndGame() 함수 부분입니다.
	매개변수로 tetrisView를 받고 게임을 종료 해주는 역할을 합니다.
	====================================== */
void TetrisView_EndGame(TetrisView* tetrisView){
	PlaySound(NULL, 0, 0);  // PlaySound()함수를 호출해, 노래를 꺼줍니다.
	system("cls"); // system() 함수를 호출해 화면을 리셋(지워줌)합니다.
	
	// 게임 스테이터스 출력
	printf("***** End *****\n%d level / %d lines deleted\n",
		tetrisView->tetrisManager.speedLevel,
		tetrisView->tetrisManager.deletedLineCount);
	system("pause");
	exit(-1);
}