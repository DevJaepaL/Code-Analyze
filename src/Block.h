/* ������� �ߺ� ������ �����ϴ� Guard. */
#ifndef _BLOCK_H
#define _BLOCK_H

/*	Point ����ü�� ���Ǻ� �Դϴ�.
	����δ� x ��ǥ �� y ��ǥ�� ������ �ֽ��ϴ�. */
typedef struct _point{
	int x;
	int y;
}Point;

/*	Block ����ü�� ����� ����
	POSITIONS �迭�� ũ�⸦ ����� �����մϴ�.(=4) */
#define POSITIONS_SIZE 4

/*	=========================================
	Block ����ü�� ���Ǻ� �Դϴ�.
	Block ����ü�� ��Ʈ�������� ���Ǵ� �ϳ��� ���� ��Ÿ���ϴ�.
	
	(���� ��, ���� ���� ��) ����δ� positions[��� 4] �迭�� �ִµ�,
	�̴� 4 X 4 ũ���� 2���� �迭�� Point��(x,y ��ǥ)�� ���� �Ǿ��ֽ��ϴ�.
	�̴� �ϳ��� ���� 4 ���� ���簢������ �����Ǿ� �ֱ� ������,
	4���� ���ڿ� x,y ��ǥ�� ��� ���� ���˴ϴ�.
	
	int �ڷ����� current�� ���� Ÿ���� ���� �߿��� ���� �̵��ϴ�
	���� Ÿ���� ��Ÿ���ϴ�.
	int �ڷ����� next�� ���� Ÿ���� ���� �߿��� ������ ���� ����
	Ÿ���� ��Ÿ���ϴ�.
	int �ڷ����� direction�� ���� ������ ��Ÿ����, �����¿츦 ��Ÿ���ϴ�. 
	========================================= */
typedef struct _block{
	Point positions[POSITIONS_SIZE][POSITIONS_SIZE];
	int current;
	int next;
	int direction;
}Block;

// �ҽ� ���Ͽ��� ���� ����ü Block ���� �Լ� ����� �Դϴ�.
Block Block_Make(int isFirst, Block block);
Block Block_Move(Block block, int direction);
Point* Block_GetPositions(Block block);
void Block_PrintNext(Block block, int x, int y);

// ������� �ߺ� ������ �����ϴ� Guard.
#endif