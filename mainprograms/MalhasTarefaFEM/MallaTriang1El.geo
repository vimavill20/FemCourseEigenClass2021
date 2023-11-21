SetFactory("OpenCASCADE");

h=0;
Point(1) = {0,0,0,h};
Point(2) = {0,1,0,h};
Point(3) = {1,1,0,h};
Point(4) = {1,0,0,h};
Point(5) = {0.5, 0.5, 0, h}; // punto central

Line(1) = {1, 4};
Line(2) = {4, 3};
Line(3) = {3, 2};
Line(4) = {2, 1};
Line(5) = {1, 5}; // línea desde el punto 1 al punto central
Line(6) = {5, 3}; // línea desde el punto central al punto 3

Curve Loop(1) = {5, 1, -6};
Curve Loop(2) = {6, 3, -4};

Plane Surface(1) = {1};
Plane Surface(2) = {2};

Physical Surface("mat", 1) = {1, 2};
Physical Curve("bc1", 2) = {1};
Physical Curve("bc2", 3) = {2};
Physical Curve("bc3", 4) = {3};
Physical Curve("bc4", 5) = {4};

Transfinite Curve{:}=3;
Transfinite Surface{1, 2};
