#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <Windows.h>

#define WINDOW_WIDTH  1500
#define WINDOW_HEIGHT 750
#define TIMER_ON        1 // 0:disable timer, 1:enable timer
#define D2R 0.0174532
#define STARTX -400
#define STARTY -375
#define INCREASEY 75
#define INCREASEX 100
#define STARTSC 1
#define WINNERSC 2 
#define LOSESC 3
#define ACTIONSC 4

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
char screen;
int  winWidth, winHeight, timer, lvl, activesquare, pactivesquare, lastindex;
bool leftright, moveaction, wait, keydown, inaction;

typedef struct {
	int x, y;
	bool active;
}square_t;

square_t square[40];

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void disp_Lines()
{
	if (screen == ACTIONSC)
	{
		glLineWidth(4);
		glColor3f(1, 1, 0);

		for (int i = -375; i <= 375; i += 75)
		{
			glBegin(GL_LINES);
			glVertex2f(-400, i);
			glVertex2f(400, i);
			glEnd();
		}

		for (int i = -400; i <= 400; i += 100)
		{
			glBegin(GL_LINES);
			glVertex2f(i, 375);
			glVertex2f(i, -375);
			glEnd();
		}
	}
}

void placesquares(square_t* square, int activesquare, int* lastindex)
{
	if (screen == ACTIONSC)
	{
		//squares
		for (int i = 0; i < activesquare; i++)
		{
			square[*lastindex].x = STARTX - i * INCREASEX;
			square[*lastindex].y = STARTY + lvl * INCREASEY;
			square[*lastindex].active = true;

			(*lastindex)++;
		}
	}
}

void disp_Squares(square_t* square, int lastindex)
{
	if (screen == ACTIONSC && moveaction == false) //deðiþtir moveaction sil
	{
		for (int i = 0; i < lastindex; i++)
		{
			if (square[i].active == true)
			{
				//squares
				{
					//top
					{
						glBegin(GL_TRIANGLE_STRIP);
						glColor3ub(220, 100, 150);
						glVertex2f(square[i].x - INCREASEX / 2.0, square[i].y - INCREASEY / 2.0);

						glColor3ub(255, 10, 50);
						glVertex2f(square[i].x - INCREASEX, square[i].y);
						glVertex2f(square[i].x, square[i].y);

						glEnd();
					}
					//right
					{
						glBegin(GL_TRIANGLE_STRIP);
						glColor3ub(220, 100, 150);
						glVertex2f(square[i].x - INCREASEX / 2.0, square[i].y - INCREASEY / 2.0);

						glColor3ub(255, 10, 50);
						glVertex2f(square[i].x, square[i].y);
						glVertex2f(square[i].x, square[i].y - INCREASEY);

						glEnd();
					}
					//left
					{
						glBegin(GL_TRIANGLE_STRIP);
						glColor3ub(220, 100, 150);
						glVertex2f(square[i].x - INCREASEX / 2.0, square[i].y - INCREASEY / 2.0);

						glColor3ub(255, 10, 50);
						glVertex2f(square[i].x - INCREASEX, square[i].y);
						glVertex2f(square[i].x - INCREASEX, square[i].y - INCREASEY);

						glEnd();
					}
					//bottom
					{
						glBegin(GL_TRIANGLE_STRIP);
						glColor3ub(220, 100, 150);
						glVertex2f(square[i].x - INCREASEX / 2.0, square[i].y - INCREASEY / 2.0);

						glColor3ub(255, 10, 50);
						glVertex2f(square[i].x - INCREASEX, square[i].y - INCREASEY);
						glVertex2f(square[i].x, square[i].y - INCREASEY);

						glEnd();
					}

					/*glColor3f(1, 0, 0);
					glBegin(GL_QUADS);
					glVertex2f(square[i].x, square[i].y);
					glVertex2f(square[i].x - INCREASEX, square[i].y);
					glVertex2f(square[i].x - INCREASEX, square[i].y - INCREASEY);
					glVertex2f(square[i].x, square[i].y - INCREASEY);
					glEnd();*/
				}

				//frames
				{
					glColor3f(1, 1, 0);
					glBegin(GL_LINE_LOOP);
					glVertex2f(square[i].x, square[i].y);
					glVertex2f(square[i].x - INCREASEX, square[i].y);
					glVertex2f(square[i].x - INCREASEX, square[i].y - INCREASEY);
					glVertex2f(square[i].x, square[i].y - INCREASEY);
					glEnd();
				}
			}
		}
	}
	inaction = false;
}

void move_Squares(square_t* square, int lastindex, int activesquare)
{
	inaction = true;
	if (screen == ACTIONSC && keydown == false)
	{
		if (square[lastindex - activesquare].x == 400)
			leftright = true;

		if (square[lastindex - 1].x == -400 + INCREASEX)
			leftright = false;

		moveaction = true;

		for (int i = lastindex - activesquare; i < lastindex; i++)
		{
			if (square[i].active == true)
			{
				if (leftright == true)
					square[i].x -= INCREASEX;
				else
					square[i].x += INCREASEX;
			}
		}

		moveaction = false;
	}
}

void disp_Background()
{
	if (screen == ACTIONSC)
	{
		//left
		{
			glColor3ub(78, 185, 127);
			glBegin(GL_QUADS);
			glVertex2f(-401, 400);
			glVertex2f(-401, -400);

			glColor3f(0, 0, 0);
			glVertex2f(-800, -400);
			glVertex2f(-800, 400);
			glEnd();
		}

		//right
		{
			glColor3ub(78, 185, 127);
			glBegin(GL_QUADS);
			glVertex2f(401, 400);
			glVertex2f(401, -400);

			glColor3f(0, 0, 0);
			glVertex2f(800, -400);
			glVertex2f(800, 400);
			glEnd();
		}

		//bottom
		{
			glColor3ub(78, 185, 127);
			glRectf(-401, -376, 401, -450);
		}

		//top
		{
			glColor3ub(78, 185, 127);
			glRectf(-401, 376, 401, 450);
		}
	}
}

void disp_start_screen()
{
	if (screen == STARTSC)
	{
		//background
		{
			glBegin(GL_TRIANGLE_STRIP);
			glColor3ub(246, 57, 105);
			glVertex2f(-768, 400);

			glColor3ub(136, 210, 239);
			glVertex2f(-768, -400);
			glVertex2f(768, 400);


			glColor3ub(246, 57, 105);
			glVertex2f(768, -400);
			glEnd();
		}

		//display messages
		{
			glLineWidth(3);
			glColor3f(0, 1, 0);
			vprint2(-346, 4, 1, "Tetris Game");
			glColor3f(0, 0.5, 0);
			vprint2(-348, 2, 1, "Tetris Game");
			glColor3f(0, 0.2, 0);
			vprint2(-350, 0, 1, "Tetris Game");
			glColor3f(0, 0, 0);
			vprint2(-352, -2, 1, "Tetris Game");


			glColor3f(0, 0, 0);
			vprint2(-285, -130, 0.4, "Press Spacebar To Start");
			glColor3f(0, 1, 0);
			vprint2(-113, -132, 0.4, "Spacebar ");
			glColor3f(0.3, 0, 0);
			vprint2(-110, -128, 0.4, "Spacebar ");
			glColor3f(0.5, 0, 0);
			vprint2(-108, -126, 0.4, "Spacebar ");
			glColor3f(0.8, 0, 0);
			vprint2(-106, -124, 0.4, "Spacebar ");
			glColor3f(1, 0, 0);
			vprint2(-104, -122, 0.4, "Spacebar ");

			glColor3f(0, 0, 1);
			vprint2(0, -350, 0.5, "By Ahmet Oguz Ergin");
		}
	}
}

void disp_winner_screen()
{
	if (screen == WINNERSC)
	{
		//background
		{
			glColor3ub(191, 252, 101);
			glBegin(GL_QUADS);
			glVertex2f(-768, 400);
			glVertex2f(-768, -400);

			glColor3ub(45, 131, 27);
			glVertex2f(768, -400);
			glVertex2f(768, 400);
			glEnd();
		}

		//display messages
		{
			glLineWidth(3);
			glColor3f(0, 0, 1);
			vprint2(-366, 4, 1, "! You Win !");
			glColor3f(0, 0, 0.5);
			vprint2(-368, 2, 1, "! You Win !");
			glColor3f(0, 0, 0.2);
			vprint2(-370, 0, 1, "! You Win !");
			glColor3f(0, 0, 0);
			vprint2(-372, -2, 1, "! You Win !");


			glColor3f(0, 0, 0);
			vprint2(-285, -130, 0.4, "Press Spacebar To Start");
			glColor3f(0, 1, 0);
			vprint2(-113, -132, 0.4, "Spacebar ");
			glColor3f(0.3, 0, 0);
			vprint2(-110, -128, 0.4, "Spacebar ");
			glColor3f(0.5, 0, 0);
			vprint2(-108, -126, 0.4, "Spacebar ");
			glColor3f(0.8, 0, 0);
			vprint2(-106, -124, 0.4, "Spacebar ");
			glColor3f(1, 0, 0);
			vprint2(-104, -122, 0.4, "Spacebar ");
		}
	}
}

void disp_lose_screen()
{
	if (screen == LOSESC)
	{
		//background
		{
			glColor3ub(255, 174, 26);
			glBegin(GL_QUAD_STRIP);
			glVertex2f(-768, 400);
			glVertex2f(-768, -400);

			glColor3ub(223, 59, 52);
			glVertex2f(0, 400);
			glVertex2f(0, -400);

			glColor3ub(160, 22, 136);
			glVertex2f(768, 400);
			glVertex2f(768, -400);

			glEnd();
		}

		//display messages
		{
			glLineWidth(3);
			glColor3f(0, 0, 1);
			vprint2(-416, 4, 1, "< You Lose >");
			glColor3f(0, 0, 0.5);
			vprint2(-418, 2, 1, "< You Lose >");
			glColor3f(0, 0, 0.2);
			vprint2(-420, 0, 1, "< You Lose >");
			glColor3f(0, 0, 0);
			vprint2(-422, -2, 1, "< You Lose >");

			glColor3f(0, 0, 0);
			vprint2(-285, -130, 0.4, "Press Spacebar To Start");
			glColor3f(0, 0, 1);
			vprint2(-113, -132, 0.4, "Spacebar ");
			glColor3f(0, 0.3, 0);
			vprint2(-110, -128, 0.4, "Spacebar ");
			glColor3f(0, 0.5, 0);
			vprint2(-108, -126, 0.4, "Spacebar ");
			glColor3f(0, 0.8, 0);
			vprint2(-106, -124, 0.4, "Spacebar ");
			glColor3f(0, 1, 0);
			vprint2(-104, -122, 0.4, "Spacebar ");
		}
	}
}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//START SCREEN
	{
		disp_start_screen();
	}

	//WINNER SCREEN
	{
		disp_winner_screen();
	}

	//WINNER SCREEN
	{
		disp_lose_screen();
	}

	//ACTIVE SCREEN
	{
		//display lines
		disp_Lines();

		//display squares
		disp_Squares(square, lastindex);

		//display left and right backgrounds
		disp_Background();
	}

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	keydown = true;
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == 32)
	{
		if (screen == STARTSC)
		{
			screen = ACTIONSC;
		}

		else if (screen == WINNERSC)
		{
			screen = ACTIONSC;

			//initialize
			{
				//timer
				timer = 240;

				//movement of squares is right
				leftright = false;

				//initialize all squares to inactive
				{
					for (int i = 0; i < 40; i++)
						square[i].active = false;
				}

				//initialize active squares
				{
					activesquare = 4;
					pactivesquare = 4;
					lastindex = 0;
				}

				//initialize lvl
				lvl = 0;
			}
		}

		else if (screen == LOSESC)
		{
			screen = ACTIONSC;

			//initialize
			{
				//timer
				timer = 240;

				//movement of squares is right
				leftright = false;

				//initialize all squares to inactive
				{
					for (int i = 0; i < 40; i++)
						square[i].active = false;
				}

				//initialize active squares
				{
					activesquare = 4;
					pactivesquare = 4;
					lastindex = 0;
				}

				//initialize lvl
				lvl = 0;
			}
		}

		if (screen == ACTIONSC && moveaction == false)
		{
			if (inaction == true)
			{
				//	printf("Kayma engellendi\n");

				if (square[lastindex - activesquare].x == 400)
					leftright = true;

				if (square[lastindex - 1].x == -400 + INCREASEX)
					leftright = false;


				for (int i = lastindex - activesquare; i < lastindex; i++)
				{
					if (square[i].active == true)
					{
						if (leftright == true)
							square[i].x += INCREASEX;
						else
							square[i].x -= INCREASEX;
					}
				}

				if (activesquare == 0)
					screen = LOSESC;

				moveaction = false;
			}

			if (wait)
				Sleep(300);
			leftright = false;
			int newactivesquare = 0;

			//update active squares
			if (lvl > 1)
			{
				int flag;
				for (int i = 1; i <= activesquare; i++)
				{
					flag = 0;

					for (int j = 1; j <= pactivesquare; j++)
					{
						if (square[lastindex - i].x > -400 && square[lastindex - i].x == square[lastindex - activesquare - j].x && square[lastindex - activesquare - j].active == true)
							flag = 1;
					}
					if (flag == 1)
					{
						square[lastindex - i].active = true;
						newactivesquare++;
					}
					else
						square[lastindex - i].active = false;
				}
				pactivesquare = activesquare;
				activesquare = newactivesquare;
			}

			//increase speed
			{
				if (activesquare != 0)
				{

					if (timer >= 140)
						timer -= 40;
					else if (timer >= 120)
						timer -= 20;
					else if (timer >= 60)
						timer -= 10;
					else if (timer > 10)
						timer -= 5;
				}
			}

			if (lvl < 11)
			{
				lvl++;
				if (lvl < 11)
					placesquares(square, activesquare, &lastindex);
			}

			if (activesquare == 0)
			{
				screen = LOSESC;

				//initialize
				{
					//timer
					timer = 240;

					//movement of squares is right
					leftright = false;

					//initialize all squares to inactive
					{
						for (int i = 0; i < 40; i++)
							square[i].active = false;
					}

					//initialize active squares
					{
						activesquare = 4;
						pactivesquare = 4;
						lastindex = 0;
					}

					//initialize lvl
					lvl = 0;
				}
			}
		}
	}

	if (key == 'b' || key == 'B')
		wait = !wait;

	keydown = false;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	if (up == true && timer >= 60)
		timer -= 30;
	if (down == true && timer <= 1000)
		timer += 30;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// GLUT to OpenGL coordinate conversion:
	x = x - winWidth / 2;
	y = winHeight / 2 - y;

	if (stat == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		printf("%d , %d\n", x, y);

	if (stat == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
		printf("glVertex2f(%d , %d);\n", x, y);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.
	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.
	x = x - winWidth / 2;
	y = winHeight / 2 - y;
	printf("%d , %d\n", x, y);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(timer, onTimer, 0);
	// Write your codes here.

	//move squares
	if (lvl < 11 && screen == ACTIONSC)
		move_Squares(square, lastindex, activesquare);

	//winner screen
	if (lvl == 11 && activesquare != 0 && screen == ACTIONSC)
	{
		if (wait)
			Sleep(300);
		screen = WINNERSC;
	}

	else if (activesquare == 0 && screen == ACTIONSC)
	{
		if (wait)
			Sleep(300);
		screen = LOSESC;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

void Init() {
	//screen
	screen = STARTSC;

	//timer
	timer = 240;
	wait = true;

	//movement of squares is right
	leftright = false;

	//initialize all squares to inactive
	{
		for (int i = 0; i < 40; i++)
			square[i].active = false;
	}

	//initialize active squares
	{
		activesquare = 4;
		pactivesquare = 4;
		lastindex = 0;
	}

	//initialize lvl
	lvl = 0;

	// Smoothing shapes
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(20, 50);
	glutCreateWindow("--> Tetris Game <--");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	// glutMouseFunc(onClick);
	// glutMotionFunc(onMoveDown);
	// glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(timer, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}