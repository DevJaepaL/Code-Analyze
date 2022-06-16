/* 헤더파일 중복 포함을 방지하는 Guard. */
#ifndef _BLOCK_H
#define _BLOCK_H

/*	Point 구조체의 정의부 입니다.
	멤버로는 x 좌표 및 y 좌표를 가지고 있습니다. */
typedef struct _point{
	int x;
	int y;
}Point;

/*	Block 구조체의 멤버로 사용될
	POSITIONS 배열의 크기를 상수로 지정합니다.(=4) */
#define POSITIONS_SIZE 4

/*	=========================================
	Block 구조체의 정의부 입니다.
	Block 구조체는 테트리스에서 사용되는 하나의 블럭을 나타냅니다.
	
	(현재 블럭, 다음 나올 블럭) 멤버로는 positions[상수 4] 배열이 있는데,
	이는 4 X 4 크기의 2차원 배열로 Point형(x,y 좌표)로 구성 되어있습니다.
	이는 하나의 블럭이 4 개의 정사각형으로 구성되어 있기 때문에,
	4개의 상자에 x,y 좌표를 담기 위해 사용됩니다.
	
	int 자료형인 current는 여러 타입의 블럭들 중에서 현재 이동하는
	블럭의 타입을 나타냅니다.
	int 자료형인 next는 여러 타입의 블럭들 중에서 다음에 나올 블럭의
	타입을 나타냅니다.
	int 자료형인 direction은 블럭의 방향을 나타내고, 상하좌우를 나타냅니다. 
	========================================= */
typedef struct _block{
	Point positions[POSITIONS_SIZE][POSITIONS_SIZE];
	int current;
	int next;
	int direction;
}Block;

// 소스 파일에서 사용될 구조체 Block 관련 함수 선언부 입니다.
Block Block_Make(int isFirst, Block block);
Block Block_Move(Block block, int direction);
Point* Block_GetPositions(Block block);
void Block_PrintNext(Block block, int x, int y);

// 헤더파일 중복 포함을 방지하는 Guard.
#endif