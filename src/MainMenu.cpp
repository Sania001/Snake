#include "Snake.h"

int button_id = 0;

void DrawButton(char* title, int nx, int ny, int bid) {
	Color bc;
	if (button_id == bid) {
		bc = getColor(200, 200, 0);
	} else {
		bc = getColor(128, 128, 255);
	}
	int button_width = 80 + 40 / 3;
	int button_height = 20;
	ImageFilledRectangle(10, 10, 10 + button_width, 30, bc);
	ImageText(10 + button_width / 2 - ImageTextWidth(title) / 2, 24, title, getColor(0, 0, 0));
	ImageCopy(10, 10, 100, 30, nx, ny, 3);
}

void DrawMainMenuScreen() {
	Color logocolor = getColor(50, 255, 50);
	Color menucolor = getColor(83, 83, 83);

	ImageFillScreen(menucolor);

	//Рисуем лого
	char logo[5][50];
	strcpy(logo[0], ".XXXXX.X...X.XX....X...X.XXXXX.");
	strcpy(logo[1], ".X.....XX..X.X.X...X..X..X.....");
	strcpy(logo[2], ".XXXXX.X.X.X.X..X..XXX...XXXXX.");
	strcpy(logo[3], ".....X.X..XX.XXXXX.X..X..X.....");
	strcpy(logo[4], ".XXXXX.X...X.X...X.X...X.XXXXX.");
	for (int i = 0; i < 5; i++) {
		for (unsigned int j = 0; j < strlen(logo[i]); j++) {
			if (logo[i][j] == 'X') {
				ImagePixel(10 + j, 10 + i, logocolor);
			} else {
				ImagePixel(10 + j, 10 + i, menucolor);
			}
		}
	}
	ImageCopy(10, 5, 35, 11, 150 - 63, 100 - 5 * 15, 15);
	//стираем следы
	ImageFilledRectangle(5, 5, 45, 20, menucolor);

	//рисуем кнопочки
	DrawButton("Play", 180, 220, 0);
	DrawButton("Highscore", 180, 290, 1);
	DrawButton("Exit", 180, 360, 2);
	//и снова стираем следы
	ImageFilledRectangle(5, 5, 140, 50, menucolor);
}

void MainMenuPressKey(unsigned char key) {
	if (key == 27) { //esc
		exit(0);
	}
	if (key == 13) { //enter
		switch (button_id) {
			case 0:
				Window = W_GAMESETTINGS;
				break;
			case 1:
				Window = W_HIGHSCORES;
				break;
			case 2:
				exit(0);
		}
	}
}

void MainMenuSpecPressKey(int key) {
	if (key == GLUT_KEY_UP || key == GLUT_KEY_LEFT) {
		button_id += 2;
	}
	if (key == GLUT_KEY_DOWN || key == GLUT_KEY_RIGHT) {
		button_id += 1;
	}
	button_id %= 3;
}