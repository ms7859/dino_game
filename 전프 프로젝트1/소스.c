/* stdbool.h standard header */

#ifndef _STDBOOL
#define _STDBOOL

#define __bool_true_false_are_defined  1

#ifndef __cplusplus

#define bool   _Bool
#define false  0
#define true   1

#endif /* __cplusplus */

#endif /* _STDBOOL */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>

#define DINO_BOTTOM_Y 12
#define TREE_BOTTOM_Y 24
#define OBSTACLE_START_X 90
#define STONE_TOP_Y_M 11
#define STONE_TOP_Y_H 5
#define STONE_TOP_Y_L 20

// �߰� ��� ����
#define MAX_NUM_OF_LIVES 3
#define MAX_NUM_OF_SKILLS 5
#define LIFE_CHARGE_COOL 500
#define SKILL_CHARGE_COOL 300

#define ENTER 13
#define ARROW_KEY 224
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

char map[25][100]; // �ܼ�â ũ��

char stance1[13][100]; // �������� �޹�
char stance2[13][100]; // �������� ������
char stance3[10][100]; // ���帱�� �޹�
char stance4[10][100]; // ���帱�� ������
char stance5[13][100]; // �Ͼ�� ����
char stance6[10][100]; // �ɾƼ� ����

// �߰� ��� ���� - ����, ������ ��ġ
int level;
int stonePos[3] = { STONE_TOP_Y_M, STONE_TOP_Y_H, STONE_TOP_Y_L };

enum Obs
{
	Stone, Tree
};
enum Leg
{
	Right, Left
};

typedef struct _status1
{
	int dinoY;
	bool isJumping;
	bool isBottom;
	bool isDown;
	bool legFlag;

	// �߰� ��� ����
	int numOfSkills;
	int skillTimer;
} DinoStatus;
typedef struct _status2
{
	bool obstacleOn;
	bool obsKind;
	int obsSpeed;
	int obstacleX; 
	int obstacleY; // �߰� ��� ����
	int treeHeight;
} ObsStatus;

// �߰� ��� ���� - ��ŷ
typedef struct _ranking
{
	char initial[4];
	int score;
} Ranking;

// �ʱ�ȭ
void init(DinoStatus* dinoStatus, ObsStatus* obsStatus);

// �׸� ���� �Լ�
void DrawDinoStand(DinoStatus* status);
void DrawDinoDown(DinoStatus* status);
void DrawGameOver(const int score);
void DrawTree(const int treeX, const int treeHeight);
void DrawStone(const int StoneX, const int StoneY);
void DrawObstacle(const ObsStatus status);

// �ý��� ���� �Լ� (�м� �ʿ� X)
void SetConsoleView();
void GotoXY(int x, int y);
void SetCursorVisible(int visible);

// �浹 ����
bool CollisionDetection(const DinoStatus dinoStatus, const ObsStatus obsStatus);

// ����, ��ֹ� ����
void* OperateDino(DinoStatus* status);
void ManageJump(DinoStatus* status);
void ManageObstacle(ObsStatus* status);

// �߰� ��� ���� - ��ŷ
void DrawRanking(Ranking rank[]);
void rankingUpdate(Ranking rank[], int score);


int main(void)
{
	SetConsoleView();

	DinoStatus dinoStatus;
	ObsStatus obsStatus;
	Ranking ranking[10] = { 0, };

	bool gameOver;
	int score, life, collisionTimer;
	int lifeCtrl = 0, skillCtrl = 0;
	int keyInput = 0, arrowKeyInput = 0;
	int menu, cursorX, cursorY;
	void(*DrawDino)(DinoStatus * status) = DrawDinoStand;

	while (true)
	{
		gameOver = false, score = 0, life = MAX_NUM_OF_LIVES, collisionTimer = 0;
		lifeCtrl = skillCtrl = 0;
		init(&dinoStatus, &obsStatus);

		system("cls");
		printf("�� 1. Game start\n");
		printf("   2. How to play\n");
		printf("   3. Ranking\n");
		printf("   4. Exit\n");

		cursorX = cursorY = keyInput = arrowKeyInput = 0;
		GotoXY(cursorX, cursorY);

		/*** �޴� ���� ***/
		while (1)
		{
			if (_kbhit())
			{
				keyInput = _getch();
				if (keyInput == ENTER)
				{
					menu = cursorY + 1;
					system("cls");
					break;
				}
				else if (keyInput == ARROW_KEY)
				{
					arrowKeyInput = _getch();
					if (arrowKeyInput == UP)
					{
						if (cursorY > 0)
						{
							GotoXY(0, cursorY--);
							printf("  ");
							GotoXY(0, cursorY);
							printf("��");
						}
					}
					else if (arrowKeyInput == DOWN)
					{
						if (cursorY < 3)
						{
							GotoXY(0, cursorY++);
							printf("  ");
							GotoXY(0, cursorY);
							printf("��");
						}
					}
				}
			}
		}

		if (menu == 1)
		{
			/*** ���� ���� ***/
			while (true)
			{
				/*** ����, ���� ���� ***/
				if (dinoStatus.isDown) score += 1;
				else				   score += 2;
				level = score / 100 + 1;
				GotoXY(60, 0);
				printf("level: %d   ", level);
				printf("score: %3d   ", score);

				/*** ������ ����(����) ***/
				if ((score % LIFE_CHARGE_COOL == 0) || (score % LIFE_CHARGE_COOL == 1))
				{
					if (score - lifeCtrl != 1)
					{
						if (life < MAX_NUM_OF_LIVES) life++;
						lifeCtrl = score;
					}
				}
				for (int i = 0; i < life; i++)						printf("�� ");
				for (int i = 0; i < MAX_NUM_OF_LIVES - life; i++)	printf("�� ");

				/*** ��ų ����(����) ***/
				if ((score % SKILL_CHARGE_COOL == 0) || (score % SKILL_CHARGE_COOL == 1))
				{
					if (score - skillCtrl != 1)
					{
						if (dinoStatus.numOfSkills < MAX_NUM_OF_SKILLS)
							dinoStatus.numOfSkills++;
						skillCtrl = score;
					}
				}
				GotoXY(84, 2);
				for (int i = 0; i < dinoStatus.numOfSkills; i++)						printf("�� ");
				for (int i = 0; i < MAX_NUM_OF_SKILLS - dinoStatus.numOfSkills; i++)	printf("�� ");

				/*** ����, ��ֹ� ���� ***/
				DrawDino = OperateDino(&dinoStatus);
				ManageJump(&dinoStatus);
				ManageObstacle(&obsStatus);

				/*** �浹 ���� ***/
				if (collisionTimer == 0 && dinoStatus.skillTimer == 0)
				{
					if (CollisionDetection(dinoStatus, obsStatus))
					{
						life--;
						collisionTimer = 6;
					}
				}
				else
				{
					if (collisionTimer > 0)			collisionTimer--;
					if (dinoStatus.skillTimer > 0)	dinoStatus.skillTimer--;
				}

				/*** ���� ���� ***/
				if (life == 0) gameOver = true;
				if (gameOver)
				{
					DrawGameOver(score);
					rankingUpdate(ranking, score);

					GotoXY(0, 16);
					printf("����ȭ������ ���ư����� �ƹ� Ű�� �����ּ���.");
					while (!_kbhit()) Sleep(100); if (_getch() == ARROW_KEY) _getch();

					break;
				}

				/*** ���� �׸��� ***/
				if (collisionTimer % 2 == 0) DrawDino(&dinoStatus);
				DrawObstacle(obsStatus);
				Sleep(40);
				system("cls");
			}
		}
		else if (menu == 2)
		{
			printf("1. Z�� ������ ����, X�� ������ ������ �ڼ��� �ٲٰ� �˴ϴ�.\n");
			printf("2. ��ֹ��� �΋H���� �������� �����ϸ�, �������� 0�� �Ǹ� ������ �й��մϴ�.\n");
			printf("3. Enter�� ���� ��ų�� ����� �� ������, ��ų�� ����ϸ� 15tick���� ��ֹ��� �΋H���� �ʽ��ϴ�.\n");
			printf("4. 300������ �߰� ��ų�� ���޵Ǹ�, 500������ �߰� �������� ���޵˴ϴ�.\n");
			printf("5. �� tick���� ������ ����������� 1��, �Ͼ������ 2���� ȹ���մϴ�.\n\n");

			printf("����ȭ������ ���ư����� �ƹ� Ű�� �����ּ���.");
			while (!_kbhit()) Sleep(100); if (_getch() == ARROW_KEY) _getch();
		}
		else if (menu == 3)
		{
			DrawRanking(ranking);

			printf("����ȭ������ ���ư����� �ƹ� Ű�� �����ּ���.");
			while (!_kbhit()) Sleep(100); if (_getch() == ARROW_KEY) _getch();
		}
		else
		{
			printf("������ �����մϴ�.");
			break;
		}
	}

	return 0;
}


// ����, ��ֹ� �ʱ�ȭ
void init(DinoStatus* dinoStatus, ObsStatus* obsStatus)
{
	dinoStatus->dinoY = DINO_BOTTOM_Y;
	dinoStatus->isJumping = false;
	dinoStatus->isBottom = true;
	dinoStatus->isDown = false;
	dinoStatus->legFlag = true;

	dinoStatus->numOfSkills = 3;
	dinoStatus->skillTimer = 0;

	obsStatus->obstacleOn = false;
	obsStatus->obsSpeed = 0;
	obsStatus->obstacleX = OBSTACLE_START_X;
}

// ���ִ� ���� �׸���
void DrawDinoStand(DinoStatus* status)
{
	GotoXY(0, status->dinoY);
	printf("        $$$$$$$ \n");
	printf("       $$ $$$$$$\n");
	printf("       $$$$$$$$$\n");
	printf("$      $$$      \n");
	printf("$$     $$$$$$$  \n");
	printf("$$$   $$$$$     \n");
	printf(" $$  $$$$$$$$$$ \n");
	printf(" $$$$$$$$$$$    \n");
	printf("  $$$$$$$$$$    \n");
	printf("    $$$$$$$$    \n");
	printf("     $$$$$$     \n");

	if (status->dinoY < DINO_BOTTOM_Y)
	{
		printf("     $    $      \n");
		printf("     $$   $$       ");
	}
	else
	{
		if (status->legFlag == Left)
		{
			printf("     $    $$$    \n");
			printf("     $$          ");
			status->legFlag = Right;
		}
		else
		{
			printf("     $$$  $     \n");
			printf("          $$    ");
			status->legFlag = Left;
		}
	}
}
void DrawDinoStandGhost(DinoStatus* status)
{
	GotoXY(0, status->dinoY);
	printf("        ^^^^^^^ \n");
	printf("       ^^ ^^^^^^\n");
	printf("       ^^^^^^^^^\n");
	printf("^      ^^^      \n");
	printf("^^     ^^^^^^^  \n");
	printf("^^^   ^^^^^     \n");
	printf(" ^^  ^^^^^^^^^^ \n");
	printf(" ^^^^^^^^^^^    \n");
	printf("  ^^^^^^^^^^    \n");
	printf("    ^^^^^^^^    \n");
	printf("     ^^^^^^     \n");

	if (status->dinoY < DINO_BOTTOM_Y)
	{
		printf("     ^    ^      \n");
		printf("     ^^   ^^       ");
	}
	else
	{
		if (status->legFlag == Left)
		{
			printf("     ^    ^^^    \n");
			printf("     ^^          ");
			status->legFlag = Right;
		}
		else
		{
			printf("     ^^^  ^     \n");
			printf("          ^^    ");
			status->legFlag = Left;
		}
	}
}

// ���帰 ���� �׸���
void DrawDinoDown(DinoStatus* status)
{
	GotoXY(0, status->dinoY + 3);
	printf("                 $$$$$$$$\n");
	printf("$               $$$  $$$$$\n");
	printf("$$             $$$$$$$$$$$ \n");
	printf(" $$$          $$$$$$$$     \n");
	printf("  $$$        $$$$$$$$$$ \n");
	printf("   $$$$$$$$$$$$$$$$    \n");
	printf("     $$$$$$$$$$$$$$    \n");
	printf("       $$$$$$$$$    \n");


	if (status->dinoY < DINO_BOTTOM_Y)
	{
		printf("        $    $     \n");
		printf("        $$   $$      ");
	}
	else
	{
		if (status->legFlag == Left)
		{
			printf("        $    $$    \n");
			printf("        $$          ");
			status->legFlag = Right;
		}
		else
		{
			printf("        $$  $     \n");
			printf("            $$    ");
			status->legFlag = Left;
		}
	}
}
void DrawDinoDownGhost(DinoStatus* status)
{
	GotoXY(0, status->dinoY + 3);
	printf("                 ^^^^^^^^\n");
	printf("^               ^^^  ^^^^^\n");
	printf("^^             ^^^^^^^^^^^ \n");
	printf(" ^^^          ^^^^^^^^     \n");
	printf("  ^^^        ^^^^^^^^^^ \n");
	printf("   ^^^^^^^^^^^^^^^^    \n");
	printf("     ^^^^^^^^^^^^^^    \n");
	printf("       ^^^^^^^^^    \n");


	if (status->dinoY < DINO_BOTTOM_Y)
	{
		printf("        ^    ^     \n");
		printf("        ^^   ^^      ");
	}
	else
	{
		if (status->legFlag == Left)
		{
			printf("        ^    ^^    \n");
			printf("        ^^          ");
			status->legFlag = Right;
		}
		else
		{
			printf("        ^^  ^     \n");
			printf("            ^^    ");
			status->legFlag = Left;
		}
	}
}

// GAME OVER�� �� ���ӿ��� ǥ���ϱ�
void DrawGameOver(const int score)
{
	system("cls");
	int x = 18;
	int y = 8;

	GotoXY(2 * x, y);
	printf("===========================");
	GotoXY(2 * x, y + 1);
	printf("======G A M E O V E R======");
	GotoXY(2 * x, y + 2);
	printf("===========================");

	printf("\n\n");
	printf("  Score: %d", score);
	printf("\n\n\n\n\n\n\n");

	printf("����Ϸ��� �ƹ� Ű�� �����ּ���...");
	while (!_kbhit()) Sleep(100); if(_getch() == ARROW_KEY) _getch();
}

// ��ֹ� �׸���
void DrawObstacle(const ObsStatus status)
{
	if (status.obsKind == Stone) DrawStone(status.obstacleX, status.obstacleY);
	else 						 DrawTree(status.obstacleX, status.treeHeight);

}
void DrawTree(const int treeX, const int treeHeight)
{
	GotoXY(treeX, TREE_BOTTOM_Y);
	printf("$$$$");

	for (int i = 1; i < treeHeight; i++)
	{
		GotoXY(treeX, TREE_BOTTOM_Y - i);
		printf(" $$ ");
	}
}
void DrawStone(const int StoneX, const int StoneY)
{
	GotoXY(StoneX, StoneY);
	printf("@@@@");
	GotoXY(StoneX, StoneY + 1);
	printf("@@@@");
	GotoXY(StoneX, StoneY + 2);
	printf("@@@@");
}

// �ܼ� â ũ��� ���� ����, Ŀ�� �����, Ŀ�� �̵�
void SetConsoleView()
{
	system("mode con:cols=100 lines=25");
	system("Google Dinosaurs. C project2");
	SetCursorVisible(0);
}
void SetCursorVisible(int visible)
{
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = visible;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}
void GotoXY(int x, int y)
{
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// �浹����
bool CollisionDetection(const DinoStatus dinoStatus, const ObsStatus obsStatus)
{
	int dinoR, dinoT, dinoB = dinoStatus.dinoY + 12;
	int obsL = obsStatus.obstacleX, obsT = 0, obsB = 0;

	// ���� ��ġ ���ϱ�
	if (dinoStatus.isDown)
	{
		dinoT = dinoStatus.dinoY + 3;
		dinoR = 25;
	}
	else
	{
		dinoT = dinoStatus.dinoY;
		dinoR = 15;
	}

	// ��ֹ� ��ġ ���ϱ�
	if (obsStatus.obsKind == Tree)
	{
		obsB = 24;
		obsT = 24 - obsStatus.treeHeight + 1;
	}
	else
	{
		obsT = obsStatus.obstacleY;
		obsB = obsT + 2;
	}

	if (obsL <= dinoR) return !(dinoB < obsT || dinoT > obsB);
	return false;
}

// ����, ��ֹ� ����
void* OperateDino(DinoStatus* status)
{
	static void(*Execute)(DinoStatus * status) = DrawDinoStand;
	char getKey;

	// ����ڿ��� Ű���� �Է��� �޾� z�� ���� ���� Ȱ��ȭ, x�� ���� �ڼ� ���, ENTER�� ���� ��ų ���
	if (_kbhit())
	{
		getKey = _getch();

		if ((getKey == 'z' || getKey == 'Z') && status->isBottom)
		{
			status->isJumping = true;
		}
		else if (getKey == 'x' || getKey == 'X')
		{
			status->isDown = !status->isDown;
		}
		else if (getKey == ENTER)
		{
			if (status->numOfSkills > 0)
			{
				status->skillTimer = 15;
				status->numOfSkills--;
			}
		}
	}

	// dino ���¿� ���� ��� �Լ� ����
	if (status->skillTimer == 0)
	{
		if (status->isDown) Execute = DrawDinoDown;
		else				Execute = DrawDinoStand;
	}
	else
	{
		if (status->isDown) Execute = DrawDinoDownGhost;
		else				Execute = DrawDinoStandGhost;
	}

	return Execute;
}
void ManageObstacle(ObsStatus* status)
{
	int random, idx;
	random = (rand() % 2);

	if (!status->obstacleOn)
	{
		status->obstacleOn = true;
		status->obsKind = random;
		status->obsSpeed = 4 + (level >= 8 ? 8 : level);
		status->obstacleX = OBSTACLE_START_X;
		status->treeHeight = rand() % 3 + 3;
		if (status->obsKind == Stone)
		{
			if (level >= 10)	 idx = rand() % 3;
			else if (level >= 8) idx = rand() % 2;
			else				 idx = 0;
			status->obstacleY = stonePos[idx];
		}
	}
	else
	{
		status->obstacleX -= status->obsSpeed;
	}

	if (status->obstacleX - status->obsSpeed < 0)
		status->obstacleOn = false;
}
void ManageJump(DinoStatus* status)
{
	static int jumpAndFallingSpeed = 3;

	if (status->isJumping)
	{
		status->dinoY -= jumpAndFallingSpeed;
		status->isBottom = false;
	}
	else
	{
		if (!status->isBottom)
			status->dinoY += jumpAndFallingSpeed;
	}

	if (status->dinoY == 0)
		status->isJumping = false;
	else if (status->dinoY == DINO_BOTTOM_Y)
		status->isBottom = true;
}

// �߰� ��� ���� - ��ŷ
void DrawRanking(Ranking rank[])
{
	system("cls");
	printf("===============\n");
	printf("=== RANKING ===\n");
	printf("===============\n\n");
	for (int i = 0; i < 10; i++)
	{
		if (rank[i].initial[0] == 0) printf("%2d. ___ : %d\n", i + 1, rank[i].score);
		else printf("%2d. %s : %d\n", i + 1, rank[i].initial, rank[i].score);
	}
}
void rankingUpdate(Ranking rank[], int score)
{
	int i, key = 0, arrowKey;
	int index = 0, cursorX, cursorY;

	if (score > rank[9].score)
	{
		// ���� ���ھ��� ���� ã��
		for (i = 9; i >= 0; i--)
			if (score <= rank[i].score)
				break;
		i++;
		
		// ��ŷ �ٲ� �� ���� ����
		for (int j = 9; j > i; j--) rank[j] = rank[j - 1];
		rank[i].score = score;
		rank[i].initial[0] = 'A'; rank[i].initial[1] = 'A'; rank[i].initial[2] = 'A';
		cursorX = 4, cursorY = i + 4;

		// �̴ϼ� �Է¹ޱ�
		DrawRanking(rank);
		GotoXY(4, i + 4);
		SetCursorVisible(1);
		printf("AAA\b\b\b");
		while (1)
		{
			if (_kbhit())
			{
				key = _getch();
				if (key == ARROW_KEY)
				{
					arrowKey = _getch();
					if (arrowKey == UP)
					{
						rank[i].initial[index]++;
						if (rank[i].initial[index] == 'Z' + 1)
							rank[i].initial[index] = 'A';
						printf("%c\b", rank[i].initial[index]);
					}
					else if (arrowKey == DOWN)
					{
						rank[i].initial[index]--;
						if (rank[i].initial[index] == 'A' - 1)
							rank[i].initial[index] = 'Z';
						printf("%c\b", rank[i].initial[index]);
					}
					else if (arrowKey == LEFT)
					{
						if (index > 0) index--;
						GotoXY(4 + index, cursorY);
					}
					else if (arrowKey == RIGHT)
					{
						if (index < 2) index++;
						GotoXY(4 + index, cursorY);
					}
				}
				else if ('A' <= key && key <= 'Z' || 'a' <= key && key <= 'z')
				{
					if ('a' <= key && key <= 'z') key -= ('a' - 'A');
					rank[i].initial[index] = key;
					printf("%c\b", key);
				}
				else if(key == ENTER)
				{
					break;
				}
			}
		}
	}
	else
	{
		DrawRanking(rank);
	}
	SetCursorVisible(0);
}