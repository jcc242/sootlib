#include <catch2/catch.hpp>

#include "state.h"

using namespace std;
using namespace soot;

TEST_CASE("state object initialization with default values", "[state]") {

    state S = state();

    REQUIRE(S.T == 0);
    REQUIRE(S.P == 0);
    REQUIRE(S.rhoGas == 0);
    REQUIRE(S.MWGas == 0);
    REQUIRE(S.muGas == 0);
    REQUIRE(S.cMin == 100);

    // sample gasFractions map
    REQUIRE(S.gasFractions.at(gasSp::C2H2) == 0);
    REQUIRE(S.gasFractions.at(gasSp::H2O) == 0);
    REQUIRE(S.gasFractions.at(gasSp::CO) == 0);

    // sample pahFractions map
    REQUIRE(S.pahFractions.at(pahSp::C10H8) == 0);
    REQUIRE(S.pahFractions.at(pahSp::C12H10) == 0);
    REQUIRE(S.pahFractions.at(pahSp::C16H10) == 0);

    // check sootVar
    REQUIRE(S.sootVar.empty());

}

TEST_CASE("setState function call with 'good' values", "[state]") {

    state S = state();

    vector<double> yGas = {0, 0.1, 0.1, 0.01, 0.02, 0.03, 0.04, 0.2};   // [H, H2, O, O2, OH, H2O, CO, C2H2]
    vector<double> yPAH = {0, 0.1, 0.1, 0.01, 0.02, 0};                 // [C10H8, C12H8, C12H10, C14H10, C16H10, C18H10]
    vector<double> ySootVar{0.003, 1.5E-5};

    S.setState(2500, 101325, 0.1, 0.5, 29, yGas, yPAH, ySootVar, 150);

    SECTION("correct values are assigned to scalar values") {
        REQUIRE(S.T == 2500);
        REQUIRE(S.P == 101325);
        REQUIRE(S.rhoGas == 0.1);
        REQUIRE(S.muGas == 0.5);
        REQUIRE(S.MWGas == 29);
        REQUIRE(S.cMin == 150);
    }

    SECTION("gas and PAH mass fractions are assigned correctly") {
        REQUIRE(S.gasFractions.at(gasSp::H) == 0);
        REQUIRE(S.gasFractions.at(gasSp::C2H2) == 0.2);
        REQUIRE(S.gasFractions.at(gasSp::H2O) == 0.03);
        REQUIRE(S.pahFractions.at(pahSp::C12H8) == 0.1);
        REQUIRE(S.pahFractions.at(pahSp::C16H10) == 0.02);
    }

    SECTION("sootVar is resized properly with correct values") {
        REQUIRE(S.sootVar.size() == 2);
        REQUIRE(S.sootVar.at(0) == 0.003);
        REQUIRE(S.sootVar.at(1) == 1.5E-5);
    }

    SECTION("on subsequent function calls") {

        vector<double> yGas2 = {0.5, 0.001, 0.12, 0.24, 0.02, 0, 0, 0.02};   // [H, H2, O, O2, OH, H2O, CO, C2H2]
        vector<double> yPAH2 = {0.003, 0.01, 0.1, 0.1, 0.0002, 0.25};          // [C10H8, C12H8, C12H10, C14H10, C16H10, C18H10]
        vector<double> ySootVar2{0.1, 1.4E-3, 0.0006};

        S.setState(1234, 101325, 0.01, 0.006, 29, yGas2, yPAH2, ySootVar2, 150);

        SECTION("old scalar values do not persist") {
            REQUIRE(S.T == 1234);
            REQUIRE(S.rhoGas == 0.01);
            REQUIRE(S.muGas == 0.006);
        }

        SECTION("old gas and PAH mass fraction values do not persist") {
            REQUIRE(S.gasFractions.at(gasSp::H) == 0.5);
            REQUIRE(S.gasFractions.at(gasSp::C2H2) == 0.02);
            REQUIRE(S.gasFractions.at(gasSp::H2O) == 0);
            REQUIRE(S.pahFractions.at(pahSp::C12H8) == 0.01);
            REQUIRE(S.pahFractions.at(pahSp::C16H10) == 0.0002);
        }

        SECTION("sootVar sized correctly and old sootVar values do not persist") {
            REQUIRE(S.sootVar.size() == 3);
            REQUIRE(S.sootVar.at(0) == 0.1);
            REQUIRE(S.sootVar.at(1) == 1.4E-3);
            REQUIRE(S.sootVar.at(2) == 0.0006);
        }
    }
}

TEST_CASE("setState function call with 'bad' values", "[state]") {

    SECTION("negative values for scalar variables") {}
    SECTION("incorrect gas fractions vector size") {}
    SECTION("incorrect PAH fractions vector size") {}

}

//WHEN("function call with 'bad' values") {
//
//vector<double> yGas = {0, 0.1, 0.1, 0.01, 0.02, 0.03, 0.04, 0.2};   // [H, H2, O, O2, OH, H2O, CO, C2H2]
//vector<double> yPAH = {0, 0.1, 0.1, 0.01, 0.02, 0};                 // [C10H8, C12H8, C12H10, C14H10, C16H10, C18H10]
//vector<double> ySootVar{0.003, 1.5E-5};
//
//S.setState(2500, 101325, 0.1, 0.5, 29, yGas, yPAH, ySootVar, 150);
//
//}