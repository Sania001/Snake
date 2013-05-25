#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <Winuser.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include <assert.h>

const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

const int CELL_WIDTH = 40;
const int CELL_HEIGHT = 40;
const int GAME_ROWS = 10;
const int GAME_COLS = 15;

const double pi = 3.1415926535;

struct Color {
	int red, green, blue;
};
/*inline bool operator ==(Color &r1, Color &r2) {
	return r1.red == r2.red && r1.green == r2.green && r1.blue == r2.blue;
}*/

struct GlPixel{
	GLubyte red, green, blue;
};

enum WindowType {
	W_MAINMENU, W_GAME, W_GAMESETTINGS, W_HIGHSCORES, W_CHAMPION
};

#pragma pack(push, 1)
struct HighScoreRecord {
	char nickname[25];
	int points;
	int ticks;
};
#pragma pack(pop)

struct SnakeCell {
	int x;
	int y; 
	bool fat;
	bool visible;
	SnakeCell* next;
	SnakeCell* prev;
};

struct AppleCell {
	int x;
	int y;
};

struct BonusCell {
	int x;
	int y;
	int living;
	bool isActive;
};

extern WindowType Window;

//переменные игры
extern int SnakeDirection;
extern bool isGameOver;
extern bool isGamePaused;
extern int LivingTicks;
extern int GamePoints;
extern int GameBonusPoints;
extern int LastBonusSpawn;
extern int BonusSpawnInterval;
extern int BonusEffect;
extern int BonusLive;
extern int CurrentDifficulty;
extern int CurrentGamemode;
extern AppleCell Apple;
extern BonusCell Bonus;
extern SnakeCell* Head;

extern int vObstacles[GAME_ROWS + 10][GAME_COLS + 10];
extern int gObstacles[GAME_ROWS + 10][GAME_COLS + 10];

extern HighScoreRecord hsr[3][3][3];
extern int checked[GAME_ROWS + 3][GAME_COLS + 3];

Color getColor(int red, int green, int blue);
void ImageDrawString(const char* string);
int ImageTextWidth(const char* string);
void ImageSetColor(Color c);
void ImageLineWidth(int w);
void ImageLine(int x1, int y1, int x2, int y2, Color color);
void ImageDashedLine(int x1, int y1, int x2, int y2, Color color);
void ImageFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color c);
void ImageText(int x1, int y1, char* text, Color c);
void ImageFillScreen(Color c);
void PrintColor(Color c);
void ImagePixel(int x, int y, Color color);
void ImageRectangle(int x1, int y1, int x2, int y2, Color color);
void ImageFilledRectangle(int x1, int y1, int x2, int y2, Color color);
void ImageCopy(int x, int y, int w, int h, int nx, int ny, int mult);
void ImageFilledCircle(int x, int y, int rad, Color c);

void RenderAppleCell(int x, int y);
void RenderBodyCell(int x, int y, bool fat);
void RenderHeadCell(int x, int y, int d, bool fat);
void RenderTailCell(int x, int y, int d, bool fat);
void RenderBonusCell(int x, int y);

void onScreenRender();
void onScreenReshape(int w, int h);
void onTick(int value);
void onKeyPressed(unsigned char key, int x, int y);
void onSpecialKeyPressed(int key, int x, int y);

void DrawMainMenuScreen();
void MainMenuPressKey(unsigned char key);
void MainMenuSpecPressKey(int key);

void DrawGameScreen();
void GamePressKey(unsigned char key);
void GameSpecPressKey(int key);
void StartGame(int difficulty, int gamemode);
void GameTick();
void WipeSnake(bool limited);
bool isCellReachable(int x, int y, int d);

void DrawHighScoresScreen();
void HighScoresPressKey(unsigned char key);
void HighScoresSpecPressKey(int key);
void WipeHighScores();
void LoadHighScores();
void SaveHighScores();

void DrawGameSettingsScreen();
void GameSettingsPressKey(unsigned char key);
void GameSettingsSpecPressKey(int key);

void DrawChampionScreen();
void ChampionPressKey(unsigned char key);
void HandleHighscoreUpdate(int difficulty, int gamemode, int points, int ticks);
void ChampionTick();