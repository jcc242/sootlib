#include "sootModels/sootChemistry/psdModels/psdModel_SECT.h"

using namespace std;
using namespace soot;

psdModel_SECT::psdModel_SECT(size_t n) {

    if (n < 1)
        throw runtime_error("Invalid number of soot sections requested");

    mechType = psdMech::SECT;
}

void psdModel_SECT::setSourceTerms(state& state, std::ostream* out) const {

    if (out) {
        *out << " === [SootModel SECT] ===" << endl;
        *out << endl;
    }

    //---------- get weights and abscissas

    vector<double> wts(nsoot, 0);
    vector<double> absc(nsoot, 0);

    for (size_t k = 0; k < nsoot; k++)
        absc[k] = state.cMin * pow(2, k) * gasSpMW[(int)gasSp::C] / Na;

    for (double& num : wts) {
        if (num < 0)
            num = 0;
    }

    double Jnuc = nuc->getNucleationSootRate(state, absc, wts);


    vector<double> Kgrw(nsoot, 0);
    for (double& num : Kgrw)
        num = grw->getGrowthSootRate(state);
        // this is not how the growth model is treated anywhere else and it doesn't really work that way in this setup

    if (out) {
        *out << "Kgrw (" << Kgrw.size() << ")" << endl;
        for (size_t i = 0; i < Kgrw.size(); i++)
            *out << i << ": " << Kgrw[i] << endl;
        *out << endl;
    }

    vector<double> Koxi(nsoot, 0);
    for (double& num : Koxi)
        num = oxi->getOxidationSootRate(state);
        // this is not how the oxidation model is treated anywhere else and it doesn't really work that way in this setup

    if (out) {
        *out << "Koxi (" << Koxi.size() << ")" << endl;
        for (size_t i = 0; i < Koxi.size(); i++)
            *out << i << ": " << Koxi[i] << endl;
        *out << endl;
    }

    vector<double> Coag(nsoot, 0);
    double leaving;
    vector<double> divided;
    for (size_t i = 0; i < Coag.size(); i++) {
        for (size_t j = 0; j < Coag.size(); j++) {
            leaving = 0.5 * coa->getCoagulationSootRate(state, absc[i], absc[j]) * wts[i] * wts[i];
            Coag[i] -= leaving;
            Coag[j] -= leaving;
            divided = getDivision((state.sootVar[i] + state.sootVar[j]), leaving, absc);
            for (size_t k = 0; k < Coag.size(); k++)
                Coag[k] += divided[k];
        }
    }

    if (out) {
        *out << "Coag (" << Coag.size() << ")" << endl;
        for (size_t i = 0; i < Coag.size(); i++)
            *out << i << ": " << Coag[i] << endl;
        *out << endl;
    }

    vector<double> N0(state.nsoot, 0);
    N0[0] = Jnuc;
    const double N_tot = Jnuc * state.cMin * gasSpMW[(int)gasSp::C] / Na;

    if (out) {
        *out << "N0 (" << N0.size() << ")" << endl;
        for (size_t i = 0; i < N0.size(); i++)
            *out << i << ": " << N0[i] << endl;
        *out << "N tot: " << N_tot << endl;
        *out << endl;
    }

    vector<double> Cnd0(state.nsoot, 0);
    double Cnd_tot = 0;
    if (sootModel.nuc->getMechanism() == nucleationMech::PAH) {
        for (size_t i = 0; i < Cnd0.size(); i++) {
            Cnd0[i] = state.getDimer() * state.getMDimer() * sootModel.coa->getCoagulationRate(state, state.getMDimer(), absc[i]) * wts[i];
            Cnd_tot += Cnd0[i] * absc[i];
        }
    }

    if (out) {
        *out << "Cnd0 (" << Cnd0.size() << ")" << endl;
        for (size_t i = 0; i < Cnd0.size(); i++)
            *out << i << ": " << Cnd0[i] << endl;
        *out << "Cnd tot: " << Cnd_tot << endl;
        *out << endl;
    }

    vector<double> Am2m3(nsoot, 0);
    for (size_t i = 0; i < nsoot; i++) {
        if (wts[i] > 0)
            Am2m3[i] = M_PI * pow(abs(6 / (M_PI * rhoSoot) * state.sootVar[i]), 2.0 / 3) * abs(wts[i]);
    }

    if (out) {
        *out << "Am2m3 (" << Am2m3.size() << ")" << endl;
        for (size_t i = 0; i < Am2m3.size(); i++)
            *out << i << ": " << Am2m3[i] << endl;
        *out << endl;
    }

    vector<double> G0(nsoot, 0);
    double G_tot = 0;
    double Ngrw;
    for (size_t i = 0; i < nsoot; i++) {
        if (i == 0)
            Ngrw = -Kgrw[i] * Am2m3[i] * wts[i] / (absc[i + 1] - absc[i]);
        else if (i == (nsoot - 1))
            Ngrw = Kgrw[i - 1] * Am2m3[i - 1] * wts[i - 1] / (absc[i] - absc[i - 1]);
        else
            Ngrw = Kgrw[i - 1] * Am2m3[i - 1] * wts[i - 1] / (absc[i] - absc[i - 1]) - Kgrw[i] * Am2m3[i] * wts[i] / (absc[i + 1] - absc[i]);

        G0[i] = Ngrw;
        G_tot += Ngrw * absc[i];
    }

    if (out) {
        *out << "G0 (" << G0.size() << ")" << endl;
        for (size_t i = 0; i < G0.size(); i++)
            *out << i << ": " << G0[i] << endl;
        *out << "G tot: " << G_tot << endl;
        *out << endl;
    }

    vector<double> X0(nsoot, 0);
    double X_tot = 0;
    double Noxi;
    for (size_t i = 0; i < nsoot; i++) {
        if (i == 0)
            Noxi = Koxi[i] * Am2m3[i] * wts[i] / (absc[i + 1] - absc[i]);
        else if (i == (nsoot - 1))
            Noxi = -Koxi[i - 1] * Am2m3[i - 1] * wts[i - 1] / (absc[i] - absc[i - 1]);
        else
            Noxi = -Koxi[i - 1] * Am2m3[i - 1] * wts[i - 1] / (absc[i] - absc[i - 1]) + Koxi[i] * Am2m3[i] * wts[i] / (absc[i + 1] - absc[i]);

        X0[i] = Noxi;
        X_tot += Noxi * absc[i];
    }

    if (out) {
        *out << "X0 (" << X0.size() << ")" << endl;
        for (size_t i = 0; i < X0.size(); i++)
            *out << i << ": " << X0[i] << endl;
        *out << "X tot: " << X_tot << endl;
        *out << endl;
    }

    vector<double>& C0 = Coag;

    vector<double> sootSourceTerms(nsoot, 0);
    for (size_t i = 0; i < sootSourceTerms.size(); i++)
        sootSourceTerms[i] = (N0[i] + Cnd0[i] + G0[i] + X0[i] + C0[i]) / rhoSoot;

    if (out) {
        *out << "Soot Source Terms (" << sootSourceTerms.size() << ")" << endl;
        for (size_t i = 0; i < sootSourceTerms.size(); i++)
            *out << i << ": " << sootSourceTerms[i] << endl;
        *out << endl;
    }

    // There was a commented out section in the old code here labeled "computer gas source terms", but it looked like it didn't do anything anymore

    //---------- set gas source terms

    map<gasSp, double> gasSourceTerms = sootChemistry.getGasSourceTerms(state, massRateRatios, N_tot, G_tot, X_tot, Cnd_tot);
    map<size_t, double> PAHSourceTerms = sootChemistry.getPAHSourceTerms(state, massRateRatios, N_tot, 0, X_tot, Cnd_tot);

    if (out) {
        *out << "Gas Source Terms (" << gasSourceTerms.size() << ")" << endl;
        for (const auto& [g, t] : gasSourceTerms)
            *out << (int) g << ": " << t << endl;
        *out << "PAH Source Terms (" << PAHSourceTerms.size() << ")" << endl;
        for (const auto& [s, t] : PAHSourceTerms)
            *out << s << ": " << t << endl;
        *out << endl;
    }


    return sourceTermStruct(sootSourceTerms, gasSourceTerms, PAHSourceTerms);
}

vector<double> psdModel_SECT::getDivision(double mass, double num, const vector<double>& absc) {
    size_t loc = 0;
    bool found = false;
    vector<double> toReturn(absc.size(), 0);
    while (!found) {
        loc++;
        if (loc >= absc.size()) {
            loc = absc.size() - 1;  // FIXME this is the problematic statement, it doesn't account for the scenario where the size of absc is 1 or less
            found = true;  // TODO I think it would probably be more appropriate to have a break here
        }
        if (absc[loc] > mass)
            found = true;
    }

    // FIXME it's possible for loc to be 0 here, which is a problem
    const double right = (mass - absc[loc - 1]) / (absc[loc] - absc[loc - 1]) * num;
    const double left = (absc[loc] - mass) / (absc[loc] - absc[loc - 1]) * num;
    toReturn[loc - 1] += left;
    toReturn[loc] += right;

    return toReturn;
}