#ifndef OXIDATIONMODEL_LEE_NEOH_H
#define OXIDATIONMODEL_LEE_NEOH_H

#include "sootlib/oxidation_model/OxidationModel.h"

namespace soot
{
class OxidationModel_LEE_NEOH : public OxidationModel
{
public:
	OxidationModel_LEE_NEOH() : OxidationModel(OxidationMechanism::LEE_NEOH) {}

	[[nodiscard]] double getOxidationRate(const MomentState& state, MassRateRatio& ratio) const override;

	~OxidationModel_LEE_NEOH() override = default;
};
}

#endif //OXIDATIONMODEL_LEE_NEOH_H
