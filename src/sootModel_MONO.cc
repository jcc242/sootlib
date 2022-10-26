#include "sootModel_MONO.h"

using namespace std;
using namespace soot;

////////////////////////////////////////////////////////////////////////////////

sootModel_MONO::sootModel_MONO(size_t            nsoot_,
                               nucleationModel  *nucl_,
                               growthModel      *grow_,
                               oxidationModel   *oxid_,
                               coagulationModel *coag_) :
        sootModel(nsoot_, nucl_, grow_, oxid_, coag_) {

    if (nsoot_ != 2)
        throw runtime_error("MONO requires nsoot=2");

    nsoot = 2;
}

////////////////////////////////////////////////////////////////////////////////

void sootModel_MONO::getSourceTerms(state &state, 
                                    std::vector<double> &sootSources,
                                    std::vector<double> &gasSources,
                                    std::vector<double> &pahSources) const {

    //---------- get moments

    double M0 = state.sootVar[0];
    double M1 = state.sootVar[1];

    //---------- set weights and abscissas

    if (M0 > 0) {
        state.wts[0] = M0;
        state.absc[0] = M1 / M0;
    }

    //---------- get chemical rates

    double jNuc = nucl->getNucleationSootRate(state);        // #/m3*s
    double kGrw = grow->getGrowthSootRate(state);
    double kOxi = oxid->getOxidationSootRate(state);
    double coa  = coag->getCoagulationSootRate(state, state.absc[0], state.absc[0]);

    //---------- nucleation terms

    double N0 = 0;
    double N1 = 0;

    N0 = jNuc;                                              // #/m3*s
    N1 = jNuc * state.cMin * gasSpMW[(int)gasSp::C] / Na;   // kg_soot/m3*s (as carbon)

    //---------- PAH condensation terms

    double Cnd0 = 0;
    double Cnd1 = 0;

    if (nucl->mechType == nucleationMech::PAH) {
        double nDimer = nucl->DIMER.nDimer;
        double mDimer = nucl->DIMER.mDimer;

        Cnd1 = nDimer * mDimer * coag->getCoagulationSootRate(state, mDimer, state.absc[0]) * state.wts[0];
    }

    //---------- growth terms

    double G0 = 0;
    double G1 = 0;

    double Am2m3 = M0 > 0 ? M_PI * pow(abs(6 / (M_PI * rhoSoot) * M1 / M0), 2.0 / 3) * abs(M0) : 0;

    G1 = kGrw * Am2m3;

    //---------- oxidation terms

    double X0 = 0;
    double X1 = 0;

    X1 = -kOxi * Am2m3;

    //---------- coagulation terms

    double C0 = 0;
    double C1 = 0;

    //C0 = -0.5 * coa  * state.wts[0] * state.wts[0];
    C0 = -coa  * state.wts[0] * state.wts[0];

    //---------- combine to make soot source terms

    sootSources[0] = (N0 + Cnd0 + G0 + X0 + C0);      // #/m3*s
    sootSources[1] = (N1 + Cnd1 + G1 + X1 + C1);      // kg/m3*s

    //---------- set gas source terms

    vector<double> nucl_gasSources((size_t)gasSp::size, 0.0);
    vector<double> grow_gasSources((size_t)gasSp::size, 0.0);
    vector<double> oxid_gasSources((size_t)gasSp::size, 0.0);

    nucl->getNucleationGasRates(state, N1, nucl_gasSources);
    grow->getGrowthGasRates(    state, G1, grow_gasSources);
    oxid->getOxidationGasRates( state, X1, oxid_gasSources);

    for (size_t sp=0; sp<(size_t)gasSp::size; sp++)
        gasSources[sp] = nucl_gasSources[sp] + grow_gasSources[sp] + oxid_gasSources[sp];

    //---------- set PAH source terms

    if(nucl->mechType == nucleationMech::PAH)
        pahSources = nucl->nucleationPahRxnRates;

    //todo: what about pah condensation? (here and in other models)

}