//
//  ShapeQuad.cpp
//  FemSC
//
//  Created by Philippe Devloo on 03/04/18.
//

#include "DataTypes.h"
#include "tpanic.h"
#include "Shape1d.h"
#include "ShapeQuad.h"

/// computes the shape functions in function of the coordinate in parameter space and orders of the shape functions (size of orders is number of sides of the element topology)
void ShapeQuad::Shape(const VecDouble &xi, VecInt &orders, VecDouble &phi, MatrixDouble &dphi){
    
    for (int i = 0; i < orders.size(); i++)
    {
        if (orders[i] < 0) {
            std::cout << "ShapeQuad::Shape: Invalid dimension for arguments: order\n";
            DebugStop();
        }
    }
    if (orders[0] > 1 || orders[1] > 1 || orders[2] > 1 || orders[3] > 1) {
        std::cout << "ShapeQuad::Shape: Invalid dimension for arguments: order\n";
        DebugStop();
    }

    auto nf = NShapeFunctions(orders);

    if (orders[nf-1] > 2) {
        std::cout << "ShapeQuad::Shape, only implemented until order = 2" << std::endl;
        DebugStop();
    }

    // Linear shape functions
    double x[2],dx[2],y[2],dy[2];
    x[0]  =  (1.-xi[0])/2.;
    x[1]  =  (1.+xi[0])/2.;
    dx[0] = -0.5;
    dx[1] =  0.5;
    y[0]  =  (1.-xi[1])/2.;
    y[1]  =  (1.+xi[1])/2.;
    dy[0] = -0.5;
    dy[1] =  0.5;
    phi[0]  = x[0]*y[0];
    phi[1]  = x[1]*y[0];
    phi[2]  = x[1]*y[1];
    phi[3]  = x[0]*y[1];
    dphi(0,0) = dx[0]*y[0];
    dphi(1,0) = x[0]*dy[0];
    dphi(0,1) = dx[1]*y[0];
    dphi(1,1) = x[1]*dy[0];
    dphi(0,2) = dx[1]*y[1];
    dphi(1,2) = x[1]*dy[1];
    dphi(0,3) = dx[0]*y[1];
    dphi(1,3) = x[0]*dy[1];

    if (orders[0]==2) {
        int is;
        for(is=4; is<8; is++)
        {
            phi[is] = phi[is%4]*phi[(is+1)%4];
            dphi(0,is) = dphi(0,is%4)*phi[(is+1)%4]+phi[is%4]*dphi(0,(is+1)%4);
            dphi(1,is) = dphi(1,is%4)*phi[(is+1)%4]+phi[is%4]*dphi(1,(is+1)%4);
        }
        phi[8] = phi[0]*phi[2];
        dphi(0,8) = dphi(0,0)*phi[2]+phi[0]*dphi(0,2);
        dphi(1,8) = dphi(1,0)*phi[2]+phi[0]*dphi(1,2);
        
        for(is=4; is<8; is++)
        {
            phi[is] += phi[8];
            dphi(0,is) += dphi(0,8);
            dphi(1,is) += dphi(1,8);
            phi[is] *= 4.;
            dphi(0,is) *= 4.;
            dphi(1,is) *= 4.;
        }
        phi[8] *= 16.;
        dphi(0,8) *= 16.;
        dphi(1,8) *= 16.;
    }
    
}

/// returns the number of shape functions associated with a side
int ShapeQuad::NShapeFunctions(int side, int order){
    if(side<4)
        return 1;//0 a 4
    else if(side<8)
        return (order-1);//6 a 14
    else if(side==8)
        return ((order-1)*(order-1));
    
    std::cout << "ShapeQuad::NShapeFunctions, bad parameter side " << side << std::endl;
    DebugStop();
    
    return 0;
}

/// returns the total number of shape functions
int ShapeQuad::NShapeFunctions(VecInt &orders){
    
    int res=4;
    for(int in=4; in<orders.size(); in++) {
        res += NShapeFunctions(in, orders[in]);
    }
    
    return res;
}
