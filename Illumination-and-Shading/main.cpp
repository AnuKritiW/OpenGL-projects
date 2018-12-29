// CS3241Lab3.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#include "glut.h"
#define M_PI 3.141592654
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

// global variable

bool disco_lights = false;
bool m_Smooth = false;
bool m_Highlight = false;
GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
GLfloat field_of_view = 40.0;
GLfloat x_translation = 0.0;

float white[] = { 1.0f, 1.0f, 1.0f, 0.0f };
float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float red[] = {0.85f, 0.1f, 0.2f, 1.0f};
float orange[] = {1.0f, 0.5f, 0.3f, 1.0f};
float yellow[] = {1.0f, 0.85f, 0.0f, 1.0f};
float green[] = {0.6f, 0.8f, 0.6f, 1.0f};
float ogblue[] = {0.1f, 0.5f, 0.8f, 1.0f};
float indigo[] = {0.3f, 0.0f, 0.5f, 1.0f};
float violet[] = {0.9f, 0.5f, 0.9f, 1.0f};
float brown[] = {0.55f, 0.27f, 0.1f, 1.0f};
float grey[] = {0.50196f, 0.50196f, 0.50196f, 1.0f};

int mouseButton = 0;
int moving, startx, starty;

GLfloat zNear = 1.0;
GLfloat zFar = 80.0;
GLfloat aspect = 1.0;
GLfloat clipping = 0.5;

GLfloat eyeX = 1;
GLfloat eyeY = 1;
GLfloat eyeZ = 1;
GLfloat centerX = 0.75;
GLfloat centerY = 0.75;
GLfloat centerZ = -0.75;
GLfloat upX = 0;
GLfloat upY = 1;
GLfloat upZ = 0;

#define NO_OBJECT 4;
int current_object = 0;

using namespace std;

void setupLighting()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	// Lights, material properties
	GLfloat	ambientProperties[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat	diffuseProperties[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat	specularProperties[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPosition[] = { -100.0f, 100.0f, 100.0f, 1.0f };

    if (disco_lights) {
        float temp = fmod((glutGet(GLUT_ELAPSED_TIME) * 0.3), 768);
        GLfloat r = 0, g = 0, b = 0;
        if (temp < 256) {
            r = temp;
            g = 255 - temp;
            b = 255;
        } else if (temp < 512) {
            temp = temp - 256;
            r = 255;
            g = temp;
            b = 255 - temp;
        } else if (temp < 768) {
            temp = temp - 512;
            r = 255 - temp;
            g = 255;
            b = temp;
        }
        GLfloat newSpecularProperties[] = {static_cast<GLfloat>(r/255.0), static_cast<GLfloat>(g/255.0), static_cast<GLfloat>(b/255.0), 1.0f };
        for (int i = 0; i < 4; i++) {
            specularProperties[i] = newSpecularProperties[i];
        }
    }
    
	glClearDepth(1.0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

}


void drawSphere(double r, int type, float mat_diffuse[])
{
    //type is 2 for full sphere and 1 for hemisphere
	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	//float mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	float no_shininess = 0.0f;
    float shininess = 100.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight)
	{
		// your codes for highlight here
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}
	else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}


	int i, j;
	int n = 20;
	for (i = 0; i<n; i++)
		for (j = 0; j<type * n; j++)
			if (m_Smooth)
			{
				glBegin(GL_POLYGON);

                    // the normal of each vertex is actaully its own coordinates normalized for a sphere
                    // from slides: vector from origin to the point is the normal
                    // x = r*cos(theta)*sin(phi)
                    // y = r*sin(theta)*sin(phi)
                    // z = r*cos(phi)
                
                    // your normal here
                    glNormal3d(sin(i*M_PI / n)*cos(j*M_PI / n), cos(i*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
                    glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                    // your normal here
                    glNormal3d(sin((i + 1)*M_PI / n)*cos(j*M_PI / n), cos((i + 1)*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
                    glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                    // your normal here
                    glNormal3d(sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n));
                    glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
                    // your normal here
                    glNormal3d(sin(i*M_PI / n)*cos((j + 1)*M_PI / n), cos(i*M_PI / n)*cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n));
                    glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			}
			else	{
				glBegin(GL_POLYGON);
                    // Explanation: the normal of the  polygon is the coordinate of the center of the sphere
                    glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n));
                    glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                    glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                    glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
                    glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			}

}

void drawEllipsoid(double a, double b, double c, float mat_diffuse[], float type) {
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
    float mat_spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float no_shininess = 0.0f;
    float shininess = 100.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    if (m_Highlight) {
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    } else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    
    int i, j;
    int n = 20;
    for (i = 0; i < type * n; i++) {
        for (j = 0; j < 2 * n; j++) {
            if (m_Smooth) {
                glBegin(GL_POLYGON);
                    glNormal3d(cos(j*M_PI / n)*sin(i*M_PI / n), sin(j*M_PI / n)*sin(i*M_PI / n), cos(i*M_PI / n));
                    glVertex3d(a*cos(j*M_PI / n)*sin(i*M_PI / n), b*sin(j*M_PI / n)*sin(i*M_PI / n), c*cos(i*M_PI / n));
                
                    glNormal3d(cos(j*M_PI / n)*sin((i + 1)*M_PI / n), sin(j*M_PI / n)*sin((i + 1)*M_PI / n), cos((i + 1)*M_PI / n));
                    glVertex3d(a*cos(j*M_PI / n)*sin((i + 1)*M_PI / n), b*sin(j*M_PI / n)*sin((i + 1)*M_PI / n), c*cos((i + 1)*M_PI / n));
                
                    glNormal3d(cos((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), sin((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), cos((i + 1)*M_PI / n));
                    glVertex3d(a*cos((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), b*sin((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), c*cos((i + 1)*M_PI / n));
                
                    glNormal3d(cos((j + 1)*M_PI / n)*sin(i*M_PI / n), sin((j + 1)*M_PI / n)*sin(i*M_PI / n), cos(i*M_PI / n));
                    glVertex3d(a*cos((j + 1)*M_PI / n)*sin(i*M_PI / n), b*sin((j + 1)*M_PI / n)*sin(i*M_PI / n), c*cos(i*M_PI / n));
                glEnd();
            } else {
                glBegin(GL_POLYGON);
                    glNormal3d(cos((j + 0.5)*M_PI / n)*sin((i + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n)*sin((i + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n));
                    glVertex3d(a*cos(j*M_PI / n)*sin(i*M_PI / n), b*sin(j*M_PI / n)*sin(i*M_PI / n), c*cos(i*M_PI / n));
                    glVertex3d(a*cos(j*M_PI / n)*sin((i + 1)*M_PI / n), b*sin(j*M_PI / n)*sin((i + 1)*M_PI / n), c*cos((i + 1)*M_PI / n));
                    glVertex3d(a*cos((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), b*sin((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), c*cos((i + 1)*M_PI / n));
                    glVertex3d(a*cos((j + 1)*M_PI / n)*sin(i*M_PI / n), b*sin((j + 1)*M_PI / n)*sin(i*M_PI / n), c*cos(i*M_PI / n));
                glEnd();
            }
        }
    }
}

void drawEve(){
    glPushMatrix();
        glTranslatef(0, 0.4, 0);
        glPushMatrix();
            glRotatef(90, -180, 0, 0);
            drawSphere(0.5, 1, white);
        glPopMatrix();
        glPushMatrix();
            glRotatef(90, 180, 0, 0);
            drawEllipsoid(0.5, 0.5, 0.25, white, 0.5);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 0, 0);
        glRotatef(90, 1, 0, 0);
        drawEllipsoid(0.5, 0.5, 1, white, 0.5);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, -0.4, 0.8);
        glRotatef(180, 0, 0, 1);
        glRotatef(90, 0.7, 0, 1.5);
        drawEllipsoid(0.4, 0.25, 0.1, white, 0.5);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, -0.4, -0.8);
        glRotatef(-180, 0, 0, 0);
        glRotatef(90, 0.7, 0, -1.5);
        drawEllipsoid(0.4, 0.25, 0.1, white, 0.5);
    glPopMatrix();
}

void drawTorus(double a, double c, float mat_diffuse[]) {
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
    float mat_spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float no_shininess = 0.0f;
    float shininess = 100.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    if (m_Highlight) {
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    } else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    
    int i, j;
    int n = 20;
    for (i = 0; i < 2 * n; i++) {
        for (j = 0; j < 2 * n; j++) {
            if (m_Smooth) {
                glBegin(GL_POLYGON);
                    glNormal3d((cos(j*M_PI / n) * cos(i*M_PI / n)), (cos(j*M_PI / n) * sin(i*M_PI / n)), (sin(j*M_PI / n)));
                    glVertex3d(((c + a * cos(j*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin(i*M_PI / n)), (a * sin(j*M_PI / n)));
                
                    glNormal3d((cos(j*M_PI / n) * cos((i + 1)*M_PI / n)), (cos(j*M_PI / n) * sin((i + 1)*M_PI / n)), (sin(j*M_PI / n)));
                    glVertex3d(((c + a * cos(j*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin(j*M_PI / n)));
                
                    glNormal3d((cos((j + 1)*M_PI / n) * cos((i + 1)*M_PI / n)), (cos((j + 1)*M_PI / n) * sin((i + 1)*M_PI / n)), (sin((j + 1)*M_PI / n)));
                    glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin((j + 1)*M_PI / n)));
                
                    glNormal3d((cos((j + 1)*M_PI / n) * cos(i*M_PI / n)), (cos((j + 1)*M_PI / n) * sin(i*M_PI / n)), (sin((j + 1)*M_PI / n)));
                    glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin(i*M_PI / n)), (a * sin((j + 1)*M_PI / n)));
                glEnd();
            } else {
                glBegin(GL_POLYGON);
                    glNormal3d((cos((j + 0.5)*M_PI / n) * cos((i + 0.5)*M_PI / n)), (cos((j + 0.5)*M_PI / n) * sin((i + 0.5)*M_PI / n)), (sin((j + 0.5)*M_PI / n)));
                    glVertex3d(((c + a * cos(j*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin(i*M_PI / n)), (a * sin(j*M_PI / n)));
                    glVertex3d(((c + a * cos(j*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin(j*M_PI / n)));
                    glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin((j + 1)*M_PI / n)));
                    glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin(i*M_PI / n)), (a * sin((j + 1)*M_PI / n)));
                glEnd();
            }
        }
    }
}

void drawCylinder(double h, double r, float mat_diffuse[]) {
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
    float mat_spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float no_shininess = 0.0f;
    float shininess = 100.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    if (m_Highlight) {
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    } else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    
    int i, j;
    int n = 20;
    for (j = 0; j < 2 * n; j++) {
        if (m_Smooth) {
            glBegin(GL_POLYGON);
                glNormal3d(cos(j*M_PI / n), sin(j*M_PI / n), 0);
                glVertex3d(r*cos(j*M_PI / n), r*sin(j*M_PI / n), 0);
            
                glNormal3d(cos(j*M_PI / n), sin(j*M_PI / n), h);
                glVertex3d(r*cos(j*M_PI / n), r*sin(j*M_PI / n), h);
            
                glNormal3d(cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n), h);
                glVertex3d(r*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n), h);
            
                glNormal3d(cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n), 0);
                glVertex3d(r*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n), 0);
            glEnd();
        } else {
            glBegin(GL_POLYGON);
                glNormal3d(cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n), h / 2);
                glVertex3d(r*cos(j*M_PI / n), r*sin(j*M_PI / n), 0);
                glVertex3d(r*cos(j*M_PI / n), r*sin(j*M_PI / n), h);
                glVertex3d(r*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n), h);
                glVertex3d(r*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n), 0);
            glEnd();
        }
    }
    
    glBegin(GL_POLYGON);
        glNormal3d(0, 0, 1);
        for (int i = 0; i < 360; i++)
        {
            float rad = i*M_PI / 180;
            glVertex3f(r * cos(rad), r* sin(rad), h);
        }
    glEnd();
    
    glPushMatrix();
        glRotatef(180, 1, 0, 0);
        glTranslatef(0, 0, -h);
    
        glBegin(GL_POLYGON);
            glNormal3d(0, 0, 1);
            for (int i = 0; i < 360; i++)
            {
                float rad = i*M_PI / 180;
                glVertex3f(r * cos(rad), r* sin(rad), h);
            }
        glEnd();
    glPopMatrix();
}

void drawTower(){
    glPushMatrix();
        glTranslatef(0, 0, -1.1);
        drawCylinder(2.7, 0.2, brown);
        drawCylinder(0.1, 1.3, brown);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 0, -0.9);
        drawTorus(0.2, 1, red);
        glTranslatef(0, 0, 0.34);
        drawTorus(0.2, 0.9, orange);
        glTranslatef(0, 0, 0.34);
        drawTorus(0.2, 0.8, yellow);
        glTranslatef(0, 0, 0.34);
        drawTorus(0.2, 0.7, green);
        glTranslatef(0, 0, 0.34);
        drawTorus(0.2, 0.6, ogblue);
        glTranslatef(0, 0, 0.34);
        drawTorus(0.2, 0.5, indigo);
        glTranslatef(0, 0, 0.34);
        drawTorus(0.2, 0.4, violet);
    glPopMatrix();
}


void display(void)
{//Add Projection tool and Camera Movement somewhere here
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(field_of_view, aspect, zNear, zFar);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

	glPushMatrix();
	glTranslatef(0, 0, -6);
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glScalef(zoom, zoom, zoom);
	
	switch (current_object) {
	case 0:
		drawSphere(1, 2, ogblue);
		break;
	case 1:
        drawEllipsoid(1, 0.5, 1, grey, 1);
		break;
	case 2:
		// draw your first composite object here
        drawEve();
		break;
	case 3:
		// draw your second composite object here
        drawTower();
		break;
	default:
		break;
	};
	glPopMatrix();
	glutSwapBuffers();
    
    setupLighting();
}


void resetCamera(){
    disco_lights = false;
	//fill in values below.
    zoom = 1;
    angle = 0;
    angle2 = 0;
    zoom = 1;
    field_of_view = 40;
    x_translation = 0;
    zNear = 1;
    zFar = 80;
	// include resetting of gluPerspective & gluLookAt.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    eyeX = 1;
    eyeY = 1;
    eyeZ = 1;
    centerX = 0.75;
    centerY = 0.75;
    centerZ = -0.75;
    upX = 0;
    upY = 1;
    upZ = 0;
	return;
}

void setCameraBestAngle() {
	//fill in values below
    zoom = 1;
    angle = 0;
    angle2 = 0;
    zoom = 1;
    field_of_view = 40;
    x_translation = 0;
    zNear = 1;
    zFar = 80;
	//TIPS: Adjust gluLookAt function to change camera position
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    switch (current_object) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            angle = 90;
            break;
        case 3:
            angle2 = -90;
            break;
    }
	return;
}

void keyboard(unsigned char key, int x, int y)
{//add additional commands here to change Field of View and movement
	switch (key) {
	case 'p':
	case 'P':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'w':
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'v':
	case 'V':
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 's':
	case 'S':
		m_Smooth = !m_Smooth;
		break;
	case 'h':
	case 'H':
		m_Highlight = !m_Highlight;
		break;

	case 'n':
		// -Insert code to adjust Near-
        zNear = zNear - 0.1;
        if (zNear <= 0) {
            zNear = 0.1;
        }
		break;

	case 'N':
		// -Insert code to adjust Near-
        zNear = zNear + 0.1;
		break;

	case 'f':
		// -Insert code to adjust Far-
        zFar = zFar - 0.1;
        if (zFar <= 0) {
            zFar = 0.1;
        }
		break;

	case 'F':
		// -Insert code to adjust Far-
        zFar = zFar + 0.1;
		break;

	case 'o':
		// -Insert code to adjust Fovy-
        field_of_view = field_of_view - 2;
		break;

	case 'O':
		// -Insert code to adjust Fovy-
        field_of_view = field_of_view + 2;
		break;

	case 'r':
		resetCamera();
		break;

	case 'R':
		setCameraBestAngle();
		break;
            
    case 'd':
        if(disco_lights){
            disco_lights = false;
        } else {
            disco_lights = true;
        }
        break;

	case '1':
	case '2':
	case '3':
	case '4':
		current_object = key - '1';
		break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}

	glutPostRedisplay();
}



void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		mouseButton = button;
		moving = 1;
		startx = x;
		starty = y;
	}
	if (state == GLUT_UP) {
		mouseButton = button;
		moving = 0;
	}
}

void motion(int x, int y)
{
	if (moving) {
		if (mouseButton == GLUT_LEFT_BUTTON)
		{
			angle = angle + (x - startx);
			angle2 = angle2 + (y - starty);
		}
		else zoom += ((y - starty)*0.01);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}

}


int main(int argc, char **argv)
{
	cout << "CS3241 Lab 3" << endl << endl;

	cout << "1-4: Draw different objects" << endl;
	cout << "S: Toggle Smooth Shading" << endl;
	cout << "H: Toggle Highlight" << endl;
	cout << "W: Draw Wireframe" << endl;
	cout << "P: Draw Polygon" << endl;
	cout << "V: Draw Vertices" << endl;
	cout << "n, N: Reduce or increase the distance of the near plane from the camera" << endl;
	cout << "f, F: Reduce or increase the distance of the far plane from the camera" << endl;
	cout << "o, O: Reduce or increase the distance of the povy plane from the camera" << endl;
	cout << "r: Reset camera to the initial parameters when the program starts" << endl;
	cout << "R: Change camera to another setting that is has the best viewing angle for your object" << endl;
    cout << "d: Disco lights" << endl;
	cout << "ESC: Quit" << endl << endl;


	cout << "Left mouse click and drag: rotate the object" << endl;
	cout << "Right mouse click and drag: zooming" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("CS3241 Assignment 3");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(display);
    glutIdleFunc(display);
	glMatrixMode(GL_PROJECTION);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glutMainLoop();

	return 0;
}
