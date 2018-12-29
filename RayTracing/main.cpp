// CS3241Lab5.cpp
#include <cmath>
#include <iostream>
#include "vector3D.h"
#include <chrono>
#include <float.h>

/* Include header files depending on platform */
#ifdef _WIN32
#include "glut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

#define WINWIDTH 600
#define WINHEIGHT 400
#define NUM_OBJECTS 4
#define MAX_RT_LEVEL 50
#define NUM_SCENE 3

float* pixelBuffer = new float[WINWIDTH * WINHEIGHT * 3];

class Ray { // a ray that start with "start" and going in the direction "dir"
public:
    Vector3 start, dir;
};

class RtObject {
    
public:
    virtual double intersectWithRay(Ray, Vector3& pos, Vector3& normal) = 0; // return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
    
    // Materials Properties
    double ambiantReflection[3] ;
    double diffusetReflection[3] ;
    double specularReflection[3] ;
    double speN = 300;
    
    
};

class Sphere : public RtObject {
    
    Vector3 center_;
    double r_;
public:
    Sphere(Vector3 c, double r) { center_ = c; r_ = r; };
    Sphere() {};
    void set(Vector3 c, double r) { center_ = c; r_ = r; };
    double intersectWithRay(Ray, Vector3& pos, Vector3& normal);
};

//Extra feature: Cylinder
class Cylinder : public RtObject{
    
    Vector3 center_;
    double r_;
    double h_; //height
public:
    Cylinder(Vector3 c, double r, double h) { center_ = c; r_ = r; h_ = h; };
    Cylinder() {};
    void set(Vector3 c, double r, double h) { center_ = c; r_ = r; h_ = h; };
    double intersectWithRay(Ray, Vector3& pos, Vector3& normal);
};

RtObject **objList; // The list of all objects in the scene


// Global Variables
// Camera Settings
Vector3 cameraPos(0,0,-500);

// assume the the following two vectors are normalised
Vector3 lookAtDir(0,0,1);
Vector3 upVector(0,1,0);
Vector3 leftVector(1, 0, 0);
float focalLen = 500;

// Light Settings

Vector3 lightPos(900,1000,-1500);
double ambiantLight[3] = { 0.4,0.4,0.4 };
double diffusetLight[3] = { 0.7,0.7, 0.7 };
double specularLight[3] = { 0.5,0.5, 0.5 };


double bgColor[3] = { 0.1,0.1,0.4 };

int sceneNo = 0;


double Sphere::intersectWithRay(Ray r, Vector3& intersection, Vector3& normal)
// return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
{
    
    // Step 1
    double alpha, beta, gamma, det, t1, t2, t;
    Vector3 d = r.dir;
    Vector3 p = r.start;
    Vector3 c = center_;
    double rad = r_;
    
    alpha = dot_prod(d, d);
    beta = dot_prod(d*2, p - c);
    gamma = dot_prod(p - c, p - c) - pow(rad, 2);
    det = pow(beta, 2) - (4*alpha*gamma);
    
    if(det > 0){
        //find roots (-b+sqrt(det))/2a
        t1 = (-beta + sqrt(det))/(2*alpha);
        t2 = (-beta - sqrt(det))/(2*alpha);
        //pick smaller root
        if (t1 < t2) {
            t = t1;
        } else {
            t = t2;
        }
    } else if (det == 0) {
        t = -beta / (2*alpha);
    } else {
        return -1;
    }
    
    intersection = p + (d*t); //sub vals in ray eqn
    
    normal = (intersection - c);
    normal.normalize();
    
    return t;
}

//Extra feature: Cylinder
double Cylinder::intersectWithRay(Ray r, Vector3& intersection, Vector3& normal) {
// return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
    
    // Step 1
    double alpha, beta, gamma, det, t1, t2, t;
    Vector3 d = r.dir;
    Vector3 p = r.start;
    Vector3 c = center_;
    double rad = r_;
    double height = h_;
    
    alpha = d.x[0] * d.x[0] + d.x[2] * d.x[2];
    beta = 2 * (p.x[0] - c.x[0]) * d.x[0] + 2 * (p.x[2] - c.x[2]) * d.x[2];
    gamma = (p.x[0] - c.x[0]) * (p.x[0] - c.x[0]) + (p.x[2] - c.x[2]) * (p.x[2] - c.x[2]) - pow(rad, 2);
    det = pow(beta, 2) - (4*alpha*gamma);

    double ymax = c.x[1] + (height / 2);
    double ymin = c.x[1] - (height / 2);
    
    if (det > 0) {
        //find roots (-b+sqrt(det))/2a
        t1 = (-beta + sqrt(det))/(2*alpha);
        t2 = (-beta - sqrt(det))/(2*alpha);
        
        double z1 = p.x[1] + (d.x[1] * t1);
        double z2 = p.x[1] + (d.x[1] * t2);
        
        if (ymin <= z1 && z1 <= ymax && ymin <= z2 && z2 <= ymax) {
            if (t1 > 0 && t2 > 0) {
                t = min(t1, t2);
            } else if (t1 > 0) {
                t = t1;
            } else if (t2 > 0) {
                t = t2;
            } else {
                return -1;
            }
        } else if (ymin <= z1 && z1 <= ymax) {
            if (t1 > 0) {
                t = t1;
            } else {
                return -1;
            }
        } else if (ymin <= z2 && z2 <= ymax) {
            if (t2 > 0) {
                t = t2;
            } else {
                return -1;
            }
        } else {
            return -1;
        }
        
        intersection = p + (d * t);
        normal = Vector3(intersection.x[0] - c.x[0], 0, intersection.x[2] - c.x[2]);
        normal.normalize();
        
        return t;
    }
    
    return -1;
}


Vector3 directionToViewPt(Ray ray, Vector3& intersection) {
    Vector3 viewDir;
    
    viewDir = (ray.start - intersection);
    viewDir.normalize();
    
    return viewDir;
}

double distanceToLight(Ray r, double t, Vector3& intersection, Vector3& lightV) {
    double distance = sqrt(pow((lightPos.x[0] - intersection.x[0]), 2) +
                           pow((lightPos.x[1] - intersection.x[1]), 2) +
                           pow((lightPos.x[2] - intersection.x[2]), 2));

    // calculate directional vector to light
    lightV = (lightPos - intersection);
    lightV.normalize();

    return distance;
}


void rayTrace(Ray ray, double& r, double& g, double& b, int fromObj = -1 ,int level = 0)
{
    // Step 4
    if (level == MAX_RT_LEVEL) { return;}
    
    int goBackGround = 1, i = 0;
    
    Vector3 intersection, normal;
    Vector3 lightV;
    Vector3 viewV;
    Vector3 lightReflectionV;
    Vector3 rayReflectionV;
    
    Ray newRay;
    double mint = DBL_MAX, t;
    
    double tempR, tempG, tempB;
    int tempI;
    
    for (i = 0; i < NUM_OBJECTS; i++) {
        if ((t = objList[i]->intersectWithRay(ray, intersection, normal)) > 0) {
            // Step 2
            if((t<mint) && (i != fromObj)) {
                
                mint = t;
                double ambR = ambiantLight[0] * (objList[i]->ambiantReflection[0]);
                double ambG = ambiantLight[1] * (objList[i]->ambiantReflection[1]);
                double ambB = ambiantLight[2] * (objList[i]->ambiantReflection[2]);
                

                // Step 3
                double xDiff = lightPos.x[0] - intersection.x[0];
                double yDiff = lightPos.x[1] - intersection.x[1];
                double zDiff = lightPos.x[2] - intersection.x[2];
                //double distFromLight =  distanceToLight(ray, t, intersection, lightV);
                double distFromLight = sqrt(pow(xDiff, 2) + pow(yDiff, 2) + pow(zDiff, 2));
                lightV = (lightPos - intersection);
                lightV.normalize();
                double a = 0.00000025;
                double f = (1/(a+a*distFromLight+a*pow(distFromLight, 2)));
                double nDotL = dot_prod(normal, lightV);
                double diffuR = f * diffusetLight[0] * (objList[i]->diffusetReflection[0]) * nDotL;
                double diffuG = f * diffusetLight[1] * (objList[i]->diffusetReflection[1]) * nDotL;
                double diffuB = f * diffusetLight[2] * (objList[i]->diffusetReflection[2]) * nDotL;

                if(diffuR < 0) { diffuR = 0;}
                if(diffuG < 0) { diffuG = 0;}
                if(diffuB < 0) { diffuB = 0;}

                lightReflectionV = (normal * (2*nDotL)) - lightV;
                //Vector3 viewDir =(ray.start - intersection);
                viewV = directionToViewPt(ray, intersection);
                double rDotVn = pow(dot_prod(lightReflectionV, viewV), objList[i]->speN);
                double specR = f * specularLight[0] * objList[i]->specularReflection[0] * rDotVn;
                double specG = f * specularLight[1] * objList[i]->specularReflection[1] * rDotVn;
                double specB = f * specularLight[2] * objList[i]->specularReflection[2] * rDotVn;
                
                // Step 4
                double nDotV = dot_prod(normal, viewV);
                rayReflectionV = (normal * (2 * nDotV)) - viewV;
                newRay.start = intersection;
                newRay.dir = rayReflectionV;
                double newR = 0;
                double newG = 0;
                double newB = 0;
                //rayTrace(newRay, newR, newG, newB, i, level + 1);
                
                double refWeight = (objList[i]->speN - 50) / 1000.0;
                
                tempI = i;
                
                tempR = ambR+diffuR+specR+(refWeight*newR);
                tempG = ambG+diffuG+specG+(refWeight*newG);
                tempB = ambB+diffuB+specB+(refWeight*newB);
                
                goBackGround = 0;
                
                //Extra Feature: Shadow
                Ray shadowRay;
                shadowRay.dir = lightV;
                shadowRay.start = intersection;
                
                for (int j = 0; j < NUM_OBJECTS; j++)
                {
                    if (i == j) continue;
                    if ((t = objList[j]->intersectWithRay(shadowRay, intersection, normal)) > 0) {
                        tempR = ambiantLight[0] * objList[i]->ambiantReflection[0];
                        tempG = ambiantLight[1] * objList[i]->ambiantReflection[1];
                        tempB = ambiantLight[2] * objList[i]->ambiantReflection[2];
                        break;
                    }
                }

            }
        }
    }
    
    if (goBackGround) {
        r = bgColor[0];
        g = bgColor[1];
        b = bgColor[2];
    } else {
        rayTrace(newRay, r, g, b, tempI, level + 1);
        r = specularLight[0] * objList[tempI]->specularReflection[0] * r + tempR;
        g = specularLight[0] * objList[tempI]->specularReflection[1] * g + tempG;
        b = specularLight[0] * objList[tempI]->specularReflection[2] * b + tempB;
    }
    
}


void drawInPixelBuffer(int x, int y, double r, double g, double b)
{
    pixelBuffer[(y*WINWIDTH + x) * 3] = (float)r;
    pixelBuffer[(y*WINWIDTH + x) * 3 + 1] = (float)g;
    pixelBuffer[(y*WINWIDTH + x) * 3 + 2] = (float)b;
}

void renderScene()
{
    int x, y;
    Ray ray;
    double r, g, b;
    
    cout << "Rendering Scene " << sceneNo << " with resolution " << WINWIDTH << "x" << WINHEIGHT << "........... ";
    long long int time1 = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count(); // marking the starting time
    
    ray.start = cameraPos;
    
    Vector3 vpCenter = cameraPos + lookAtDir * focalLen;  // viewplane center
    Vector3 startingPt = vpCenter + leftVector * (-WINWIDTH / 2.0) + upVector * (-WINHEIGHT / 2.0);
    Vector3 currPt;
    
    for(x=0;x<WINWIDTH;x++)
        for (y = 0; y < WINHEIGHT; y++)
        {
            currPt = startingPt + leftVector*x + upVector*y;
            ray.dir = currPt-cameraPos;
            ray.dir.normalize();
            rayTrace(ray, r, g, b);
            drawInPixelBuffer(x, y, r, g, b);
        }
    
    long long int time2 = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count(); // marking the ending time
    
    cout << "Done! \nRendering time = " << time2 - time1 << "ms" << endl << endl;
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_DOUBLEBUFFER);
    glDrawPixels(WINWIDTH, WINHEIGHT, GL_RGB, GL_FLOAT, pixelBuffer);
    glutSwapBuffers();
    glFlush ();
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(-10, 10, -10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void setScene(int i = 0)
{
    if (i > NUM_SCENE)
    {
        cout << "Warning: Invalid Scene Number" << endl;
        return;
    }
    
    if (i == 0)
    {
        objList[3] = new Sphere();
        ((Sphere*)objList[0])->set(Vector3(-130, 80, 120), 100);
        ((Sphere*)objList[1])->set(Vector3(130, -80, -80), 100);
        ((Sphere*)objList[2])->set(Vector3(-130, -80, -80), 100);
        ((Sphere*)objList[3])->set(Vector3(130, 80, 120), 100);
        
        objList[0]->ambiantReflection[0] = 0.1;
        objList[0]->ambiantReflection[1] = 0.4;
        objList[0]->ambiantReflection[2] = 0.4;
        objList[0]->diffusetReflection[0] = 0;
        objList[0]->diffusetReflection[1] = 1;
        objList[0]->diffusetReflection[2] = 1;
        objList[0]->specularReflection[0] = 0.2;
        objList[0]->specularReflection[1] = 0.4;
        objList[0]->specularReflection[2] = 0.4;
        objList[0]->speN = 300;
        
        objList[1]->ambiantReflection[0] = 0.6;
        objList[1]->ambiantReflection[1] = 0.6;
        objList[1]->ambiantReflection[2] = 0.2;
        objList[1]->diffusetReflection[0] = 1;
        objList[1]->diffusetReflection[1] = 1;
        objList[1]->diffusetReflection[2] = 0;
        objList[1]->specularReflection[0] = 0.0;
        objList[1]->specularReflection[1] = 0.0;
        objList[1]->specularReflection[2] = 0.0;
        objList[1]->speN = 50;
        
        objList[2]->ambiantReflection[0] = 0.1;
        objList[2]->ambiantReflection[1] = 0.6;
        objList[2]->ambiantReflection[2] = 0.1;
        objList[2]->diffusetReflection[0] = 0.1;
        objList[2]->diffusetReflection[1] = 1;
        objList[2]->diffusetReflection[2] = 0.1;
        objList[2]->specularReflection[0] = 0.3;
        objList[2]->specularReflection[1] = 0.7;
        objList[2]->specularReflection[2] = 0.3;
        objList[2]->speN = 650;
        
        objList[3]->ambiantReflection[0] = 0.3;
        objList[3]->ambiantReflection[1] = 0.3;
        objList[3]->ambiantReflection[2] = 0.3;
        objList[3]->diffusetReflection[0] = 0.7;
        objList[3]->diffusetReflection[1] = 0.7;
        objList[3]->diffusetReflection[2] = 0.7;
        objList[3]->specularReflection[0] = 0.6;
        objList[3]->specularReflection[1] = 0.6;
        objList[3]->specularReflection[2] = 0.6;
        objList[3]->speN = 650;
        
    }
    
    if (i == 1)
    {
        
        // Step 5
        objList[3] = new Sphere();
        ((Sphere*)objList[0])->set(Vector3(80, 50, -250), 50); //blue
        ((Sphere*)objList[1])->set(Vector3(-80, 100, -150), 70); //yellow
        ((Sphere*)objList[2])->set(Vector3(-100, 0, -300), 30); //green
        ((Sphere*)objList[3])->set(Vector3(0, -250, -100), 250); //purple
        
        objList[0]->ambiantReflection[0] = 0.1;
        objList[0]->ambiantReflection[1] = 0.4;
        objList[0]->ambiantReflection[2] = 0.4;
        objList[0]->diffusetReflection[0] = 0;
        objList[0]->diffusetReflection[1] = 1;
        objList[0]->diffusetReflection[2] = 1;
        objList[0]->specularReflection[0] = 0.2;
        objList[0]->specularReflection[1] = 0.4;
        objList[0]->specularReflection[2] = 0.4;
        objList[0]->speN = 300;
        
        objList[1]->ambiantReflection[0] = 0.6;
        objList[1]->ambiantReflection[1] = 0.6;
        objList[1]->ambiantReflection[2] = 0.2;
        objList[1]->diffusetReflection[0] = 1;
        objList[1]->diffusetReflection[1] = 1;
        objList[1]->diffusetReflection[2] = 0;
        objList[1]->specularReflection[0] = 0.0;
        objList[1]->specularReflection[1] = 0.0;
        objList[1]->specularReflection[2] = 0.0;
        objList[1]->speN = 50;
        
        objList[2]->ambiantReflection[0] = 0.1;
        objList[2]->ambiantReflection[1] = 0.6;
        objList[2]->ambiantReflection[2] = 0.1;
        objList[2]->diffusetReflection[0] = 0.1;
        objList[2]->diffusetReflection[1] = 1;
        objList[2]->diffusetReflection[2] = 0.1;
        objList[2]->specularReflection[0] = 0.3;
        objList[2]->specularReflection[1] = 0.7;
        objList[2]->specularReflection[2] = 0.3;
        objList[2]->speN = 650;
        
        objList[3]->ambiantReflection[0] = 0.3;
        objList[3]->ambiantReflection[1] = 0.3;
        objList[3]->ambiantReflection[2] = 0.3;
        objList[3]->diffusetReflection[0] = 0.7;
        objList[3]->diffusetReflection[1] = 0.7;
        objList[3]->diffusetReflection[2] = 0.7;
        objList[3]->specularReflection[0] = 0.6;
        objList[3]->specularReflection[1] = 0.6;
        objList[3]->specularReflection[2] = 0.6;
        objList[3]->speN = 650;
        
    }
    if (i==2) {
        // Step 5
        //Extra feature: Cylinder
        objList[3] = new Cylinder();
        ((Sphere*)objList[0])->set(Vector3(100, 0, -250), 50); //blue
        ((Sphere*)objList[1])->set(Vector3(-150, 100, -100), 70); //yellow
        ((Sphere*)objList[2])->set(Vector3(-50, -50, -300), 30); //green
        ((Cylinder*)objList[3])->set(Vector3(10, 0, -200), 25, 150); //purple
        
        objList[0]->ambiantReflection[0] = 0.1;
        objList[0]->ambiantReflection[1] = 0.4;
        objList[0]->ambiantReflection[2] = 0.4;
        objList[0]->diffusetReflection[0] = 0;
        objList[0]->diffusetReflection[1] = 1;
        objList[0]->diffusetReflection[2] = 1;
        objList[0]->specularReflection[0] = 0.2;
        objList[0]->specularReflection[1] = 0.4;
        objList[0]->specularReflection[2] = 0.4;
        objList[0]->speN = 300;
        
        objList[1]->ambiantReflection[0] = 0.6;
        objList[1]->ambiantReflection[1] = 0.6;
        objList[1]->ambiantReflection[2] = 0.2;
        objList[1]->diffusetReflection[0] = 1;
        objList[1]->diffusetReflection[1] = 1;
        objList[1]->diffusetReflection[2] = 0;
        objList[1]->specularReflection[0] = 0.0;
        objList[1]->specularReflection[1] = 0.0;
        objList[1]->specularReflection[2] = 0.0;
        objList[1]->speN = 50;
        
        objList[2]->ambiantReflection[0] = 0.1;
        objList[2]->ambiantReflection[1] = 0.6;
        objList[2]->ambiantReflection[2] = 0.1;
        objList[2]->diffusetReflection[0] = 0.1;
        objList[2]->diffusetReflection[1] = 1;
        objList[2]->diffusetReflection[2] = 0.1;
        objList[2]->specularReflection[0] = 0.3;
        objList[2]->specularReflection[1] = 0.7;
        objList[2]->specularReflection[2] = 0.3;
        objList[2]->speN = 650;
        
        objList[3]->ambiantReflection[0] = 0.3;
        objList[3]->ambiantReflection[1] = 0.3;
        objList[3]->ambiantReflection[2] = 0.3;
        objList[3]->diffusetReflection[0] = 0.7;
        objList[3]->diffusetReflection[1] = 0.7;
        objList[3]->diffusetReflection[2] = 0.7;
        objList[3]->specularReflection[0] = 0.6;
        objList[3]->specularReflection[1] = 0.6;
        objList[3]->specularReflection[2] = 0.6;
        objList[3]->speN = 650;
    }
    
}

void keyboard (unsigned char key, int x, int y)
{
    //keys to control scaling - k
    //keys to control rotation - alpha
    //keys to control translation - tx, ty
    switch (key) {
        case 's':
        case 'S':
            sceneNo = (sceneNo + 1 ) % NUM_SCENE;
            setScene(sceneNo);
            renderScene();
            glutPostRedisplay();
            break;
        case 'q':
        case 'Q':
            exit(0);
            
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    
    
    cout<<"<<CS3241 Lab 5>>\n\n"<<endl;
    cout << "S to go to next scene" << endl;
    cout << "Q to quit"<<endl;
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (WINWIDTH, WINHEIGHT);
    
    glutCreateWindow ("CS3241 Lab 5: Ray Tracing");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutKeyboardFunc(keyboard);
    
    objList = new RtObject*[NUM_OBJECTS];
    
    // create four spheres
    objList[0] = new Sphere(Vector3(-130, 80, 120), 100);
    objList[1] = new Sphere(Vector3(130, -80, -80), 100);
    objList[2] = new Sphere(Vector3(-130, -80, -80), 100);
    objList[3] = new Sphere(Vector3(130, 80, 120), 100);
    
    setScene(0);
    
    setScene(sceneNo);
    renderScene();
    
    glutMainLoop();
    
    for (int i = 0; i < NUM_OBJECTS; i++)
        delete objList[i];
    delete[] objList;
    
    delete[] pixelBuffer;
    
    return 0;
}
