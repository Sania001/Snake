#include "Snake.h"

WindowType Window = W_MAINMENU;

int main(int argc, char **argv) {
	srand(time(0));

	//�������������� GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("Snake");

	glutReshapeFunc(onScreenReshape);
	glutDisplayFunc(onScreenRender);
	glutKeyboardFunc(onKeyPressed);
	glutSpecialFunc(onSpecialKeyPressed);

	//���� ���� � �������
	HWND hWnd = FindWindow(L"GLUT", NULL);
	if (hWnd == NULL) {
		printf("OpenGL window not found!");
		exit(1);
	} else {
		printf("OpenGL window pointer: %d\n", hWnd);
	}

	//������� � ���� ������ � �������� �� ���� �����
	int styles = GetWindowLongPtr(hWnd, GWL_STYLE);
	printf("Window style = %d\n", styles);
	SetWindowLongPtr(
		hWnd, GWL_STYLE, styles ^ WS_SIZEBOX ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX
	);

	//����������� ���� � ������ ������
	int scrWidth = GetSystemMetrics(SM_CXSCREEN);
	int scrHeight = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(
		hWnd, NULL, (scrWidth - WIN_WIDTH) / 2, (scrHeight - WIN_HEIGHT) / 2, 
		WIN_WIDTH + 6, WIN_HEIGHT + 28, SWP_NOOWNERZORDER
	); // 6 � 28 - �������� �� ����� ����
	
	//�������� �������, ���� ��������� �� �������� � ������ �����������
	if (argc < 2) {
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	//��������� ������� �� �����
	LoadHighScores();

	glutMainLoop();
	return 0;
}