////////////////////////////////////////////////////////////////////////////////
/*! PAH nucleation by Blanquart et al. (2009)
 *
 *      Rate from Blanquart & Pitsch (2009) article "A joint
 *      volume-surface-hydrogen multi-variate model for soot formation," ch. 27
 *      in Combustion Generated Fine Carbonaceous Particles ed. Bockhorn et al.
 *      Returns chemical nucleation rate in #/m3*s.
 *
 *      @param particleSizes        /input vector of soot particle sizes
 *      @param particleWeights      /input vector of soot particle weights
 *
 *
 */

#include "NucleationModel_PAH.h"
double soot::NucleationModel_PAH::getNucleationRate(MomentState& state,
                                                    const std::vector<double>& particleSizes,
                                                    const std::vector<double>& particleWeights) const
{

    // get dimer self-collision rate
	std::unique_ptr<CoagulationModel> coagModel = std::make_unique<CoagulationModel_FRENK>();
	const double beta_DD = coagModel->getCoagulationRate(state, state.getMDimer(), state.getMDimer());

	return 0.5 * beta_DD * state.getDimer() * state.getDimer();     // #/m3*s
}
