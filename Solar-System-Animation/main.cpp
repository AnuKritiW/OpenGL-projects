// CS3241Lab1.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>
#include <stdlib.h>

/* Include header files depending on platform */
#ifdef _WIN32
#include "glut.h"
#include "GL\freeglut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

#define numStars 100
#define numPlanets 9

void drawCircle(float radius, float r, float g, float b);
void drawSemiCircle(float radius, float r, float g, float b);

class Planet {
public:
    float distFromRef;
    float angularSpeed;
    GLfloat color[3];
    float size;
    float angle;
    float x = 0;
    float y = 0;
    float z = 1;
    
    Planet() {
        distFromRef = 0;
        angularSpeed = 0;
        color[0] = color[1] = color[2] = 0;
        size = 0;
        angle = 0;
    }
    
    Planet(float distFromRef, float angularSpeed, float r, float g, float b, float size, float angle) {
        this->distFromRef = distFromRef;
        this->angularSpeed = angularSpeed;
        this->color[0] = r;
        this->color[1] = g;
        this->color[2] = b;
        this->size = size;
        this->angle = angle;
    }
    
    void draw() {
        glPushMatrix();
        glRotatef(angle, x, y, z); //rotation around the z axis
        glTranslatef(0, distFromRef, 0);
        glutSolidSphere(size, 1000, 1000);
        drawCircle(size, color[0], color[1], color[2]);
        drawSemiCircle(size, color[0]+0.2, color[1]+0.2, color[2]+0.2);
        glPopMatrix();
    }
    
    
    void update() {
        angle = angularSpeed + angle;
    }
    
    void setX(){
        if(x==1) {
            x = 0;
        } else {
            x = 1;
        }
    }
    
    void setY(){
        if(y==1) {
            y = 0;
        } else {
            y = 1;
        }
    }
    
    void setZ(){
        if(z==1) {
            z = 0;
        } else {
            z = 1;
        }
    }
};

float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;
Planet planetList[numPlanets];

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(-10, 10, -10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

Planet sun;
Planet planet2;
Planet planet3;
Planet planet4;
Planet moon;

void init(void) {
    glClearColor (0.0, 0.0, 0.3, 1.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    sun = Planet(0, 0, 0.99216, 0.72157, 0.07451, 1.5, 0);
    planet2 = Planet(3, 2, 0.5, 0.8, 0.3, 0.5, 0);
    planet3 = Planet(5, -3, 0, 0, 1, 0.75, 0);
    planet4 = Planet(8, 1, 1, 0.5, 1, 1, 0);
}

void drawCircle(float radius, float r, float g, float b) {
    glBegin(GL_POLYGON);
    for(int j = 0; j <=360; j++) {
        for(int i =0; i <= 360; i++){
            double angle = 2 * M_PI * i / 360;
            float x = radius*cos(angle);
            float y = radius*sin(angle);
            glColor3f(r, g, b);
            glVertex2d(x,y);
        }
    }
    glEnd();
}

void drawSemiCircle(float radius, float r, float g, float b)
{
    glBegin(GL_TRIANGLE_FAN);
    for (float i = M_PI/2; i <= (M_PI*1.5); i += 0.001) {
        glColor3f(r, g, b);
        glVertex2f((sin(i)*radius), (cos(i)*radius));
    }
    glEnd();
}

bool clockModeOn = true;

void drawSystem(){
    sun.draw();
    planet2.draw();
    planet3.draw();
    planet4.draw();
}

void updateSystem() {
    planet2.update();
    planet3.update();
    planet4.update();
}

void solarMode() {
    drawSystem();
    updateSystem();
}

void clockMode() {
    sun.draw();
    glPushMatrix();
    time_t current_time = time (NULL);
    struct tm * timeinfo = localtime(&current_time);
    double angle = 360-(float)timeinfo->tm_sec/60*360;
    glRotatef(angle,0,0,1);
    planet2.draw();
    glPopMatrix();
    glPushMatrix();
    double angleMin = 360-(float)timeinfo->tm_min/60*360;
    glRotatef(angleMin,0,0,1);
    planet3.draw();
    glPopMatrix();
    glPushMatrix();
    double angleHr = 360-(float)timeinfo->tm_hour/60*360;
    glRotatef(angleHr,0,0,1);
    planet4.draw();
    glPopMatrix();
    
}

//Star positions
GLfloat starX;
GLfloat starY;
GLfloat starZ;
GLint starNum = 0;
float transparency = 1;

void stars(GLfloat x, GLfloat y, GLfloat z){
    float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3));
    glPointSize(r2);
    glBegin(GL_POINTS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor4f(1.0, 1.0, 1.0, transparency);
    //glColor3f(1.0, 1.0, 1.0);
    glVertex3f(x, y, z);
    glEnd();
}

typedef GLfloat star2[100];
star2 randomX = {};
star2 randomY = {};
star2 randomZ = {};

void drawStars() {
    //create random no. of stars
    if (starNum < 100){
        //Store location to an array
        for (starNum = 0; starNum < 100; starNum++) {
            starX = -4.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 8.0));
            starY = -4.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 8.0));
            starZ = -4.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 8.0));
            
            randomX[starNum] = starX;
            randomY[starNum] = starY;
            randomZ[starNum] = starZ;
        }
    } else {
        //draw 100 stars
        for (int i = 0; i < starNum; i++){
            glPushMatrix();
            glScalef(2.4, 2.4, 1);
            transparency = ((double) rand() / (RAND_MAX));
            stars(randomX[i],randomY[i], randomZ[i]);
            glPopMatrix();
        }
    }
    
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    //controls transformation
    glScalef(k, k, k);
    glTranslatef(tx, ty, 0);
    glRotatef(alpha, 0, 0, 1);
    
    //draw stuff here!
    
    //draw 100 stars
    drawStars();
    
    //draw solar system
    if(clockModeOn) {
        solarMode();
    } else {
        clockMode();
    }
    
    /* swap the drawing buffers */
    glutSwapBuffers();
    glPopMatrix();
}

void idle()
{
    //update animation here
    //transparency -= 0.01;
    glutPostRedisplay();    //after updating, draw the screen again
}

void keyboard (unsigned char key, int x, int y)
{
    //keys to control scaling - k
    //keys to control rotation - alpha
    //keys to control translation - tx, ty
    switch (key) {
            
        case 'a':
            alpha+=10;
            glutPostRedisplay();
            break;
            
        case 'd':
            alpha-=10;
            glutPostRedisplay();
            break;
            
        case 'q':
            k+=0.1;
            glutPostRedisplay();
            break;
            
        case 'e':
            if(k>0.1)
                k-=0.1;
            glutPostRedisplay();
            break;
            
        case 'z':
            tx-=0.1;
            glutPostRedisplay();
            break;
            
        case 'c':
            tx+=0.1;
            glutPostRedisplay();
            break;
            
        case 's':
            ty-=0.1;
            glutPostRedisplay();
            break;
            
        case 'w':
            ty+=0.1;
            glutPostRedisplay();
            break;
            
        case 'i':
            if(clockModeOn) {
                clockModeOn = false;
            } else {
                clockModeOn = true;
            }
            glutPostRedisplay();
            break;
            
        case 'j':
            planet2.setX();
            planet3.setX();
            planet4.setX();
            break;
            
        case 'k':
            planet2.setY();
            planet3.setY();
            planet4.setY();
            break;
            
        case 'l':
            planet2.setZ();
            planet3.setZ();
            planet4.setZ();
            break;
            
        case 27:
            exit(0);
            break;
            
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    cout<<"CS3241 Lab 2\n\n";
    cout<<"+++++CONTROL BUTTONS+++++++\n\n";
    cout<<"Scale Up/Down: Q/E\n";
    cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
    cout<<"Move Up/Down: W/S\n";
    cout<<"Move Left/Right: Z/C\n";
    cout <<"ESC: Quit\n";
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (50, 50);
    glutCreateWindow (argv[0]);
    init ();
    //srand(static_cast <unsigned> (time(0)));
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    //glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}
