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

float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;
bool lampOn;

void drawCircle(float radius, float r, float g, float b) {
    //radius is the radius of the circle
    //r, g and b are the rgb values for the color of the circle
    glBegin(GL_POLYGON);
    for(int i =0; i <= 360; i++){
        double angle = 2 * M_PI * i / 300;
        double x = radius*cos(angle);
        double y = radius*sin(angle);
        glColor3f(r, g, b);
        glVertex2d(x,y);
    }
    glEnd();
}

void drawRec(float x, float y, float r, float g, float b) {
    //x and y are the width and lengths of the rectangle respectively
    //r, g and b are the rgb values for the color of the rectangle
    glBegin(GL_POLYGON);
        glColor3f(r, g, b);
        glVertex2f(0, 0);
        glVertex2f(x, 0);
        glVertex2f(x, y);
        glVertex2f(0, y);
    glEnd();
}

float bulbRadius = 1.5;
float innerLampRadius = 3.0;
float outerLampRadius = 3.75;

void drawLampOn() {
    glPushMatrix();
        glTranslatef(2.0, 5.3, 0);
        drawCircle(outerLampRadius, 0.55686, 0.63137, 0.65490);
        drawCircle(innerLampRadius, 0.99608, 0.93725, 0.70588);
        drawCircle(bulbRadius, 1.0, 1.0, 1.0);
    glPopMatrix();
}

void drawLampOff() {
    glPushMatrix();
        glTranslatef(2.0, 5.3, 0);
        drawCircle(outerLampRadius, 0.55686, 0.63137, 0.65490);
        drawCircle(innerLampRadius, 0.53333, 0.62353, 0.66667);
        drawCircle(bulbRadius, 0.901, 0.901, 0.901);
    glPopMatrix();
}

void drawStand() {
    float r = 0.33725;
    float g = 0.43529;
    float b = 0.48235;
    glBegin(GL_POLYGON);
        glColor3f(r, g, b);
        glVertex2f(0.3, 2.2);
        glVertex2f(-3, -0.2);
        glVertex2f(-4.5, -3.5);
        glVertex2f(-1.0, -2.5);
        glVertex2f(1.0, -0.2);
        glVertex2f(1.4, 1.6);
    glEnd();
    glBegin(GL_QUADS);
        glColor3f(r, g, b);
        glVertex2f(-4.5, -3.5);
        glVertex2f(-1.5, -6);
        glVertex2f(1.2, -4.1);
        glVertex2f(-1.0, -2.5);
    glEnd();
}

void drawSupport() {
    glBegin(GL_POLYGON);
        glColor3f(0.53333, 0.62353, 0.66667);
        glVertex2f(-2, -0.3);
        glVertex2f(-2.2, -0.5);
        glVertex2f(-2.35, -1);
        glVertex2f(-1.1, -1.4);
        glVertex2f(-0.5, -0.8);
    glEnd();
}

void drawGaps() {
    float r = 1.0;
    float g = 1.0;
    float b = 1.0;
    glBegin(GL_POLYGON);
        glColor3f(r, g, b);
        glVertex2f(0.7, 1.8);
        glVertex2f(0.5, 1.9);
        glVertex2f(-2.2, -0.5);
        glVertex2f(-2.6, -2.3);
        glVertex2f(-1.9, -2.2);
        glVertex2f(0.4, 0.2);
    glEnd();
    drawSupport();
    glBegin(GL_QUADS);
        glColor3f(r, g, b);
        glVertex2f(-1.8, -2.9);
        glVertex2f(-3.4, -3.6);
        glVertex2f(-1.5, -5.1);
        glVertex2f(-0.5, -3.8);
    glEnd();
}



void drawUpperBase() {
    float r = 0.27451;
    float g = 0.34510;
    float b = 0.39216;
    float width = 0.8;
    float radius = 0.33;
    glPushMatrix();
        glTranslatef(-1.9, -6.9, 0);
        drawRec(width, 1.3, r, g, b);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-1.1, -6.57, 0);
        drawCircle(radius, r, g, b);
        glTranslatef(-width, 0, 0);
        drawCircle(radius, r, g, b);
    glPopMatrix();
}

void drawLowerBase() {
    float r = 0.53333;
    float g = 0.62353;
    float b = 0.66667;
    glBegin(GL_TRIANGLES);
        glColor3f(r, g, b);
        glVertex2f(-5.8, -7.7);
        glVertex2f(-1.5, -6.7);
        glVertex2f(2.7, -7.7);
    glEnd();
    glBegin(GL_QUADS);
        glColor3f(r, g, b);
        glVertex2f(-5.8, -7.7);
        glVertex2f(2.7, -7.7);
        glVertex2f(2.9, -8.7);
        glVertex2f(-6, -8.7);
    glEnd();
}

void drawFloor() {
    glBegin(GL_LINES);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(-500, -8.7);
        glVertex2f(500, -8.7);
    glEnd();
}

void drawWholeLamp() {
    drawLowerBase();
    drawUpperBase();
    drawStand();
    drawGaps();
    drawFloor();
    if(lampOn) {
        drawLampOff();
    } else {
        drawLampOn();
    }
}

void glRasterPos2f(float x , float y);

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    
        //controls transformation
        glScalef(k, k, k);
        glTranslatef(tx, ty, 0);
        glRotatef(alpha, 0, 0, 1);
        
        //draw your stuff here
        drawWholeLamp();
    
    glPopMatrix();
    
    glFlush ();
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(-10, 10, -10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init(void) {
    glClearColor (1.0, 1.0, 1.0, 1.0);
    glShadeModel (GL_SMOOTH);
}


void keyboard (unsigned char key, int x, int y) {
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
            
        case ' ':
            if(lampOn){
                lampOn = false;
            } else {
                lampOn = true;
            }
            glutPostRedisplay();
            
            break;
            
        case 27:
            exit(0);
            break;
            
        default:
            break;
    }
}

int main(int argc, char **argv) {
    cout<<"CS3241 Lab 1\n\n";
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
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}
