#pragma once
#include "Model3D.h"

using namespace std;

class AMC
{
public:
	AMC();
	~AMC();

	vector<vector<float>> getAMCTemplate(pf::Model3D * model);
};

