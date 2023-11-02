//
//  TestOneDProblem.cpp
//  FemSC
//
//  Created by Eduardo Ferri on 08/17/15.
//
//
//TestOneDProblem cpp
/*
        Os testes foram preparados com um proposito educacional,
        recomenda-se que o aluno entenda a funcionalidade de cada
        teste e posteriormente use com seu código caso a caso
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
#include "GeoElement.h"
#include "GeoElementTemplate.h"
#include "MathStatement.h"
#include "L2Projection.h"
#include "Analysis.h"
#include "IntRule.h"
#include "PostProcessTemplate.h"
#include "Poisson.h"
#include "VTKGeoMesh.h"

using std::cout;
using std::endl;
using std::cin;

void exact(const VecDouble &point,VecDouble &val, MatrixDouble &deriv);

int main ()
{
    //Criacao de malha geometrica e preenchimento das propiedades da mesma
    //basado na malha Gmesh chamada oneD.mesh
    GeoMesh gmesh;
    ReadGmsh read;
    std::string filename("/Users/victorvillegassalabarria/Documents/Github/FemCourseEigenClass2021/mainprograms/oneD.msh");
    //std::string filename("TestVictor.msh");
#ifdef MACOSX
    filename = "../"+filename;
#endif
    read.Read(gmesh,filename);
    const std::string filenamePrint("testPrintGeo.vtk");
    VTKGeoMesh::PrintGMeshVTK(&gmesh, filenamePrint);
//Criacao de malha computacional e de mathstatement de Poisson com matid 1
    CompMesh cmesh(&gmesh);
    MatrixDouble perm(3,3);
    perm.setZero();
    perm(0,0) = 1.;
    perm(1,1) = 1.;
    perm(2,2) = 1.;
    Poisson *mat1 = new Poisson(1/*materialid*/,perm);
    mat1->SetDimension(1);
    //Funcao conhecida da nossa equacao diferencial que ira para o vetor de carga funcao lambda/
    auto force = [](const VecDouble &x, VecDouble &res)
    {
        res[0] = 1.;
    };
    mat1->SetForceFunction(force);
    MatrixDouble proj(1,1),val1(1,1),val2(1,1);
    proj.setZero();
    val1.setZero();
    val2.setZero();
    //0 dctype significa dirichlet ou imposicao u
    L2Projection *bc_linha = new L2Projection(0/*dctype*/,2/*matid1*/,proj,val1,val2);
    L2Projection *bc_point = new L2Projection(0,3/*matid2*/,proj,val1,val2);
    //Setando todos os mathstatements do problema na malha computacional incluimndo as
    // condicoes de contorno
    std::vector<MathStatement *> mathvec = {0,mat1,bc_point,bc_linha};
    cmesh.SetMathVec(mathvec);
    cmesh.SetDefaultOrder(1);
    cmesh.AutoBuild();
    cmesh.Resequence();

    //Criacao do analysis e rodando a simulacao que envolve Assemble() e Solve()
    Analysis AnalysisLoc(&cmesh);
    AnalysisLoc.RunSimulation();
    //Posprocessamento do erro basado na solucao exxata (exact)
    PostProcessTemplate<Poisson> postprocess;
    postprocess.SetExact(exact);
    mat1->SetExactSolution(exact);
  

    if(!mat1->SolutionExact){
        DebugStop();
    }
    const std::string filenameSol("solution.vtk");
    const std::string namevar("Sol");
    const std::string namevar2("SolExact");
    postprocess.AppendVariable(namevar);
    postprocess.AppendVariable(namevar2);
    AnalysisLoc.PostProcessSolution(filenameSol, postprocess);
    VecDouble errvec;
    errvec = AnalysisLoc.PostProcessError(std::cout, postprocess);
    
   // gmesh.Print(std::cout);
    
    
    return 0;
}
void exact(const VecDouble &point,VecDouble &val, MatrixDouble &deriv){

    deriv(0,0) = 4-point[0];
    val[0]=point[0]*(8.-point[0])/2.;
    return;
}


