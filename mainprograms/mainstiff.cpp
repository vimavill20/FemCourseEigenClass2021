#include <iostream>
#include <math.h>
#include "VTKGeoMesh.h"
#include "Geom1d.h"
#include "CompElementTemplate.h"
#include "Shape1d.h"
#include "CompMesh.h"
#include "GeoElementTemplate.h"
#include "GeoMesh.h"
#include "Poisson.h"
#include "Analysis.h"

using namespace std;


void CallStiffFunc();

int main() {
    CallStiffFunc();
    return 0;
}
void Fvictor (const VecDouble &co, VecDouble &result){
    result[0]=co[0];
};
void CallStiffFunc(){
    
    int ordenP=1;
    const int dim = 1;
    const int numnodes = 2;
    GeoMesh gmesh;
    gmesh.SetDimension(dim);
    gmesh.SetNumNodes(numnodes);
    GeoNode gnod0, gnod1, gnod2;
    VecDouble co0(3), co1(3), co2(3);
    co0[0]=0.0;
    co1[0]=1.0;
    co2[0]=2.0;
//    co2[0]=6.0;
   // co0 << 0.;
   // co1 << 4.;
    gnod0.SetCo(co0);
    gnod1.SetCo(co1);
    gnod2.SetCo(co2);
//    gnod1.SetCo(co2);
    
    
    gmesh.Node(0) = gnod0;
    gmesh.Node(1) = gnod1;
    gmesh.Node(2) = gnod2;
    
    int materialid = 0;
    VecInt nodeindices(2);
    nodeindices <<0,1;
  
    int index=0 ;
//  const VecInt &nodeindices, int materialid, GeoMesh *gmesh, int index
    GeoElementTemplate<Geom1d> geo(nodeindices,materialid,&gmesh,index);
    nodeindices<<1,2;
    GeoElementTemplate<Geom1d> geo2(nodeindices,materialid,&gmesh,index);
        
    CompMesh cmesh(&gmesh);
    cmesh.SetDefaultOrder(ordenP);
    // Criando material
    MatrixDouble perm(3,3);
    perm.setIdentity();
    perm(0,0) = 1.;
    //Criando math statement de clase poisson e adiciounando na malha
    Poisson poi(materialid, perm);
    poi.SetForceFunction(Fvictor);
    //Criando elemento computacional basado em elemento geometrico, se adicionando na malha computacional
    //Calculando a matriz de T
    cmesh.SetMathStatement(materialid, &poi);
    CompElementTemplate<Shape1d> cel(index,&cmesh,&geo);
    int dimension = cel.Dimension();
    
    int nel=gmesh.NumElements();
    //Aqui test commit
    MatrixDouble ek(nel+1,nel+1),ef(nel+1,1);
    IntRule1d intrule(ordenP);
    cel.SetIntRule(&intrule);
    cel.CalcStiff(ek, ef);
    
    // Create Analysis
    
  
    //Configure Analysis
    
    //Assemble Analysis
    
    //Solve Analysis
    
    //Postprocess
    
    std::cout<<"********************Element Matrix********************"<<std::endl;
    cout << ek << endl;
    std::cout<<"********************Force Vector********************"<<std::endl;
    cout << ef << endl;
    //Analysis analystest(&cmesh);
    //analystest.RunSimulation();
}
