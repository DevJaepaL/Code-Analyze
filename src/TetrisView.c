// �ʿ��� ������� �ҷ�����
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "TetrisView.h"
#include "TetrisManager.h"
#include "Constant.h"

#pragma comment(lib, "winmm.lib") // �Ҹ� �� ���� ����� ���� winmm.lib ���̺귯���� �ε��մϴ�.

// �͹̳� ����� ��������� ���� ���� ���ϸ��� ��ũ�� ����� �����մϴ�.
#define TETRIS_BACKGROUND_MUSIC_FILE_NAME "..\\res\\tetris_background_music.wav"

/*	======================================
	TetrisView_StartGame() �Լ� �κ��Դϴ�.
	�Ű� ������ ����ü ����� tetrisView�� �޾�
	��Ʈ���� ������ �����ϱ� ���� ���� ������ �ϰ�
	������ ���� ���ִ� ������ �մϴ�.
	====================================== */
void TetrisView_StartGame(TetrisView* tetrisView){
	int speedLevel;
	PlaySound(TEXT(TETRIS_BACKGROUND_MUSIC_FILE_NAME), NULL, SND_ASYNC | SND_LOOP);
	// PlaySound() ��� �Լ��� ȣ���Ͽ� ��������� ����մϴ�.
	printf("* * * * * Tetris * * * * * \n Select Speed level (1 ~ 10) : "); // ���� ���۽� ��� ����
	scanf_s("%d", &speedLevel); // ����ڷκ��� ���ǵ� ���� ���� �ޱ�
	
	// speedLevel�� ������ ��ȿ���� ���� ��� ���Ƿ� MIN_SPEED_LEVEL(=1)�� �����մϴ�.
	if (speedLevel > MAX_SPEED_LEVEL || speedLevel < MIN_SPEED_LEVEL){
		speedLevel = MIN_SPEED_LEVEL;
	}
	TetrisManager_Init(&tetrisView->tetrisManager, speedLevel);
	// TetrisManager_Init()�Լ��� ȣ���� tetrisManager ������ �ʱ�ȭ�մϴ�.

	TetrisManager_Print(&tetrisView->tetrisManager);
	// TetrisManager_print()�Լ��� ȣ���� ��Ʈ���� �� , �� , ���� ���¸� ������ݴϴ�.
}

/*	======================================
	TetrisView_ProcessGame() �Լ� �κ��Դϴ�.
	�Ű�������	tetrisManager,
				processType(������� �Է°��� �����ϴ� ����),
				direction(����) �� ���޹޾Ƽ�
	������ ������ �������ִ� ����Դϴ�.
	====================================== */
void TetrisView_ProcessGame(TetrisView* tetrisView, int processType, int direction){
	/*	62 - 70 ����
		�б����� ������ ������ processType�� Ȯ���Ͽ�
		DIRECTION(����Ű�� ���� ���)
		DIRECT_DOWN(�����̽� �ٸ� ���� ���)
		AUTO(�ڵ����� �������� ���)
		�� 3������ �б�� ������ �����մϴ�.

		processType == DIRECT�� ���,
			TetrisManager_ChangeBoardByDirection() �Լ��� ȣ����
			���� ������ ������Ʈ �ϰ� ȭ���� �������ݴϴ�.
		processType == DIRECT_DOWN�� ���,
			TetrisManager_ProcesDirectDown() �Լ��� ȣ����
			���� ���� ������ �� ���� �� ���� ������, ȭ���� �������ݴϴ�.
		processType == AUTO�� ���,
			TetrisManager_ChangeBoardByAuto() �Լ��� ȣ����
				�� ĭ ������ ���� ���� �� ȭ���� �����մϴ�. */
	if (processType == DIRECTION){
		TetrisManager_ChangeBoardByDirection(&tetrisView->tetrisManager, direction);
	}
	else if (processType == DIRECT_DOWN){
		TetrisManager_ProcessDirectDown(&tetrisView->tetrisManager);
	}
	else if (processType == AUTO){
		TetrisManager_ChangeBoardByAuto(&tetrisView->tetrisManager);
	}

	/*	TetrisManager_IsReachedToBottom()�Լ��� ����,
		���� ���� �ٴڿ� ��Ҵ��� ���θ� üũ �մϴ�.
		���� ���� �ٴڿ� ��Ҵٸ�, ����ڰ� �¿�Ű�� ��������
		Ȯ�� �� �� ������ ������� Ȯ�� �մϴ�.

		�����̳�, �������� ������� Ȯ���ϱ� ����
		TetrisManager_CheckValidPosition() �Լ��� ȣ���Ͽ�
		üũ ���ݴϴ�. ���� �� �ΰ����� �ϳ��� ������ �����Ǹ�
		TetrisManager_ChangeBoardByDirection() �Լ��� ȣ����
		����ڰ� ���ϴ� �������� ���� �̵��� �� �ֵ��� ���ݴϴ�. */
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
	// ...ProcessDeletingLines() �Լ��� ȣ���� ������ ������ ��� �����ݴϴ�.
	TetrisManager_ProcessDeletingLines(&tetrisView->tetrisManager);
	TetrisManager_Print(&tetrisView->tetrisManager);

	/* processType�� AUTO�̸� ...Sleep()�Լ��� ȣ����
		�Է��� speedLevel�� ���� �����ð����� ������ ���������ݴϴ�.*/
	if (processType == AUTO){
		TetrisManager_Sleep(&tetrisView->tetrisManager);
	}
}

/*	======================================
	TetrisView_EndGame() �Լ� �κ��Դϴ�.
	�Ű������� tetrisView�� �ް� ������ ���� ���ִ� ������ �մϴ�.
	====================================== */
void TetrisView_EndGame(TetrisView* tetrisView){
	PlaySound(NULL, 0, 0);  // PlaySound()�Լ��� ȣ����, �뷡�� ���ݴϴ�.
	system("cls"); // system() �Լ��� ȣ���� ȭ���� ����(������)�մϴ�.
	
	// ���� �������ͽ� ���
	printf("***** End *****\n%d level / %d lines deleted\n",
		tetrisView->tetrisManager.speedLevel,
		tetrisView->tetrisManager.deletedLineCount);
	system("pause");
	exit(-1);
}