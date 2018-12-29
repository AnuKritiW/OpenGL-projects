CS3241-Lab1

Name: Anu Kriti Wadhwa
Matric Number: A0170594Y

What I am drawing: Pixar Lamp - Luxo

Primitives and Transformations I have used:
  Primitives:
    Polygons, Triangles, Quads, Lines
  Transformations:
    Translation to draw Circles of the head of the Lamp
    Translation to draw rectangle and circle components of the upper base of the lamp

Methods I have created and modified:
  Methods created:
    - drawCircle(float radius, float a, float b, float c);
    - drawRec(float x, float y, float a, float b, float c);
    - drawLampOn();
    - drawLampOff();
    - drawStand();
    - drawSupport();
    - drawGaps();
    - drawUpperBase();
    - drawLowerBase();
    - drawFloor();
    - drawWholeLamp();
  Existing Methods Modified:
    - called drawWholeLamp(); in display Method
    - in keyboard(unsigned char key, int x, int y), created new case for the space key
      On pressing the space key, the boolean value of book lampOn gets switched from current value. This triggers the lamp to appear to be switching on and off

What is the coolest thing(s) in my drawing:
  Press the space key to switch the lamp on and off.
  This drawing uses 14 shapes (incl polygons(9), quads(3), lines(1) and triangles(1)).

Any other thing the TA should know?
  1. The assignment was created on a Mac using Xcode. It was subsequently successfully tested on the windows machine in the lab.
  2. This isn't about the assignment, but I have recently been diagnosed with an auto-immune condition, which currently affects my mobility and information processing. This has already been informed to the UG Office. I hope this can be taken into consideration when evaluating my work. Thank you :)
