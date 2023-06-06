/*********
   CTIS164 - HOMEWORK II Z-Com | Alien Defense
----------
STUDENT : Göktuð Þenkoyun 22103191
SECTION : 164-01
HOMEWORK: HOMEWORK II
----------
PROBLEMS: 
----------
ADDITIONAL FEATURES: Background Gradient

Game Over and You Win Title Screen: if the UFO Flagship hits the turret you lose,
but if you rapidly click and repel the flagship until timer runs out, you win.

DOOMSDAY CLOCK: when the DOOMSDAY CLOCK hits 0 an alien FLAGSHIP tries to attack the turret,
in order to win you must rapidly click on the screen.

Targets dont spawn on turret.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include<time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define PI  3.14159265
#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int fireac=1;
int count = 0;
int start = 1;
int lose = 0;
typedef struct {
    float x, y;
} point_t;

typedef struct {
    point_t pos;    //position of the turret
    float   angle;  //view angle 
    float   r;
} player_t;

typedef struct {
    point_t pos;  //position of fire 
    float angle;   // fire angle
    bool active;
} fire_t;

typedef struct {
    int r, g, b; //random colors
} color_t;

typedef struct {
    point_t center;   //location of target
    color_t color;
    float radius;
} target_t;
typedef struct {
    point_t center;      //location of flagship
    color_t color;
    float radius;
} flagship_t;


// initialization for structures and timer functions


flagship_t fs = {{0,winHeight / 2 + 350}, {0,0,1}, {}};
player_t player = { {0, 0}, 180, 20 };
fire_t   fire = { { 0, 0 }, 0, false };
target_t target = { {rand() % 700 + 1 - 350 ,rand() % 500 + 1 - 250},{48,45,24},{10} };
double timer = 0;
double doom = 30;



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



void drawPlayer(player_t tp) //function to draw player
{
    int angle;
    glColor3f(0.2, 0.5, 0.9);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(tp.pos.x, tp.pos.y);
    glVertex2f(tp.pos.x + 50 * cos(tp.angle * D2R), tp.pos.y + 50 * sin(tp.angle * D2R));
    glEnd();
    glLineWidth(1);

    glColor3f(0.8, 0.5, 0.2);
    circle(tp.pos.x, tp.pos.y, tp.r);
    glColor3f(0.7, 0.3, 0.3);
    circle(tp.pos.x, tp.pos.y, tp.r - 4);

    glColor3f(1, 1, 1);
    if (tp.angle < 0)
        angle = 360 + tp.angle;
    else
        angle = tp.angle;
    
    vprint(tp.pos.x - 12, tp.pos.y - 5, GLUT_BITMAP_8_BY_13, "%d",angle%360 );


}
void drawFlagship(flagship_t t)
{
    glColor3f(255 / 255.0, 255 / 255.0, 153 / 255.0);
    circle(t.center.x, t.center.y - 5, 45);
    glColor3f(204 / 255.0, 204 / 255.0, 255 / 255.0);
    circle(t.center.x, t.center.y + 15, 50);

    //Upper and lower body
    glColor3f(71 / 255.0, 71 / 255.0, 107 / 255.0);
    glRectf(t.center.x - 150 / 2, t.center.y - 30 / 2, t.center.x + 150 / 2, t.center.y + 30 / 2);
    glRectf(t.center.x - 110 / 2, t.center.y - 70 / 2, t.center.x + 110 / 2, t.center.y + 30 / 2);

    //Upper part
    glBegin(GL_TRIANGLES);
    glVertex2f(t.center.x - 75, t.center.y + 15);
    glVertex2f(t.center.x - 75, t.center.y - 15);
    glVertex2f(t.center.x - 105, t.center.y - 15);
    glVertex2f(t.center.x + 75, t.center.y + 15);
    glVertex2f(t.center.x + 75, t.center.y - 15);
    glVertex2f(t.center.x + 105, t.center.y - 15);
    //Lower part
    glVertex2f(t.center.x - 55, t.center.y - 15);
    glVertex2f(t.center.x - 55, t.center.y - 35);
    glVertex2f(t.center.x - 75, t.center.y - 15);
    glVertex2f(t.center.x + 55, t.center.y - 15);
    glVertex2f(t.center.x + 55, t.center.y - 35);
    glVertex2f(t.center.x + 75, t.center.y - 15);
    glEnd();
    //Windows
    glColor3f(0, 0, 0);
    circle(t.center.x, t.center.y, 10);
    circle(t.center.x - 40, t.center.y, 10);
    circle(t.center.x + 40, t.center.y, 10);
    glColor3f(217 / 255.0, 217 / 255.0, 217 / 255.0);
    circle(t.center.x, t.center.y, 6);
    circle(t.center.x - 40, t.center.y, 6);
    circle(t.center.x + 40, t.center.y, 6);

    //Line
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glLineWidth(2);
    glVertex2f(t.center.x - 75, t.center.y - 15);
    glVertex2f(t.center.x + 75, t.center.y - 15);
    glEnd();
    vprint(-150, -200, GLUT_BITMAP_TIMES_ROMAN_24, "%s", "RAPIDLY CLICK TO DEFEND!");

    
}

void drawFire(fire_t f) 
{
    if (f.active) 
    {
        glColor3f(1, 1, 1);
        circle(f.pos.x, f.pos.y, 6);
    }
}

void drawTarget(target_t t) 
{
    

    glColor3f(255 / 255.0, 255 / 255.0, 153 / 255.0);
    circle(t.center.x, t.center.y - 5, 15);
    glColor3f(t.color.r/255.0, t.color.g/255.0, t.color.b/255.0);
    circle(t.center.x, t.center.y + 5, 16);
    t.radius = 17;
    //Upper and lower body
    glColor3f(71 / 255.0, 71 / 255.0, 107 / 255.0);
    glRectf(t.center.x - 50 / 2, t.center.y - 10 / 2, t.center.x + 50 / 2, t.center.y + 10 / 2);
    glRectf(t.center.x - 36 / 2, t.center.y - 23 / 2, t.center.x + 36 / 2, t.center.y + 10 / 2);

    //Upper part
    glBegin(GL_TRIANGLES);
    glVertex2f(t.center.x - 25, t.center.y + 5);
    glVertex2f(t.center.x - 25, t.center.y - 5);
    glVertex2f(t.center.x - 35, t.center.y - 5);
    glVertex2f(t.center.x + 25, t.center.y + 5);
    glVertex2f(t.center.x + 25, t.center.y - 5);
    glVertex2f(t.center.x + 35, t.center.y - 5);
    //Lower part
    glVertex2f(t.center.x - 18, t.center.y - 5);
    glVertex2f(t.center.x - 18, t.center.y - 11);
    glVertex2f(t.center.x - 25, t.center.y - 5);
    glVertex2f(t.center.x + 18, t.center.y - 5);
    glVertex2f(t.center.x + 18, t.center.y - 11);
    glVertex2f(t.center.x + 25, t.center.y - 5);
    glEnd();
    //Windows
    glColor3f(0, 0, 0);
    circle(t.center.x, t.center.y, 3);
    circle(t.center.x - 13, t.center.y, 3);
    circle(t.center.x + 13, t.center.y, 3);
    glColor3f(217 / 255.0, 217 / 255.0, 217 / 255.0);
    circle(t.center.x, t.center.y, 2);
    circle(t.center.x - 13, t.center.y, 2);
    circle(t.center.x + 13, t.center.y, 2);

    //Line
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glLineWidth(2);
    glVertex2f(t.center.x - 25, t.center.y - 5);
    glVertex2f(t.center.x + 25, t.center.y - 5);
    glEnd();
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
    glRectf(-(winWidth/2),winHeight/2,winWidth/2,winHeight/2-30);
    
    char c[20] = "Time:";
    glColor3f(179 / 255.0, 179 / 255.0, 179 / 255.0);
    circle(-(winWidth / 2) + 200, (winHeight / 2) - 100, 50);
    glColor3f(89 / 255.0, 89 / 255.0, 89 / 255.0);
    circle(-(winWidth / 2) + 180, (winHeight / 2) - 80, 7);
    circle(-(winWidth / 2) + 220, (winHeight / 2) - 120, 8);
    circle(-(winWidth / 2) + 190, (winHeight / 2) - 85, 10);
    circle(-(winWidth / 2) + 185, (winHeight / 2) - 120, 14);
    glColor3f(65 / 255.0, 65 / 255.0, 65 / 255.0);
    circle(-(winWidth / 2) + 220, (winHeight / 2) - 75, 17);
    circle(-(winWidth / 2) + 170, (winHeight / 2) - 100, 18);
    glColor3f(0,0,0);
  
    vprint(winWidth/2 - 40, winHeight/2 -20, GLUT_BITMAP_8_BY_13, "%0.1f", timer);
    vprint(winWidth / 2 - 85, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "%s",c);
    vprint(winWidth / 2 - 150, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "%d", count);
    vprint(-(winWidth / 2)+20, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "%s", "GOKTUG SENKOYUN 22103191");
    vprint(winWidth / 2 - 300, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "%s", "DOOMSDAY:");
    vprint(winWidth / 2 - 225, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "%0.1f", doom);

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
void drawWinTitle()
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
    vprint(-50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "%s", "YOU WIN!");
}
//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 31 / 255.0, 77 / 255.0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
   
        background();
        drawFire(fire);
        drawPlayer(player);
        if (timer >= 30)
        {
            drawFlagship(fs);
        }
        drawTarget(target);
        if (fs.center.y <= 31)
        {
            drawTitle();
            lose = 1;
        }
        if (timer >= 60)
            drawWinTitle();
        glutSwapBuffers();
   
}




void resetTarget() 
{
    float yPos = rand() % 500 +1 - 250;
    float xPos = rand() % 700 +1 -350;
    target.center = { xPos, yPos };
    if ((target.center.x<80 && target.center.x > -80) && (target.center.y<80 && target.center.y > -80))
        resetTarget();
    
    target.color.r = rand() % 256;
    target.color.g = rand() % 256;
    target.color.b = rand() % 256;

    
}
//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
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

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && timer>=30 && timer<60 && lose!=1)
    {
        count++;
        fs.center.y += 35;

    }
        
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



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

int turnPlayer(player_t* tp, float inc)
{
    tp->angle += inc;
    return tp->angle;
}

bool testCollision(fire_t fr, target_t t) 
{
    float dx = t.center.x - fr.pos.x;
    float dy = t.center.y - fr.pos.y;
    float d = sqrt(dx * dx + dy * dy);
    if (d <= t.radius)
    {
        return 1;
    }
    else
        return 0;
}

#if TIMER_ON == 1
void onTimer(int v) 
{
    
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (!(fs.center.y <= 30))
    {
        if (start == 1)
        {
            target.center.x = rand() % 700 + 1 - 350;
            target.center.y = rand() % 500 + 1 - 250;
            start = 0;
        }
        if (timer < 60)
        {

            fireac = 1;

            int xtan = (atan2(target.center.y, target.center.x) / D2R);
            if (xtan < 0)
                xtan += 360;

            if (fabs(xtan - player.angle) <= 180)
            {
                if (xtan < player.angle)
                {
                    turnPlayer(&player, -1);
                    fireac = 0;

                }
                else if (xtan > player.angle)
                {
                    turnPlayer(&player, 1);
                    fireac = 0;
                }
                else if (xtan == player.angle)
                {
                    if (fire.active == 0)
                    {
                        fire.pos = player.pos;
                        fire.angle = player.angle;
                        fire.active = true;
                    }
                    if (fireac == 1)
                    {
                        fire.active = true;
                        fire.pos.x += 10 * cos(player.angle * D2R);
                        fire.pos.y += 10 * sin(player.angle * D2R);

                        if (testCollision(fire, target) == 1)
                        {
                            fire.active = false;
                            resetTarget();
                        }
                    }

                }
            }
            else if (fabs(xtan - player.angle) > 180)
            {
                if (xtan < player.angle)
                    player.angle = turnPlayer(&player, 1) % 360;
                else if (xtan > player.angle)
                    player.angle = 360 + turnPlayer(&player, -1);
                else if (xtan == player.angle)
                {
                    if (fire.active == 0)
                    {
                        fire.pos = player.pos;
                        fire.angle = player.angle;
                        fire.active = true;
                    }
                    if (fireac == 1)
                    {
                        fire.active = true;
                        fire.pos.x += 10 * cos(player.angle * D2R);
                        fire.pos.y += 10 * sin(player.angle * D2R);

                        if (testCollision(fire, target) == 1)
                        {
                            fire.active = false;
                            resetTarget();
                        }
                    }

                }
            }
            else if (xtan == player.angle)
            {
                if (fire.active == 0)
                {
                    fire.pos = player.pos;
                    fire.angle = player.angle;
                    fire.active = true;
                }
                if (fireac == 1)
                {
                    fire.active = true;
                    fire.pos.x += 10 * cos(player.angle * D2R);
                    fire.pos.y += 10 * sin(player.angle * D2R);

                    if (testCollision(fire, target) == 1)
                    {
                        fire.active = false;
                        resetTarget();
                    }
                }
            }


            timer += 0.016;
        }
        if (timer < 30)
        {
            doom -= 0.016;
        }
        if (timer >= 30 && timer < 60)
        {
            
            fs.center.y += -3;
            


        }

        
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

void main(int argc, char* argv[]) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Goktug Senkoyun | Z-COM: Alien Defense");

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