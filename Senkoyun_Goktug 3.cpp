/*********
   CTIS164 - HOMEWORK III UFO CHASE
----------
STUDENT : Göktuð Þenkoyun 22103191
SECTION : 164-01
HOMEWORK: HOMEWORK III
----------
PROBLEMS: 
-Rarely a GAME OVER screen randomly appears. couldn't figure out why it is the case (might be solved but im still unsure).
-I couldnt able to add rotation function because I couldnt figure out how to use vertex function with
circles and other parts in my complex shape. Otherwise everything is working correctly
----------
ADDITIONAL FEATURES: 
-Background Gradient
-Game Over screen when enemy reaches the mouse
-Automatically adjusting difficulty according to timer. Each 15 seconds enemy chase speed increases by 0.5 points


*********/
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define  TIMER_PERIOD  16 // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
bool space = true;   // start pause
bool  inside = true; // inside: if inside ufo follows object
double time = 0; // timer functionalities
int time2=0;
int k=1;
int t;
/* User defined Types and variables for the application */
#define ENEMY_SPEED 2
#define PLAYER_SPEED 5
double espeed = 2;
int mx = -400; // positions
int my = -200;
bool game = TRUE; //if game false it will be gameover
polar_t enemyangle; 
polar_t playerangle; 
typedef struct {
	vec_t pos;
	float angle;
} player_t;

typedef struct {
	vec_t pos;
	vec_t vel;  //velocity
	float angle;
} enemy_t;

player_t P = { { 0, 0 }, 45 };
enemy_t  E = { { 0, 200 } };



//
// to draw circle, center at (x,y)
//  radius r
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

void vertex(enemy_t E,player_t P, double angle) 
{
	double xe = (E.pos.x * cos(angle) - E.pos.y * sin(angle)) + P.pos.x;
	double ye = (E.pos.x * sin(angle) + E.pos.y * cos(angle)) + P.pos.y;
	glVertex2d(xe, ye);
}
void background()
{
	glBegin(GL_POLYGON);
	glColor3f(0, 31 / 255.0, 77 / 255.0);
	glVertex2f(-(winWidth / 2), -(winHeight / 2));
	glVertex2f((winWidth / 2), -(winHeight / 2));
	glColor3f(0, 85 / 255.0, 213 / 255.0);
	glVertex2f((winWidth / 2), (winHeight / 2));
	glVertex2f(-(winWidth / 2), (winHeight / 2));

	glEnd();


	glColor3f(173 / 255.0, 8 / 255.0, 51 / 255.0);
	glRectf(-(winWidth / 2), winHeight / 2, winWidth / 2, winHeight / 2 - 60);
	glRectf(-(winWidth / 2), winHeight / 2, -(winWidth / 2)+60, -(winHeight / 2) );

	glRectf( winWidth / 2, winHeight / 2, (winWidth / 2)-60, -(winHeight / 2));
	glRectf(-(winWidth / 2), -(winHeight / 2), winWidth / 2, -(winHeight / 2) + 60);

	
	glColor3f(179 / 255.0, 179 / 255.0, 179 / 255.0);
	circle(-(winWidth / 2) + 200, (winHeight / 2) - 150, 50);
	glColor3f(89 / 255.0, 89 / 255.0, 89 / 255.0);
	circle(-(winWidth / 2) + 180, (winHeight / 2) - 130, 7);
	circle(-(winWidth / 2) + 220, (winHeight / 2) - 170, 8);
	circle(-(winWidth / 2) + 190, (winHeight / 2) - 135, 10);
	circle(-(winWidth / 2) + 185, (winHeight / 2) - 170, 14);
	glColor3f(65 / 255.0, 65 / 255.0, 65 / 255.0);
	circle(-(winWidth / 2) + 220, (winHeight / 2) - 125, 17);
	circle(-(winWidth / 2) + 170, (winHeight / 2) - 150, 18);
	glColor3f(0, 0, 0);

	
	
	
	vprint(-(winWidth / 2) + 300, winHeight / 2 - 35, GLUT_BITMAP_8_BY_13, "%s", "GOKTUG SENKOYUN 22103191");
	vprint(-(winWidth / 2) + 60, -(winHeight / 2) + 35, GLUT_BITMAP_8_BY_13, "%s", "Press Spacebar to");
	vprint(-(winWidth / 2) + 380, -(winHeight / 2) + 35, GLUT_BITMAP_8_BY_13, "%s", "Mouse is");
	vprint(-(winWidth / 2) + 650, -(winHeight / 2) + 35, GLUT_BITMAP_8_BY_13, "%s", "Angle =");
	vprint(-(winWidth / 2) + 150, winHeight / 2 - 35, GLUT_BITMAP_8_BY_13, "%s", "Time ");
	vprint(-(winWidth / 2) + 190, winHeight / 2 - 35, GLUT_BITMAP_8_BY_13, "%0.0f", time);
	vprint(-(winWidth / 2) + 700, winHeight / 2 - 35, GLUT_BITMAP_8_BY_13, "%0.1f", espeed);
	vprint(-(winWidth / 2) + 650, winHeight / 2 - 35, GLUT_BITMAP_8_BY_13, "%s", "Speed");
	
	
}
void drawPlayer(player_t p) 
{

	// Body of the Player Object.
	glColor3f(1, 1, 1);
	circle_wire(p.pos.x, p.pos.y, 9);

	glColor4f(0, 0, 1, 0.3);
	circle(p.pos.x, p.pos.y, 8);
}

void drawEnemy(enemy_t e) 
{

	
	double angle = e.angle * D2R;
	glColor3f(255 / 255.0, 255 / 255.0, 153 / 255.0);
	circle(e.pos.x, e.pos.y - 5, 15);
	glColor3f( 46/ 255.0,  47/ 255.0, 48/ 255.0);
	circle(e.pos.x, e.pos.y + 5, 16);
	
	//Upper and lower body
	glColor3f(71 / 255.0, 71 / 255.0, 107 / 255.0);
	glRectf(e.pos.x - 50 / 2, e.pos.y - 10 / 2, e.pos.x + 50 / 2, e.pos.y + 10 / 2);
	glRectf(e.pos.x - 36 / 2, e.pos.y - 23 / 2, e.pos.x + 36 / 2, e.pos.y + 10 / 2);

	//Upper part
	glBegin(GL_TRIANGLES);
	glVertex2f(e.pos.x - 25, e.pos.y + 5);
	glVertex2f(e.pos.x - 25, e.pos.y - 5);
	glVertex2f(e.pos.x - 35, e.pos.y - 5);
	glVertex2f(e.pos.x + 25, e.pos.y + 5);
	glVertex2f(e.pos.x + 25, e.pos.y - 5);
	glVertex2f(e.pos.x + 35, e.pos.y - 5);
	//Lower part
	glVertex2f(e.pos.x - 18, e.pos.y - 5);
	glVertex2f(e.pos.x - 18, e.pos.y - 11);
	glVertex2f(e.pos.x - 25, e.pos.y - 5);
	glVertex2f(e.pos.x + 18, e.pos.y - 5);
	glVertex2f(e.pos.x + 18, e.pos.y - 11);
	glVertex2f(e.pos.x + 25, e.pos.y - 5);
	glEnd();
	//Windows
	glColor3f(0, 0, 0);
	circle(e.pos.x, e.pos.y, 3);
	circle(e.pos.x - 13, e.pos.y, 3);
	circle(e.pos.x + 13, e.pos.y, 3);
	glColor3f(217 / 255.0, 217 / 255.0, 217 / 255.0);
	circle(e.pos.x, e.pos.y, 2);
	circle(e.pos.x - 13, e.pos.y, 2);
	circle(e.pos.x + 13, e.pos.y, 2);

	//Line
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glLineWidth(2);
	glVertex2f(e.pos.x - 25, e.pos.y - 5);
	glVertex2f(e.pos.x + 25, e.pos.y - 5);
	glEnd();
	vprint(-(winWidth / 2) + 720, -(winHeight / 2) + 35, GLUT_BITMAP_8_BY_13, "%0.0f", enemyangle.angle);
}
void drawTitle()
{
	glBegin(GL_POLYGON);
	glColor3f(0, 31 / 255.0, 77 / 255.0);
	glVertex2f(-(winWidth / 2), -(winHeight / 2));
	glVertex2f((winWidth / 2), -(winHeight / 2));
	glColor3f(0, 85 / 255.0, 213 / 255.0);
	glVertex2f((winWidth / 2), (winHeight / 2));
	glVertex2f(-(winWidth / 2), (winHeight / 2));

	glEnd();
	glColor3f(0, 0, 0);
	vprint(-50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "%s", "GAME OVER!");
}
void display()
{

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	background();
	
	
		drawEnemy(E);

		if ((P.pos.x > -(winWidth / 2) + 51) && (P.pos.x < (winWidth / 2) - 51) && (P.pos.y > -(winHeight / 2) + 51) && (P.pos.y < (winHeight / 2) - 51))
		{
			inside = TRUE;
			glColor3f(0, 188 / 255.0, 255 / 255.0);
			vprint(-(winWidth / 2) + 450, -(winHeight / 2) + 35, GLUT_BITMAP_8_BY_13, "INSIDE");
			drawPlayer(P);
		}
		else
		{
			inside = FALSE;
			glColor3f(0, 188 / 255.0, 255 / 255.0);
			vprint(-(winWidth / 2) + 450, -(winHeight / 2) + 35, GLUT_BITMAP_8_BY_13, "OUTSIDE");
		}

		glColor3f(0, 188 / 255.0, 255 / 255.0);
		if (space == TRUE)
			vprint(-(winWidth / 2) + 205, -(winHeight / 2) + 35, GLUT_BITMAP_8_BY_13, "RESTART");
		else
			vprint(-(winWidth / 2) + 205, -(winHeight / 2) + 35, GLUT_BITMAP_8_BY_13, "PAUSE");
	
		if (game==false)
	    {
		drawTitle();
	    }



	glutSwapBuffers();


}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 32)
	{

		space = !space;
	}
	
	if (key == 27)
		exit(0);
	
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
	// Write your codes here.



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

void onMove(int x, int y) {
	// Write your codes here.
	
	
	my = winHeight / 2 - y;  
	mx = x - winWidth / 2; 
		

	 // to refresh the window it calls display() function
	glutPostRedisplay();
}
void reflect() 	 //function to reflect
{
	E.pos = addV(E.pos, E.vel);
	if ((E.pos.x < -(winWidth / 2) + 69) || (E.pos.x > (winWidth / 2) - 69))
	{
		E.vel.x = -1* E.vel.x;
	}

	if ((E.pos.y < -(winHeight / 2) + 69) || (E.pos.y > (winHeight / 2) - 69))
	{
		E.vel.y = -1* E.vel.y;
	}
}
void chasefunc() //function to chase mouse
{
		E.vel = mulV(espeed, unitV(subV(P.pos, E.pos)));
		E.pos = addV(E.pos, E.vel); 
}

#if TIMER_ON == 1
void onTimer(int v) 
{

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	
	if (game == TRUE)
	{
		P.pos.x = mx;
		P.pos.y = my;
		if (!space)
		{

			if (inside)
			{

				chasefunc();






			}
			else
			{

				reflect();

			}


			time += 0.016;
			time2 = time;
			if (time2 == 15 * k) //every 15 seconds speed increases
			{
				k += 1;
				espeed += 0.5;
			}
			
		}
		enemyangle = rec2pol(E.pos); 
		playerangle = rec2pol(P.pos); 
		
	}
	
	if (sqrt(pow(E.pos.x - P.pos.x, 2) * pow(E.pos.y - P.pos.y, 2) < 30)) // if enemy touches mouse it will be gameover
	{
		game = FALSE;
	}
// to refresh the window it calls display() function
			glutPostRedisplay(); // display()
}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}


void main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("UFO CHASE GOKTUG SENKOYUN ");

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
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}