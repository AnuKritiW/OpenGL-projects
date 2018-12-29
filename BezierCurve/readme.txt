This file draws Bezier curves. Click on the window to create a control point. Once there are 4 or more control points, a bezier curve is formed. 

Left mouse click: Add a control point
Q: Quit
P: Toggle displaying control points
L: Toggle displaying control lines
E: Erase all points (Clear)
U: Undo the visible last point
Y: Redo the visible last point
D: Toggle delete mode; delete mode enables you to delete a point from the middle
C: Toggle C1 continuity
T: Toggle displaying tangent vectors
O: Toggle displaying objects
R: Read in control points from "savefile.txt"
W: Write control points to "savefile.txt"

savefile.txt is currently the Batman symbol

Key Methods created:
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