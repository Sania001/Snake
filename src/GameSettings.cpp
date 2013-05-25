#include "Snake.h"

int button_id[2];
int setting_id = 0;

void DrawButton(char* title, int nx, int ny, int bid, int sid) {
	Color bc;
	if (button_id[sid] == bid && setting_id == sid) {
		bc = getColor(200, 200, 0);
	} else if (button_id[sid] == bid) {
		bc = getColor(140, 140, 20);
	} else {
		bc = getColor(128, 128, 255);
	}
	int button_width = 80 + 40 / 3;
	int button_height = 20;
	ImageFilledRectangle(10, 10, 10 + button_width, 30, bc);
	ImageText(10 + button_width / 2 - ImageTextWidth(title) / 2, 24, title, getColor(0, 0, 0));
	ImageCopy(10, 10, 100, 30, nx, ny, 2);
}

void DrawGameSettingsScreen() {
	Color menucolor = getColor(103, 103, 103);
	Color textcolor = getColor(255, 220, 220);
	ImageFillScreen(menucolor);

	ImageText(10, 30, "Game settings", getColor(255, 128, 128));
	ImageCopy(10, 10, 150, 30, 148, 30, 3);
	ImageFilledRectangle(5, 5, 140, 50, menucolor);

	//рисуем надписи и кнопки
	ImageText(27, 130, "Difficulty:", textcolor);
	DrawButton("Easy", 27, 150, 0, 0);
	DrawButton("Medium", 227, 150, 1, 0);
	DrawButton("Hard", 427, 150, 2, 0);

	ImageText(27, 280, "Game mode:", textcolor);
	DrawButton("Empty", 27, 300, 0, 1);
	DrawButton("Box", 227, 300, 1, 1);
	DrawButton("Labyrinth", 427, 300, 2, 1);

	ImageText(170, 410, "Press <ENTER> to start the game", getColor(255, 255, 255));
	ImageText(166, 430, "Press <ESC> to back to main menu", getColor(255, 255, 255));
	//стираем за собой следы
	ImageFilledRectangle(5, 5, 140, 50, menucolor);
}

void GameSettingsPressKey(unsigned char key) {
	if (key == 13) {
		Window = W_GAME;
		StartGame(button_id[0], button_id[1]);
		return;
	}
	if (key == 27) {
		Window = W_MAINMENU;
		return;
	}
}

void GameSettingsSpecPressKey(int key) {
	switch (key) {
		case GLUT_KEY_UP: 
			setting_id = (setting_id + 1) % 2;
			break;
		case GLUT_KEY_RIGHT: 
			button_id[setting_id] = (button_id[setting_id] + 1) % 3;
			break;
		case GLUT_KEY_DOWN: 
			setting_id = (setting_id + 1) % 2;
			break;
		case GLUT_KEY_LEFT: 
			button_id[setting_id] = (button_id[setting_id] + 2) % 3;
	}
}