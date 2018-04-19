#include "AMC.h"

AMC::AMC()
{
}


AMC::~AMC()
{
}

vector<vector<float>> AMC::getAMCTemplate(pf::Model3D * model) {
	vector<vector<float>> modelState;
	pf::MotionData::loadStateVectorFromAMC("ETC/modelTemplate.amc", model->getNamesMovingBones(), modelState);
	return modelState;
}