/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "IntRule1d.h"
///\cond
#include <iostream> 
#include <vector>
#include <math.h>
#include <cmath>
///\endcond
using namespace std;

#define PI 3.141592654

IntRule1d::IntRule1d(){
    SetOrder(0);
}

IntRule1d::IntRule1d(int order) : IntRule(order) {
    SetOrder(order);
}

void IntRule1d::SetOrder(int order) {
    if(order<0 || order>MaxOrder()){
        DebugStop();
    };
    fOrder = order;
    int np=order/2 +1;
    VecDouble co(np);
    fWeights.resize(np,1);
    gauleg(-1., 1., co, fWeights);
    this->fPoints.resize(np,1);
    for(int ip=0;ip<np;ip++){
        fPoints(ip,0)=co[ip];
    }
    
}

int IntRule1d::TestStatic(int order) {
    return order*order;
}


void IntRule1d::gauleg(const double x1, const double x2, VecDouble &co, VecDouble &w){
    int n = w.size();

    double EPS = 1.0e-14;
    int m, j, i;
    double z1, z, xm, xl, pp, p3, p2, p1;    
    
    m = (n + 1) / 2;
    xm = 0.5 * (x2 + x1);
    xl = 0.5 * (x2 - x1);
    for (i = 0; i < m; i++) {
        z = cos(PI * (i + 0.75) / (n + 0.5));
        do {
            p1 = 1.0;
            p2 = 0.0;
            for (j = 0; j < n; j++) {
                p3 = p2;
                p2 = p1;
                p1 = ((2.0 * j + 1.0) * z * p2 - j * p3) / (j + 1);
            }
            pp = n * (z * p1 - p2) / (z * z - 1.0);
            z1 = z;
            z = z1 - p1 / pp;
        } while (fabs(z - z1) > EPS);
        co[i] = xm - xl*z;
        co[n - 1 - i] = xm + xl*z;
        w[i] = 2.0 * xl / ((1.0 - z * z) * pp * pp);
        w[n - 1 - i] = w[i];
    }
    
}

