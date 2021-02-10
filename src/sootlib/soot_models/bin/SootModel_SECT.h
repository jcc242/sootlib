#ifndef SOOTMODEL_SECT_H
#define SOOTMODEL_SECT_H

#include <memory>

#include "sootlib/SootSourceTerms.h"
#include "sootlib/BinSootModel.h"
#include "sootlib/state/soot/BinSootState.h"
#include "sootlib/state/gas/GasState.h"

namespace soot
{
class SootModel_SECT : public BinSootModel
{
public:
	[[nodiscard]] static SootModel_SECT* getInstance(std::unique_ptr<CoagulationModel> coagulationModel,
	                                                 std::unique_ptr<GrowthModel> growthModel,
	                                                 std::unique_ptr<NucleationModel> nucleationModel,
	                                                 std::unique_ptr<OxidationModel> oxidationModel);

	[[nodiscard]] SootSourceTerms getSourceTerms(const GasState& gasState, const BinSootState& binSootState) const override;

	~SootModel_SECT() override = default;

private:
	SootModel_SECT(std::unique_ptr<CoagulationModel> coagulationModel,
	               std::unique_ptr<GrowthModel> growthModel,
	               std::unique_ptr<NucleationModel> nucleationModel,
	               std::unique_ptr<OxidationModel> oxidationModel);
};
}

#endif //SOOTMODEL_SECT_H
