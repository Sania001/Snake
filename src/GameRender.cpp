#include "Snake.h"

Color lightFat = getColor(15, 15, 255);
Color darkFat = getColor(0, 0, 200);
Color lightRunning = getColor(15, 255, 15);
Color darkRunning = getColor(0, 200, 0);
Color lightDied = getColor(255, 15, 15);
Color darkDied = getColor(200, 0, 0);

//прорисовка части тела (середины) змейки
void RenderBodyCell(int x, int y, bool fat) {
	Color bc, sbc;
	if (isGameOver) {
		bc = darkDied;
		sbc = lightDied;
	} else if (fat) {
		bc = darkFat;
		sbc = lightFat;
	} else {
		bc = darkRunning;
		sbc = lightRunning;
	}
	int bx = 20 + x * CELL_HEIGHT;
	int by = 20 + y * CELL_WIDTH;
	ImageFilledRectangle(
		bx + 5, by + 5, bx + 35, by + 35, bc
	);
	ImageFilledTriangle(bx + 20, by + 10, bx + 30, by + 10, bx + 30, by + 20, sbc);
	ImageFilledTriangle(bx + 10, by + 30, bx + 20, by + 30, bx + 10, by + 20, sbc);
}

//прорисовка головы змеи
void RenderHeadCell(int x, int y, int d, bool fat) {
	Color bc, sbc;
	if (isGameOver) {
		bc = darkDied;
		sbc = lightDied;
	} else if (fat) {
		bc = darkFat;
		sbc = lightFat;
	} else {
		bc = darkRunning;
		sbc = lightRunning;
	}
	Color b = getColor(0, 0, 0);
	int bx = 20 + x * CELL_HEIGHT;
	int by = 20 + y * CELL_WIDTH;
	switch (d) {
		case 0:
			ImageFilledRectangle(bx + 5, by + 15, bx + 35, by + 35, bc);
			ImageFilledRectangle(bx + 15, by + 5, bx + 25, by + 35, bc);
			ImageFilledTriangle(bx + 15, by + 5, bx + 5, by + 15, bx + 15, by + 15, bc);
			ImageFilledTriangle(bx + 25, by + 5, bx + 35, by + 15, bx + 25, by + 15, bc);
			ImageFilledTriangle(bx + 20, by + 7, bx + 25, by + 12, bx + 15, by + 12, b);
			break;
		case 1:
			ImageFilledRectangle(bx + 5, by + 5, bx + 25, by + 35, bc);
			ImageFilledRectangle(bx + 5, by + 15, bx + 35, by + 25, bc);
			ImageFilledTriangle(bx + 25, by + 5, bx + 25, by + 15, bx + 35, by + 15, bc);
			ImageFilledTriangle(bx + 25, by + 25, bx + 35, by + 25, bx + 25, by + 35, bc);
			ImageFilledTriangle(bx + 33, by + 20, bx + 28, by + 15, bx + 28, by + 25, b);
			break;
		case 2:
			ImageFilledRectangle(bx + 5, by + 5, bx + 35, by + 25, bc);
			ImageFilledRectangle(bx + 15, by + 5, bx + 25, by + 35, bc);
			ImageFilledTriangle(bx + 5, by + 25, bx + 15, by + 25, bx + 15, by + 35, bc);
			ImageFilledTriangle(bx + 25, by + 25, bx + 35, by + 25, bx + 25, by + 35, bc);
			ImageFilledTriangle(bx + 20, by + 33, bx + 25, by + 28, bx + 15, by + 28, b);
			break;
		case 3:
			ImageFilledRectangle(bx + 15, by + 5, bx + 35, by + 35, bc);
			ImageFilledRectangle(bx + 5, by + 15, bx + 35, by + 25, bc);
			ImageFilledTriangle(bx + 15, by + 5, bx + 15, by + 15, bx + 5, by + 15, bc);
			ImageFilledTriangle(bx + 15, by + 25, bx + 15, by + 35, bx + 5, by + 25, bc);
			ImageFilledTriangle(bx + 7, by + 20, bx + 12, by + 15, bx + 12, by + 25, b);
	}
	ImageFilledRectangle(bx + 16, by + 16, bx + 24, by + 24, sbc);
}

//прорисовка хвоста змеи
void RenderTailCell(int x, int y, int d, bool fat) {
	Color bc, sbc;
	if (isGameOver) {
		bc = darkDied;
		sbc = lightDied;
	} else if (fat) {
		bc = darkFat;
		sbc = lightFat;
	} else {
		bc = darkRunning;
		sbc = lightRunning;
	}
	int bx = 20 + x * CELL_HEIGHT;
	int by = 20 + y * CELL_WIDTH;
	switch (d) {
		case 0:
			ImageFilledRectangle(bx + 5, by + 5, bx + 35, by + 15, bc);
			ImageFilledTriangle(bx + 5, by + 15, bx + 20, by + 35, bx + 35, by + 15, bc);
			break;
		case 1:
			ImageFilledRectangle(bx + 5, by + 5, bx + 15, by + 35, bc);
			ImageFilledTriangle(bx + 15, by + 5, bx + 35, by + 20, bx + 15, by + 35, bc);
			break;
		case 2:
			ImageFilledRectangle(bx + 5, by + 25, bx + 35, by + 35, bc);
			ImageFilledTriangle(bx + 5, by + 25, bx + 20, by + 5, bx + 35, by + 25, bc);
			break;
		case 3:
			ImageFilledRectangle(bx + 25, by + 5, bx + 35, by + 35, bc);
			ImageFilledTriangle(bx + 5, by + 20, bx + 25, by + 5, bx + 25, by + 35, bc);
	}
}

//прорисовка яблока
void RenderAppleCell(int x, int y) {
	int bx = 20 + x * CELL_HEIGHT;
	int by = 20 + y * CELL_WIDTH;
	ImageFilledTriangle(bx + 15, by + 15, bx + 25, by + 25, bx + 35, by + 10, getColor(0, 210, 0));
	ImageFilledTriangle(bx + 15, by + 15, bx + 30, by + 5, bx + 35, by + 10, getColor(0, 210, 0));
	ImageFilledCircle(bx + 20, by + 20, 13, getColor(255, 50, 50));
}

//прорисовка бонуса
void RenderBonusCell(int x, int y) {
	int bx = 20 + x * CELL_HEIGHT;
	int by = 20 + y * CELL_WIDTH;
	Color black = getColor(0, 0, 0);
	Color orange = getColor(253, 133, 0);
	ImageFilledTriangle(bx + 20, by + 7, bx + 20, by + 33, bx + 7, by + 20, black);
	ImageFilledTriangle(bx + 20, by + 7, bx + 20, by + 33, bx + 33, by + 20, black);

	ImageFilledTriangle(bx + 5, by + 10, bx + 5, by + 30, bx + 15, by + 20, orange);
	ImageFilledTriangle(bx + 10, by + 5, bx + 30, by + 5, bx + 20, by + 15, orange);
	ImageFilledTriangle(bx + 35, by + 10, bx + 35, by + 30, bx + 25, by + 20, orange);
	ImageFilledTriangle(bx + 10, by + 35, bx + 30, by + 35, bx + 20, by + 25, orange);
}

//прорисовка игрового поля в целом
void DrawGameScreen() {
	//Рисуем сетку
	for (int i = 1; i < GAME_COLS; i++) {
		ImageDashedLine(20 + i * CELL_WIDTH, 20, 20 + i * CELL_WIDTH, 20 + CELL_HEIGHT * GAME_ROWS, getColor(170, 170, 170));
	}
	for (int i = 1; i < GAME_ROWS; i++) {
		ImageDashedLine(20, 20 + i * CELL_HEIGHT, 20 + CELL_WIDTH * GAME_COLS, 20 + i * CELL_HEIGHT, getColor(170, 170, 170));
	}

	ImageRectangle(20, 20, 20 + CELL_WIDTH * GAME_COLS, 20 + CELL_HEIGHT * GAME_ROWS, getColor(0, 0, 0));
	ImagePixel(20, 20, getColor(0, 0, 0));

	//создание игровой статус-строки
	char status[100];
	int points = GamePoints + GameBonusPoints * BonusEffect;
	if (isGameOver) {
		sprintf(status, "Game Over! %d points, %d ticks. Press <ENTER> to exit...", points, LivingTicks);
	} else if (isGamePaused) {
		sprintf(status, "Pause: %d points, %d ticks. Press <ESC> to exit...", points, LivingTicks);
	} else {
		sprintf(status, "Running: %d points, %d ticks, %d bonus ticks", points, LivingTicks, Bonus.living);
	}
	ImageFilledRectangle(10, 30 + CELL_HEIGHT * GAME_ROWS, WIN_WIDTH - 10, WIN_HEIGHT - 10, getColor(200, 250, 200));
	ImageText(25, WIN_HEIGHT - 28, status, getColor(0, 0, 250));

	SnakeCell* f = Head;
	SnakeCell* nowCell;
	Color sbc;
	if (isGameOver) {
		sbc = darkDied;
	} else {
		sbc = darkRunning;
	}
	//прорисовка межсегментных переходов
	do {
		nowCell = f;
		f = f->next;
		if (f != NULL && f->visible) {
			int baseX = 20 + nowCell->x * CELL_HEIGHT;
			int baseY = 20 + nowCell->y * CELL_WIDTH;
			do {
				if (nowCell->x + 1 == f->x) {
					ImageFilledRectangle(baseX + 15, baseY + 15, baseX + 40 + 25, baseY + 25, sbc);
					break;
				}
				if (nowCell->x - 1 == f->x) {
					ImageFilledRectangle(baseX - 40 + 15, baseY + 15, baseX + 25, baseY + 25, sbc);
					break;
				}
				if (nowCell->y + 1 == f->y) {
					ImageFilledRectangle(baseX + 15, baseY + 15, baseX + 25, baseY + 40 + 25, sbc);
					break;
				}
				if (nowCell->y - 1 == f->y) {
					ImageFilledRectangle(baseX + 15, baseY + 25, baseX + 25, baseY - 40 + 15, sbc);
					break;
				}
				//прорисовка межсегментных переходов "как бы" из-за границы экрана
				if (nowCell->y == f->y) {
					ImageFilledRectangle(20, baseY + 15, 20 + 25, baseY + 25, sbc);
					ImageFilledRectangle(20 + GAME_COLS * CELL_WIDTH - 25, baseY + 15, 20 + GAME_COLS * CELL_WIDTH - 1, baseY + 25, sbc);
					break;
				}
				if (nowCell->x == f->x) {
					ImageFilledRectangle(baseX + 15, 20, baseX + 25, 20 + 25, sbc);
					ImageFilledRectangle(baseX + 15, 20 + GAME_ROWS * CELL_HEIGHT - 25, baseX + 25, 20 + GAME_ROWS * CELL_HEIGHT - 1, sbc);
				}
			} while (false);
		}
	} while (f != NULL);
	//прорисовка сегментов
	f = Head;
	do {
		f = f->next;
		if (f != NULL && f->visible) {
			if (f->next == NULL || f->next->visible == false) {
				//определяем, откуда "растут ноги" у хвоста
				int dir = -1;
				if (f->prev->x + 1 == f->x) {
					dir = 1;
				} else if (f->prev->x - 1 == f->x) {
					dir = 3;
				} else if (f->prev->y + 1 == f->y) {
					dir = 0;
				} else if (f->prev->y - 1 == f->y) {
					dir = 2;
				} else {
					if (f->prev->y == f->y) {
						if (f->x == 0) {
							dir = 1;
						} else {
							dir = 3;
						}
					} else {
						if (f->y == 0) {
							dir = 0;
						} else {
							dir = 2;
						}
					}
				}
				RenderTailCell(f->x, f->y, dir, f->fat);
			} else {
				RenderBodyCell(f->x, f->y, f->fat);
			}
		}
	} while (f != NULL);

	//прорисовка стен
	int oh = 2; // - "радиус" стен
	Color oc = getColor(0, 0, 0);
	for (int i = 0; i < GAME_ROWS; i++) {
		for (int j = 0; j <= GAME_COLS; j++) {
			if (vObstacles[i][j] != 0) {
				ImageFilledRectangle(
					20 + CELL_WIDTH * j - oh, 
					20 + CELL_HEIGHT * i - oh, 
					20 + CELL_WIDTH * j + oh, 
					20 + CELL_HEIGHT * (i + 1) + oh, 
					oc
				);
			}
		}
	}
	for (int i = 0; i <= GAME_ROWS; i++) {
		for (int j = 0; j < GAME_COLS; j++) {
			if (gObstacles[i][j] != 0) {
				ImageFilledRectangle(
					20 + CELL_WIDTH * j - oh, 
					20 + CELL_HEIGHT * i - oh, 
					20 + CELL_WIDTH * (j + 1) + oh, 
					20 + CELL_HEIGHT * i + oh, 
					oc
				);
			}
		}
	}

	//прорисовка бонуса, головы, яблока
	RenderAppleCell(Apple.x, Apple.y);
	if (Bonus.isActive) {
		RenderBonusCell(Bonus.x, Bonus.y);
	}
	RenderHeadCell(Head->x, Head->y, SnakeDirection, Head->fat);
}
