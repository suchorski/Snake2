#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

#include <windows.h>
#include <graphics.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "resource.h"

#include "main.hpp"

int main(void) {
	short int choice_menu = 0;
	level = (short int) GetPrivateProfileInt("Game", "Level", 0, SNAKE_DB);
	maxscore = GetPrivateProfileInt("Game", "MaxScore", 0, SNAKE_DB);
	initwindow(600, 300, GAME_TITLE, 100, 100, true, true);
	setactivepage(1);
	moveto(getmaxx() / 2, getmaxy() / 2);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	outtext(GAME_LOADING);
	setvisualpage(1);
	setactivepage(2);
	delay(100);
	do {
		choice_menu = drawMenu(choice_menu, 4, mainmenu);
		switch (choice_menu) {
			case 0:
				gameStart();
				break;
			case 1:
				char temp[1];
				level = drawMenu(level, 5, levelname);
				itoa((int) level, temp, 10);
				WritePrivateProfileString("Game", "Level", temp, SNAKE_DB);
				break;
			case 2:
				drawBox(3, about, true);
				break;
			case 3:
				break;
		}
	} while (choice_menu != 3);
	closegraph();
	return 0;
}

void swapBuffer(void) {
	int vp = getvisualpage(), ap = getactivepage();
	setvisualpage(ap);
	setactivepage(vp);
	cleardevice();
}

void drawGame(void) {
	char tmp[8];
	setcolor(COLOR_BORDER);
	rectangle(4, 4, getmaxx() - 4, getmaxy() - 4);
	line(154, 4, 154, getmaxy() - 4);
	line(5, 70, 154, 70);
	setcolor(COLOR_TEXT);
	moveto(79, 42);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
	outtext(NAME);
	line(32, 48, 125, 48);
	moveto(79, 80);
	settextjustify(CENTER_TEXT, TOP_TEXT);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	outtext(MENU_SCORE);
	moverel(0, 25);
	itoa((int) score, tmp, 10);
	outtext(tmp);
	moverel(0, 35);
	outtext(MENU_LEVEL);
	moverel(0, 25);
	outtext(levelname[level]);
	moverel(0, 80);
	outtext(MENU_MAX);
	moverel(0, 25);
	itoa((int) maxscore, tmp, 10);
	outtext(tmp);
}

short int drawMenu(short int sel, short int itens, char *item[]) {
	int selection[itens], key, yt = (itens * 25 + 30), y = (((getmaxy() - 10) - yt) / 2 + 5);
	selection[0] = y + 15;
	for (short int i = 1; i < itens; i++) {
		selection[i] = selection[i - 1] + 25;
	}
	do {
		drawGame();
		setcolor(COLOR_BORDER);
		setfillstyle(SOLID_FILL, getbkcolor());
		bar3d(255, y, 495, y + yt, 0, 0);
		rectangle(275, selection[sel], 475, selection[sel] + 24);
		setcolor(COLOR_TEXT);
		moveto(375, y + 20);
		for (short int i = 0; i < itens; i++) {
			outtext(item[i]);
			moverel(0, 25);
		}
		swapBuffer();
		do {
			key = getch();
		} while (key != KEY_UP && key != KEY_DOWN && key != 13);
		switch (key) {
			case KEY_UP:
				sel--;
				if (sel == -1)
					sel = itens - 1;
				break;
			case KEY_DOWN:
				sel++;
				if (sel == itens)
					sel = 0;
				break;
			default:
				break;
		}
	} while (key != 13);
	return sel;
}

void drawBox(short int lines, char *item[], bool confirm) {
	drawGame();
	setcolor(COLOR_BORDER);
	setfillstyle(SOLID_FILL, getbkcolor());
	int key, yt = (lines * 25 + 30), y = (((getmaxy() - 10) - yt) / 2 + 5);
	bar3d(205, y, 545, y + yt, 0, 0);
	setcolor(COLOR_TEXT);
	moveto(375, y + 20);
	for (short int i = 0; i < lines; i++) {
		outtext(item[i]);
		moverel(0, 25);
	}
	swapBuffer();
	if (confirm) {
		do {
			key = getch();
		} while (key != 13);
	}
}

void drawItem(short int x, short int y, short int item) {
	short int drawX = 155 + (x * 10), drawY = 5 + (y * 10);
	switch (item) {
		case SNAKE_BODY:
			setcolor(COLOR_SKIN);
			setfillstyle(SOLID_FILL, COLOR_SNAKE);
			bar3d(drawX, drawY, (drawX + 10), (drawY + 10), 0, 0);
			break;
		case SNAKE_APPLE:
			setcolor(COLOR_APPLE);
			setfillstyle(SOLID_FILL, COLOR_APPLE);
			fillellipse((drawX + 5), (drawY + 5), 2, 2);
			break;
	}
}

void drawSnake(void) {
	cell snakeCellTmp;
	for (short int i = 0; i < (short int) snake.size(); i++) {
		snakeCellTmp = snake[i];
		drawItem(snakeCellTmp.x, snakeCellTmp.y, SNAKE_BODY);
	}
}

void drawApple(void) {
	drawItem(apple.x, apple.y, SNAKE_APPLE);
}

void changeSnake(short int x, short int y, short int cmd) {
	cell snakeCellTmp;
	snakeCellTmp.x = x;
	snakeCellTmp.y = y;
	switch (cmd) {
		case SNAKE_ADD:
			snake.push_back(snakeCellTmp);
			changeEmpty(x, y, false);
			break;
		case SNAKE_MOVE:
			snake.push_back(snakeCellTmp);
			changeEmpty(x, y, false);
			changeEmpty(snake[0].x, snake[0].y, true);
			snake.erase(snake.begin());
			break;
		case SNAKE_CLEAR:
			snake.clear();
			fillEmpty();
			break;
	}
}

bool changeApple(void) {
	if (!blank.empty()) {
		uniform_int_distribution <short int> r(0, (short int) blank.size());
		short int randTemp = r(random);
		apple.x = blank[randTemp].x;
		apple.y = blank[randTemp].y;
		return true;
	}
	return false;
}

void changeEmpty(short int x, short int y, bool cmd) {
	cell emptyCellTmp;
	if (cmd) {
		emptyCellTmp.x = x;
		emptyCellTmp.y = y;
		blank.push_back(emptyCellTmp);
	} else {
		for (short int i = 0; i < (short int) blank.size(); i++) {
			emptyCellTmp = blank[i];
			if (blank[i].x == x && blank[i].y == y) {
				blank.erase(blank.begin() + i);
				break;
			}
		}
	}
}

void fillEmpty(void) {
	blank.clear();
	cell emptyCellTmp;
	for (short int i = 0; i <= 43; i++) {
		for (short int j = 0; j <= 28; j++) {
			emptyCellTmp.x = i;
			emptyCellTmp.y = j;
			blank.push_back(emptyCellTmp);
		}
	}
}

bool isSnake(short int x, short int y) {
	cell snakeCellTmp;
	for (short int i = 0; i < (short int) snake.size(); i++) {
		snakeCellTmp = snake[i];
		if (snake[i].x == x && snake[i].y == y) {
			return true;
		}
	}
	return false;
}

void gameStart(void) {
	srand(time(NULL));
	int keyTemp;
	changeSnake(0, 0, SNAKE_CLEAR);
	for (short int i = 0; i < 5; i++) {
		changeSnake((16 + i), 14, SNAKE_ADD);
	}
	changeApple();
	alive = true;
	movingSnake = CreateThread(NULL, 0, moveSnake, NULL, 0, NULL);
	while (true) {
		do {
			keyTemp = getch();
		} while (keyTemp != KEY_UP && keyTemp != KEY_DOWN && keyTemp != KEY_LEFT && keyTemp != KEY_RIGHT && keyTemp != KEY_ESC && keyTemp != 13 && keyTemp != 112);
		if (keyTemp == 13) {
			if (!alive) {
				break;
			}
		} else if (alive) {
			if (keyTemp == 112) {
				SuspendThread(movingSnake);
				drawBox(4, gamepaused, true);
				ResumeThread(movingSnake);
			}
			keyMove = keyTemp;
			if (!life) {
				life = true;
				TerminateThread(movingSnake, 0);
				movingSnake = CreateThread(NULL, 0, moveSnake, NULL, 0, NULL);
			}
		}
		if (keyMove == KEY_ESC) {
			TerminateThread(movingSnake, 0);
			break;
		}
	}
	if (maxnew) {
		maxnew = false;
		char temp[8];
		itoa((int) maxscore, temp, 10);
		WritePrivateProfileString("Game", "MaxScore", temp, SNAKE_DB);
	}
	alive = false;
	life = true;
	keyDir = RIGHT;
	keyMove = KEY_RIGHT;
	score = 0;
}

DWORD WINAPI moveSnake(LPVOID lpParam) {
	int mx, my;
	while (true) {
		if (keyMove == KEY_ESC) {
			break;
		}
		if ((keyDir == RIGHT || keyDir == LEFT) && (keyMove == KEY_UP || keyMove == KEY_DOWN)) {
			keyDir = (keyMove == KEY_UP) ? UP : DOWN;
		} else if ((keyDir == UP || keyDir == DOWN) && (keyMove == KEY_LEFT || keyMove == KEY_RIGHT)) {
			keyDir = (keyMove == KEY_LEFT) ? LEFT : RIGHT;
		}
		mx = snake.back().x;
		my = snake.back().y;
		switch (keyDir) {
			case UP:
				my--;
				break;
			case DOWN:
				my++;
				break;
			case LEFT:
				mx--;
				break;
			case RIGHT:
				mx++;
				break;
			default:
				break;
		}
		if (mx == -1 || mx == 44 || my == -1 || my == 29 || isSnake(mx, my)) {
			if (life) {
				life = false;
				sleep(levelspeed[0]);
				continue;
			}
			alive = false;
			drawBox(4, gameover, false);
			break;
		}
		if (mx == apple.x && my == apple.y) {
			changeSnake(mx, my, SNAKE_ADD);
			if (!changeApple()) {
				alive = false;
				drawBox(3, youwin, false);
				break;
			}
			score++;
			if (score > maxscore) {
				maxscore = score;
				maxnew = true;
			}
		} else {
			changeSnake(mx, my, SNAKE_MOVE);
		}
		drawGame();
		drawSnake();
		drawApple();
		swapBuffer();
		sleep(levelspeed[level]);
	}
	return 0;
}
