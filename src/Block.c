// �ʿ��� �⺻ ���̺귯�� ��� �� ����� ���� ��� �ε�.
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include "Block.h"
#include "Util.h"
#include "Constant.h"

// ���� �� �� �ִ� ���� �� Ÿ�Ե��� ����� ��� ����.
#define BLOCK_EXAMPLES_SIZE 6

/*	26 ~ 74 ����
	������ �� �ִ� ����ҵ��� �̸� ��� �迭�� ���� �մϴ�.
	const�� ���� ������, ���ȭ�� �����̰� static�� ���� ���� ��������
	���������� ����ϱ� ���ؼ��Դϴ�. �� 6���� Ÿ���� ������
	�����¿� ���⿡ ���� �����ǥ�� ������ �����ϴ�.

	���� ���, �������� �� ���� ����� ���� ���� ������ ��
	(0,5)(0,6)(0,7)(0,8)�� ��ġ�� �����ϴ�.
	���� ȸ���Ͽ� ������ ���� �� ��� (-2,6)(-1,6)(0,6)(1,6)�� ��ġ�� �����ϴ�.
	�ѹ� �� ȸ�� �� ��� ���� ������� ���ƿ� ���̰�,
	���������� �ѹ� �� ȸ���Ͽ� ���� ������ ���� �����ʰ� ��Ī�Ǵ� ���� ������ �� ���Դϴ�.
	��ó�� x��ǥ�� 0�� ��������, y��ǥ�� ��Ʈ���� ���� �߰� ������ ��������
	�Ͽ� ���� 6���� ��� ����, �����¿�� ȸ�� ���� ���� ��ǥ ��ġ��
	�迭�� �̸� ������ �����ϴ�. */
const static Point blockExamples[BLOCK_EXAMPLES_SIZE][POSITIONS_SIZE][POSITIONS_SIZE] = {
	//��������
	{
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -2, 6 }, { -1, 6 }, { 0, 6 }, { 1, 6 } },
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -2, 6 }, { -1, 6 }, { 0, 6 }, { 1, 6 } }
	},
	//    ��
	//������
	{
		{ { 0, 8 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 6 } },
		{ { -1, 6 }, { -1, 7 }, { 0, 7 }, { 1, 7 } }
	},
	//  ����
	//����
	{
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},
	//����
	//  ����
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } }
	},
	//��
	//������
	{
		{ { 0, 6 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { -1, 7 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 6 } }
	},
	//����
	//����
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } }
	}
};

// Block.c �ҽ� ���� ���ο����� ���� ���� �Լ��� ����.
static Block _Block_MoveToDown(Block block);
static Block _Block_MoveToLeft(Block block);
static Block _Block_MoveToRight(Block block);
static Block _Block_RotateRight(Block block);

/*	================================
	Block_Make �Լ��� ���Ǻ� �Դϴ�.
	�Ű������� isFirst(����� ��Ʈ������ �����ϰ� �� �Ŀ�
	���� ó���� ������������� ����)�� �ް�, ������� block�� ��ȯ.
	================================ */
Block Block_Make(int isFirst, Block block){
	int i;
	int j;

	/*	srand�� time �Լ��� ����� ���� ���� �� �õ带 �����ݴϴ�.
	�׸��� isFirst�� True�� ���, block.current�� ������
	�����Ͽ� �����ϰ�(0-5 �� ������ ����), ó���� �ƴ� ���
	block.current���� ������ ������ �ξ���, block.next��
	�����ͼ� �����մϴ�.

	��, ó�� ���� �����ϴ� ��쿡�� current�� ������ �����ؾ� �ϰ�,
	�ι�° ���ʹ� �ٽ� ������ �����ϴ°� �ƴ�, next����
	�������� �ȴٴ� �ǹ��Դϴ�.*/
	srand((unsigned int)time(NULL));
	if (isFirst){
		block.current = rand() % BLOCK_EXAMPLES_SIZE;
	}
	else{
		block.current = block.next;
	}

	/*	block.positions �迭�� ������ �����Ѱų� next���� ������ ����
	block.currrent�� ������� blockExamples(6�� ����� ��ϵ���
	4���� ��� �����ǥ ��ġ�� ������ �迭)���� ���� �����ͼ�
	4 ���� ������ ��ġ�� ������ �Ӵϴ�.*/
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
	Block_Move �Լ��� ���Ǻ� �Դϴ�.
	�Ű� ������ ����ü block�� direction(����ڰ� �Է��� Ű��)�� �޾Ƽ�
	4 ���� ����Ű�� �ش��ϴ� ó���� �� ���� ��ȯ���ݴϴ�.
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
	Block_GetPositions() �Լ� �κ��Դϴ�.
	�Ű������� ����ü Block�� �޾�
	positions���� ���� ���⿡ �ش��ϴ� ��Ҹ� ��ȯ���ݴϴ�.
	=================================== */
Point* Block_GetPositions(Block block){
	return block.positions[block.direction];
	/* �Ű������� ���� block�� positions��
	�迭 [block.positions] ��° ��Ҹ� ��ȯ�մϴ�.
	��ȯ���� Point*(������)�� ������ positions��
	2���� �迭�̱� ������, N ��° ��Ҵ� 1���� �迭��
	�Ǳ� �����Դϴ�. */
}

/*	===================================
	Block_PrintNext()�Լ��� ���Ǻ� �Դϴ�.
	�Ű� ������ ����ü block�� int �� x,y�� �޽��ϴ�.
	�ش� ��ǥ(x,y)�� ���� ����
	��Ÿ���� �ϴ� ������ ���ݴϴ�.
	=================================== */
void Block_PrintNext(Block block, int x, int y){
	CursorUtil_GotoXY(x, y);
	/*	CursorUtil_GotoXY()�Լ��� �̿���
	x,y ��ǥ�� ��� Ŀ���� �̵� ���� �ݴϴ�. */
	printf("Next block : ");
	x += 3;
	y += 2;
	CursorUtil_GotoXY(x, y++);
	/*	switch ���� �̿��� ���� block.next�� Ȯ�� ��
	0 - 5 ���� �ش��ϴ� Ÿ���� �� ����� ����� �ݴϴ�. */
	switch (block.next){
	case 0:
		printf("�����");
		CursorUtil_GotoXY(x, y++);
		printf("        ");
		break;
	case 1:
		printf("      ��");
		CursorUtil_GotoXY(x, y++);
		printf("  ����");
		break;
	case 2:
		printf("    ���");
		CursorUtil_GotoXY(x, y++);
		printf("  ���  ");
		break;
	case 3:
		printf("  ���  ");
		CursorUtil_GotoXY(x, y++);
		printf("    ���");
		break;
	case 4:
		printf("��      ");
		CursorUtil_GotoXY(x, y++);
		printf("����  ");
		break;
	case 5:
		printf("  ���  ");
		CursorUtil_GotoXY(x, y++);
		printf("  ���  ");
		break;
	}
}

/*	===================================
	_Block_MoveToDown() �Լ� �κ��Դϴ�.
	�� �Լ��� ���ο����� ���̵��� static���� ���ǵǾ� �ֽ��ϴ�.
	�Ű� ������ block�� �޾� ������ ��ĭ �̵���Ű��
	��ȯ�ϴ� ������ ���ݴϴ�.
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
	Block_MoveToLeft() �Լ� �κ� �Դϴ�.
	�� �Լ��� �Ű������� ����ü block�� �޾�
	�������� ��ĭ �̵� ��Ű�� return, �� ��ȯ�����ִ�
	������ �մϴ�.
	=================================== */
static Block _Block_MoveToLeft(Block block){
	int i;
	int j;

	/*	for���� ���� POSITIONS_SIZE(=4) ����
	�����ϰ�, �ݺ����� �ѹ� ���� �ɶ�����
	block.positions(���ȿ� �ִ� 4�� ������ ��ǥ��)
	���� y ��ǥ ���� ���ҽ�ŵ�ϴ�. (�������� ��ĭ �̵�) */
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j].y--;
		}
	}
	return block;
}

/*	===================================
	Block_MoveToRight() �Լ� �κ��Դϴ�.
	�� �Լ��� ���� �Լ��� ���������� �Ű�������
	����ü block�� �޾� ���������� ��ĭ �̵���Ű��
	������ ���ݴϴ�.
	===================================*/
static Block _Block_MoveToRight(Block block){
	int i;
	int j;
	/*	for���� ���� block.positions�� y ��ǥ��
	���� ��ŵ�ϴ�. (���������� ��ĭ �̵�) */
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j].y++;
		}
	}
	return block;
}

/*	=========================================
	_Block_RotateRight() �Լ��� ���Ǻ� �Դϴ�.
	�� �Լ��� �Ű������� block(����ü)�� �޾�
	������(�ð� ����)���� ���� �����ְ�
	return, ��ȯ�ϴ� ������ �ϴ� �Լ��Դϴ�.
	========================================= */
static Block _Block_RotateRight(Block block){
	/*	block.direction�� ���� 1 ���� ��ŵ�ϴ�. (���� �������� ��ȯ�մϴ�.)
	%(������ �� ���)�� �� ������ , 0 -> 1 -> 2 -> 3 -> 0 ����
	��ȯ�ϰ� ����� ���� �� �Դϴ�.*/
	block.direction = (block.direction + 1) % POSITIONS_SIZE;
	return block;
}