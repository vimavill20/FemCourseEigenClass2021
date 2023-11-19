/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "Assemble.h"
#include "CompMesh.h"
#include "GeoMesh.h"
#include "MathStatement.h"
#include "CompElement.h"



Assemble::Assemble() {
}

Assemble::Assemble(CompMesh *mesh) {
    cmesh = mesh;
}

Assemble::Assemble(const Assemble &copy) {
    cmesh = copy.cmesh;
}

Assemble &Assemble::operator=(const Assemble &copy) {
    cmesh = copy.cmesh;
    return *this;
}

void Assemble::SetMesh(CompMesh *mesh) {
    cmesh = mesh;
}

int64_t Assemble::NEquations() {
    int64_t neq = 0;
    int64_t i, ncon = cmesh->GetDOFVec().size();
    for (i = 0; i < ncon; i++) {
        DOF dof = cmesh->GetDOF(i);
        int64_t dofsize = dof.GetNShape() * dof.GetNState();
        neq += dofsize;
    }
    return neq;
}

void Assemble::OptimizeBandwidth() {    
}

void Assemble::Compute(SparseMat &globmat, MatrixDouble &rhs) {
    
    auto neq = NEquations();
    globmat.resize(neq, neq);
    globmat.setZero();
    rhs.resize(neq, 1);
    rhs.setZero();
    
    int64_t nelem = cmesh->GetGeoMesh()->NumElements();
    for (int el = 0; el < nelem; el++) {
        CompElement *cel = cmesh->GetElement(el);
        int dimension = cel->Dimension();
        int nshape = cel->NShapeFunctions();
        int nstate = cel->GetStatement()->NState();
        const int neqel=nshape*nstate;
        MatrixDouble ek(neqel, neqel);
        MatrixDouble ef(neqel, 1);
        ek.setZero();
        ef.setZero();
//neql en funcao de nshape
        cel->CalcStiff(ek, ef);
        std::cout<<ek<<std::endl;
        std::cout<<ef<<std::endl;
        // Calcular los indices en la matriz global
        int count=0;
        const int64_t ndof =cel->NDOF();
        VecInt destind(ndof);
        destind.setZero();
        for(int i=0; i<ndof; i++){
            const DOF& dofi =cel->GetDOF(i);
            const int64_t firsteq= dofi.GetFirstEquation();
            const int64_t neqdof = dofi.GetNShape()* dofi.GetNState();
            for(int ieqdof=0;ieqdof<neqdof;ieqdof++){
                destind[count++]=firsteq+ieqdof;
            }
        }
        for(int i=0;i<neqel;i++){
            const int64_t dest_i=destind[i];
            rhs(dest_i,0)+= ef(i,0);
            for(int j=0;j<neqel; j++){
                const int64_t dest_j=destind[j];
                globmat.coeffRef(dest_i, dest_j)+=ek(i,j);
                
            }
        }
        // Colocar la contribucion en la matriz y vector global
        //Utilizar ele para assemblar ek e ef globmat e rhs.
        
    }
}
