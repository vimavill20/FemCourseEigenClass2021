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
//struct TarefaSimulations
//{
//    // geometric element index of the interface element
//    std::vector<int> sim_order;
//    // computational element index of the interface element
//    std::vector<std::string> meshnames;
//
//    // computational element index of the interface element
//    std::vector<const std::function<void(const VecDouble &loc, VecDouble &result, MatrixDouble &deriv)>> exact_sols;
////
//    std::vector<std::function<void(const VecDouble &co, VecDouble &result)>> forces;
//
//    std::vector<std::string> vtk_names;

int main()
{
    GeoMesh gmesh;
    ReadGmsh read;
    std::string filename=("/Users/victorvillegassalabarria/Downloads/MalhaAnelQuad.msh");;
    #ifdef MACOSX
    filename = "../"+filename;
    #endif
    gmesh.SetDimension(2);
    read.Read(gmesh,filename);

    VTKGeoMesh::PrintGMeshVTK(&gmesh, "geomesh.vtk");
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
        const double tempx=x[0], tempy=x[1];
        res[0]=-(2*tempx*tempx) -(2*tempy*tempy);
        //res[0]=10*(M_PI*M_PI)*sin(3*M_PI*tempx)*sin(M_PI*tempy);
        
        
    };
    auto exact = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
        {
            const double tempx=x[0], tempy=x[1];
//            val[0]= sin(3 * M_PI * tempx) * sin( M_PI * tempy);// + tempy*tempy;
//            deriv(0,0)=3 * M_PI * cos(3* M_PI *tempx) * sin(  M_PI * tempy);//*2
//            deriv(1,0)= M_PI * cos( M_PI * tempy) * sin(3*M_PI * tempx);//*2
            val[0]=(tempx*tempx)*(tempy*tempy);// + tempy*tempy;
            deriv(0,0)=2*tempx*tempy*tempy;//*2
            deriv(1,0)=2*tempx*tempx*tempy;//*2
        };
    mat1->SetForceFunction(force);
    mat1->SetExactSolution(exact);
    MatrixDouble proj(1,1),val1(1,1),val2(1,1);
    proj.setZero();
    val1.setZero();
    val2.setZero();
    //    int matIdBC1 = 2;
    //    int matIdBC2 = 3;
    //    int matIdBC3 = 4;
    //    int matIdBC4 = 5;
    //    int bcN = 1;
    //    int bcD = 0;
    //    val1(0,0)=0.0;
    //    val2(0,0)=0.0;
    //    L2Projection *bc_linha1 = new L2Projection(bcD,matIdBC1,proj,val1,val2);
    //    L2Projection *bc_linha2 = new L2Projection(bcD,matIdBC2,proj,val1,val2);
    //    L2Projection *bc_linha3 = new L2Projection(bcD,matIdBC2,proj,val1,val2);
    //    L2Projection *bc_linha4 = new L2Projection(bcD,matIdBC3,proj,val1,val2);
    //
    //        L2Projection *bc_point = new L2Projection(0,3,proj,val1,val2);
    //    std::vector<MathStatement *> mathvec = {0,mat1,bc_linha1,bc_linha2,bc_linha3,bc_linha4};
    //    cmesh.SetMathVec(mathvec);
    L2Projection *bc_linha = new L2Projection(0,5,proj,val1,val2);
    bc_linha->SetExactSolution(exact);
    cmesh.SetMathStatement(6, mat1);
    cmesh.SetMathStatement(5,bc_linha);
//    int order = 2;
    cmesh.SetDefaultOrder(2);
    cmesh.AutoBuild();
    cmesh.Resequence();

    Analysis locAnalysis(&cmesh);
    locAnalysis.RunSimulation();
    PostProcessTemplate<Poisson> postprocess;


    postprocess.SetExact(exact);


    //    if (!strcmp("Sol", name.c_str())) return ESol;
    //    if (!strcmp("DSol", name.c_str())) return EDSol;
    //    if (!strcmp("Flux", name.c_str())) return EFlux;
    //    if (!strcmp("Force", name.c_str())) return EForce;
    //    if (!strcmp("SolExact", name.c_str())) return ESolExact;
    //    if (!strcmp("DSolExact", name.c_str())) return EDSolExact;
    postprocess.AppendVariable("Sol");
    postprocess.AppendVariable("SolExact");
    std::string filevtk = "ProjetoTestTrianOrd2Funcorreta.vtk";
    locAnalysis.PostProcessSolution(filevtk, postprocess);

    VecDouble errvec;
    errvec = locAnalysis.PostProcessError(std::cout, postprocess);

    return 0;
}
//void CaseTwoD(TarefaSimulations alldata, int idata);
//int main ()
//{
//    TarefaSimulations test;
//    std::string common_name="/Users/victorvillegassalabarria/Documents/Github/FemCourseEigenClass2021/mainprograms/MalhasTarefaFEM/";
//       std::string name_mesh1 ="MallaTriangulos.msh";
//       std::string name_mesh2 ="MallaCuadrados.msh";
//       auto force1 = [](const VecDouble &x, VecDouble &res)
//       {
//           res[0]=0.;
//           double xi= x[0];
//           double y= x[1];
//       };
//       auto exact1 = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
//       {
//           const double tempx=x[0], tempy=x[1];
//           val[0]=1.;// + tempy*tempy;
//           deriv(0,0)=0;//*2
//           deriv(1,0)=0;//*2
//       };
//
//       auto exact2 = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
//       {
//           const double tempx=x[0], tempy=x[1];
//           val[0]=tempx;// + tempy*tempy;
//           deriv(0,0)=0;//*2
//           deriv(1,0)=0;//*2
//       };
//       auto force2 = [](const VecDouble &x, VecDouble &res)
//       {
//           double xi= x[0];
//           double y= x[1];
//           res[0]=0;
//
//       };
//
//       auto exact3 = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
//       {
//           const double tempx=x[0], tempy=x[1];
//           val[0]= tempy;// + tempy*tempy;
//           deriv(0,0)=0;//*2
//           deriv(1,0)=1;//*2
//       };
//       auto force3 = [](const VecDouble &x, VecDouble &res)
//       {
//
//           res[0]=0;
//
//       };
//    auto exact4 = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
//    {
//        const double tempx=x[0], tempy=x[1];
//        val[0]= tempx*tempy;// + tempy*tempy;
//        deriv(0,0)=tempy;//*2
//        deriv(1,0)=tempx;//*2
//    };
//    auto force4 = [](const VecDouble &x, VecDouble &res)
//    {
//
//        res[0]=-0;
//
//    };
//    auto exact5 = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
//    {
//        const double tempx=x[0], tempy=x[1];
//        val[0]= tempx*tempx;// + tempy*tempy;
//        deriv(0,0)=2*tempx;//*2
//        deriv(1,0)=0;//*2
//    };
//    auto force5 = [](const VecDouble &x, VecDouble &res)
//    {
//
//        res[0]=-2.;
//
//    };
//    auto exact6 = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
//    {
//        const double tempx=x[0], tempy=x[1];
//        val[0]= tempy*tempy;// + tempy*tempy;
//        deriv(0,0)=0;//*2
//        deriv(1,0)=2*tempy;//*2
//    };
//    auto force6 = [](const VecDouble &x, VecDouble &res)
//    {
//
//        res[0]=-2.;
//
//    };
//    auto exact7 = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
//    {
//        const double tempx=x[0], tempy=x[1];
//        val[0]= (tempx*tempx)*(tempy*tempy);// + tempy*tempy;
//        deriv(0,0)=2*tempx*tempy*tempy;//*2
//        deriv(1,0)=2*tempx*tempx*tempy;//*2
//    };
//    auto force7 = [](const VecDouble &x, VecDouble &res)
//    {
//
//        double xi= x[0];
//        double y= x[1];
//        res[0]=-(2*xi*xi) - (2*y*y);
//
//    };
//    auto exact8 = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
//    {
//        const double tempx=x[0], tempy=x[1];
//        val[0]= sin(2 * M_PI * tempx) * sin(2 * M_PI * tempy);// + tempy*tempy;
//        deriv(0,0)=2 * M_PI * cos(2 * M_PI *tempx) * sin(2 * M_PI * tempy);//*2
//        deriv(1,0)=2 * M_PI * cos(2 * M_PI * tempy) * sin(2 * M_PI * tempx);//*2
//    };
//    auto force8 = [](const VecDouble &x, VecDouble &res)
//    {
//
//        double xi= x[0];
//        double y= x[1];
//        res[0]=8 * (M_PI) * (M_PI)*sin(2 * M_PI * xi) * sin(2 * M_PI * y);
//
//    };
    

   //    auto exact4 = [](const VecDouble &x, VecDouble &val, MatrixDouble &deriv)
   //    {
   //        const double tempx=x[0], tempy=x[1];
   //        val[0]= sin(2*M_PI*tempx)*sin(2*M_PI*tempy);// + tempy*tempy;
   //        deriv(0,0)=2*M_PI*cos(2*M_PI*tempx)*sin(2*M_PI*tempy);//*2
   //        deriv(1,0)=2*M_PI*cos(2*M_PI*tempy)*sin(2*M_PI*tempx);//*2
   //    };
   //    auto force4 = [](const VecDouble &x, VecDouble &res)
   //    {
   //        const double tempx=x[0], tempy=x[1];
   //        res[0]=8*(M_PI*M_PI)*sin(2*M_PI*tempx)*sin(2*M_PI*tempy);
   //
   //    };


       //Simulación 1 funcion 1
//       test.sim_order.push_back(1);
//       test.meshnames.push_back(common_name + name_mesh1);
//       test.exact_sols.push_back(exact1);
//       test.forces.push_back(force1);
//       test.vtk_names.push_back("MallaTri_func1_ordem1.vtk");
//
//       //Simulation2 funcion 1
//       test.sim_order.push_back(2);
//       test.meshnames.push_back(common_name + name_mesh1);
//       test.exact_sols.push_back(exact1);
//       test.forces.push_back(force1);
//       test.vtk_names.push_back("MallaTri_func1_ordem2.vtk");
//
//       //Simulación 3 funcion 1
//       test.sim_order.push_back(1);
//       test.meshnames.push_back(common_name + name_mesh2);
//       test.exact_sols.push_back(exact1);
//       test.forces.push_back(force1);
//       test.vtk_names.push_back("MallaQuad_func1_ordem1.vtk");
//
//       //Simulation4 funcion 1
//       test.sim_order.push_back(2);
//       test.meshnames.push_back(common_name + name_mesh2);
//       test.exact_sols.push_back(exact1);
//       test.forces.push_back(force1);
//       test.vtk_names.push_back("MallaQuad_func1_ordem2.vtk");
//
//       // funcion 2
//       //Simulación 5 funcion 2
//       test.sim_order.push_back(1);
//       test.meshnames.push_back(common_name + name_mesh1);
//       test.exact_sols.push_back(exact2);
//       test.forces.push_back(force2);
//       test.vtk_names.push_back("MallaTri_func2_ordem1.vtk");
//
//       //Simulation6 funcion 2
//       test.sim_order.push_back(2);
//       test.meshnames.push_back(common_name + name_mesh1);
//       test.exact_sols.push_back(exact2);
//       test.forces.push_back(force2);
//       test.vtk_names.push_back("MallaTri_func2_ordem2.vtk");
//
//       //Simulación 7 funcion 2
//       test.sim_order.push_back(1);
//       test.meshnames.push_back(common_name + name_mesh2);
//       test.exact_sols.push_back(exact2);
//       test.forces.push_back(force2);
//       test.vtk_names.push_back("MallaQuad_func2_ordem1.vtk");
//
//       //Simulation8 funcion 2
//       test.sim_order.push_back(2);
//       test.meshnames.push_back(common_name + name_mesh2);
//       test.exact_sols.push_back(exact2);
//       test.forces.push_back(force2);
//       test.vtk_names.push_back("MallaQuad_func2_ordem2.vtk");
//
//       // funcion 3
//       //Simulación 9 funcion 3
//       test.sim_order.push_back(1);
//       test.meshnames.push_back(common_name + name_mesh1);
//       test.exact_sols.push_back(exact3);
//       test.forces.push_back(force3);
//       test.vtk_names.push_back("MallaTri_func3_ordem1.vtk");
//
//       //Simulation10 funcion 3
//       test.sim_order.push_back(2);
//       test.meshnames.push_back(common_name + name_mesh1);
//       test.exact_sols.push_back(exact3);
//       test.forces.push_back(force3);
//       test.vtk_names.push_back("MallaTri_func3_ordem2.vtk");
//
//       //Simulación 11 funcion 3
//       test.sim_order.push_back(1);
//       test.meshnames.push_back(common_name +name_mesh2);
//       test.exact_sols.push_back(exact3);
//       test.forces.push_back(force3);
//       test.vtk_names.push_back("MallaQuad_func3_ordem1.vtk");
//
//       //Simulation12 funcion 3
//       test.sim_order.push_back(2);
//       test.meshnames.push_back(common_name + name_mesh2);
//       test.exact_sols.push_back(exact3);
//       test.forces.push_back(force3);
//       test.vtk_names.push_back("MallaQuad_func3_ordem2.vtk");
//
//    // funcion 4
//    //Simulación 13 funcion 3
//    test.sim_order.push_back(1);
//    test.meshnames.push_back(common_name + name_mesh1);
//    test.exact_sols.push_back(exact4);
//    test.forces.push_back(force4);
//    test.vtk_names.push_back("MallaTri_func4_ordem1.vtk");
//
//    //Simulation14 funcion 3
//    test.sim_order.push_back(2);
//    test.meshnames.push_back(common_name + name_mesh1);
//    test.exact_sols.push_back(exact4);
//    test.forces.push_back(force4);
//    test.vtk_names.push_back("MallaTri_func4_ordem2.vtk");
//
//    //Simulación 15 funcion 3
//    test.sim_order.push_back(1);
//    test.meshnames.push_back(common_name +name_mesh2);
//    test.exact_sols.push_back(exact4);
//    test.forces.push_back(force4);
//    test.vtk_names.push_back("MallaQuad_func4_ordem1.vtk");
//
//    //Simulation16 funcion 3
//    test.sim_order.push_back(2);
//    test.meshnames.push_back(common_name + name_mesh2);
//    test.exact_sols.push_back(exact4);
//    test.forces.push_back(force4);
//    test.vtk_names.push_back("MallaQuad_func4_ordem2.vtk");
//
//    // funcion 5
//    //Simulación 17 funcion 5
//    test.sim_order.push_back(1);
//    test.meshnames.push_back(common_name + name_mesh1);
//    test.exact_sols.push_back(exact5);
//    test.forces.push_back(force5);
//    test.vtk_names.push_back("MallaTri_func5_ordem1.vtk");
//
//    //Simulation18 funcion 5
//    test.sim_order.push_back(2);
//    test.meshnames.push_back(common_name + name_mesh1);
//    test.exact_sols.push_back(exact5);
//    test.forces.push_back(force5);
//    test.vtk_names.push_back("MallaTri_func5_ordem2.vtk");
//
//    //Simulación 19 funcion 5
//    test.sim_order.push_back(1);
//    test.meshnames.push_back(common_name +name_mesh2);
//    test.exact_sols.push_back(exact5);
//    test.forces.push_back(force5);
//    test.vtk_names.push_back("MallaQuad_func5_ordem1.vtk");
//
//    //Simulation 20 funcion 5
//    test.sim_order.push_back(2);
//    test.meshnames.push_back(common_name + name_mesh2);
//    test.exact_sols.push_back(exact5);
//    test.forces.push_back(force5);
//    test.vtk_names.push_back("MallaQuad_func5_ordem2.vtk");
//
//    // funcion 6
//    //Simulación 21 funcion 6
//    test.sim_order.push_back(1);
//    test.meshnames.push_back(common_name + name_mesh1);
//    test.exact_sols.push_back(exact6);
//    test.forces.push_back(force6);
//    test.vtk_names.push_back("MallaTri_func6_ordem1.vtk");
//
//    //Simulation 22 funcion 6
//    test.sim_order.push_back(2);
//    test.meshnames.push_back(common_name + name_mesh1);
//    test.exact_sols.push_back(exact6);
//    test.forces.push_back(force6);
//    test.vtk_names.push_back("MallaTri_func6_ordem2.vtk");
//
//    //Simulación 23 funcion 6
//    test.sim_order.push_back(1);
//    test.meshnames.push_back(common_name +name_mesh2);
//    test.exact_sols.push_back(exact6);
//    test.forces.push_back(force6);
//    test.vtk_names.push_back("MallaQuad_func6_ordem1.vtk");
//
//    //Simulation 24 funcion 6
//    test.sim_order.push_back(2);
//    test.meshnames.push_back(common_name + name_mesh2);
//    test.exact_sols.push_back(exact6);
//    test.forces.push_back(force6);
//    test.vtk_names.push_back("MallaQuad_func6_ordem2.vtk");
//
//    // funcion 7
//    //Simulación 25 funcion 7
//    test.sim_order.push_back(1);
//    test.meshnames.push_back(common_name + name_mesh1);
//    test.exact_sols.push_back(exact7);
//    test.forces.push_back(force7);
//    test.vtk_names.push_back("MallaTri_func7_ordem1.vtk");
//
//    //Simulation 26 funcion 7
//    test.sim_order.push_back(2);
//    test.meshnames.push_back(common_name + name_mesh1);
//    test.exact_sols.push_back(exact7);
//    test.forces.push_back(force7);
//    test.vtk_names.push_back("MallaTri_func7_ordem2.vtk");
//
//    //Simulación 27 funcion 7
//    test.sim_order.push_back(1);
//    test.meshnames.push_back(common_name +name_mesh2);
//    test.exact_sols.push_back(exact7);
//    test.forces.push_back(force7);
//    test.vtk_names.push_back("MallaQuad_func7_ordem1.vtk");
//
//    //Simulation 28 funcion 7
//    test.sim_order.push_back(2);
//    test.meshnames.push_back(common_name + name_mesh2);
//    test.exact_sols.push_back(exact7);
//    test.forces.push_back(force7);
//    test.vtk_names.push_back("MallaQuad_func7_ordem2.vtk");
//
//    // funcion 8
//    //Simulación 25 funcion 7
//    test.sim_order.push_back(1);
//    test.meshnames.push_back(common_name + name_mesh1);
//    test.exact_sols.push_back(exact8);
//    test.forces.push_back(force8);
//    test.vtk_names.push_back("MallaTri_func8_ordem1.vtk");
//
//    //Simulation 26 funcion 7
//    test.sim_order.push_back(2);
//    test.meshnames.push_back(common_name + name_mesh1);
//    test.exact_sols.push_back(exact8);
//    test.forces.push_back(force8);
//    test.vtk_names.push_back("MallaTri_func8_ordem2.vtk");
//
//    //Simulación 27 funcion 7
//    test.sim_order.push_back(1);
//    test.meshnames.push_back(common_name +name_mesh2);
//    test.exact_sols.push_back(exact8);
//    test.forces.push_back(force8);
//    test.vtk_names.push_back("MallaQuad_func8_ordem1.vtk");
//
//    //Simulation 28 funcion 7
//    test.sim_order.push_back(2);
//    test.meshnames.push_back(common_name + name_mesh2);
//    test.exact_sols.push_back(exact8);
//    test.forces.push_back(force8);
//    test.vtk_names.push_back("MallaQuad_func8_ordem2.vtk");
//
//
//       int nsim= test.sim_order.size();
//
//       for(int i=0; i<nsim; i++){
//           std::cout<<"Funcion: "<<i<<std::endl;
//           CaseTwoD(test, i);
//       }
//}
//void CaseTwoD(TarefaSimulations alldata, int idata){
//    GeoMesh gmesh;
//    ReadGmsh read;
//    std::string filename=alldata.meshnames[idata];;
//#ifdef MACOSX
//filename = "../"+filename;
//#endif
//gmesh.SetDimension(2);
//read.Read(gmesh,filename);
//
//VTKGeoMesh::PrintGMeshVTK(&gmesh, "geomesh.vtk");
//CompMesh cmesh(&gmesh);
//MatrixDouble perm(3,3);
//perm.setZero();
//perm(0,0) = 1.;
//perm(1,1) = 1.;
//perm(2,2) = 1.;
//Poisson *mat1 = new Poisson(1,perm);
//mat1->SetDimension(2);
//
//auto force = alldata.forces[idata];
//auto exact = alldata.exact_sols[idata];
//mat1->SetForceFunction(force);
//mat1->SetExactSolution(exact);
//MatrixDouble proj(1,1),val1(1,1),val2(1,1);
//proj.setZero();
//val1.setZero();
//val2.setZero();
////    int matIdBC1 = 2;
////    int matIdBC2 = 3;
////    int matIdBC3 = 4;
////    int matIdBC4 = 5;
////    int bcN = 1;
////    int bcD = 0;
////    val1(0,0)=0.0;
////    val2(0,0)=0.0;
////    L2Projection *bc_linha1 = new L2Projection(bcD,matIdBC1,proj,val1,val2);
////    L2Projection *bc_linha2 = new L2Projection(bcD,matIdBC2,proj,val1,val2);
////    L2Projection *bc_linha3 = new L2Projection(bcD,matIdBC2,proj,val1,val2);
////    L2Projection *bc_linha4 = new L2Projection(bcD,matIdBC3,proj,val1,val2);
////
////        L2Projection *bc_point = new L2Projection(0,3,proj,val1,val2);
////    std::vector<MathStatement *> mathvec = {0,mat1,bc_linha1,bc_linha2,bc_linha3,bc_linha4};
////    cmesh.SetMathVec(mathvec);
//L2Projection *bc_linha = new L2Projection(0,2,proj,val1,val2);
//bc_linha->SetExactSolution(exact);
//cmesh.SetMathStatement(1, mat1);
//cmesh.SetMathStatement(2,bc_linha);
//int order = alldata.sim_order[idata];
//cmesh.SetDefaultOrder(order);
//cmesh.AutoBuild();
//cmesh.Resequence();
//
//Analysis locAnalysis(&cmesh);
//locAnalysis.RunSimulation();
//PostProcessTemplate<Poisson> postprocess;
//
//
//postprocess.SetExact(exact);
//
//
////    if (!strcmp("Sol", name.c_str())) return ESol;
////    if (!strcmp("DSol", name.c_str())) return EDSol;
////    if (!strcmp("Flux", name.c_str())) return EFlux;
////    if (!strcmp("Force", name.c_str())) return EForce;
////    if (!strcmp("SolExact", name.c_str())) return ESolExact;
////    if (!strcmp("DSolExact", name.c_str())) return EDSolExact;
//postprocess.AppendVariable("Sol");
//postprocess.AppendVariable("SolExact");
//std::string filevtk = alldata.vtk_names[idata];
//locAnalysis.PostProcessSolution(filevtk, postprocess);
//
//VecDouble errvec;
//errvec = locAnalysis.PostProcessError(std::cout, postprocess);
//
//return 0;
//}
