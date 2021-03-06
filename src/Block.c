// 琶推廃 奄沙 虞戚崎君軒 伯希 貢 紫遂切 持失 伯希 稽球.
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include "Block.h"
#include "Util.h"
#include "Constant.h"

// 持失 吃 呪 赤澗 食君 鷺薫 展脊級税 紫戚綜研 雌呪 識情.
#define BLOCK_EXAMPLES_SIZE 6

/*	26 ~ 74 虞昔
	持失吃 呪 赤澗 鷺薫推社級聖 耕軒 雌呪 壕伸稽 舛税 杯艦陥.
	const研 細昔 戚政澗, 雌呪鉢研 是敗戚壱 static精 薄仙 督析 鎧拭辞幻
	鎧採旋生稽 紫遂馬奄 是背辞脊艦陥. 恥 6鯵税 展脊税 鷺薫級聖
	雌馬疎酔 号狽拭 企廃 雌企疎妊稽 煽舌背 兜柔艦陥.

	森研 級檎, けけけけ 人 旭精 乞丞税 鷺薫精 是楕 号狽析 凶
	(0,5)(0,6)(0,7)(0,8)税 是帖研 亜増艦陥.
	幻鉦 噺穿馬食 神献楕 号狽 析 井酔 (-2,6)(-1,6)(0,6)(1,6)税 是帖研 亜増艦陥.
	廃腰 希 噺穿 拝 井酔 据掘 乞丞生稽 宜焼臣 依戚壱,
	原走厳生稽 廃腰 希 噺穿馬食 図楕 号狽析 凶澗 神献楕引 企暢鞠澗 葵聖 亜走惟 吃 依脊艦陥.
	戚坦軍 x疎妊澗 0聖 奄層生稽, y疎妊澗 砺闘軒什 己税 掻娃 走繊聖 奄層生稽
	馬食 唖唖 6鯵税 乞丞 紺稽, 雌馬疎酔稽 噺穿 梅聖 凶税 疎妊 是帖研
	壕伸拭 耕軒 煽舌背 兜柔艦陥. */
const static Point blockExamples[BLOCK_EXAMPLES_SIZE][POSITIONS_SIZE][POSITIONS_SIZE] = {
	//けけけけ
	{
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -2, 6 }, { -1, 6 }, { 0, 6 }, { 1, 6 } },
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -2, 6 }, { -1, 6 }, { 0, 6 }, { 1, 6 } }
	},
	//    け
	//けけけ
	{
		{ { 0, 8 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 6 } },
		{ { -1, 6 }, { -1, 7 }, { 0, 7 }, { 1, 7 } }
	},
	//  けけ
	//けけ
	{
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},
	//けけ
	//  けけ
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } }
	},
	//け
	//けけけ
	{
		{ { 0, 6 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { -1, 7 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 6 } }
	},
	//けけ
	//けけ
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } }
	}
};

// Block.c 社什 督析 鎧採拭辞幻 紫遂吃 鎧採 敗呪税 識情.
static Block _Block_MoveToDown(Block block);
static Block _Block_MoveToLeft(Block block);
static Block _Block_MoveToRight(Block block);
static Block _Block_RotateRight(Block block);

/*	================================
	Block_Make 敗呪税 舛税採 脊艦陥.
	古鯵痕呪稽 isFirst(鷺系戚 砺闘軒什亜 獣拙馬壱 貝 板拭
	亜舌 坦製拭 幻級嬢走澗走税 食採)研 閤壱, 幻級嬢遭 block聖 鋼発.
	================================ */
Block Block_Make(int isFirst, Block block){
	int i;
	int j;

	/*	srand人 time 敗呪研 紫遂背 貝呪 持失 穿 獣球研 鹿嬢掃艦陥.
	益軒壱 isFirst亜 True析 井酔, block.current拭 貝呪研
	持失馬食 煽舌馬壱(0-5 葵 紫戚税 舛呪), 坦製戚 焼諌 井酔
	block.current拭澗 戚穿拭 持失背 砧醸揮, block.next研
	亜閃人辞 煽舌杯艦陥.

	聡, 坦製 鷺薫聖 持失馬澗 井酔拭澗 current研 貝呪稽 持失背醤 馬壱,
	砧腰属 採斗澗 陥獣 貝呪稽 持失馬澗惟 焼観, next拭辞
	亜閃神檎 吉陥澗 税耕脊艦陥.*/
	srand((unsigned int)time(NULL));
	if (isFirst){
		block.current = rand() % BLOCK_EXAMPLES_SIZE;
	}
	else{
		block.current = block.next;
	}

	/*	block.positions 壕伸拭 是拭辞 持失廃暗蟹 next拭辞 亜閃紳 葵昔
	block.currrent研 奄鋼生稽 blockExamples(6鯵 乞丞税 鷺系級聖
	4号狽 乞砧 雌企疎妊 是帖研 煽舌廃 壕伸)拭辞 鷺薫聖 亜閃人辞
	4 鯵税 雌切税 是帖研 煽舌背 脚艦陥.*/
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
	Block_Move 敗呪税 舛税採 脊艦陥.
	古鯵 痕呪稽 姥繕端 block引 direction(紫遂切亜 脊径廃 徹葵)聖 閤焼辞
	4 亜走 号狽徹拭 背雁馬澗 坦軒研 廃 鷺薫聖 鋼発背掃艦陥.
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
	Block_GetPositions() 敗呪 採歳脊艦陥.
	古鯵痕呪稽 姥繕端 Block聖 閤焼
	positions拭辞 薄仙 号狽拭 背雁馬澗 推社研 鋼発背掃艦陥.
	=================================== */
Point* Block_GetPositions(Block block){
	return block.positions[block.direction];
	/* 古鯵痕呪稽 閤精 block税 positions税
	壕伸 [block.positions] 腰属 推社研 鋼発杯艦陥.
	鋼発莫戚 Point*(匂昔斗)昔 戚政澗 positions亜
	2託据 壕伸戚奄 凶庚拭, N 腰属 推社澗 1託据 壕伸戚
	鞠奄 凶庚脊艦陥. */
}

/*	===================================
	Block_PrintNext()敗呪税 舛税採 脊艦陥.
	古鯵 痕呪稽 姥繕端 block引 int 莫 x,y研 閤柔艦陥.
	背雁 疎妊(x,y)拭 陥製 鷺薫戚
	蟹展蟹惟 馬澗 蝕拝聖 背掃艦陥.
	=================================== */
void Block_PrintNext(Block block, int x, int y){
	CursorUtil_GotoXY(x, y);
	/*	CursorUtil_GotoXY()敗呪研 戚遂背
	x,y 疎妊稽 窒径 朕辞研 戚疑 獣佃 掃艦陥. */
	printf("Next block : ");
	x += 3;
	y += 2;
	CursorUtil_GotoXY(x, y++);
	/*	switch 庚聖 戚遂背 痕呪 block.next研 溌昔 板
	0 - 5 猿走 背雁馬澗 展脊税 鷺薫 乞丞聖 窒径背 掃艦陥. */
	switch (block.next){
	case 0:
		printf("＝＝＝＝");
		CursorUtil_GotoXY(x, y++);
		printf("        ");
		break;
	case 1:
		printf("      ＝");
		CursorUtil_GotoXY(x, y++);
		printf("  ＝＝＝");
		break;
	case 2:
		printf("    ＝＝");
		CursorUtil_GotoXY(x, y++);
		printf("  ＝＝  ");
		break;
	case 3:
		printf("  ＝＝  ");
		CursorUtil_GotoXY(x, y++);
		printf("    ＝＝");
		break;
	case 4:
		printf("＝      ");
		CursorUtil_GotoXY(x, y++);
		printf("＝＝＝  ");
		break;
	case 5:
		printf("  ＝＝  ");
		CursorUtil_GotoXY(x, y++);
		printf("  ＝＝  ");
		break;
	}
}

/*	===================================
	_Block_MoveToDown() 敗呪 採歳脊艦陥.
	戚 敗呪澗 鎧採拭辞幻 床戚亀系 static生稽 舛税鞠嬢 赤柔艦陥.
	古鯵 痕呪稽 block聖 閤焼 購生稽 廃牒 戚疑獣徹壱
	鋼発馬澗 蝕拝聖 背掃艦陥.
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
	Block_MoveToLeft() 敗呪 採歳 脊艦陥.
	戚 敗呪澗 古鯵痕呪稽 姥繕端 block聖 閤焼
	図楕生稽 廃牒 戚疑 獣徹壱 return, 聡 鋼発獣佃爽澗
	蝕拝聖 杯艦陥.
	=================================== */
static Block _Block_MoveToLeft(Block block){
	int i;
	int j;

	/*	for庚聖 搭背 POSITIONS_SIZE(=4) 猿走
	装亜馬壱, 鋼差庚戚 廃腰 叔楳 吃凶原陥
	block.positions(鷺薫照拭 赤澗 4鯵 雌切税 疎妊級)
	級税 y 疎妊 葵聖 姶社獣典艦陥. (図楕生稽 廃牒 戚疑) */
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j].y--;
		}
	}
	return block;
}

/*	===================================
	Block_MoveToRight() 敗呪 採歳脊艦陥.
	戚 敗呪亀 是税 敗呪人 原濁亜走稽 古鯵痕呪稽
	姥繕端 block聖 閤焼 神献楕生稽 廃牒 戚疑獣徹澗
	蝕拝聖 背掃艦陥.
	===================================*/
static Block _Block_MoveToRight(Block block){
	int i;
	int j;
	/*	for庚聖 搭背 block.positions税 y 疎妊研
	装亜 獣典艦陥. (神献楕生稽 廃牒 戚疑) */
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j].y++;
		}
	}
	return block;
}

/*	=========================================
	_Block_RotateRight() 敗呪税 舛税採 脊艦陥.
	戚 敗呪澗 古鯵痕呪稽 block(姥繕端)聖 閤焼
	神献楕(獣域 号狽)生稽 鷺薫聖 宜形爽壱
	return, 鋼発馬澗 蝕拝聖 馬澗 敗呪脊艦陥.
	========================================= */
static Block _Block_RotateRight(Block block){
	/*	block.direction税 葵聖 1 装亜 獣典艦陥. (陥製 号狽生稽 穿発杯艦陥.)
	%(蟹袴走 葵 域至)聖 廃 戚政澗 , 0 -> 1 -> 2 -> 3 -> 0 生稽
	授発馬惟 幻級奄 是廃 依 脊艦陥.*/
	block.direction = (block.direction + 1) % POSITIONS_SIZE;
	return block;
}