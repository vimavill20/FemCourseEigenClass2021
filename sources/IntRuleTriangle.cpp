/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

///\cond
#include <iostream> 
///\endcond
#include "IntRuleTriangle.h"
#include "IntRule1d.h"

IntRuleTriangle::IntRuleTriangle(){

}

IntRuleTriangle::IntRuleTriangle(int order) {
    SetOrder(order);
}

void IntRuleTriangle::SetOrder(int order) {
    
    fOrder = order;
//    VecDouble co,w;
    //gaulegTrian(-1., 1., co, fWeights);
    if (order == 0 || order == 1) {
        fPoints.resize(1, 2);
        fWeights.resize(1);
        fPoints(0,0) = 1.0/3.0;
        fPoints(0,1) = 1.0/3.0;
        fWeights[0] = 1/2;
    } else if (order == 2) {
        fPoints.resize(3, 2);
        fWeights.resize(3);
        fPoints(0,0) = 0.166666666666667;
        fPoints(0,1) = 0.666666666666667;
        fWeights[0] = 0.333333333333333/2;
        fPoints(1,0) = 0.166666666666667;
        fPoints(1,1) = 0.166666666666667;
        fWeights[1] = 0.333333333333333/2;
        fPoints(2,0) = 0.666666666666667;
        fPoints(2,1) = 0.166666666666667;
        fWeights[2] = 0.333333333333333/2;
    } else if (order == 3) {
        fPoints.resize(4, 2);
        fWeights.resize(4);
        fPoints(0,0) = 0.333333333333333 ;
        fPoints(0,1) = 0.333333333333333;
        fWeights[0] = -0.5625/2;
        fPoints(1,0) = 0.2;
        fPoints(1,1) = 0.6;
        fWeights[1] = 0.520833333333333/2;
        fPoints(2,0) = 0.2;
        fPoints(2,1) = 0.2;
        fWeights[2] = 0.520833333333333/2;
        fPoints(3,0) = 0.6;
        fPoints(3,1) = 0.2;
        fWeights[3] = 0.520833333333333/2;
    }else if (order == 4) {
        fPoints.resize(6, 2);
        fWeights.resize(6);
        fWeights[0] = 0.223381589678011/2;
        fPoints(0,0) = 0.445948490915965;
        fPoints(0,1) = 0.108103018168070;
        fWeights[1] = 0.223381589678011/2;
        fPoints(1,0) = 0.445948490915965;
        fPoints(1,1) = 0.445948490915965;
        fWeights[2] = 0.223381589678011/2;
        fPoints(2,0) = 0.108103018168070;
        fPoints(2,1) = 0.445948490915965;
        fWeights[3] = 0.109951743655322/2;
        fPoints(3,0) = 0.091576213509771;
        fPoints(3,1) = 0.816847572980459;
        fWeights[4] = 0.109951743655322/2;
        fPoints(4,0) = 0.091576213509771;
        fPoints(4,1) = 0.091576213509771;
        fWeights[5] = 0.109951743655322/2;
        fPoints(5,0) = 0.816847572980459;
        fPoints(5,1) = 0.091576213509771;
    }

}
void IntRuleTriangle::gaulegTrian(const double x1, const double x2, VecDouble &co, VecDouble &w) {
    IntRule1d x;
    IntRule1d y;
        
    int n = w.size();

    VecDouble cox(n);
    VecDouble coy(n);
    VecDouble wx(n);
    VecDouble wy(n);

    x.gauleg(x1, x2, cox, wx);
    y.gauleg(x1, x2, coy, wy);
        
    co.resize(2*n*n);
    w.resize(n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            co[i+j*n] = cox[j];
            co[j + i * n + n * n] = coy[i];
            w[n * i + j] = wx[i] * wy[j];
            }
        }
    }

