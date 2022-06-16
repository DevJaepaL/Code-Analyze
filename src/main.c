// ���� ���� �ڵ� �κ� //
#include <stdio.h>
#include <conio.h>
#include "Constant.h"
#include "TetrisView.h"

int main(int argc, char* argv[]){
	TetrisView tetrisView; // tetrisView ���� ����(����ü TetrisView�� ����)
	TetrisView_StartGame(&tetrisView); // �Լ� ȣ��� ���� ����.
	int processType; // DIRECTION || DIRECT_DOWN || AUTO �� ������ ���� ����
	int direction; // ������ ������ ���� ����
	while (True){ // Ư�� ������ �������� �ʴ� �̻� ���ѹݺ� �ϵ��� while�� ���.
		processType = AUTO; // �Է°��� ������ �ڵ������� ���� �ڵ������� �����̱�.
		direction = DOWN; //			''''			�Ʒ� �������� �������� �ϱ�.
		if (_kbhit()){ // _khbit() �Լ� ȣ���Ͽ� ������� Ű �Է¿� ���۰� �ִ��� üũ.
			int key = _getch(); // �Լ��� key��� ������ �ֱ�.
			if (_kbhit()){ // ���� Ű �Է��� �߰ߵǸ�
				key = _getch(); // _getch() �Լ� ȣ���Ͽ� ������� Ű���� �ҷ��ɴϴ�.
				switch (key){ // �б⸦ ������ Ű���� ���� ������ �ٲ�� �����մϴ�.

				/*	����Ű�� �Է� ���ۿ� 2���� ���� �������� ���ٰ� �մϴ�.
					ù��°�� 0xE0(=224), �׸��� ����Ű�� ���� ���� ��,
					�ΰ��� ���� ���� ������ �� ���� �Է¹��۷κ���
					���� �����ͼ� Ȯ���ؾ��մϴ�. �ｼ,
					17���ٿ��� _kbhit() �Լ��� ȣ�� ��, ��ȯ ���� 0 �ƴϸ�
					�� �� �� �� ����Ű�� �ϳ��� �Է�ٴ� ���̱� ������
					switch-case ������ �Է°��� ���� �б⸶��
					processType �� direction ���� �������ݴϴ�.*/
				case UP_KEY_CODE: 
					processType = DIRECTION;
					direction = UP;
					break;
				case LEFT_KEY_CODE:
					processType = DIRECTION;
					direction = LEFT;
					break;
				case RIGHT_KEY_CODE:
					processType = DIRECTION;
					direction = RIGHT;
					break;
				case DOWN_KEY_CODE:
					processType = DIRECTION;
					direction = DOWN;
					break;
				}
			}
			else{ // ���ۿ� Ű ���� �ϳ��� �ִٸ� �����̽����� ������ Ȯ�� ��
				  // procesType�� DIRECT_DOWN�� �������ݴϴ�.
				switch (key){
				case SPACE_BAR_KEY_CODE:
					processType = DIRECT_DOWN;
				}
			}
		}
		TetrisView_ProcessGame(&tetrisView, processType, direction);
		/*	����ڷκ��� �Է� ���� Ű�� �з� ó���� ������ ���
			TetrisView_ProcessGame()�Լ��� ȣ���� ��Ʈ���� ������ ��������ݴϴ�.
			�Ű������� tetrisView,processType,direction�� �Ѱ��ݴϴ�. */
	}
	return 0; // ���α׷� ����
}