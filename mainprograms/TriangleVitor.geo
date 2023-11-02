// Gmsh project created on Sun Oct  8 10:49:25 2023

lc = 3e-2;
Point(1) = {0, 0 , 0, lc};
Point(2) = {1, 0, 0, lc};
Point(3) = {0, 1, 0, lc};
Point(4) = {1, 1, 0, lc};
Point(5) = {-0.5, 0.5, 0, lc};
Point(6) = {1.5, 0.5, 0, lc};
Point(7) = {0, 0.5, 0, lc};
Point(8) = {1, 0.5, 0, lc};

Line(1)={1, 2};
Line(2)={2, 3};
Line(3)={3, 1};
Line(4)={2, 4};
Line(5)={4, 3};
Line(6)={1, 5};
Line(7)={3, 5};
Line(8)={2, 6};
Line(9)={4, 6};
Circle(10)={3,7,1};
Circle(11)={2,8 ,4};
Curve Loop(1)={1,2,3};
Curve Loop(2)={-2,4,5};
Curve Loop(3) = {6, -7, 3};
Curve Loop(4) = {4,9,-8};


Plane Surface(1)={1};
Plane Surface(2)={2};
Plane Surface(3)={3};
Plane Surface(4) = {4};




//+
Point(9) = {0.5, -0, 0, 1.0};
//+
Point(10) = {0.5, 1, 0, 1.0};
