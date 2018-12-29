Name: Anu Kriti Wadhwa
Matric number: A0170594Y

What you are drawing:
- savefile.txt is the batman symbol
- objects over it are black circles to outline the symbol

Primitives and transformations you have used:
GL_LINE_STRIP
GL_POLYGON
GL_POINTS
GL_LINES
glRotatef
glTranslatef

Methods you have modified:
- display(void) modified to have appropriate function calls
- keyboard (unsigned char key, int x, int y) modified to make delete (d), undo(u) and redo(y) keys functional
- mouse(int button, int state, int x, int y) modified to enable deleting of points
  press 'd' to toggle delete mode. When in delete mode, click on existing vertices to delete them
  when you delete from the middle, all excess points will also be deleted

Methods you have created:
- drawCircle(float radius, float r, float g, float b);
- drawCtrlPts();
  draws control points
- drawCtrlLines();
  draws control lines
- eraseCtrlPts();
  erases all vertices
- undoPt();
  un-does the last visible vertex
- redoPt();
  re-does the last undone vertex
- xStaticForm(double t, Point p1, Point p2, Point p3, Point p4);
  applies static formula for curve to x-coordinate
- yStaticForm(double t, Point p1, Point p2, Point p3, Point p4);
  applies static formula for curve to y-coordinate
- xDerivative(double t, Point p1, Point p2, Point p3, Point p4);
  differentiates eqn for x-coordinate to obtain tangent at point
- yDerivative(double t, Point p1, Point p2, Point p3, Point p4);
  differentiates eqn for y-coordinate to obtain tangent at point
- drawThings(int a, int b);
  a decides between CURVE (0) and TANGENT OR OBJECT (1)
  b decides between TANGENT (0) and OBJECT (1)
- makeC1Continuous();
  makes curve c1 continuous
- addToC1List();
  copies points from ptList to c1List
- deletePt(int x, int y);
  deletes point from middle of array

Coolest thing(s) in your drawing:
- undo function removes the last visible vertex
- redo function adds the last undone vertex
- delete function allows vertex to be deleted from middle and deletes all excess points
- savefile.txt is the batman symbol
