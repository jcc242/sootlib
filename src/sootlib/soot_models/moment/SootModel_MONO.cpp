#include "SootModel_MONO.h"
soot::SootModel_MONO::SootModel_MONO(std::unique_ptr<CoagulationModel> coagulationModel,
                                     std::unique_ptr<GrowthModel> growthModel,
                                     std::unique_ptr<NucleationModel> nucleationModel,
                                     std::unique_ptr<OxidationModel> oxidationModel,
                                     size_t numSootVars)
	: SootModel_Base(std::move(coagulationModel),
	                 std::move(growthModel),
	                 std::move(nucleationModel),
	                 std::move(oxidationModel),
	                 numSootVars)
{
}
soot::SootModel_MONO* soot::SootModel_MONO::getInstance(std::unique_ptr<CoagulationModel> coagulationModel,
                                                        std::unique_ptr<GrowthModel> growthModel,
                                                        std::unique_ptr<NucleationModel> nucleationModel,
                                                        std::unique_ptr<OxidationModel> oxidationModel,
                                                        size_t numSootVars)
{
	return new SootModel_MONO(std::move(coagulationModel),
	                          std::move(growthModel),
	                          std::move(nucleationModel),
	                          std::move(oxidationModel),
	                          numSootVars);
}
soot::SootSourceTerms soot::SootModel_MONO::getSourceTerms(const soot::GasState& gasState, const MomentSootState& sootState)
{
	return SootSourceTerms({});
}