#include "Snake.h"

int HSPos = -1;
char HSNick[25];
int HSTicks, HSPoints;
int HSGamemode, HSDifficulty;
Color fieldColor;
Color redField = getColor(120, 50, 50);
Color greenField = getColor(50, 120, 50);

//добавление текущих очков в таблицу рекордов
void InsertHSRecord() {
	for (int i = 2; i > HSPos; i--) {
		hsr[HSGamemode][HSDifficulty][i].points = hsr[HSGamemode][HSDifficulty][i - 1].points;
		hsr[HSGamemode][HSDifficulty][i].ticks = hsr[HSGamemode][HSDifficulty][i - 1].ticks;
		strcpy(hsr[HSGamemode][HSDifficulty][i].nickname, hsr[HSGamemode][HSDifficulty][i - 1].nickname);
	}
	hsr[HSGamemode][HSDifficulty][HSPos].points = HSPoints;
	hsr[HSGamemode][HSDifficulty][HSPos].ticks = HSTicks;
	strcpy(hsr[HSGamemode][HSDifficulty][HSPos].nickname, HSNick);
	SaveHighScores();
}

//обработка результатов игры:
//  если очков достаточно, то просим ввести имя для занесения в таблицу
//  иначе отправляемся в главное меню
void HandleHighscoreUpdate(int difficulty, int gamemode, int points, int ticks) {
	HSGamemode = gamemode;
	HSDifficulty = difficulty;
	HSTicks = ticks;
	HSPoints = points;
	strcpy(HSNick, "");
	
	int pos = -1;
	for (int i = 0; i < 3; i++) {
		if (hsr[gamemode][difficulty][i].points < points) {
			pos = i;
			break;
		} else if (hsr[gamemode][difficulty][i].points == points && hsr[gamemode][difficulty][i].ticks > ticks) {
			pos = i;
			break;
		}
	}
	if (pos == -1) {
		Window = W_MAINMENU;
	} else {
		HSPos = pos;
	}
	fieldColor = greenField;
}

void DrawChampionScreen() {
	Color mcolor = getColor(100, 100, 255);
	ImageFillScreen(mcolor);

	ImageFilledRectangle(10, 10, 250, 40, fieldColor);
	char hsnick2[100];
	strcpy(hsnick2, HSNick);
	strcat(hsnick2, "_");
	ImageText(13, 33, hsnick2, getColor(200, 200, 0));
	ImageCopy(10, 10, 250, 35, 20, 151, 2);
	ImageFilledRectangle(10, 10, 250, 50, mcolor);

	ImageText(10, 33, "Enter your name and press <ENTER>:", getColor(220, 220, 0));
	ImageCopy(10, 10, 350, 35, 20, 101, 2);
	ImageFilledRectangle(10, 10, 350, 50, mcolor);

	ImageText(10, 33, "Congratulations!", getColor(220, 220, 0));
	ImageCopy(10, 10, 350, 35, 20, 51, 2);
	ImageFilledRectangle(10, 10, 350, 50, mcolor);
}

void ChampionPressKey(unsigned char key) {
	if (key == 13 && strlen(HSNick) > 0) { //enter
		InsertHSRecord();
		Window = W_MAINMENU;
	} else if (key == 8) { //backspace
		if (strlen(HSNick) > 0) {
			HSNick[strlen(HSNick) - 1] = 0;	
		} else {
			fieldColor = redField;
			glutTimerFunc(200, onTick, W_CHAMPION);
		}
	} else if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z') 
		|| (key >= '0' && key <= '9') || key == '-' || key == '_') {
		if (strlen(HSNick) < 20) {
			char tmp[2];
			tmp[0] = key;
			tmp[1] = 0;
			strcat(HSNick, tmp);
		} else {
			fieldColor = redField;
			glutTimerFunc(200, onTick, W_CHAMPION);
		}
	} else {
		fieldColor = redField;
		glutTimerFunc(200, onTick, W_CHAMPION);
	}	
}

void ChampionTick() {
	fieldColor = greenField;
}