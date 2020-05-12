//Copyright c. 2020 jackjoo. All right reserved.
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define GO 1
#define OVER -1
#define LEVEL_UP 0

#define UP 53		//키보드 위
#define DOWN 50		//키보드 아래
#define LEFT 49 	//키보드 왼
#define RIGHT 51	//키보드 오른
#define SPACEBAR 32	//키보드 스페이스바

#define MIN_Y 2		// 위 끝
#define MAX_Y 25	// 아래 끝
#define MIN_X 5		// 왼 끝
#define MAX_X 22	// 오른쪽 끝

#define EMPTY 0		// 빈 부분
#define WALL 1		// 벽
#define BLOCK 2		// 블록
#define BLOCK_D 3	// 끝난 블록
#define CEILING -1	// 천장(MIN_Y)
#define CRUSH 0

#define _DO 261.6256	// 도
#define _RAE 293.6648	// 레
#define _MI 329.6276	// 미
#define _PA 349.2282	// 파
#define _SOL 391.9954	// 솔
#define _RA 440.0000	// 라
#define _SI 493.8833	// 시

int bx;	//블록의 현재 x
int by; //블록의 현재 y
int b_type; //블록 종류를 저장 
int b_rotation; //블록 회전값 저장 
int game = GO;
int level = 1;
int speed = 100;
int score = 0;
int score_goal = 1000;

char key;
int key_able=1;
void check_key(void);
void reset_map(void);
void draw_map(void);
void new_block(void);
void move_block(int dir);
void done_block(void);
int check_crush(int bx, int by, int b_rotation);
int check_crush_wall(int bx, int by, int b_rotation);
void check_continue(void);
void block_cycle(void);
void check_line(void);
void delete_line(int);
void title(void);
void update_board(void);
void check_score(void);
void game_notice(int);
void game_sound(int);
void check_rotation(void);

int game_map[MAX_Y][MAX_X];
int game_map_copy[MAX_Y][MAX_X];

int blocks[7][4][4][4] = {
{{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
{{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
{{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
{{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
{{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용 


void gotoxy(int x, int y) { //커서 이동 함수 
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hidecursor()	//커서 숨기는 함수
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

int main(void) {
	int menu=0;
	system("cls");
	hidecursor();
	reset_map();
	// 0. game start
	// 1. level change
	// 2. exit

	while (1) {
		title();

		if (_kbhit()) {
			menu = _getch();
			switch (menu)
			{
			case 27:
				printf("게임 종료\n");
				exit(0);
			default:
				//GAME
				game = GO;
				game_sound(GO);
				reset_map();
				draw_map();
				update_board();
				Sleep(1000);
				while (game == GO) {	//block setting
					block_cycle();
				}
			}
		}

		
	}
	return 0;
}

void block_cycle(void) {
	int i;

	new_block();
	draw_map();

	//BLOCK DOWN
	while (1) {
		for (i = 0; i < 5; i++) {
			check_key();
			draw_map();
			Sleep(speed);
		}
		if (check_crush(bx, by + 1, b_rotation) != CRUSH) {
			move_block(DOWN);
			draw_map();
		}
		else {// BLOCK END
			done_block();
			draw_map();
			break;
		}
	}
	check_continue();
	check_line();
	check_score();
}

void check_key(void) {

	if ( key_able == 1 && _kbhit()) {
		key = _getch();
		switch (key)
		{
		case UP:
			check_rotation();
			break;
		case DOWN:
			if (check_crush(bx, by + 1, b_rotation) != CRUSH) {
				move_block(DOWN);
			}
			break;
		case LEFT:
			if (check_crush(bx - 1, by, b_rotation) != CRUSH) {
				move_block(LEFT);
			}
			break;
		case RIGHT:
			if (check_crush(bx + 1, by, b_rotation) != CRUSH) {
				move_block(RIGHT);
			}
			break;
		case SPACEBAR:
			key_able = 0; // 키 입력 멈춤
			while (check_crush(bx, by + 1, b_rotation) != CRUSH) {
				move_block(DOWN);
			}
			game_sound(SPACEBAR); // 효과음
			done_block(); // 더 이상 움직이지 못하게 하기
			score += 10; // space 사용 보너스
			update_board();
			key_able = 1; // 키 입력시작
			break;
		default:
			break;
		}
	}
}

void reset_map(void) {
	int i, j;

	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
			game_map[i][j] = EMPTY;
		}
	}

	for (i = MIN_X; i < MAX_X; i++) {
		game_map[MIN_Y][i] = CEILING;
		game_map[MAX_Y - 1][i] = WALL;
	}

	for (i = MIN_Y; i < MAX_Y; i++) {
		game_map[i][MIN_X] = WALL;
		game_map[i][MAX_X - 1] = WALL;
	}
}

void draw_map(void) {
	int i, j;

	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
			if (game_map[i][j] != game_map_copy[i][j]) {
				gotoxy(j, i);
				switch (game_map[i][j]) {
				case EMPTY:
					printf(" ");
					break;
				case WALL:
					printf("▩");
					break;
				case BLOCK:
				case BLOCK_D:
					printf("■");
					break;
				}
			}
		}
	}

	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
			game_map_copy[i][j] = game_map[i][j];
		}
	}
}

void new_block(void) { //새로운 블록 생성  
	int i, j;
	int random;

	srand(time(NULL));
	random = rand();
	b_type = random % 7;
	b_rotation = 0;

	bx = (MIN_X + MAX_X) / 2 - 1; //블록 생성 위치x좌표(게임판의 가운데) 
	by = MIN_Y-1;  //블록 생성위치 y좌표(제일 위) 

	for (i = 0; i < 4; i++) { //게임판 bx, by위치에 블럭생성  
		for (j = 0; j < 4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1) game_map[by + i][bx + j] = BLOCK;
		}
	}
}

void done_block(void) {//움직임이 끝난 블럭의 처리
	int i, j;

	for (i = 0; i < 4; i++) { 
		for (j = 0; j < 4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1) game_map[by + i][bx + j] = BLOCK_D;
		}
	}
}


void move_block(int dir) {
	int i, j;

	switch (dir) {
	case LEFT:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + i][bx + j] != WALL && game_map[by + i][bx + j] != BLOCK_D) {
					game_map[by + i][bx + j] = EMPTY;
				}
			}
		}
		bx--;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					game_map[by + i][bx + j] = BLOCK;
				}
			}
		}
		break;
	case RIGHT:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + i][bx + j] != WALL && game_map[by + i][bx + j] != BLOCK_D) {
					game_map[by + i][bx + j] = EMPTY;
				}
			}
		}
		bx++;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					game_map[by + i][bx + j] = BLOCK;
				}
			}
		}  
		break;
	case UP:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + i][bx + j] != WALL && game_map[by + i][bx + j] != BLOCK_D) {
					game_map[by + i][bx + j] = EMPTY;
				}
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					game_map[by + i][bx + j] = BLOCK;
				}
			}
		}
		break;
	case DOWN:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + i][bx + j] != WALL && game_map[by + i][bx + j] != BLOCK_D) {
					game_map[by + i][bx + j] = EMPTY;
				}
			}
		}
		by++;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					game_map[by + i][bx + j] = BLOCK;
				}
			}
		}
		break;
	}
}

int check_crush(int bx, int by, int b_rotation) { //지정된 좌표와 회전값으로 충돌이 있는지 검사 
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) { //지정된 위치의 게임판과 블럭모양을 비교해서 겹치면 false를 리턴 
			if (blocks[b_type][b_rotation][i][j] == 1 && game_map[by + i][bx + j] == WALL) return CRUSH;
			if (blocks[b_type][b_rotation][i][j] == 1 && game_map[by + i][bx + j] == BLOCK_D) return CRUSH; //다른 블럭과 겹치는지 확인
			if (blocks[b_type][b_rotation][i][j] == BLOCK_D && game_map[by + i][bx + j] == CEILING) return OVER;
		}
	}
	return 1; //하나도 안겹치면 true리턴 
}

int check_crush_wall(int bx, int by, int b_rotation) { //지정된 좌표와 회전값으로 충돌이 있는지 검사 
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) { //지정된 위치의 게임판과 블럭모양을 비교해서 겹치면 false를 리턴 
			if (blocks[b_type][b_rotation][i][j] == 1 && game_map[by + i][bx + j] == WALL) return CRUSH;
		}
	}
	return 1; //하나도 안겹치면 true리턴 
}

void check_continue(void) {
	int i;
	//블록이 벽을 넘었을때
	for (i = MIN_X; i < MAX_X; i++) {
		if (game_map[MIN_Y + 3][i] == BLOCK_D) {
			game = OVER;
			game_notice(2);
			reset_map();
			draw_map();
			score = 0;
			update_board();
			game = GO;
			return;
		}
	}
}

void check_score(void) {
	if (score > score_goal*level) {
		game = OVER;
		game_notice(1);
		reset_map();
		draw_map();
		score = 0;
		level++;
		speed -= 10;
		game = GO;
		update_board();
	}
	return;
}

void check_line(void) {
	int i, j;
	int block_num=0;

	//완성된 줄이 있는지 체크

	for (i = 0; i < 4; i++) {
		for (j = MIN_X+1; j < MAX_X-1; j++) {
			if (game_map[by + i][j] == BLOCK_D) block_num++;
		}
		if (block_num == (MAX_X - MIN_X - 2)) {	//꽉찬 줄이 있으면 삭제
			delete_line(by+i);
		}
		block_num = 0;
	}
}

void delete_line(int line) {
	int i;
	int j;
	for (j = MIN_X+1; j < MAX_X-1; j++) {
		game_map[line][j] = EMPTY;
	}
	i = line;
	while (i > MIN_Y) {
		for (j = MIN_X + 1; j < MAX_X - 1; j++) {
			game_map[i][j] = game_map[i - 1][j];
		}
		i--;
	}
	draw_map();
	score +=50;
	update_board();
}

void title(void) {
	int x = MAX_X + 2; //타이틀화면이 표시되는 x좌표 
	int y = 4; //타이틀화면이 표시되는 y좌표 
	int cnt; //타이틀 프레임을 세는 변수  

	gotoxy(x, y + 0); printf("■□□□■■■□□■■□□■■"); Sleep(100);
	gotoxy(x, y + 1); printf("■■■□  ■□□    ■■□□■"); Sleep(100);
	gotoxy(x, y + 2); printf("□□□■              □■  ■"); Sleep(100);
	gotoxy(x, y + 3); printf("■■□■■  □  ■  □□■□□"); Sleep(100);
	gotoxy(x, y + 4); printf("■■  ■□□□■■■□■■□□"); Sleep(100);
	gotoxy(x, y + 5); printf("		    jackjoo"); Sleep(100);
	gotoxy(x + 5, y + 2); printf("T E T R I S"); Sleep(100);
	gotoxy(x, y + 7); printf("Please Enter Any Key to Start..");
	gotoxy(x, y + 8); printf("+-------+");
	gotoxy(x, y + 9); printf("| SCORE |");
	gotoxy(x, y + 10); printf("+-------+");
	gotoxy(x, y + 11); printf("| LEVEL |");
	gotoxy(x, y + 12); printf("+-------+");
	gotoxy(x, y + 13); printf("BONUS FOR HARD DROPS & COMBOS");

	gotoxy(x, y + 15); printf("Hard Drop : SPACE");
	gotoxy(x, y + 16); printf("Quit : ESC ");
}

void update_board(void) {
	int x = MAX_X + 7; //타이틀화면이 표시되는 x좌표 
	int y = 4; //타이틀화면이 표시되는 y좌표 
	gotoxy(x, y + 9); printf("  %d  ", score);
	gotoxy(x, y + 11); printf("  %d  ", level);
}

void game_notice(int notice) {

	switch (notice) {
	case 1://level_up
		gotoxy((MAX_X - MIN_X), MAX_Y-1); printf("LEVEL UP!!");
		game_sound(LEVEL_UP);
		break;
	case 2://game_over
		gotoxy((MAX_X - MIN_X), MAX_Y-1); printf("GAME OVER");
		game_sound(OVER);
		break;
	default:
		break;

	}
	Sleep(1000);
	gotoxy((MAX_X - MIN_X), MAX_Y-1); printf("▩▩▩▩▩");
	return;
}

void game_sound(int sound) {
	switch (sound) {
		case SPACEBAR:	// spacebar sound
			Beep(_PA, 100);
			break;
		case OVER:		// game over sound
			Beep(_MI, 500);
			break;
		case GO:		// start sound
			Beep(_SI, 200);
			break;
		case LEVEL_UP:
			Beep(_RA, 200);
			break;
		default:
			break;
	}
}

void check_rotation(void) {
	int i;
	int j = 0;

		//충돌체크
	if (check_crush(bx, by, (b_rotation + 1) % 4) != CRUSH) {
		move_block(UP);
		return;
	}
	else if (check_crush_wall(bx, by, (b_rotation + 1) % 4) == CRUSH) {
		for (i = 1; i < 4; i++) {
			//왼쪽으로 옮겨보기
			if (check_crush(bx - i, by, (b_rotation + 1) % 4) != CRUSH) {
				while (j < i) {
					move_block(LEFT);
					j++;
				}
				move_block(UP);
				return;
			}
			//오른쪽으로 옮겨보기
			if (check_crush(bx + i, by, (b_rotation + 1) % 4) != CRUSH) {
				while (j < i) {
					move_block(RIGHT);
					j++;
				}
				move_block(UP);
				return;
			}
		}

	}
	else return;
}
