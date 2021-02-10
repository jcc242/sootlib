#include "BinSootModelFactory.h"
soot::BinSootModelFactory::BinSootModelFactory()
{
	model = BinModel::SECT;
}
void soot::BinSootModelFactory::setModel(soot::BinModel model)
{
	this->model = model;
}
soot::BinSootModel* soot::BinSootModelFactory::getModel() const
{
	/* create helper models */
	std::unique_ptr<CoagulationModel> cm;
	std::unique_ptr<GrowthModel> gm;
	std::unique_ptr<NucleationModel> nm;
	std::unique_ptr<OxidationModel> om;

	switch (model)
	{
		case BinModel::SECT:
			return SootModel_SECT::getInstance(std::move(cm),
									  std::move(gm),
									  std::move(nm),
									  std::move(om));
		default:
			throw std::domain_error("Bad soot model type");
	}
}
std::unique_ptr<soot::BinSootModel> soot::BinSootModelFactory::getModelUnique() const
{
	return std::unique_ptr<BinSootModel>(getModel());
}
std::shared_ptr<soot::BinSootModel> soot::BinSootModelFactory::getModelShared() const
{
	return std::shared_ptr<BinSootModel>(getModel());
}
