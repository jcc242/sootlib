#ifndef COAGULATIONMODEL_LL_H
#define COAGULATIONMODEL_LL_H

#include "sootlib/coagulation_model/CoagulationModel.h"

namespace soot
{
class CoagulationModel_LL : public CoagulationModel
{
public:
	CoagulationModel_LL() : CoagulationModel(CoagulationMechanism::LL) {}

	[[nodiscard]] double getCoagulationRate(const MomentState& state, double m1, double m2) const override;

	~CoagulationModel_LL() override = default;
};
}

#endif //COAGULATIONMODEL_LL_H
