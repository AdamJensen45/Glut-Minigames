/*********
   CTIS164 - Homoework-1 UFO Kidnapping
----------
STUDENT :G�ktu� �enkoyun 22103191
SECTION :164-001
HOMEWORK:Homework-1
----------
PROBLEMS:I tried to add a UFO beam fonctionality with button F2 to kidnap the bystander but 
couldnt figure out the functions to create collision effects.
----------
ADDITIONAL FEATURES: Walking Bystander and UFO kidnapping (touch the bystander with the bottom of the UFO to kidnap)
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

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 400

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define AUTONOMOUS 0
#define MANUAL 1
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, F1 = false, F2 = false;
int  winWidth, winHeight; // current Window width and height
int xA=2000, yA=2000;  
int xB=0, yB=0; // Bystander positions
int modei = MANUAL;
char modes[2][30] = { "Autonomous", "Manual" };
char statusc[5][30] = { "Up","Down","Left","Right","NotSelected"};
int dstatus=4;
int stickman=0;
bool activeTimer = true;
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

//
// To display onto window using OpenGL commands
//

void display() {
    //
    // clear window to black
    //
    glClearColor(0, 31 / 255.0, 77 / 255.0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    //Background
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
    //Bystander
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glLineWidth(2);
    glVertex2f(xB  , yB - (winHeight / 2) + 50);
    glVertex2f(xB  , yB - (winHeight / 2) + 20);
    glVertex2f(xB  , yB - (winHeight / 2) + 50);
    glVertex2f(xB  -15, yB - (winHeight / 2) + 30);
    glVertex2f(xB , yB - (winHeight / 2) + 50);
    glVertex2f(xB  + 15, yB - (winHeight / 2) + 30);
    glVertex2f(xB , yB - (winHeight / 2) + 20);
    glVertex2f(xB  + 15, yB - (winHeight / 2));
    glVertex2f(xB  , yB - (winHeight / 2) + 20);
    glVertex2f(xB  -15, yB - (winHeight / 2));

    glEnd();
    circle(xB  , yB - (winHeight / 2) + 60, 10);



    //UFO
    //Cockpit
    glColor3f(255 / 255.0, 255 / 255.0, 153 / 255.0);
    circle(xA, yA-5, 45);
    glColor3f(204 / 255.0, 204 / 255.0, 255 / 255.0);
    circle(xA, yA+15, 50);

    //Upper and lower body
    glColor3f(71 / 255.0, 71 / 255.0, 107 / 255.0);
    glRectf(xA -  150/ 2, yA - 30 / 2, xA + 150 / 2, yA + 30 / 2);
    glRectf(xA - 110 / 2, yA - 70 / 2, xA + 110 / 2, yA + 30 / 2);
   
    //Upper part
    glBegin(GL_TRIANGLES); 
    glVertex2f(xA-75, yA+15);
    glVertex2f(xA-75, yA-15);
    glVertex2f(xA-105,yA-15);
    glVertex2f(xA+75, yA+15);
    glVertex2f(xA+75, yA-15);
    glVertex2f(xA+105, yA -15);
    //Lower part
    glVertex2f(xA - 55, yA-15);
    glVertex2f(xA - 55, yA-35);
    glVertex2f(xA-75, yA-15);
    glVertex2f(xA+55, yA-15);
    glVertex2f(xA+55, yA-35);
    glVertex2f(xA+75, yA-15);
    glEnd();
    //Windows
    glColor3f(0, 0, 0);
    circle(xA, yA, 10);
    circle(xA-40, yA, 10);
    circle(xA+40, yA, 10);
    glColor3f(217 / 255.0, 217 / 255.0, 217 / 255.0);
    circle(xA, yA, 6);
    circle(xA-40, yA, 6);
    circle(xA+40, yA, 6);

    //Line
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glLineWidth(2);
    glVertex2f(xA-75, yA-15);
    glVertex2f(xA+75, yA-15);
    glEnd();

    if (F2 == true)
    {
        glColor3f(255 / 255.0, 255 / 255.0, 153 / 255.0);
        glBegin(GL_TRIANGLES);
        glVertex2f(xA, yA - 40);
        glVertex2f(xA - 75, yA - 70);
        glVertex2f(xA + 75, yA - 70);
        glEnd();
    }
    glColor3f(1, 1, 0); // white
    vprint(-(winWidth / 2) +10, (winHeight / 2) - 20, GLUT_BITMAP_8_BY_13, "MODE:%s",modes[modei]);
    vprint(-(winWidth / 2) + 10, (winHeight / 2) - 40, GLUT_BITMAP_8_BY_13, "DIRECTION:%s", statusc[dstatus]);
    glColor3f(1, 1, 1); // yellow
    vprint((winWidth / 2) - 125, (winHeight / 2) - 20, GLUT_BITMAP_8_BY_13, "GOKTUG SENKOYUN");

    glColor3f(0, 1, 1); // blue

    vprint((winWidth / 2) - 100, (winHeight / 2) - 30, GLUT_BITMAP_8_BY_13, "22103191");
    vprint(-200, -(winHeight / 2) + 20, GLUT_BITMAP_8_BY_13, "Press <F1> to switch the mode and Arrow Keys to control the UFO");
    if (yA -50 <= -winHeight/2+70 && (xA + 15 <= xB + 30 && xA + 15 >= xB - 30))
    {
        yB = 50000;
    }

    glutSwapBuffers();
}

void moveup()
{
    
        yA += 5;
}
void movedown()
{
   
        yA -= 5;
}
void moveleft()
{
        xA -= 5;
}
void moveright()
{
    
        xA += 5;
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
    switch (key) 
    {
    case GLUT_KEY_UP: up = true; 
        if (modei == AUTONOMOUS)
            moveup();

        break;
    case GLUT_KEY_DOWN: down = true;
        if (modei == AUTONOMOUS)
            movedown();
        break;
    case GLUT_KEY_LEFT: left = true;
        if (modei == AUTONOMOUS)
            moveleft();
        break;
    case GLUT_KEY_RIGHT: right = true;
        if (modei == AUTONOMOUS)
            moveright();
        break;
    case GLUT_KEY_F1:F1 = true; break;
    case GLUT_KEY_F2:F2 = true; break;
    }
    if (key == F1)
    {
        if (modei == AUTONOMOUS)
        {
            modei++;
            dstatus = 4;
        }
        else if (modei == MANUAL)
        {
            modei--;
            dstatus = 4;
        }
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
    switch (key) 
    {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    case GLUT_KEY_F1:F1 = false; break;

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
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        xA = x - winWidth / 2;
        yA = winHeight / 2 - y;
        
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

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    
    if (activeTimer) 
    {
        // animate the current shape until it touches the edge of the window.
        switch (modei) 
        {
        
         case AUTONOMOUS:
            if (up == true)
            {
                dstatus = UP;
            }
            else if (down == true)
            {
                dstatus = DOWN;
            }
            else if (left == true)
            {
                dstatus = LEFT;
            }
            else if (right == true)
            {
                dstatus = RIGHT;
            }
            switch (dstatus)
            {
             case UP:
                if (yA + 70 >= winHeight / 2)
                    dstatus = DOWN;
                else
                    moveup();
                
                break;
             case DOWN:
                if (yA - 55 <= -winHeight / 2)
                    dstatus = UP;
                else
                    movedown();
                
                break;
              case LEFT:
                if (xA - 110 <= -winWidth / 2)
                    dstatus = RIGHT;
                else
                    moveleft();

               
                break;
              case RIGHT:
                if (xA + 110 >= winWidth / 2)
                    dstatus = LEFT;
                else
                    moveright();
                
                break;
            }
            
            break;
            

        case MANUAL:
            if (up == true)
            {
                dstatus = UP;
            }
            else if (down == true)
            {
                dstatus = DOWN;
            }
            else if (left == true)
            {
                dstatus = LEFT;
            }
            else if (right == true)
            {
                dstatus = RIGHT;
            }
            if (up == true)
            {
                if (yA + 70 >= winHeight / 2)
                    yA += 0;
                else
                    yA += 5;
            }
            else if (down == true)
            {
                if (yA - 55 <= -winHeight / 2)
                    yA -= 0;
                else
                    yA -= 5;
            }
            else if (left == true)
            {
                if (xA - 110 <= -winWidth / 2)
                    xA -= 0;
                else
                    xA -= 5;
            }
            else if (right == true)
            {
                if (xA + 110 >= winWidth / 2)
                    xA += 0;
                else
                    xA += 5;
            }
            break;
            
        }

        if (stickman == 0)
        {
            if (xB + 15 >= winWidth / 2)
                stickman = 1;
            else
                xB += 2;
        }
        if (stickman == 1)
        {
            if (xB - 15 <= -winWidth / 2)
                stickman = 0;
            else
                xB -= 2;
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Homework-1 UFO Kidnap");

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