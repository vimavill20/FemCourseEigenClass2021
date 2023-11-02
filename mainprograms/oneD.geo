// Gmsh project created on Sun Jun 20 19:58:37 2021
//+
Point(1) = {0, 0, 0, 1.0};
//+
Point(2) = {4, 0, 0, 1.0};
Point(3) = {8, 0, 0, 1.0};
//+
Line(1) = {1, 2};
<<<<<<< HEAD
Line(2) = {2, 3};
Transfinite Line{2}=2;
Transfinite Line{1}=2;


Physical Curve("line", 1) = {1,2};
Physical Point("left", 2) = {1};
Physical Point("right", 3) = {3};
=======
//+
//+
Physical Curve("line", 1) = {1};
//+
Physical Point("left", 2) = {1};
//+
Physical Point("right", 3) = {2};
>>>>>>> parent of f1bddaa (PostPorcess solution)
