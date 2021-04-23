

#include <iostream>
#include <math.h>
#include "DataTypes.h"
#include "IntRule1d.h"
#include "tpanic.h"
#include "DataTypes.h"

#include <catch2/catch.hpp>
using namespace Catch::literals;


using std::cout;
using std::endl;
using std::cin;


TEST_CASE("integrate_1d","[integration]")
{
    auto order = GENERATE(0,1,2,3,4,5);
    //  test1D();
    //  testTriangle();
    //  testQuad();
    
    //Teste 1 - Regra de integração - Tabela de valores;
    
    std::vector<double> results(order+1,0.);
    std::vector<double> correct(order+1,0.);
    for(int pol=0; pol<=order; pol+=2) correct[pol] = 2./(pol+1);
    
    IntRule1d TestInt1(order);
    VecDouble CoordXi(1);
    int NPoint = TestInt1.NPoints();
    
    for (int i=0; i<NPoint; i++) {
        double weight;
        TestInt1.Point(i, CoordXi, weight);
        for (int pol=0; pol<=order; pol++) {
            results[pol] += std::pow(CoordXi[0],pol)*weight;
        }
    }
    CAPTURE(order);
    REQUIRE_THAT(results, Catch::Matchers::Approx(correct));
}

TEST_CASE("integrate_1d_out_of_range","[integration]")
{
    int maxorder = IntRule1d::gMaxOrder();
    IntRule1d intrule(0);
    int order = -1;
    CAPTURE(order);
    REQUIRE_THROWS(IntRule1d(order));
    REQUIRE_THROWS(intrule.SetOrder(order));
    order = maxorder+1;
    CAPTURE(order);
    REQUIRE_THROWS(IntRule1d(order));
    REQUIRE_THROWS(intrule.SetOrder(order));
}

