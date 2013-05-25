#include "Snake.h"

void onScreenRender() {
	printf("onRender()\n");
	
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (Window == W_MAINMENU) {
		DrawMainMenuScreen();
	} else if (Window == W_GAME) {
		DrawGameScreen();
	} else if (Window == W_GAMESETTINGS) {
		DrawGameSettingsScreen();
	} else if (Window == W_HIGHSCORES) {
		DrawHighScoresScreen();
	} else if (Window == W_CHAMPION) {
		DrawChampionScreen();
	} else {
		printf("Unknown window value\n");
	}

	glutSwapBuffers();
}

void onScreenReshape(int w, int h) {
	printf("onReshape(%d, %d)\n", w, h);
	glViewport(0, 0, w, h);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	onScreenRender();
}

void onTick(int value) {
	printf("onTick: value = %d\n", value);
	if (Window != value) {
		printf("Current window (%d) != tick id!\n", Window);
		return;
	}
	
	if (Window == W_GAME) {
		GameTick();
	} else if (Window == W_CHAMPION) {
		ChampionTick();
	} else {
		printf("Unknown window value\n");
	}
	glutPostRedisplay();
}

void onKeyPressed(unsigned char key, int x, int y) {
	printf("onKeyPressed: char = %c | %d\n", key, key);

	if (Window == W_MAINMENU) {
		MainMenuPressKey(key);
	} else if (Window == W_GAME) {
		GamePressKey(key);
	} else if (Window == W_GAMESETTINGS) {
		GameSettingsPressKey(key);
	} else if (Window == W_HIGHSCORES) {
		HighScoresPressKey(key);
	} else if (Window == W_CHAMPION) {
		ChampionPressKey(key);
	} else {
		printf("Unknown window value\n");
	}
	glutPostRedisplay();
}

void onSpecialKeyPressed(int key, int x, int y) {
	printf("onSpecialKeyPressed: char = %c | %d\n", key, key);

	if (glutGetModifiers() == GLUT_ACTIVE_ALT && key == GLUT_KEY_F4) {
		//нажат alt+f4
		exit(0);
	}

	if (Window == W_MAINMENU) {
		MainMenuSpecPressKey(key);
	} else if (Window == W_GAME) {
		GameSpecPressKey(key);
	} else if (Window == W_GAMESETTINGS) {
		GameSettingsSpecPressKey(key);
	} else if (Window == W_HIGHSCORES) {
		HighScoresSpecPressKey(key);
	} else {
		printf("Unknown window value\n");
	}
	glutPostRedisplay();
}