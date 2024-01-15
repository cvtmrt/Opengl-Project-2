/*********
   CTIS164 - Template Source Program
----------
STUDENT : CAVIT MERT ERGUL
SECTION : 2. SECTION
HOMEWORK: CTIS-164-222-Homework-2
----------
PROBLEMS:
----------
ADDITIONAL FEATURES: 1-) there are 3 different teams. Basically, they are in competition. If at least 2 have the equal score,  extra random time will be added.

                     2-) the machine throwing balls can be controlled by special keys such as up or left.
                     3-) balls staying behind the machine has their own order. when their turn come, they get thrown by machine and they get in order again.
                     4-) there is a scoreboard that shows team's score and remaining time including extra time.
                     5-) there are 3 different balls that represent country by carrying its flag
                     6-)
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
#include <time.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define TARGET_RADIUS 31

#define D2R 0.0174532

 /* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, spacebar = false, scoreb = false;
int  winWidth, winHeight; // current Window width and height
int r, b, c;
int xtop, ytop;
int ypos = 0;
int salise = 0;
int saniye = 0;
bool active = 0;
int order = 1;
int countGER = 0, countTUR = 0, countRUS = 0;
int extime = 0;
int totaltime = 0;
int finishtime = 19;
int finish = 0;
int win = 0;
bool pr = false;

typedef struct {
    float x, y;
} point_t;

typedef struct {
    point_t pos;    // position of the object
    float   r;
} player_t;

typedef struct {
    point_t pos;
    bool active;
} fire_t;

typedef struct {
    point_t center;
    float radius;
    float speed;
} target_t;

#define MAX_FIRE 20 // 20 fires at a time.
#define FIRE_RATE 60 // after 8 frames you can throw another one.

player_t p = { {-520,0}, 45, };
fire_t   fr[MAX_FIRE];
int fire_rate = 0;
target_t target[5];

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
void pie_filled(float x, float y, float r, float start, float end) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    for (float angle = start; angle < end; angle += 10) {
        glVertex2f(r * cos(angle * D2R) + x, r * sin(angle * D2R) + y);
    }
    glVertex2f(r * cos(end * D2R) + x, r * sin(end * D2R) + y);
    glEnd();
}
void myCircle_filled(float x, float y, float r) {
    glBegin(GL_POLYGON);
    for (float angle = 0; angle < 360; angle += 10) {
        glVertex2f(r * cos(angle * D2R) + x, r * sin(angle * D2R) + y);
    }
    glEnd();
}
void field()
{
    glColor3f(0, 153 / 255., 0);
    glRectf(700, 400, -700, 350);
    glRectf(700, -400, -700, -300);
    glRectf(600, 400, 700, -400);
    glRectf(-600, 400, -700, -400);
    int a;
    for (a = 350; a > -350; a -= 100)
    {
        glRectf(700, a - 50, -700, a);
    }
}
void whitelines()
{
    glColor3f(1, 1, 1);
    glRectf(-604, 300, -600, -300);
    glRectf(604, 300, 600, -300);
    glRectf(604, -300, -604, -304);
    glRectf(604, 300, -604, 304);
    glRectf(2, 300, -2, -300);
    circle(0, 0, 6);
    for (int a = 1; a < 5; a++)
    {
        glColor3f(1, 1, 1);
        circle_wire(0, 0, 100 + a);
    }

    glRectf(600, 100, 510, 104);
    glRectf(600, -100, 510, -104);
    glRectf(510, 100, 514, -100);

    glRectf(-600, 100, -500, 104);
    glRectf(-600, -100, -500, -104);
    glRectf(-500, 100, -504, -100);

    circle(-455, 0, 4);

    glRectf(600, 160, 410, 164);
    glRectf(600, -160, 410, -164);
    glRectf(410, 160, 414, -164);

    glRectf(-600, 160, -410, 164);
    glRectf(-600, -160, -410, -164);
    glRectf(-410, 160, -414, -164);

    circle(455, 0, 4);

}
void displaywinn()
{
    switch (win)
    {
    case 1:
        glColor3f(1, 1, 1);
        circle(0, 0, 31);

        glColor3f(204 / 255., 0, 0);
        circle(0, 0, 30);

        glColor3f(1, 1, 1);
        circle(-5, 0, 15);

        glColor3f(204 / 255., 0, 0);
        circle(-2, 0, 13);

        glColor3f(1, 1, 1);
        circle(10, 0, 3);
        glBegin(GL_LINES);
        glVertex2f(10, 5);
        glVertex2f(10, -5);
        glVertex2f(15, 0);
        glVertex2f(5, 0);
        glEnd();

        break;
    case 2:

        glColor3f(1, 1, 1);
        circle(0, 0, 31);

        glColor3f(0, 0, 0);
        pie_filled(0, 0, 30, 0, 90);
        pie_filled(0, 0, 30, 90, 180);

        glColor3f(1, 214 / 255., 1 / 255.);
        pie_filled(0, 0, 30, 180, 270);
        pie_filled(0, 0, 30, 270, 360);


        glColor3f(1, 0, 0);
        pie_filled(-12, 0, 18, 135, 225);
        pie_filled(12, 0, 18, 315, 405);
        glRectf(-25, (9 * sqrt(2)), 25, (-9 * sqrt(2)));
        break;
    case 3:
        glColor3f(0, 0, 0);
        circle(0, 0, 31);

        glColor3f(1, 1, 1);
        pie_filled(0, 0, 30, 0, 90);
        pie_filled(0, 0, 30, 90, 180);

        glColor3f(0, 0, 24);
        pie_filled(0, 0, 30, 180, 270);
        pie_filled(0, 0, 30, 270, 360);


        glColor3f(1, 0, 0);
        pie_filled(-12, 0, 18, 135, 225);
        pie_filled(12, 0, 18, 315, 405);
        glRectf(-25, (9 * sqrt(2)), 25 + xtop, (-9 * sqrt(2)));
    }
}
void ballRus()
{
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active) {
            glColor3f(0, 0, 0);
            circle(0 + fr[i].pos.x, 0 + fr[i].pos.y, 31);

            glColor3f(1, 1, 1);
            pie_filled(0 + fr[i].pos.x, 0 + fr[i].pos.y, 30, 0, 90);
            pie_filled(0 + fr[i].pos.x, 0 + fr[i].pos.y, 30, 90, 180);

            glColor3f(0, 0, 24);
            pie_filled(0 + fr[i].pos.x, 0 + fr[i].pos.y, 30, 180, 270);
            pie_filled(0 + fr[i].pos.x, 0 + fr[i].pos.y, 30, 270, 360);


            glColor3f(1, 0, 0);
            pie_filled(-12 + fr[i].pos.x, 0 + fr[i].pos.y, 18, 135, 225);
            pie_filled(12 + fr[i].pos.x, 0 + fr[i].pos.y, 18, 315, 405);
            glRectf(-25 + fr[i].pos.x, (9 * sqrt(2)) + fr[i].pos.y, 25 + fr[i].pos.x, (-9 * sqrt(2)) + fr[i].pos.y);
        }
    }
}
void ballGer()
{
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active) {

            glColor3f(1, 1, 1);
            circle(0 + fr[i].pos.x, 0 + fr[i].pos.y, 31);

            glColor3f(0, 0, 0);
            pie_filled(0 + fr[i].pos.x, 0 + fr[i].pos.y, 30, 0, 90);
            pie_filled(0 + fr[i].pos.x, 0 + fr[i].pos.y, 30, 90, 180);

            glColor3f(1, 214 / 255., 1 / 255.);
            pie_filled(0 + fr[i].pos.x, 0 + fr[i].pos.y, 30, 180, 270);
            pie_filled(0 + fr[i].pos.x, 0 + fr[i].pos.y, 30, 270, 360);


            glColor3f(1, 0, 0);
            pie_filled(-12 + fr[i].pos.x, 0 + fr[i].pos.y, 18, 135, 225);
            pie_filled(12 + fr[i].pos.x, 0 + fr[i].pos.y, 18, 315, 405);
            glRectf(-25 + fr[i].pos.x, (9 * sqrt(2)) + fr[i].pos.y, 25 + fr[i].pos.x, (-9 * sqrt(2)) + fr[i].pos.y);
        }
    }
}
void ballTur()
{


    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active) {




            glColor3f(1, 1, 1);
            circle(0 + fr[i].pos.x, 0 + fr[i].pos.y, 31);

            glColor3f(204 / 255., 0, 0);
            circle(0 + fr[i].pos.x, 0 + fr[i].pos.y, 30);

            glColor3f(1, 1, 1);
            circle(-5 + fr[i].pos.x, 0 + fr[i].pos.y, 15);

            glColor3f(204 / 255., 0, 0);
            circle(-2 + fr[i].pos.x, 0 + fr[i].pos.y, 13);

            glColor3f(1, 1, 1);
            circle(10 + fr[i].pos.x, 0 + fr[i].pos.y, 3);
            glBegin(GL_LINES);
            glVertex2f(+10 + fr[i].pos.x, 5 + fr[i].pos.y);
            glVertex2f(+10 + fr[i].pos.x, -5 + fr[i].pos.y);
            glVertex2f(+5 + fr[i].pos.x, 0 + fr[i].pos.y);
            glVertex2f(+15 + fr[i].pos.x, 0 + fr[i].pos.y);
            glEnd();

        }







    }
}
void germanball(int xtop)
{
    glColor3f(1, 1, 1);
    circle(0 + xtop, 0 + p.pos.y, 31);

    glColor3f(0, 0, 0);
    pie_filled(0 + xtop, 0 + p.pos.y, 30, 0, 90);
    pie_filled(0 + xtop, 0 + p.pos.y, 30, 90, 180);

    glColor3f(1, 214 / 255., 1 / 255.);
    pie_filled(0 + xtop, 0 + p.pos.y, 30, 180, 270);
    pie_filled(0 + xtop, 0 + p.pos.y, 30, 270, 360);


    glColor3f(1, 0, 0);
    pie_filled(-12 + xtop, 0 + p.pos.y, 18, 135, 225);
    pie_filled(12 + xtop, 0 + p.pos.y, 18, 315, 405);
    glRectf(-25 + xtop, (9 * sqrt(2)) + p.pos.y, 25 + xtop, (-9 * sqrt(2)) + p.pos.y);


}
void russianball(int xtop)
{

    glColor3f(0, 0, 0);
    circle(0 + xtop, 0 + p.pos.y, 31);

    glColor3f(1, 1, 1);
    pie_filled(0 + xtop, 0 + p.pos.y, 30, 0, 90);
    pie_filled(0 + xtop, 0 + p.pos.y, 30, 90, 180);

    glColor3f(0, 0, 24);
    pie_filled(0 + xtop, 0 + p.pos.y, 30, 180, 270);
    pie_filled(0 + xtop, 0 + p.pos.y, 30, 270, 360);


    glColor3f(1, 0, 0);
    pie_filled(-12 + xtop, 0 + p.pos.y, 18, 135, 225);
    pie_filled(12 + xtop, 0 + p.pos.y, 18, 315, 405);
    glRectf(-25 + xtop, (9 * sqrt(2)) + p.pos.y, 25 + xtop, (-9 * sqrt(2)) + p.pos.y);

}
void ball(int xtop)
{

    glColor3f(1, 1, 1);
    circle(0 + xtop, 0 + p.pos.y, 31);

    glColor3f(204 / 255., 0, 0);
    circle(0 + xtop, 0 + p.pos.y, 30);

    glColor3f(1, 1, 1);
    circle(-5 + xtop, 0 + p.pos.y, 15);

    glColor3f(204 / 255., 0, 0);
    circle(-2 + xtop, 0 + p.pos.y, 13);

    glColor3f(1, 1, 1);
    circle(10 + xtop, 0 + p.pos.y, 3);
    glBegin(GL_LINES);
    glVertex2f(10 + xtop, 5 + p.pos.y);
    glVertex2f(10 + xtop, -5 + p.pos.y);
    glVertex2f(15 + xtop, 0 + p.pos.y);
    glVertex2f(5 + xtop, 0 + p.pos.y);
    glEnd();







}
void scoreboard()
{
    glColor3f(0, 0, 102 / 255.);
    glRectf(-570, 380, -520, 360);
    glColor3f(0, 0, 102 / 255.);
    glRectf(-500, 380, -450, 360);
    glColor3f(0, 0, 102 / 255.);
    glRectf(-430, 380, -380, 360);
    glColor3f(1, 1, 1);
    vprint(-557, 366, GLUT_BITMAP_HELVETICA_12, "TUR");
    vprint(-487, 366, GLUT_BITMAP_HELVETICA_12, "GER");
    vprint(-417, 366, GLUT_BITMAP_HELVETICA_12, "RUS");


    glColor3f(1, 1, 1);
    glRectf(-350, 380, -300, 350);

    glColor3f(102 / 255., 1, 178 / 255.);
    glRectf(-570, 350, -520, 320);
    glRectf(-500, 350, -450, 320);
    glRectf(-430, 350, -380, 320);

    glColor3f(0, 0, 102 / 255.);
    vprint2(-550, 325, 0.15, "%d", countTUR);
    vprint2(-480, 325, 0.15, "%d", countGER);
    vprint2(-410, 325, 0.15, "%d", countRUS);

    if (scoreb) {
        glColor3f(1, 1, 1);
        glRectf(-295, 380, -270, 350);
        glColor3f(0, 0, 102 / 255.);
        vprint2(-283, 360, 0.10, "%d", extime);
        vprint(-293, 360, GLUT_BITMAP_9_BY_15, "+");
    }

}


void goalpost(target_t* t)
{
    glColor3f(1, 1, 1);
    for (int i = 0; i < 4; i++)
    {
        glBegin(GL_QUADS);
        glVertex2f(t[i].center.x, 75 + t[i].center.y);
        glVertex2f(65 + t[i].center.x, 69 + t[i].center.y);
        glVertex2f(65 + t[i].center.x, 65 + t[i].center.y);
        glVertex2f(t[i].center.x, 71 + t[i].center.y);
        glEnd();

        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        glVertex2f(t[i].center.x, -75 + t[i].center.y);
        glVertex2f(65 + t[i].center.x, -69 + t[i].center.y);
        glVertex2f(65 + t[i].center.x, -65 + t[i].center.y);
        glVertex2f(t[i].center.x, -71 + t[i].center.y);
        glEnd();


        glRectf(t[i].center.x, 75 + t[i].center.y, 4 + t[i].center.x, -75 + t[i].center.y);
        glRectf(63 + t[i].center.x, 65 + t[i].center.y, 65 + t[i].center.x, -65 + t[i].center.y);
        glColor3f(1, 1, 1);

        int y = 0;
        for (int a = 0; a <= 60; a += 6)
        {
            glBegin(GL_LINES);
            glVertex2f(4 + a + t[i].center.x, 75 + t[i].center.y + y);
            glVertex2f(4 + a + t[i].center.x, -75 + t[i].center.y - y);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(3 + a + t[i].center.x, 75 + t[i].center.y + y);
            glVertex2f(3 + a + t[i].center.x, -75 + t[i].center.y - y);
            glEnd();
            y--;
        }

        glBegin(GL_LINES);
        glVertex2f(t[i].center.x, 66 + t[i].center.y);
        glVertex2f(65 + t[i].center.x, 59 + t[i].center.y);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(t[i].center.x, -66 + t[i].center.y);
        glVertex2f(65 + t[i].center.x, -59 + t[i].center.y);
        glEnd();


        for (int a = 0; a <= 58; a += 6)
        {
            glBegin(GL_LINES);
            glVertex2f(t[i].center.x, 60 + t[i].center.y - a);
            glVertex2f(65 + t[i].center.x, 56 + t[i].center.y - a);
            glEnd();

        }
        for (int a = 0; a <= 58; a += 6)
        {
            glBegin(GL_LINES);
            glVertex2f(t[i].center.x, -60 + t[i].center.y + a);
            glVertex2f(65 + t[i].center.x, -56 + t[i].center.y + a);
            glEnd();

        }
        glBegin(GL_LINES);
        glVertex2f(t[i].center.x, 0 + t[i].center.y);
        glVertex2f(65 + t[i].center.x, 0 + t[i].center.y);
        glEnd();
    }
}
void ballmachine()
{
    glColor3f(0, 0, 0);
    glRectf(-700, 33 + p.pos.y, -520, 30 + p.pos.y);
    glRectf(-700, -33 + p.pos.y, -520, -30 + p.pos.y);

    glColor3f(96 / 255., 96 / 255., 96 / 255.);
    circle(-480, 0 + p.pos.y, 30);

    glColor3f(40 / 255., 40 / 255., 40 / 255.);
    glRectf(-540, 50 + p.pos.y, -460, -50 + p.pos.y);

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-525, -50 + p.pos.y);
    glVertex2f(-535, -60 + p.pos.y);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-475, -50 + p.pos.y);
    glVertex2f(-485, -60 + p.pos.y);

    glEnd();
    glColor3f(1, 125 / 255., 0);
    circle(-485, -60 + p.pos.y, 6);
    circle(-535, -60 + p.pos.y, 6);

    glColor3f(246 / 255., 66 / 255., 0);
    glRectf(-515, 40 + p.pos.y, -485, 10 + p.pos.y);
    glColor3f(153 / 255., 153 / 255., 1);
    circle(-500, 25 + p.pos.y, 10);


    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    glVertex2f(-502, 28 + p.pos.y);
    glVertex2f(-502, 20 + p.pos.y);
    glVertex2f(-495, 24 + p.pos.y);


    glEnd();

    vprint(-515, -5 + p.pos.y, GLUT_BITMAP_TIMES_ROMAN_10, "PRESS");
    vprint(-515, -15 + p.pos.y, GLUT_BITMAP_TIMES_ROMAN_10, "SPACE");

}

void drawFires() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active) {

            ball(fr[i].pos.x);
        }
    }
}
void display() {
    //
    // clear window to black
    //
    glClearColor(102 / 255., 204 / 255., 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);




    field();
    whitelines();


    goalpost(target);


    switch (order)
    {
    case 1:
        ball(-548);
        germanball(-609);
        russianball(-671);
        ballRus();
        break;
    case 2:
        ball(-671);
        germanball(-548);
        russianball(-609);
        ballTur();

        break;
    case 3:
        ball(-609);
        germanball(-671);
        russianball(-548);
        ballGer();

    }

    scoreboard();

    if (salise == 99 && saniye == 19) {
        glColor3f(0, 0, 102 / 255.);
        vprint2(-340, 360, 0.10, "%02d:%02d", 0, 0);
    }

    else {
        glColor3f(0, 0, 102 / 255.);
        vprint2(-340, 360, 0.10, "%02d:%02d", totaltime, salise);



    }



    ballmachine();


    if (active || salise + saniye != 0)
    {
        glColor3f(1, 1, 1);
        vprint(-210, -340, GLUT_BITMAP_9_BY_15, "<SPACEBAR> to Fire, <F1> to resume or pause.");
    }
    else
    {
        glColor3f(1, 1, 1);
        vprint(-150, -340, GLUT_BITMAP_9_BY_15, "Press <F1> to start a new game.");
    }




    glutSwapBuffers();
}

int findAvailableFire() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active == false) return i;
    }
    return -1;
}


void resetTarget(target_t target[], int i) {
    float xPos = rand() % 600;
    target[i].center = { xPos , -500 };
    target[i].radius = 32;
    target[i].speed = 5;
}

void resetTargetBeg(target_t target[], int i, float beg) {
    float xPos = rand() % 600;
    target[i].center = { xPos , -500 - beg };
    target[i].radius = 32;
    target[i].speed = 5;
}


//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ')
    {
        r = rand() % 255;
        b = rand() % 255;
        c = rand() % 255;
    }
    if (key == ' ')
    {
        spacebar = true;

    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ')
        spacebar = false;




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
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    }
    if (key == GLUT_KEY_UP)
    {
        p.pos.y += 10;
    }
    if (key == GLUT_KEY_DOWN)
    {
        p.pos.y -= 10;
    }

    if (key == GLUT_KEY_F1)
    {
        active = !active;
        pr = !pr;
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
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
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
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
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

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void movePlayer(player_t* tp, float speed) {
    float dx = speed;
    float dy = speed;

    tp->pos.x += dx;
    tp->pos.y += dy;
    float r = tp->r;

    // check rectangular boundary
    if (tp->pos.x > (0 - r) || tp->pos.x < (-600 + r) ||
        tp->pos.y >(300 - r) || tp->pos.y < (-300 + r)) {
        tp->pos.x -= dx;
        tp->pos.y -= dy;
    }
}
bool testCollisionX(fire_t fr, target_t t) {
    float dx = t.center.x + 65 - fr.pos.x;
    float d = sqrt(dx * dx);
    return d <= t.radius;
}

bool testCollisionY(fire_t fr, target_t t) {
    float dy = t.center.y - fr.pos.y;
    float d = sqrt(dy * dy);
    return d <= 75;
}

#if TIMER_ON == 1
void onTimer(int v) {


    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (active)
    {
        salise++;
        if (salise == 99)
        {
            saniye++;
            totaltime++;
            salise = 0;



        }
        if (saniye == 19 && salise == 98)
        {
            if (finishtime > 19)
            {

                saniye = 0;
                salise = 0;
                extime = 0;
                totaltime = 0;
                if (countGER > countGER && countGER > countRUS)
                    win = 2;
                if (countTUR > countGER && countTUR > countRUS)
                    win = 1;
                if (countRUS > countGER && countRUS > countTUR)
                    win = 3;
                countGER = countRUS = countTUR = 0;
                for (int i = 0; i < 5; i++)
                    resetTarget(target, i);
                finish = 1;
            }



            if (countGER > countTUR && countGER == countRUS || countGER > countRUS && countGER == countTUR || countTUR > countGER && countRUS == countTUR)
            {
                extime = rand() % 2 + 3;
                finishtime = 19 + extime;
                saniye = saniye - extime;
                scoreb = true;


                if (!active) {
                    extime = 0;
                    scoreb = false;

                }


            }
            else
            {
                if (countGER > countGER && countGER > countRUS)
                    win = 2;
                if (countTUR > countGER && countTUR > countRUS)
                    win = 1;
                if (countRUS > countGER && countRUS > countTUR)
                    win = 3;
                countGER = countRUS = countTUR = 0;
                for (int i = 0; i < 5; i++)
                    resetTarget(target, i);

                active = !active;
                saniye = 0;
                salise = 0;
                totaltime = 0;
                if (!active) {
                    extime = 0;
                    scoreb = false;

                }
            }



        }




        for (int i = 0; i < 4; i++) {
            target[i].center.y += target[i].speed;
            if (target[i].center.y > 500) {
                resetTarget(target, i);
            }
        }

        if (spacebar && fire_rate == 0) {
            int availFire = findAvailableFire();
            order++;
            if (order == 4)
                order = 1;
            if (availFire != -1) {
                fr[availFire].pos = p.pos;
                fr[availFire].active = true;
                fire_rate = FIRE_RATE;
            }
        }
        if (fire_rate > 0) fire_rate--;

        for (int i = 0; i < MAX_FIRE; i++) {
            if (fr[i].active) {
                fr[i].pos.x += 18;


                if (fr[i].pos.x > 1400) {
                    fr[i].active = false;
                    spacebar = false;
                }

                for (int j = 0; j < 5; j++) {
                    if (testCollisionX(fr[i], target[j]) && testCollisionY(fr[i], target[j])) {
                        resetTarget(target, j);
                        fr[i].active = false;
                        spacebar = false;
                        if (order == 1)
                            countRUS++;
                        else if (order == 2)
                            countTUR++;
                        else if (order == 3)
                            countGER++;



                    }
                }


            }
        }
    }



    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {
    float beg = 0;
    for (int i = 0; i < 4; i++) {
        resetTargetBeg(target, i, beg);
        beg += 250;
    }

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("KALEDEN KALEYE BY Cavit Mert Ergul");

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