#include "Snake.h"

int SnakeDirection; //0=up,1=right,2=down,3=left
int NewSnakeDirection;
int GameSpeed;
int LivingTicks;
int GamePoints;
int GameBonusPoints;
bool isGameOver;
bool isGamePaused;
int LastBonusSpawn;
int BonusSpawnInterval;
int BonusEffect;
int BonusLive;
int CurrentDifficulty;
int CurrentGamemode;

int vObstacles[GAME_ROWS + 10][GAME_COLS + 10];
int gObstacles[GAME_ROWS + 10][GAME_COLS + 10];
int checked[GAME_ROWS + 3][GAME_COLS + 3];

AppleCell Apple;
BonusCell Bonus;
SnakeCell* Head;

int queue[10000][2];
int qhead = 0, qtail = 0;

void queuePush(int x, int y) {
	queue[qtail][0] = x;
	queue[qtail][1] = y;
	qtail++;
}

bool queueEmpty() {
	return (qhead >= qtail);
}

void queueTake(int* x, int* y) {
	*x = queue[qhead][0];
	*y = queue[qhead][1];
	qhead++;
}

void getViewingCell(int x, int y, int d, int* rx, int* ry) {
	int nx = x;
	int ny = y;
	switch (d) {
		case 0: ny--; break;
		case 1: nx++; break;
		case 2: ny++; break;
		case 3: nx--;
	}
	nx = (nx + GAME_COLS) % GAME_COLS;
	ny = (ny + GAME_ROWS) % GAME_ROWS;
	*rx = nx;
	*ry = ny;
}

bool ValidateGameField() {
	/*проверка поля для игры:
		каждая ячейка должна быть доступна минимум с 2х сторон (чтобы не было тупиков)
		поле должно быть связным (чтобы не было недосягаемых, огороженных со всех сторон участков)
	*/
	printf("Validate game field...\n");

	int k = 0;
	for (int i = 0; i < GAME_ROWS; i++) {
		for (int j = 0; j < GAME_COLS; j++) {
			checked[i][j] = 0;
			k = 0;
			for (int l = 0; l < 4; l++) {
				if (isCellReachable(j, i, l)) {
					k++;
				}
			}
			if (k < 2) {
				printf("Bad field for game: dead end at (y = %d, x = %d)\n", i, j);
				checked[i][j] = 1;
				return false;
			}
		}
	}

	//проверка связности поля алгоритмом, напоминающим поиск в ширину в графе
	qhead = 0;
	qtail = 0;
	queuePush(0, 0);
	int cx, cy;
	int rx, ry;
	while (!queueEmpty()) {
		queueTake(&cx, &cy);
		checked[cy][cx] = 1;
		for (int i = 1; i < 5; i++) {
			if (!isCellReachable(cx, cy, i % 4)) {
				continue;
			}
			getViewingCell(cx, cy, i % 4, &rx, &ry);
			if (checked[ry][rx] == 1) {
				continue;
			}
			queuePush(rx, ry);
		}
	}

	printf("\n");
	for (int i = 0; i < GAME_ROWS; i++) {
		for (int j = 0; j < GAME_COLS; j++) {
			if (checked[i][j] == 0) {
				printf("Bad field for game: unreachable cell at (y = %d, x = %d)\n", i, j);
				return false;
			}
		}
	}
	
	return true;
}

void ClearObstacles() {
	for (int i = 0; i < GAME_ROWS; i++) {
		for (int j = 0; j <= GAME_COLS; j++) {
			vObstacles[i][j] = 0;
		}
	}
	for (int i = 0; i <= GAME_ROWS; i++) {
		for (int j = 0; j < GAME_COLS; j++) {
			gObstacles[i][j] = 0;
		}
	}
}

//инициализация и старт игры
void StartGame(int difficulty, int gamemode) {
	//очистка стен и их генерация
	ClearObstacles();
	switch (gamemode) {
		case 0: break; //пустое поле
		case 1: //коробка
			for (int i = 0; i < GAME_ROWS + 1; i++) {
				vObstacles[i][0] = 1;
				vObstacles[i][GAME_COLS] = 1;
			}
			for (int i = 0; i < GAME_COLS + 1; i++) {
				gObstacles[0][i] = 1;
				gObstacles[GAME_ROWS][i] = 1;
			}
			break;
		case 2: 
			/* лабиринт - генерация поля до тех пор, пока оно не станет пригодным для игры 
			   число генерируемых препятствий уменьшается на 1, 
			     если не удается подобрать подходящее поле для игры
			*/
			int OCount = 100;
			int OTick = 10;
			do {
				OTick--;
				if (OTick <= 0) {
					OTick = 10;
					OCount--;
				}
				ClearObstacles();
				for (int i = 0; i < OCount; i++) {
					vObstacles[rand() % GAME_ROWS][rand() % (GAME_COLS + 1)] = 1;
					gObstacles[rand() % (GAME_ROWS + 1)][rand() % GAME_COLS] = 1;
				}
				//расчищаем путь для змейки
				for (int i = 1; i <= 8; i++) {
					vObstacles[1][i] = 0;
				}
			} while (!ValidateGameField());
			printf("Obstacle count: %d\n", OCount);
			break;
	}

	isGameOver = false;
	isGamePaused = false;
	SnakeDirection = 1;
	NewSnakeDirection = 1;
	LivingTicks = 0;
	GamePoints = 0;
	GameBonusPoints = 0;
	LastBonusSpawn = 0;
	CurrentDifficulty = difficulty;
	CurrentGamemode = gamemode;

	//настройки игры в соответствии со сложностью
	switch (difficulty) {
		case 0: 
			BonusSpawnInterval = 4;
			BonusEffect = 3;
			BonusLive = 25;
			GameSpeed = 400;
			break;
		case 1: 
			BonusSpawnInterval = 5;
			BonusEffect = 2;
			BonusLive = 20;
			GameSpeed = 250;
			break;
		case 2: 
			BonusSpawnInterval = 7;
			BonusEffect = 1;
			BonusLive = 15;
			GameSpeed = 135;
			break;
		default:
			printf("Unknown difficulty %d\n", difficulty);
			break;
	}

	//выращивание змейки
	Head = (SnakeCell*)malloc(sizeof(SnakeCell));
	Head->x = 3;
	Head->y = 1;
	Head->prev = NULL;
	Head->fat = false;
	Head->visible = true;
	Head->next = (SnakeCell*)malloc(sizeof(SnakeCell));
	Head->next->x = 2;
	Head->next->y = 1;
	Head->next->fat = false;
	Head->next->visible = true;
	Head->next->prev = Head;
	Head->next->next = (SnakeCell*)malloc(sizeof(SnakeCell));
	Head->next->next->x = 1;
	Head->next->next->y = 1;
	Head->next->next->fat = false;
	Head->next->next->visible = true;
	Head->next->next->prev = Head->next;
	Head->next->next->next = NULL;

	//генерация первого яблока - оно не должно совпадать 
	//  ни с одним сегментом змеи по координатам
	bool goodApple;
	do {
		Apple.x = rand() % GAME_COLS;
		Apple.y = rand() % GAME_ROWS;
		goodApple = true;
		if (Apple.x == Head->x && Apple.y == Head->y) {
			goodApple = false;
		}
		if (Apple.x == Head->next->x && Apple.y == Head->next->y) {
			goodApple = false;
		}
		if (Apple.x == Head->next->next->x && Apple.y == Head->next->next->y) {
			goodApple = false;
		}
	} while (!goodApple);

	Bonus.x = 0;
	Bonus.y = 0;
	Bonus.living = 0;
	Bonus.isActive = false;

	//первый тик через GameSpeed мс
	glutTimerFunc(GameSpeed, onTick, W_GAME);
}

//функция проверки наличия стен при передвижении из ячейки (x, y) в направлении d
bool isCellReachable(int x, int y, int d) {
	bool res = 1;
	switch (d) {
		case 0: 
			if ((y == 0) && (gObstacles[GAME_ROWS][x])) {
				res = 0; break;
			}
			res = (gObstacles[y][x] == 0);
			break;
		case 1:
			if ((x == GAME_COLS - 1) && (vObstacles[y][0])) {
				res = 0;
				break;
			}
			res = (vObstacles[y][x + 1] == 0);
			break;
		case 2: 
			if ((y == GAME_ROWS - 1) && (gObstacles[0][x])) {
				res = 0; break;
			}
			res = (gObstacles[y + 1][x] == 0);
			break;
		case 3: 
			if ((x == 0) && (vObstacles[y][GAME_COLS])) {
				res = 0; break;
			}
			res = (vObstacles[y][x] == 0);
			break;
		default: res = 0; break;
	}
	return res;
}

void GameOver() {
	isGameOver = true;
}

//главная игровая функция - вызывается каждый тик
void GameTick() {
	if (isGameOver || isGamePaused) {
		return;
	}

	SnakeCell* f;

	//передвижение головы змеи. только головы
	SnakeDirection = NewSnakeDirection;
	int newX, newY;
	//newX и newY - новые координаты головы
	getViewingCell(Head->x, Head->y, SnakeDirection, &newX, &newY);
	//проверка столкновения головы с телом
	f = Head;
	while (f->next != NULL) {
		if (newX == f->x && newY == f->y) {
			GameOver();
			return;
		}
		f = f->next;
	}
	//проверка столкновения головы со стеной
	if (!isCellReachable(Head->x, Head->y, SnakeDirection)) {
		GameOver();
		return;
	}

	//движение змейки
	f = Head;
	while (f->next != NULL) {
		f = f->next;
	}
	do {
		if (f->prev != NULL) {
			f->x = f->prev->x;	
			f->y = f->prev->y;
			f->fat = f->prev->fat;
			f->visible = f->prev->visible;
		}
		f = f->prev;
	} while (f != NULL);

	Head->x = newX;
	Head->y = newY;
	Head->fat = false;

	int snakeLength = 0;
	f = Head;
	while (f != NULL) {
		snakeLength++;
		f = f->next;
	}
	if (snakeLength >= GAME_COLS * GAME_ROWS - 2) {
		//заканчиваем игру, когда не остается свободных клеток
		GameOver();
		return;
	}

	//обработка яблока
	if (Head->x == Apple.x && Head->y == Apple.y) {
		//новое яблоко
		bool badApple;
		do {
			Apple.x = rand() % GAME_COLS;
			Apple.y = rand() % GAME_ROWS;
			f = Head;
			badApple = false;
			while (f != NULL) {
				if (f->x == Apple.x && f->y == Apple.y) {
					badApple = true;
					break;
				}
				f = f->next;
			}
			if (Apple.x == Bonus.x && Apple.y == Bonus.y && Bonus.isActive) {
				badApple = true;
			}
			if (!badApple) {
				break;
			}
		} while (true);
		GamePoints++;
		Head->fat = true;
		//новый элемент в змейке - первый тик невидимый, затем плетется в хвосте
		SnakeCell* n = (SnakeCell*)malloc(sizeof(SnakeCell));
		n->x = 0;
		n->y = 0;
		n->fat = false;
		n->visible = false;
		n->next = NULL;
		//подцепление нового элемента змейки к её концу
		f = Head;
		while (true) {
			if (f->next == NULL) {
				f->next = n;
				n->prev = f;
				break;
			}
			f = f->next;
		}
	}

	//обработка получения бонуса
	if (Bonus.isActive && Head->x == Bonus.x && Head->y == Bonus.y) {
		Bonus.x = 0;
		Bonus.y = 0;
		Bonus.isActive = false;
		Bonus.living = 0;
		GameBonusPoints++;
		//змейка худеет на 3-5 сегментов в зависимости от сложности
		WipeSnake(true);
	}

	//обработка генерации бонуса
	if (GamePoints % BonusSpawnInterval == 0 && LastBonusSpawn != GamePoints) {
		LastBonusSpawn = GamePoints;
		Bonus.isActive = true;
		Bonus.living = BonusLive;

		bool badBonus;
		do {
			Bonus.x = rand() % GAME_COLS;
			Bonus.y = rand() % GAME_ROWS;
			f = Head;
			badBonus = false;
			while (f != NULL) {
				if (f->x == Bonus.x && f->y == Bonus.y) {
					badBonus = true;
					break;
				}
				f = f->next;
			}
			if (Apple.x == Bonus.x && Apple.y == Bonus.y) {
				badBonus = true;
			}
			if (!badBonus) {
				break;
			}
		} while (true);
	}

	//бонус исчезает
	if (Bonus.isActive && Bonus.living == 0) {
		Bonus.isActive = false;
		Bonus.x = 0;
		Bonus.y = 0;
	}

	if (Bonus.isActive) {
		Bonus.living--;
	}

	LivingTicks++;
	glutTimerFunc(GameSpeed, onTick, W_GAME);
}

//стирание некоторой части змеи:
//limited = true: стирается некоторое число последних сегментов, равное BonusEffect
//limited = false: стирается вся змея (нужна когда заканчивается игра)
void WipeSnake(bool limited) {
	SnakeCell* f = Head;
	while (f->next != NULL) {
		f = f->next;
	}
	SnakeCell* curr;
	if (limited) {
		for (int i = 0; (f->prev != NULL) && (i < BonusEffect); i++) {
			curr = f;
			f = f->prev;
			if (f != NULL) {
				f->next = NULL;
			}
			free(curr);
		}
	} else {
		while (f != NULL) {
			curr = f;
			f = f->prev;
			free(curr);
		}
	}
}

//вкл/выкл режима паузы
void TogglePauseGame() {
	if (isGameOver) {
		return;
	}
	isGamePaused = !isGamePaused;
	if (isGamePaused == false) {
		glutTimerFunc(GameSpeed, onTick, W_GAME);
		NewSnakeDirection = SnakeDirection;
	}
}

void GamePressKey(unsigned char key) {
	if (key == 27) {
		if (isGamePaused) {
			GameOver();
			return;
		}
		TogglePauseGame();
		return;
	}
	if (key == 13) {
		if (isGameOver) {
			//змейка худеет полностью
			WipeSnake(false);
			Window = W_CHAMPION;

			//расчет на ассемблере количества очков, полученных во время игры
			int pointsC = GamePoints + GameBonusPoints * BonusEffect;
			int pointsAsm;
			__asm {
				push eax;
				push edx;
				mov eax, GameBonusPoints;
				cdq;
				imul BonusEffect;
				add eax, GamePoints;
				mov pointsAsm, eax;
				pop edx;
				pop eax;
			}
			assert(pointsC == pointsAsm);

			HandleHighscoreUpdate(
				CurrentDifficulty, CurrentGamemode, pointsAsm, LivingTicks
			);
			return;	
		}
	}
	if (isGamePaused) {
		TogglePauseGame();
		return;		
	}
}

//обработка нажатия стрелок во время игры - 
//  новое направление головы пишется в NewSnakeDirection
void GameSpecPressKey(int key) {
	switch (key) {
		case GLUT_KEY_UP: 
			if (SnakeDirection % 2 == 1) 
				NewSnakeDirection = 0; 
			break;
		case GLUT_KEY_RIGHT: 
			if (SnakeDirection % 2 == 0) 
				NewSnakeDirection = 1; 
			break;
		case GLUT_KEY_DOWN: 
			if (SnakeDirection % 2 == 1) 
				NewSnakeDirection = 2; 
			break;
		case GLUT_KEY_LEFT: 
			if (SnakeDirection % 2 == 0) 
				NewSnakeDirection = 3;
	}
	if (isGamePaused) {
		TogglePauseGame();
		return;
	}
}