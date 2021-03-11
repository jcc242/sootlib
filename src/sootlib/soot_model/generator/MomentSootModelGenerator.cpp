#include "MomentSootModelGenerator.h"
void soot::MomentSootModelGenerator::setModel(soot::MomentModel model)
{
	this->model = model;
}
soot::MomentSootModel* soot::MomentSootModelGenerator::getModel() const
{
	/* create helper models */
	std::unique_ptr<CoagulationModel> cm = getCoagulationModel();
	std::unique_ptr<GrowthModel> gm = getGrowthModel();
	std::unique_ptr<NucleationModel> nm = getNucleationModel();
	std::unique_ptr<OxidationModel> om = getOxidationModel();

	/* create and return model ptr */
	switch (model)
	{
		case MomentModel::MONO:
			return SootModel_MONO::getInstance(std::move(cm),
			                                   std::move(gm),
			                                   std::move(nm),
			                                   std::move(om));
		case MomentModel::LOGN:
			return SootModel_LOGN::getInstance(std::move(cm),
			                                   std::move(gm),
			                                   std::move(nm),
			                                   std::move(om));
		case MomentModel::MOMIC:
			return SootModel_MOMIC::getInstance(std::move(cm),
			                                    std::move(gm),
			                                    std::move(nm),
			                                    std::move(om));
		case MomentModel::QMOM:
			return SootModel_QMOM::getInstance(std::move(cm),
			                                   std::move(gm),
			                                   std::move(nm),
			                                   std::move(om));
		default:
			throw std::domain_error("Bad soot model type");
	}
}
std::unique_ptr<soot::MomentSootModel> soot::MomentSootModelGenerator::getModelUnique() const
{
	return std::unique_ptr<MomentSootModel>(getModel());
}
std::shared_ptr<soot::MomentSootModel> soot::MomentSootModelGenerator::getModelShared() const
{
	return std::shared_ptr<MomentSootModel>(getModel());
}
soot::MomentSootModelGenerator::MomentSootModelGenerator()
{
	model = MomentModel::MONO;
}
