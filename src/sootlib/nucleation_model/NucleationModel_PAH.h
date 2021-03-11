#ifndef NUCLEATIONMODEL_PAH_H
#define NUCLEATIONMODEL_PAH_H

#include <memory>

#include "sootlib/nucleation_model/NucleationModel.h"
#include "sootlib/coagulation_model/CoagulationModel_FRENK.h"

namespace soot
{
class NucleationModel_PAH : public NucleationModel
{
public:
	NucleationModel_PAH() : NucleationModel(NucleationMechanism::PAH) {}

	[[nodiscard]] double getNucleationRate(MomentState& state, const std::vector<double>& particleSizes, const std::vector<double>& particleWeights, MassRateRatio& ratio) const override;

	~NucleationModel_PAH() override = default;
};
}

#endif //NUCLEATIONMODEL_PAH_H
