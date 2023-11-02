/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

///\cond
#include <iostream> 
///\endcond
#include "IntRule1d.h"
#include "IntRuleQuad.h"
using namespace std;

IntRuleQuad::IntRuleQuad(){
}

IntRuleQuad::IntRuleQuad(int order) {
    SetOrder(order);
}

void IntRuleQuad::SetOrder(int order) {
    fOrder = order;
    int np=order/2 +1;
    MatrixDouble co(np,2);
    fWeights.resize(np,1);
    gaulegQuad(-1., 1., co, fWeights);
    for(int ip=0;ip<np;ip++){
        this->fPoints.resize(np,2);
        fPoints(ip,0)=co(ip,0);
        fPoints(ip,1)=co(ip,1);
    }
}

void IntRuleQuad::gaulegQuad(const double x1, const double x2, MatrixDouble &co, VecDouble &w) {
    IntRule1d x;
    IntRule1d y;
        
    int n = w.size();

    VecDouble cox(n);
    VecDouble coy(n);
    VecDouble wx(n);
    VecDouble wy(n);

    x.gauleg(x1, x2, cox, wx);
    y.gauleg(x1, x2, coy, wy);
        
    co.resize(2*n*n,2);
    w.resize(n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            co(i+j*n,0) = cox[j];
            co(i*n+j,1) = coy[i];
            std::cout<< "Punto x: ("<<i*n+j<<",0) "<<co(i+j*n,0)<< " Punto y: ("<<i*n+j<<",1) "<<co(i*n+j,1)<<std::endl;
            w[n * i + j] = wx[j] * wy[i];
            }
        }
    }
