/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "L2Projection.h"
///\cond
#include <string.h>
///\endcond

L2Projection::L2Projection() {
}

L2Projection::L2Projection(int bctype, int materialid, MatrixDouble &proj, MatrixDouble Val1, MatrixDouble Val2) {
    projection = proj;
    BCType = bctype;
    BCVal1 = Val1;
    BCVal2 = Val2;
    this->SetMatID(materialid);
    
}

L2Projection::L2Projection(const L2Projection &copy) {
    projection = copy.projection;
    forceFunction = copy.forceFunction;
    SolutionExact = copy.SolutionExact;
    BCType = copy.BCType;
    BCVal1 = copy.BCVal1;
    BCVal2 = copy.BCVal2;

}

L2Projection &L2Projection::operator=(const L2Projection &copy) {
    projection = copy.projection;
    forceFunction = copy.forceFunction;
    SolutionExact = copy.SolutionExact;
    BCType = copy.BCType;
    BCVal1 = copy.BCVal1;
    BCVal2 = copy.BCVal2;
    return *this;
}

L2Projection *L2Projection::Clone() const {
    return new L2Projection(*this);
}

L2Projection::~L2Projection() {
}

MatrixDouble L2Projection::GetProjectionMatrix() const {
    return projection;
}

void L2Projection::SetProjectionMatrix(const MatrixDouble &proj) {
    projection = proj;
}

void L2Projection::Contribute(IntPointData &data, double weight, MatrixDouble &EK, MatrixDouble &EF) const {

    int nstate = this->NState();
    auto &phi=data.phi;
    const int phr=phi.rows();
    if(nstate != 1)
    
    {
        std::cout << "Please implement me\n";
        DebugStop();
    }
    auto nshape = data.phi.size();
    if(EK.rows() != nshape || EF.rows() != nshape)
    {
        DebugStop();
    }

    VecDouble result(nstate);
    result[0] = Val2()(0,0);
    MatrixDouble deriv(data.x.size(), nstate);
    deriv.setZero();
    int in, jn, iv;
    if(SolutionExact)
    {
        SolutionExact(data.x, result, deriv);
    }

    switch (this->GetBCType()) {
//Dirichlet0 Neumann1 Mixed2
        case 0:
        {
            // Your code here
            for( iv=0; iv<nstate; iv++){
                for( in=0; in<phr;in++){
                    EF(nstate*in+iv,0)+=weight*phi(in,0)*result[0]*gBigNumber;
                    for( jn=0; jn<phr;jn++){
                        EK(nstate*in+iv,nstate*jn+iv)+= gBigNumber*weight*phi(in,0)*phi(jn,0);
            }
        }
}
            ;
            break;
        }

        case 1:
        {
        for(auto iv = 0; iv < nstate; iv++){
            for(auto in = 0 ; in < phr; in++) {
                        EF(nstate*in+iv,0) += Val2()(iv,0)*phi(in,0) * weight;
            }//in
        }//iv
             // Your code here
            
            break;
        }

        default:
        {
            std::cout << __PRETTY_FUNCTION__ << " at line " << __LINE__ << " not implemented\n";
        }
    }
    //+++++++++++++++++
}

int L2Projection::NEvalErrors() const {
    return 3;
}

void L2Projection::ContributeError(IntPointData &data, VecDouble &u_exact, MatrixDouble &du_exact, VecDouble &errors) const {
    return;
}

int L2Projection::VariableIndex(const PostProcVar var) const {
    if (var == ESol) return ESol;
    if (var == EDSol) return EDSol;

    // Code should not reach this point. This return is only here to stop compiler warnings.
    DebugStop();
    return 0;
}

L2Projection::PostProcVar L2Projection::VariableIndex(const std::string & name) {
    if (!strcmp("Solution", name.c_str())) return ESol;
    if (!strcmp("Derivative", name.c_str())) return EDSol;

    // Code should not reach this point. This return is only here to stop compiler warnings.
    DebugStop();
    return ENone;
}

int L2Projection::NSolutionVariables(const PostProcVar var) {
    if (var == ESol) return this->NState();
    if (var == EDSol) return this->NState();

    // Code should not reach this point. This return is only here to stop compiler warnings.
    DebugStop();
    return 0;
}

void L2Projection::PostProcessSolution(const IntPointData &data, const int var, VecDouble &Solout) const {
    VecDouble sol = data.solution;
    int solsize = sol.size();
    int rows = data.dsoldx.rows();
    int cols = data.dsoldx.cols();
    MatrixDouble gradu(rows, cols);
    gradu = data.dsoldx;

    int nstate = this->NState();

    switch (var) {
        case 0: //None
        {
            std::cout << " Var index not implemented " << std::endl;
            DebugStop();
        }

        case 1: //ESol
        {
            //+++++++++++++++++
            // Please implement me
            Solout.resize(nstate);
            for (int i = 0; i < nstate; i++) {
            Solout[i] = sol[i];
            //+++++++++++++++++
        }
            break;

        case 2: //EDSol
        {
            //+++++++++++++++++
            // Please implement me
            Solout.resize(rows * cols);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    Solout[i * cols + j] = gradu(i, j);
            //+++++++++++++++++
                }
            }
            
        }
            break;

        default:
        {
            std::cout << " Var index not implemented " << std::endl;
            DebugStop();
        }
    }
    }
    
}
