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

int main ()
{
    GeoMesh gmesh;
    ReadGmsh read;
    std::string filename("/Users/victorvillegassalabarria/Documents/Github/MalhasTarefaFEM/MallaTriangular.msh");
#ifdef MACOSX
    filename = "../"+filename;
#endif
    gmesh.SetDimension(2);
    read.Read(gmesh,filename);

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
    int bcN = 1;
    int bcD = 0;
    val1(0,0)=0.0;
    val2(0,0)=0.0;
    L2Projection *bc_linha1 = new L2Projection(bcD,matIdBC1,proj,val1,val2);
    L2Projection *bc_linha2 = new L2Projection(bcD,matIdBC2,proj,val1,val2);
    L2Projection *bc_linha3 = new L2Projection(bcD,matIdBC2,proj,val1,val2);
    L2Projection *bc_linha4 = new L2Projection(bcD,matIdBC3,proj,val1,val2);
        //L2Projection *bc_point = new L2Projection(0,3,proj,val1,val2);
    std::vector<MathStatement *> mathvec = {0,mat1,bc_linha1,bc_linha2,bc_linha3,bc_linha4};
    cmesh.SetMathVec(mathvec);
    cmesh.SetDefaultOrder(1);
    cmesh.AutoBuild();
    cmesh.Resequence();

    Analysis locAnalysis(&cmesh);
    locAnalysis.RunSimulation();
    PostProcessTemplate<Poisson> postprocess;
    auto exact = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
    {
        val[0] = (1.-x[0])*x[0]*(1-x[1])*x[1];
        deriv(0,0) = (1.-2.*x[0])*(1-x[1])*x[1];
        deriv(1,0) = (1-2.*x[1])*(1-x[0])*x[0];
    };
    mat1->SetExactSolution(exact);
    postprocess.SetExact(exact);
    

//    if (!strcmp("Sol", name.c_str())) return ESol;
//    if (!strcmp("DSol", name.c_str())) return EDSol;
//    if (!strcmp("Flux", name.c_str())) return EFlux;
//    if (!strcmp("Force", name.c_str())) return EForce;
//    if (!strcmp("SolExact", name.c_str())) return ESolExact;
//    if (!strcmp("DSolExact", name.c_str())) return EDSolExact;
    postprocess.AppendVariable("Sol");
    postprocess.AppendVariable("SolExact");

    
    locAnalysis.PostProcessSolution("quadstar.vtk", postprocess);

    VecDouble errvec;
    errvec = locAnalysis.PostProcessError(std::cout, postprocess);
    
    return 0;
}
