// Gmsh project created on Sat Oct  7 16:53:15 2023
SetFactory("OpenCASCADE");
//+
h=1;
Point(1) = {0, 0, 0, h};
Point(2) = {0.5, 0, 0, h};
Point(3) = {1, 0, 0, h};
Line(1) = {1, 2};
Line(2) = {2, 3};
Transfinite Line{1}=5;
Transfinite Line{2}=2;//+
Physical Point("inlet", 1) = {1};
Physical Point("outlet", 2) = {3};
Physical Curve("mat1", 3) = {1, 2};
