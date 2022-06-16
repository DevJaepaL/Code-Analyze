// 필요한 기본 라이브러리 헤더 및 사용자 생성 헤더 로드.
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include "Block.h"
#include "Util.h"
#include "Constant.h"

// 생성 될 수 있는 여러 블럭 타입들의 사이즈를 상수 선언.
#define BLOCK_EXAMPLES_SIZE 6

/*	26 ~ 74 라인
	생성될 수 있는 블럭요소들을 미리 상수 배열로 정의 합니다.
	const를 붙인 이유는, 상수화를 위함이고 static은 현재 파일 내에서만
	내부적으로 사용하기 위해서입니다. 총 6개의 타입의 블럭들을
	상하좌우 방향에 대한 상대좌표로 저장해 놓습니다.

	예를 들면, ㅁㅁㅁㅁ 와 같은 모양의 블럭은 위쪽 방향일 때
	(0,5)(0,6)(0,7)(0,8)의 위치를 가집니다.
	만약 회전하여 오른쪽 방향 일 경우 (-2,6)(-1,6)(0,6)(1,6)의 위치를 가집니다.
	한번 더 회전 할 경우 원래 모양으로 돌아올 것이고,
	마지막으로 한번 더 회전하여 왼쪽 방향일 때는 오른쪽과 대칭되는 값을 가지게 될 것입니다.
	이처럼 x좌표는 0을 기준으로, y좌표는 테트리스 맵의 중간 지점을 기준으로
	하여 각각 6개의 모양 별로, 상하좌우로 회전 했을 때의 좌표 위치를
	배열에 미리 저장해 놓습니다. */
const static Point blockExamples[BLOCK_EXAMPLES_SIZE][POSITIONS_SIZE][POSITIONS_SIZE] = {
	//ㅁㅁㅁㅁ
	{
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -2, 6 }, { -1, 6 }, { 0, 6 }, { 1, 6 } },
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -2, 6 }, { -1, 6 }, { 0, 6 }, { 1, 6 } }
	},
	//    ㅁ
	//ㅁㅁㅁ
	{
		{ { 0, 8 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 6 } },
		{ { -1, 6 }, { -1, 7 }, { 0, 7 }, { 1, 7 } }
	},
	//  ㅁㅁ
	//ㅁㅁ
	{
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},
	//ㅁㅁ
	//  ㅁㅁ
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } }
	},
	//ㅁ
	//ㅁㅁㅁ
	{
		{ { 0, 6 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { -1, 7 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 6 } }
	},
	//ㅁㅁ
	//ㅁㅁ
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } }
	}
};

// Block.c 소스 파일 내부에서만 사용될 내부 함수의 선언.
static Block _Block_MoveToDown(Block block);
static Block _Block_MoveToLeft(Block block);
static Block _Block_MoveToRight(Block block);
static Block _Block_RotateRight(Block block);

/*	================================
	Block_Make 함수의 정의부 입니다.
	매개변수로 isFirst(블록이 테트리스가 시작하고 난 후에
	가장 처음에 만들어지는지의 여부)를 받고, 만들어진 block을 반환.
	================================ */
Block Block_Make(int isFirst, Block block){
	int i;
	int j;

	/*	srand와 time 함수를 사용해 난수 생성 전 시드를 섞어줍니다.
	그리고 isFirst가 True일 경우, block.current에 난수를
	생성하여 저장하고(0-5 값 사이의 정수), 처음이 아닐 경우
	block.current에는 이전에 생성해 두었던, block.next를
	가져와서 저장합니다.

	즉, 처음 블럭을 생성하는 경우에는 current를 난수로 생성해야 하고,
	두번째 부터는 다시 난수로 생성하는게 아닌, next에서
	가져오면 된다는 의미입니다.*/
	srand((unsigned int)time(NULL));
	if (isFirst){
		block.current = rand() % BLOCK_EXAMPLES_SIZE;
	}
	else{
		block.current = block.next;
	}

	/*	block.positions 배열에 위에서 생성한거나 next에서 가져온 값인
	block.currrent를 기반으로 blockExamples(6개 모양의 블록들을
	4방향 모두 상대좌표 위치를 저장한 배열)에서 블럭을 가져와서
	4 개의 상자의 위치를 저장해 둡니다.*/
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j] = blockExamples[block.current][i][j];
		}
	}
	block.next = rand() % BLOCK_EXAMPLES_SIZE;
	block.direction = UP;
	return block;
}

/*	===================================
	Block_Move 함수의 정의부 입니다.
	매개 변수로 구조체 block과 direction(사용자가 입력한 키값)을 받아서
	4 가지 방향키에 해당하는 처리를 한 블럭을 반환해줍니다.
	=================================== */
Block Block_Move(Block block, int direction){
	switch (direction){
	case LEFT:
		return _Block_MoveToLeft(block);
	case RIGHT:
		return _Block_MoveToRight(block);
	case DOWN:
		return _Block_MoveToDown(block);
	case UP:
		return _Block_RotateRight(block);
	}
	return _Block_MoveToDown(block);
}

/*	===================================
	Block_GetPositions() 함수 부분입니다.
	매개변수로 구조체 Block을 받아
	positions에서 현재 방향에 해당하는 요소를 반환해줍니다.
	=================================== */
Point* Block_GetPositions(Block block){
	return block.positions[block.direction];
	/* 매개변수로 받은 block의 positions의
	배열 [block.positions] 번째 요소를 반환합니다.
	반환형이 Point*(포인터)인 이유는 positions가
	2차원 배열이기 때문에, N 번째 요소는 1차원 배열이
	되기 때문입니다. */
}

/*	===================================
	Block_PrintNext()함수의 정의부 입니다.
	매개 변수로 구조체 block과 int 형 x,y를 받습니다.
	해당 좌표(x,y)에 다음 블럭이
	나타나게 하는 역할을 해줍니다.
	=================================== */
void Block_PrintNext(Block block, int x, int y){
	CursorUtil_GotoXY(x, y);
	/*	CursorUtil_GotoXY()함수를 이용해
	x,y 좌표로 출력 커서를 이동 시켜 줍니다. */
	printf("Next block : ");
	x += 3;
	y += 2;
	CursorUtil_GotoXY(x, y++);
	/*	switch 문을 이용해 변수 block.next를 확인 후
	0 - 5 까지 해당하는 타입의 블럭 모양을 출력해 줍니다. */
	switch (block.next){
	case 0:
		printf("■■■■");
		CursorUtil_GotoXY(x, y++);
		printf("        ");
		break;
	case 1:
		printf("      ■");
		CursorUtil_GotoXY(x, y++);
		printf("  ■■■");
		break;
	case 2:
		printf("    ■■");
		CursorUtil_GotoXY(x, y++);
		printf("  ■■  ");
		break;
	case 3:
		printf("  ■■  ");
		CursorUtil_GotoXY(x, y++);
		printf("    ■■");
		break;
	case 4:
		printf("■      ");
		CursorUtil_GotoXY(x, y++);
		printf("■■■  ");
		break;
	case 5:
		printf("  ■■  ");
		CursorUtil_GotoXY(x, y++);
		printf("  ■■  ");
		break;
	}
}

/*	===================================
	_Block_MoveToDown() 함수 부분입니다.
	이 함수는 내부에서만 쓰이도록 static으로 정의되어 있습니다.
	매개 변수로 block을 받아 밑으로 한칸 이동시키고
	반환하는 역할을 해줍니다.
	=================================== */
static Block _Block_MoveToDown(Block block){
	int i;
	int j;
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j].x++;
		}
	}
	return block;
}

/*	===================================
	Block_MoveToLeft() 함수 부분 입니다.
	이 함수는 매개변수로 구조체 block을 받아
	왼쪽으로 한칸 이동 시키고 return, 즉 반환시켜주는
	역할을 합니다.
	=================================== */
static Block _Block_MoveToLeft(Block block){
	int i;
	int j;

	/*	for문을 통해 POSITIONS_SIZE(=4) 까지
	증가하고, 반복문이 한번 실행 될때마다
	block.positions(블럭안에 있는 4개 상자의 좌표들)
	들의 y 좌표 값을 감소시킵니다. (왼쪽으로 한칸 이동) */
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j].y--;
		}
	}
	return block;
}

/*	===================================
	Block_MoveToRight() 함수 부분입니다.
	이 함수도 위의 함수와 마찬가지로 매개변수로
	구조체 block을 받아 오른쪽으로 한칸 이동시키는
	역할을 해줍니다.
	===================================*/
static Block _Block_MoveToRight(Block block){
	int i;
	int j;
	/*	for문을 통해 block.positions의 y 좌표를
	증가 시킵니다. (오른쪽으로 한칸 이동) */
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j].y++;
		}
	}
	return block;
}

/*	=========================================
	_Block_RotateRight() 함수의 정의부 입니다.
	이 함수는 매개변수로 block(구조체)을 받아
	오른쪽(시계 방향)으로 블럭을 돌려주고
	return, 반환하는 역할을 하는 함수입니다.
	========================================= */
static Block _Block_RotateRight(Block block){
	/*	block.direction의 값을 1 증가 시킵니다. (다음 방향으로 전환합니다.)
	%(나머지 값 계산)을 한 이유는 , 0 -> 1 -> 2 -> 3 -> 0 으로
	순환하게 만들기 위한 것 입니다.*/
	block.direction = (block.direction + 1) % POSITIONS_SIZE;
	return block;
}