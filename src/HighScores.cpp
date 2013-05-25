#include "Snake.h"

HighScoreRecord hsr[3][3][3];
int button_hs_id = 0;

void WipeHighScores() {
	for (int gm = 0; gm < 3; gm++) {
		for (int d = 0; d < 3; d++) {
			for (int c = 0; c < 3; c++) {
				strcpy(hsr[gm][d][c].nickname, "....................");
				hsr[gm][d][c].points = 0;
				hsr[gm][d][c].ticks = 0;
			}
		}
	}
}

//загрузка рекордов из файла
void LoadHighScores() {
	FILE* f;
	f = fopen("snake_highscores.dat", "r");
	if (f != NULL) {
		for (int gm = 0; gm < 3; gm++) {
			for (int d = 0; d < 3; d++) {
				for (int c = 0; c < 3; c++) {
					fread(hsr[gm][d][c].nickname, 1, 20, f);
					fread(&hsr[gm][d][c].points, sizeof(int), 1, f);
					fread(&hsr[gm][d][c].ticks, sizeof(int), 1, f);
				}
			}
		}
		fclose(f);
	} else {
		WipeHighScores();
	}
}

//сохранение рекордов в файл
void SaveHighScores() {
	FILE* f;
	f = fopen("snake_highscores.dat", "w");
	if (f != NULL) {
		for (int gm = 0; gm < 3; gm++) {
			for (int d = 0; d < 3; d++) {
				for (int c = 0; c < 3; c++) {
					while (strlen(hsr[gm][d][c].nickname) < 20) {
						strcat(hsr[gm][d][c].nickname, " ");
					}
					fwrite(hsr[gm][d][c].nickname, 1, 20, f);
					fwrite(&hsr[gm][d][c].points, sizeof(int), 1, f);
					fwrite(&hsr[gm][d][c].ticks, sizeof(int), 1, f);
				}
			}
		}
		fclose(f);
	} else {
		printf("Failed to save highscores into file!\n");
	}
}

void DrawHSButton(char* title, int nx, int ny, int bid) {
	Color bc;
	if (button_hs_id == bid) {
		bc = getColor(0, 0, 200);
	} else {
		bc = getColor(128, 128, 255);
	}
	int button_width = 80 + 40 / 3;
	int button_height = 20;
	ImageFilledRectangle(110, 110, 110 + button_width, 130, bc);
	ImageText(110 + button_width / 2 - ImageTextWidth(title) / 2, 124, title, getColor(0, 0, 0));
	ImageCopy(110, 110, 100, 30, nx, ny, 2);
}

void DrawHighScoresScreen() {
	Color mcolor = getColor(100, 255, 100);
	Color tcolor = getColor(0, 0, 0);
	ImageFillScreen(mcolor);
	
	//прорисовка кнопок
	DrawHSButton("Empty", 27, 15, 0);
	DrawHSButton("Box", 227, 15, 1);
	DrawHSButton("Labyrinth", 427, 15, 2);
	ImageFilledRectangle(105, 105, 240, 150, mcolor);

	int y = 100;
	int d = 2;
	int c = 0;
	char x[100];
	for (int i = 1; i <= 9; i++) {
		c = ((i - 1) % 3);
		ImageFilledRectangle(25, y, 615, y + 30, getColor(255 - 64 * c, 255 - 64 * c, 0));
		sprintf(x, "nickname = %s; points = %3d; ticks = %4d", 
			hsr[button_hs_id][d][c].nickname, hsr[button_hs_id][d][c].points, hsr[button_hs_id][d][c].ticks
		);
		ImageText(50, y + 17, x, getColor(0, 128 - 64 * c, 0));

		if (i % 3 == 0) {
			y += 60;
			d--;
		} else {
			y += 30;
		}
	}

	ImageText(27, 90, "Difficulty: hard", tcolor);
	ImageText(27, 210, "Difficulty: medium", tcolor);
	ImageText(27, 330, "Difficulty: easy", tcolor);
	ImageText(156, 460, "Press any key to back to main menu", tcolor);
}

void HighScoresPressKey(unsigned char key) {
	SaveHighScores();
	Window = W_MAINMENU;
}

void HighScoresSpecPressKey(int key) {
	if (key == GLUT_KEY_UP || key == GLUT_KEY_LEFT) {
		button_hs_id += 2;
	}
	if (key == GLUT_KEY_DOWN || key == GLUT_KEY_RIGHT) {
		button_hs_id += 1;
	}
	button_hs_id %= 3;
}