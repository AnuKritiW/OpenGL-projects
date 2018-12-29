Name: Anu Kriti Wadhwa
Matric number: A0170594Y

What you are drawing:
1. Sphere (given)
2. Ellipsoid
3. Eve from WALL-E
3. Towers of Hanoi but only one of the stacks (kind of like a kid's toy)

Primitives and transformations you have used:
  GL_POLYGON
  glRotatef
  glTranslatef
  glScalef
  gluLookAt
  gluPerspective

Methods you have modified:
  modified setUpLighting
    for disco_lights functionality
  modified drawSphere
    made color an argument
    made type an argument - '2' for a full sphere and '1' for a Hemisphere; corresponding change made in for loop
    added code for highlight
    added normals for smooth
  modified display
    Added projection tool and camera movement
    added code for the different cases
    moved setupLighting from main to display for disco_lights to work
  modified resetCamera
    filled in values and reset gluLookAt by resetting global variables
  modified setBestCameraAngle
    filled in values
    set best angle for each of the objects respectively
  modified keyboard
    added code to adjust near, far and fovy
    added case for disco_lights

Methods you have created:
  drawEllipsoid
  drawTorus
  drawCylinder
    all similar to drawSphere
  drawEve
    used Hemisphere (using the drawSphere method), Semi-ellipsoids (using the drawEllipsoid method)
  drawTower:
    used Torus and Cylinder

Coolest thing(s) in your drawing:
  press 'd' to enable disco_lights

Indicate the material for your objects:
  Plastic
