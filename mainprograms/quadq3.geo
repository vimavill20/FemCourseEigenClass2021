//+
SetFactory("OpenCASCADE");

h=1;
Point(1) = {0,0,0,h};
Point(2) = {0,2,0,h};
Point(3) = {2,2,0,h};
Point(4) = {2,0,0,h};//+
Line(1) = {1, 4};
//+
Line(2) = {4, 3};
//+
Line(3) = {3, 2};
//+
Line(4) = {2, 1};
//+
Curve Loop(1) = {4, 1, 2, 3};
//+
Plane Surface(1) = {1};
//+
Physical Surface("mat", 1) = {1};
Physical Curve("bc1", 2) = {1};
Physical Curve("bc2", 3) = {2};
Physical Curve("bc3", 4) = {3};
Physical Curve("bc4", 5) = {4};

Transfinite Curve{:}=3;
Transfinite Surface{1};
Recombine Surface{1};