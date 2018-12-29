// CS3241Lab1.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

/* header files for xcode */
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>

/* header files for lab machine */
//#include "GL\glut.h"

#define MAXPTNO 1000
#define NLINESEGMENT 32
#define NOBJECTONCURVE 8
#define MOVERANGE 4

using namespace std;

// Global variables that you can use
struct Point {
    int x,y;
};

// Storage of control points
int nPt = 0;
Point ptList[MAXPTNO];
Point c1List[MAXPTNO];

// Display options
bool displayControlPoints = true;
bool displayControlLines = true;
bool displayTangentVectors = false;
bool displayObjects = false;
bool C1Continuity = false;

void drawRightArrow()
{
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0,0);
    glVertex2f(100,0);
    glVertex2f(95,5);
    glVertex2f(100,0);
    glVertex2f(95,-5);
    glEnd();
}


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

void drawCtrlPts(){
    glPointSize(5);
    glBegin(GL_POINTS);
    
    for(int i=0;i<nPt; i++)
    {
        if(C1Continuity) {
            //if there are more than 3 points, and if i is every fourth point in each set
            if ((i > 3) && ((i + 2) % 3 == 0)) {
                
                glColor3f(1, 0, 0);
                glVertex2d(c1List[i].x, c1List[i].y);
                
                glColor3f(0.501,0.501,0.501);
                glVertex2d(ptList[i].x,ptList[i].y);
            }
        } else {
            glColor3f(0,0,0);
            glVertex2d(ptList[i].x,ptList[i].y);
        }
    }
    glEnd();
    glPointSize(1);
}

void drawCtrlLines(){
    glColor3f(0, 1, 0);
    for (int i = 1; i < nPt; i++) {
        glBegin(GL_LINES);
        if (C1Continuity) {
            glVertex2f(c1List[i - 1].x, c1List[i - 1].y);
            glVertex2f(c1List[i].x, c1List[i].y);
        } else {
            //create a line between every two consecutive points in the ptList
            glVertex2f(ptList[i - 1].x, ptList[i - 1].y);
            glVertex2f(ptList[i].x, ptList[i].y);
        }
        glEnd();
    }
}

void eraseCtrlPts() {
    //empty ptList
    nPt = 0;
}

Point undoPtList[MAXPTNO];
int undoCount = 0;

void undoPt(){
    undoCount += 1;
    undoPtList[undoCount-1] = ptList[nPt];
    nPt = nPt - 1;
}

void redoPt(){
    nPt = nPt + 1;
    ptList[nPt] = undoPtList[undoCount-1];
    undoCount -= 1;
}

int xStaticForm(double t, Point p1, Point p2, Point p3, Point p4) {
    int newX = pow((1 - t), 3) * p1.x +
    pow((1 - t), 2) * t * 3 * p2.x +
    (1 - t) * pow(t, 2) * 3 * p3.x +
    pow(t, 3) * p4.x;
    return newX;
}

int yStaticForm(double t, Point p1, Point p2, Point p3, Point p4) {
    int newY = pow((1 - t), 3) * p1.y +
    pow((1 - t), 2) * t * 3 * p2.y +
    (1 - t) * pow(t, 2) * 3 * p3.y +
    pow(t, 3) * p4.y;
    return newY;
}

int xDerivative(double t, Point p1, Point p2, Point p3, Point p4) {
    int newX = pow((1 - t), 2) * -3 * p1.x +
    (3 * pow(t, 2) - 4 * t + 1) * 3 * p2.x +
    (-3 * pow(t, 2) + 2 * t) * 3 * p3.x +
    3 * pow(t, 2) * p4.x;
    return newX;
}

int yDerivative(double t, Point p1, Point p2, Point p3, Point p4) {
    int newY = pow((1 - t), 2) * -3 * p1.y +
    (3 * pow(t, 2) - 4 * t + 1) * 3 * p2.y +
    (-3 * pow(t, 2) + 2 * t) * 3 * p3.y +
    3 * pow(t, 2) * p4.y;
    return newY;
}

void drawThings(int a, int b){
    //a decides between CURVE (0) and TANGENT OR OBJECT (1)
    //b decides between TANGENT (0) and OBJECT (1)
    Point changingPt, p1, p2, p3, p4, changingT;
    for (int i = 0; i+3 < nPt; i += 3) {
        if(C1Continuity) {
            p1 = c1List[i];
            p2 = c1List[i+1];
            p3 = c1List[i+2];
            p4 = c1List[i+3];
        } else {
            p1 = ptList[i];
            p2 = ptList[i+1];
            p3 = ptList[i+2];
            p4 = ptList[i+3];
        }
        if(a==0){
            glColor3f(0, 0, 1);
            glBegin(GL_LINE_STRIP);
            //for every line segment
            for (double j = 0; j < NLINESEGMENT + 1; j++) {
                double t = j / NLINESEGMENT;
                
                //use static formula
                changingPt.x = xStaticForm(t, p1, p2, p3, p4);
                
                changingPt.y = yStaticForm(t, p1, p2, p3, p4);
                
                glVertex2d(changingPt.x, changingPt.y);
            }
        }
        if(a == 1){
            for (double j = 0; j < NOBJECTONCURVE + 1; j++) {
                double t = j / NOBJECTONCURVE;
                changingPt.x = xStaticForm(t, p1, p2, p3, p4);
                
                changingPt.y = yStaticForm(t, p1, p2, p3, p4);
                
                //the tangent is the derivative of the curve
                changingT.x = xDerivative(t, p1, p2, p3, p4);
                
                changingT.y = yDerivative(t, p1, p2, p3, p4);
                
                glPushMatrix();
                glTranslatef(changingPt.x, changingPt.y, 0);
                
                double angle = atan((double)changingT.y / (double)changingT.x) * 180 / M_PI;
                if (changingT.x < 0) {
                    angle += 180;
                }
                
                if (b==0) {
                    glRotatef(angle, 0, 0, 1);
                    drawRightArrow();
                    glPopMatrix();
                }
                if(b==1) {
                    glRotatef(angle, 0, 0, 1);
                    drawCircle(15, 0, 0, 0);
                    glPopMatrix();
                }
            }
        }
        glEnd();
    }
}

void makeC1Continuous() {
    for (int i = 0; i < nPt; i++) {
        //if i is the second point of any curve except the first
        if ((i > 3) && ((i + 2) % 3 == 0)) {
            Point fourthPt, thirdPt;
            fourthPt = ptList[i - 1];
            thirdPt = ptList[i - 2];
            //plot x and y coordinate of the second point of the next curve
            //take difference between third and fourth and add it to the fourth to get the
            //coordinates of the second
            c1List[i].x = fourthPt.x - (thirdPt.x - fourthPt.x);
            c1List[i].y = fourthPt.y - (thirdPt.y - fourthPt.y);
        }
        else {
            c1List[i].x = ptList[i].x;
            c1List[i].y = ptList[i].y;
        }
    }
}

void addToC1List() {
    //if n is the third point of any curve except the first
    if ((nPt > 4) && ((nPt + 1) % 3 == 0)) {
        Point fourthPt, thirdPt;
        fourthPt = ptList[nPt - 2];
        thirdPt = ptList[nPt - 3];
        c1List[nPt - 1].x = fourthPt.x - (thirdPt.x - fourthPt.x);
        c1List[nPt - 1].y = fourthPt.y - (thirdPt.y - fourthPt.y);
    } else {
        c1List[nPt - 1].x = ptList[nPt - 1].x;
        c1List[nPt - 1].y = ptList[nPt - 1].y;
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    if(displayControlPoints)
    {
        drawCtrlPts();
    }
    
    if(displayControlLines)
    {
        drawCtrlLines();
    }
    
    drawThings(0, 0);
    //the second argument in this case can be any number as it is never used
    
    if (displayTangentVectors)
    {
        drawThings(1, 0);
    }
    
    if (displayObjects) {
        drawThings(1, 1);
    }
    
    glPopMatrix();
    glutSwapBuffers ();
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,w,h,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

void init(void)
{
    glClearColor (1.0,1.0,1.0, 1.0);
}

void readFile()
{
    
    std::ifstream file;
    file.open("savefile.txt");
    file >> nPt;
    
    if(nPt>MAXPTNO)
    {
        cout << "Error: File contains more than the maximum number of points." << endl;
        nPt = MAXPTNO;
    }
    
    for(int i=0;i<nPt;i++)
    {
        file >> ptList[i].x;
        file >> ptList[i].y;
    }
    
    makeC1Continuous();
    
    file.close();// is not necessary because the destructor closes the open file by default
}

void writeFile()
{
    std::ofstream file;
    file.open("savefile.txt");
    file << nPt << endl;
    
    for(int i=0;i<nPt;i++)
    {
        file << ptList[i].x << " ";
        file << ptList[i].y << endl;
    }
    file.close();// is not necessary because the destructor closes the open file by default
}

bool dlt = false;

void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 'r':
        case 'R':
            readFile();
            break;
            
        case 'w':
        case 'W':
            writeFile();
            break;
            
        case 'T':
        case 't':
            displayTangentVectors = !displayTangentVectors;
            break;
            
        case 'o':
        case 'O':
            displayObjects = !displayObjects;
            break;
            
        case 'p':
        case 'P':
            displayControlPoints = !displayControlPoints;
            break;
            
        case 'L':
        case 'l':
            displayControlLines = !displayControlLines;
            break;
            
        case 'C':
        case 'c':
            C1Continuity = !C1Continuity;
            break;
            
        case 'e':
        case 'E':
            // Do something to erase all the control points added
            eraseCtrlPts();
            break;
            
        case 'u':
        case 'U':
            //undo
            if(nPt != 0) {
                undoPt();
            }
            break;
            
        case 'y':
        case 'Y':
            if(undoCount != 0) {
                redoPt();
            }
            break;
            
        case 'D':
        case 'd':
            dlt = !dlt;
            break;
            
        case 'Q':
        case 'q':
            exit(0);
            break;
            
        default:
            break;
    }
    
    glutPostRedisplay();
}

void deletePt(int x, int y) {
    Point currentPt;
    currentPt.x = x;
    currentPt.y = y;
    for (int i = 0; i < nPt; i++) {
        if ((ptList[i].x >= x - MOVERANGE)
            && (ptList[i].x <= x + MOVERANGE)
            && (ptList[i].y >= y - MOVERANGE)
            && (ptList[i].y <= y + MOVERANGE)) {
            for (int j = i; j < nPt-1; j++) {
                ptList[j] = ptList[j+1];
                nPt--;
                drawCtrlPts();
            }
            break;
        }
    }
}



void mouse(int button, int state, int x, int y)
{
    /*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
    /*state: GLUT_UP or GLUT_DOWN */
    enum
    {
        MOUSE_LEFT_BUTTON = 0,
        MOUSE_MIDDLE_BUTTON = 1,
        MOUSE_RIGHT_BUTTON = 2,
        MOUSE_SCROLL_UP = 3,
        MOUSE_SCROLL_DOWN = 4
    };
    
    if((button == MOUSE_LEFT_BUTTON)&&(state == GLUT_UP) && dlt == false)
    {
        
        if(nPt==MAXPTNO)
        {
            cout << "Error: Exceeded the maximum number of points." << endl;
            return;
        }
        
        ptList[nPt].x=x;
        ptList[nPt].y=y;
        nPt++;
        
        addToC1List();
        
    }
    
    if(dlt == true) {
        deletePt(x, y);
    }
    
    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    cout<<"CS3241 Lab 4"<< endl<< endl;
    cout << "Left mouse click: Add a control point"<<endl;
    cout << "Q: Quit" <<endl;
    cout << "P: Toggle displaying control points" <<endl;
    cout << "L: Toggle displaying control lines" <<endl;
    cout << "E: Erase all points (Clear)" << endl;
    cout << "U: Undo the visible last point" <<endl;
    cout << "Y: Redo the visible last point" <<endl;
    cout << "D: Toggle delete mode; delete mode enables you to delete a point from the middle" <<endl;
    cout << "C: Toggle C1 continuity" <<endl;
    cout << "T: Toggle displaying tangent vectors" <<endl;
    cout << "O: Toggle displaying objects" <<endl;
    cout << "R: Read in control points from \"savefile.txt\"" <<endl;
    cout << "W: Write control points to \"savefile.txt\"" <<endl;
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (50, 50);
    glutCreateWindow ("CS3241 Assignment 4");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}
