//
//  TestOneDProblem.cpp MODIFICADO DO ORIGINAL
//  FemSC
//
//  Created by Eduardo Ferri on 08/17/15.
//
//
//TestOneDProblem cpp
/*
 Os testes foram preparados com um proposito educacional,
 recomenda-se que o aluno entenda a funcionalidade de cada
 teste e posteriormente use com seu cÛdigo caso a caso
 */
//      Obs: O xmax e xmin estao tomados como 4 e 0, respectivamente,
//      caso estes valores sejam alterados, editar o teste TestNodes.
//
//
#include <iostream>
#include <math.h>
#include "GeoMesh.h"
#include "ReadGmsh.h"
#include "CompMesh.h"
#include "Poisson.h"
#include "L2Projection.h"
#include "Analysis.h"
#include "PostProcessTemplate.h"
#include "VTKGeoMesh.h"
#include "GeoElementTemplate.h"
#include "Geom0d.h"
#include "Geom1d.h"
#include "GeomQuad.h"
#include "GeomTriangle.h"

int main ()
{
    GeoMesh gmesh;
    
    ReadGmsh read;
    std::string filename("/Users/victorvillegassalabarria/Documents/Github/FemCourseEigenClass2021/mainprograms/quadq3.msh");

#ifdef MACOSX
    filename = "../"+filename;
#endif
   
    read.Read(gmesh,filename);
    const std::string filenamevtk("geomesh.vtk");
    VTKGeoMesh::PrintGMeshVTK(&gmesh, filenamevtk);
    
    CompMesh cmesh(&gmesh);
    MatrixDouble perm(3,3);
    perm.setZero();
    perm(0,0) = 1.;
    perm(1,1) = 1.;
    perm(2,2) = 1.;
    Poisson *mat1 = new Poisson(1,perm);
    mat1->SetDimension(2);
  
    
    auto force = [](const VecDouble &x, VecDouble &res)
    {
        res[0] = 2.*(1.-x[0])*x[0]+2.*(1-x[1])*x[1];
    };
    mat1->SetForceFunction(force);
    MatrixDouble proj(1,1),val1(1,1),val2(1,1);
    proj.setZero();
    val1.setZero();
    val2.setZero();
    int matIdBC1 = 2;
    int matIdBC2 = 3;
    int matIdBC3 = 4;
    int matIdBC4 = 5;
    int bcN = 0;
    int bcD = 0;
    val1(0,0)=0;
    val2(0,0)= 0.001;
   
    L2Projection *bc_linha1 = new L2Projection(bcN,matIdBC1,proj,val1,val2);
//    bc_linha1->SetExactSolution(<#const std::function<void (const VecDouble &, VecDouble &, MatrixDouble &)> &Exact#>);
    L2Projection *bc_linha2 = new L2Projection(bcN,matIdBC2,proj,val1,val2);
    L2Projection *bc_linha3 = new L2Projection(bcD,matIdBC2,proj,val1,val2);
    L2Projection *bc_linha4 = new L2Projection(bcD,matIdBC3,proj,val1,val2);
//    L2Projection *bc_point = new L2Projection(0,3,proj,val1,val2);
//    L2Projection* bc_linha = new L2Projection(0, 2, proj, val1, val2);
//    std::vector<MathStatement *> mathvec = {0,mat1,bc_point,bc_linha};
    
    std::vector<MathStatement *> mathvec = {0,mat1,bc_linha1,bc_linha2,bc_linha3,bc_linha4};
    
    cmesh.SetMathVec(mathvec);
    cmesh.SetDefaultOrder(1);
    cmesh.AutoBuild();
    cmesh.Resequence();
    cmesh.Print();

    Analysis locAnalysis(&cmesh);
    locAnalysis.RunSimulation();
    

//    if (!strcmp("Sol", name.c_str())) return ESol;
//    if (!strcmp("DSol", name.c_str())) return EDSol;
//    if (!strcmp("Flux", name.c_str())) return EFlux;
//    if (!strcmp("Force", name.c_str())) return EForce;
//    if (!strcmp("SolExact", name.c_str())) return ESolExact;
//    if (!strcmp("DSolExact", name.c_str())) return EDSolExact;
    
    PostProcessTemplate<Poisson> postprocess;
    auto exact = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
    {
        val[0] = (1.-x[0])*x[0]*(1-x[1])*x[1];
        deriv(0,0) = (1.-2.*x[0])*(1-x[1])*x[1];
        deriv(1,0) = (1-2.*x[1])*(1-x[0])*x[0];
    };
    postprocess.SetExact(exact);
    mat1->SetExactSolution(exact);
    const std::string filenameSol("solutionQuad3.vtk");
    const std::string namevar("Sol");
    const std::string namevar2("SolExact");
    
    postprocess.AppendVariable(namevar);
    postprocess.AppendVariable(namevar2);
    locAnalysis.PostProcessSolution(filenameSol, postprocess);
    VecDouble errvec;
    errvec = locAnalysis.PostProcessError(std::cout, postprocess);
    
    return 0;
}
