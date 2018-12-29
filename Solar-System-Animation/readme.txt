CS3241-Lab2

Name: Anu Kriti Wadhwa
Matric Number: A0170594Y

What I am drawing: Solar System

Primitives and Transformations I have used:
  Primitives:
    Polygons, Triangle Fan, Points
  Transformations:
    Rotation to rotate planets around the sun
    Translation to draw the planet the same distance away from the Sun


Methods I have created and modified:
  Methods created:
    - drawCircle(float radius, float a, float b, float c);
    - drawSemiCircle(float radius, float r, float g, float b);
    - drawSystem();
    - updateSystem();
    - solarMode();
    - clockMode();
    - stars(GLfloat x, GLfloat y, GLfloat z);
    - drawStars();
  Existing Classes Modified:
    - Modified the Planet class - added draw and update methods and a Planet constructor
    - Modified init to construct planets
    - Modified display function to draw stars and planets - either solarMode or clockMode
    - in keyboard(unsigned char key, int x, int y), created new case for the 'i', 'j', 'k', and 'l' keys
      On pressing the 'i' key, the modes switch between solarMode and clockMode
      On pressing 'j','k' and 'l', the planets switch between rotating and not rotating around the x, y and z axes respectively


What is the coolest thing(s) in my drawing:
  1. On pressing the 'i' key, the modes switch between solarMode and clockMode
  2. On pressing 'j','k' and 'l', the planets switch between rotating and not rotating around the x, y and z axes respectively
  3. The stars are randomly generated and they twinkle; the stars also have different transparency

Any other thing the TA should know?
  1. The assignment was created on a Mac using Xcode. It was subsequently successfully tested on the windows machine in the lab.
  2. As earlier mentioned, I am a special consideration student due to a recently diagnosed auto-immune disease. My health has deteriorated over the past 2 weeks and I will be hospitalised for a procedure in the coming week.
  As a result, I experienced a lot of difficulty in completing this assignment, and hope that this can be taken into consideration during grading.
  SoC's UG Office has also informed the lecturer of my condition.
