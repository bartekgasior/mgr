#include "ModelHandler.h"

ModelHandler::ModelHandler()
{
}

ModelHandler::~ModelHandler()
{
}

void ModelHandler::initializeBonesRotationsMap(map<string, pf::Vec3f> &bonesRotations, vector<pf::ASFBone> asfBones) {
	bonesRotations.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		pf::Vec3f vec(0, 0, 0);
		bonesRotations[asfBones[i].name] = vec;
	}
}

void ModelHandler::initializeBonesLengthMap(map<string, float> &bonesLength, vector<pf::ASFBone> asfBones) {
	bonesLength.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		float length = asfBones[i].length;
		bonesLength[asfBones[i].name] = length;
	}
}

void ModelHandler::initializeBonesVelocityMap(map<string, vector<float>> &modelVelocity, vector<pf::ASFBone> asfBones) {
	modelVelocity.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		string str = asfBones[i].name;
		vector<float> velocityTmp;
		if (str == "root") {
			velocityTmp.push_back(160);
			velocityTmp.push_back(160);
			velocityTmp.push_back(120);
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftUpLeg_dum") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightUpLeg_dum") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine_dum") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftUpLeg") {
			velocityTmp.push_back(32);
			velocityTmp.push_back(0);
			velocityTmp.push_back(10);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftLeg") {
			velocityTmp.push_back(25);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftFoot") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftToeBase") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightUpLeg") {
			velocityTmp.push_back(32);
			velocityTmp.push_back(0);
			velocityTmp.push_back(10);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightLeg") {
			velocityTmp.push_back(25);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightFoot") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightToeBase") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine1") {
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine1_dum1") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine1_dum2") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Spine1_dum3") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Neck") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "Head") {
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			velocityTmp.push_back(10);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftShoulder") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftArm") {
			velocityTmp.push_back(25);
			velocityTmp.push_back(15);
			velocityTmp.push_back(25);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftForeArm") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(15);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftHand") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftHand_dum1") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftHand_dum2") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "LeftHandThumb") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightShoulder") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightArm") {
			velocityTmp.push_back(25);
			velocityTmp.push_back(15);
			velocityTmp.push_back(25);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightForeArm") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(15);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightHand") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightHand_dum1") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightHand_dum2") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else if (str == "RightHandThumb") {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		else {
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			velocityTmp.push_back(0);
			modelVelocity[str] = velocityTmp;
		}
		//std::string().swap(str);
		//vector<int>().swap(velocityTmp);
	}
}

void ModelHandler::initializeBonesLimitsMap(map<string, vector<int>> &modelLimits, vector<pf::ASFBone> asfBones) {
	modelLimits.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		string str = asfBones[i].name;
		vector<int> limitsTMP;
		if (str == "root") {
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(-360);
			limitsTMP.push_back(360);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftUpLeg_dum") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightUpLeg_dum") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine_dum") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftUpLeg") {
			limitsTMP.push_back(-95);
			limitsTMP.push_back(50);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(-35);
			limitsTMP.push_back(50);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftLeg") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(140);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftFoot") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftToeBase") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightUpLeg") {
			limitsTMP.push_back(-95);
			limitsTMP.push_back(50);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(-50);
			limitsTMP.push_back(35);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightLeg") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(140);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightFoot") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightToeBase") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine1") {
			limitsTMP.push_back(-15);
			limitsTMP.push_back(25);
			limitsTMP.push_back(-25);
			limitsTMP.push_back(25);
			limitsTMP.push_back(-25);
			limitsTMP.push_back(25);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine1_dum1") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine1_dum2") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Spine1_dum3") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Neck") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "Head") {
			limitsTMP.push_back(-50);
			limitsTMP.push_back(95);
			limitsTMP.push_back(-95);
			limitsTMP.push_back(95);
			limitsTMP.push_back(-50);
			limitsTMP.push_back(50);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftShoulder") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftArm") {
			limitsTMP.push_back(-150);
			limitsTMP.push_back(180);
			limitsTMP.push_back(-150);
			limitsTMP.push_back(180);
			limitsTMP.push_back(-150);
			limitsTMP.push_back(180);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftForeArm") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(-140);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftHand") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftHand_dum1") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftHand_dum2") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "LeftHandThumb") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightShoulder") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightArm") {
			limitsTMP.push_back(-180);
			limitsTMP.push_back(150);
			limitsTMP.push_back(-180);
			limitsTMP.push_back(150);
			limitsTMP.push_back(-180);
			limitsTMP.push_back(150);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightForeArm") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(140);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightHand") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightHand_dum1") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightHand_dum2") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else if (str == "RightHandThumb") {
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			limitsTMP.push_back(0);
			modelLimits[str] = limitsTMP;
		}
		else {
			limitsTMP.push_back(360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(360);
			limitsTMP.push_back(360);
			modelLimits[str] = limitsTMP;
		}
		//std::string().swap(str);
		//vector<int>().swap(limitsTMP);
	}
}

void ModelHandler::initializeBonesDOFMap(map<string, vector<bool>> &modelDOF, vector<pf::ASFBone> asfBones) {
	modelDOF.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		string str = asfBones[i].name;
		vector<bool> dofTMP;
		if (str == "root") {
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftUpLeg_dum") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightUpLeg_dum") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine_dum") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftUpLeg") {
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftLeg") {
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftFoot") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;;
		}
		else if (str == "LeftToeBase") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightUpLeg") {
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightLeg") {
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightFoot") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightToeBase") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine1") {
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine1_dum1") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine1_dum2") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Spine1_dum3") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Neck") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "Head") {
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftShoulder") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftArm") {
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftForeArm") {
			dofTMP.push_back(0);
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftHand") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftHand_dum1") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftHand_dum2") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "LeftHandThumb") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightShoulder") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightArm") {
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			dofTMP.push_back(1);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightForeArm") {
			dofTMP.push_back(0);
			dofTMP.push_back(1);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightHand") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightHand_dum1") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightHand_dum2") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else if (str == "RightHandThumb") {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		else {
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			dofTMP.push_back(0);
			modelDOF[str] = dofTMP;
		}
		//vector<bool>().swap(dofTMP);
		//std::string(str).swap(str);
	}
}

void ModelHandler::initializeBonesRadiusMap(map<string, vector<float>> &modelRadius, vector<pf::ASFBone> asfBones, vector<pf::boneGeometry> bonesGeometry) {
	modelRadius.clear();
	for (int i = 0; i < asfBones.size(); i++) {
		vector<float> radiusTmp;
		bool found = false;
		string str = asfBones[i].name;
		for (int j = 0; j < bonesGeometry.size(); j++) {
			if (str == bonesGeometry[j].name) {
				radiusTmp.push_back(bonesGeometry[j].topRadius1);
				radiusTmp.push_back(bonesGeometry[j].topRadius2);
				radiusTmp.push_back(bonesGeometry[j].bottomRadius1);
				radiusTmp.push_back(bonesGeometry[j].bottomRadius2);
				found = true;
				break;
			}
		}
		if (!found) {
			radiusTmp.push_back(20);
			radiusTmp.push_back(20);
			radiusTmp.push_back(20);
			radiusTmp.push_back(20);
		}

		modelRadius[str] = radiusTmp;
	}
}

void ModelHandler::clearBonesLengthMap(map<string, float> &bonesLength) {
	for (auto& p : bonesLength) {
		p.second = 0;
	}
}

void ModelHandler::updateLength(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float direction, float value) {
	bool found = false;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (bonesGeometry[i].name == boneName) {

			bonesGeometry[i].length += value*direction;

			if (bonesGeometry[i].length < 0)
				bonesGeometry[i].length = 0;

			model->updateBoneGeometry(boneName, bonesGeometry[i]);
			//model->updateBoneLength(boneName, bonesGeometry[i].length);
			found = true;
			break;
		}
	}
	if (!found)
		throw MyException("DAT file is incompatbile with ASF file");
}

void ModelHandler::updateLength(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float value) {
	bool found = false;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (bonesGeometry[i].name == boneName) {

			bonesGeometry[i].length = value;

			if (bonesGeometry[i].length < 0)
				bonesGeometry[i].length = 0;

			model->updateBoneGeometry(boneName, bonesGeometry[i]);
			//model->updateBoneLength(boneName, bonesGeometry[i].length);
			found = true;
			break;
		}
	}
	if (!found)
		throw MyException("DAT file is incompatbile with ASF file");
}

void ModelHandler::updateTopRadius(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float direction, float value) {
	bool found = false;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (bonesGeometry[i].name == boneName) {

			bonesGeometry[i].topRadius1 += value*direction;
			bonesGeometry[i].topRadius2 += value*direction;

			if (bonesGeometry[i].topRadius1 < 0)
				bonesGeometry[i].topRadius1 = 0;

			if (bonesGeometry[i].topRadius2 < 0)
				bonesGeometry[i].topRadius2 = 0;

			model->updateBoneGeometry(boneName, bonesGeometry[i]);
			found = true;
			break;
		}
	}
	if (!found)
		throw MyException("DAT file is incompatbile with ASF file");
}

void ModelHandler::updateTopRadius(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float value) {
	bool found = false;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (bonesGeometry[i].name == boneName) {

			bonesGeometry[i].topRadius1 = value;
			bonesGeometry[i].topRadius2 = value;

			if (bonesGeometry[i].topRadius1 < 0)
				bonesGeometry[i].topRadius1 = 0;

			if (bonesGeometry[i].topRadius2 < 0)
				bonesGeometry[i].topRadius2 = 0;

			model->updateBoneGeometry(boneName, bonesGeometry[i]);
			found = true;
			break;
		}
	}
	if (!found)
		throw MyException("DAT file is incompatbile with ASF file");
}

void ModelHandler::updateBottomRadius(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float direction, float value) {
	bool found = false;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (bonesGeometry[i].name == boneName) {

			bonesGeometry[i].bottomRadius1 += value*direction;
			bonesGeometry[i].bottomRadius2 += value*direction;

			if (bonesGeometry[i].bottomRadius1 < 0)
				bonesGeometry[i].bottomRadius1 = 0;

			if (bonesGeometry[i].bottomRadius2 < 0)
				bonesGeometry[i].bottomRadius2 = 0;

			model->updateBoneGeometry(boneName, bonesGeometry[i]);
			found = true;
			break;
		}
	}
	if (!found)
		throw MyException("DAT file is incompatbile with ASF file");
}

void ModelHandler::updateBottomRadius(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float value) {
	bool found = false;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (bonesGeometry[i].name == boneName) {

			bonesGeometry[i].bottomRadius1 = value;
			bonesGeometry[i].bottomRadius2 = value;

			if (bonesGeometry[i].bottomRadius1 < 0)
				bonesGeometry[i].bottomRadius1 = 0;

			if (bonesGeometry[i].bottomRadius2 < 0)
				bonesGeometry[i].bottomRadius2 = 0;

			model->updateBoneGeometry(boneName, bonesGeometry[i]);
			found = true;
			break;
		}
	}
	if (!found)
		throw MyException("DAT file is incompatbile with ASF file");
}

float ModelHandler::getBoneLength(vector<pf::boneGeometry> bonesGeometry, string boneName) {
	float res = 0;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (boneName == bonesGeometry[i].name) {
			res = bonesGeometry[i].length;
			break;
		}
	}
	return res;
}

float ModelHandler::getBoneTopRadius(vector<pf::boneGeometry> bonesGeometry, string boneName) {
	float res = 0;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (boneName == bonesGeometry[i].name) {
			res = bonesGeometry[i].topRadius1;
			break;
		}
	}
	return res;
}

float ModelHandler::getBoneBottomRadius(vector<pf::boneGeometry> bonesGeometry, string boneName) {
	float res = 0;
	for (int i = 0; i < bonesGeometry.size(); i++) {
		if (boneName == bonesGeometry[i].name) {
			res = bonesGeometry[i].bottomRadius1;
			break;
		}
	}
	return res;
}

int ModelHandler::getModelStateID(vector <string> usedBonesNames, string boneName, pf::Vec3 vect) {
	int result;
	//vector <string> usedBonesNames = model->getNamesMovingBones();
	for (int i = 0; i < usedBonesNames.size(); i++) {
		if (boneName == usedBonesNames[i]) {
			result = i + 3;
			break;
		}
	}
	if (vect == pf::Model3D::axisX)
		return result;
	else if (vect == pf::Model3D::axisY)
		return result + 1;
	else if (vect == pf::Model3D::axisZ)
		return result + 2;

}

void ModelHandler::setLimitsVector(vector<pf::range2> &limits, vector<pf::boneConfig> bones, vector<pf::boneGeometry> bonesGeometry) {
	limits.clear();
	pf::range2 r;
	r.min = -360;
	r.max = 360;
	/*root ma 6x -360;360*/
	limits.push_back(r);
	limits.push_back(r);
	limits.push_back(r);
	limits.push_back(r);
	limits.push_back(r);
	limits.push_back(r);
	for (int j = 0; j < bonesGeometry.size(); j++) {
		for (int i = 0; i < bones.size(); i++) {
			if (bonesGeometry[j].name == bones[i].name) {
				//cout << bones[i].name << endl;

				pf::range2 rangeX, rangeY, rangeZ;
				rangeX.min = bones[i].minRotX;
				rangeX.max = bones[i].maxRotX;

				rangeY.min = bones[i].minRotY;
				rangeY.max = bones[i].maxRotY;

				rangeZ.min = bones[i].minRotZ;
				rangeZ.max = bones[i].maxRotZ;

				limits.push_back(rangeX);
				limits.push_back(rangeY);
				limits.push_back(rangeZ);

				//cout << rangeX.min << " " << rangeX.max << endl;
				//cout << rangeY.min << " " << rangeY.max << endl;
				//cout << rangeZ.min << " " << rangeZ.max << endl;
				break;
			}
		}
	}
}

void ModelHandler::setVelocityVector(vector<float> &velocity, vector<pf::boneConfig> bones, vector<pf::boneGeometry> bonesGeometry) {
	velocity.clear();
	velocity.push_back(160);
	velocity.push_back(160);
	velocity.push_back(120);
	velocity.push_back(10);
	velocity.push_back(10);
	velocity.push_back(10);

	for (int j = 0; j < bonesGeometry.size(); j++) {
		for (int i = 0; i < bones.size(); i++) {
			if (bonesGeometry[j].name == bones[i].name) {
				//cout << bones[i].name << endl;

				float vX, vY, vZ;
				vX = bones[i].vRotX;
				vY = bones[i].vRotY;
				vZ = bones[i].vRotZ;

				velocity.push_back(vX);
				velocity.push_back(vY);
				velocity.push_back(vZ);

				break;
			}
		}
	}
}

void ModelHandler::saveModelStateToMap(map<string, pf::Vec3f> &bonesRotations, vector<float> mState, vector<pf::boneConfig> bonesConfig) {
	try {
		for (int i = 0; i < bonesConfig.size(); i++) {
			int j = i + 1;
			pf::Vec3f vec(mState[j * 3], mState[j * 3 + 1], mState[j * 3 + 2]);
			bonesRotations.at(bonesConfig[i].name) = vec;
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

void ModelHandler::saveModelStateToMap(map<string, pf::Vec3f> &bonesRotations, vector<float> mState, vector<string> allBones) {
	try {
		for (int i = 0; i < allBones.size(); i++) {
			int j = i + 1;
			pf::Vec3f vec(mState[j * 3], mState[j * 3 + 1], mState[j * 3 + 2]);
			bonesRotations.at(allBones[i]) = vec;
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

void ModelHandler::saveBonesLengthToMap(map<string, float> &bonesLength, vector<pf::boneGeometry> boneGeometry) {
	try {
		for (int i = 0; i < boneGeometry.size(); i++) {
			bonesLength.at(boneGeometry[i].name) = boneGeometry[i].length;
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

void ModelHandler::updateBonesLengthMap(map<string, float> &bonesLength, string boneName, float value) {
	try {
		bonesLength.at(boneName) = value;
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

void ModelHandler::saveRadiusToMap(map<string, vector<float>> &bonesRadius, vector<pf::boneGeometry> boneGeometry) {
	try {
		for (int i = 0; i < boneGeometry.size(); i++) {
			vector<float> tmp;
			tmp.push_back(boneGeometry[i].topRadius1);
			tmp.push_back(boneGeometry[i].topRadius2);
			tmp.push_back(boneGeometry[i].bottomRadius1);
			tmp.push_back(boneGeometry[i].bottomRadius2);
			bonesRadius.at(boneGeometry[i].name) = tmp;
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

void ModelHandler::saveDOFToMap(map<string, vector<bool>> &bonesDOF, vector<pf::boneConfig> boneConfig) {
	try {
		for (int i = 0; i < boneConfig.size(); i++) {
			vector<bool> tmp;
			if (boneConfig[i].isRotX)
				tmp.push_back(1);
			else tmp.push_back(0);

			if (boneConfig[i].isRotY)
				tmp.push_back(1);
			else tmp.push_back(0);

			if (boneConfig[i].isRotZ)
				tmp.push_back(1);
			else tmp.push_back(0);

			bonesDOF.at(boneConfig[i].name) = tmp;
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

void ModelHandler::saveLimitsToMap(map<string, vector<int>> &modelLimits, vector<pf::boneConfig> boneConfig) {
	try {
		for (int i = 0; i < boneConfig.size(); i++) {
			vector<int> tmp;
			if (boneConfig[i].name == "root") {
				tmp.push_back(-360);
				tmp.push_back(360);
				tmp.push_back(-360);
				tmp.push_back(360);
				tmp.push_back(-360);
				tmp.push_back(360);
				tmp.push_back(-360);
				tmp.push_back(360);
				tmp.push_back(-360);
				tmp.push_back(360);
				tmp.push_back(-360);
				tmp.push_back(360);
				modelLimits.at(boneConfig[i].name) = tmp;
			}
			else {
				tmp.push_back(boneConfig[i].minRotX);
				tmp.push_back(boneConfig[i].maxRotX);
				tmp.push_back(boneConfig[i].minRotY);
				tmp.push_back(boneConfig[i].maxRotY);
				tmp.push_back(boneConfig[i].minRotZ);
				tmp.push_back(boneConfig[i].maxRotZ);
				modelLimits.at(boneConfig[i].name) = tmp;
			}
			//vector<int>().swap(tmp);
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

void ModelHandler::saveVelocityToMap(map<string, vector<float>> &modelVelocity, vector<pf::boneConfig> boneConfig) {
	try {
		for (int i = 0; i < boneConfig.size(); i++) {
			vector<float> tmp;
			if (boneConfig[i].name == "root") {
				tmp.push_back(160);
				tmp.push_back(160);
				tmp.push_back(120);
				tmp.push_back(10);
				tmp.push_back(10);
				tmp.push_back(10);
				modelVelocity.at(boneConfig[i].name) = tmp;
			}
			else {
				tmp.push_back(boneConfig[i].vRotX);
				tmp.push_back(boneConfig[i].vRotY);
				tmp.push_back(boneConfig[i].vRotZ);
				modelVelocity.at(boneConfig[i].name) = tmp;
			}
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

void ModelHandler::updateModelStateFromMap(vector<vector<float>> &mState, vector<map<string, pf::Vec3f>> bonesRotations, vector<pf::boneConfig> bonesConfig, vector<vector<float>> modelTranslation) {
	try {
		mState.clear();
		for (int j = 0; j < bonesRotations.size(); j++) {
			vector<float> newModelState;

			newModelState.push_back(modelTranslation[j][0]);
			newModelState.push_back(modelTranslation[j][1]); // ############################# przesuniecie root
			newModelState.push_back(modelTranslation[j][2]);

			for (int i = 0; i < bonesConfig.size(); i++) {
				pf::Vec3f vec = bonesRotations[j].at(bonesConfig[i].name);
				//cout << vec.x() << " " << vec.y() << " " << vec.z() << endl;
				newModelState.push_back(vec.x());
				newModelState.push_back(vec.y());
				newModelState.push_back(vec.z());
			}
			mState.push_back(newModelState);
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

void ModelHandler::updateBonesLengthFromMap(map<string, float> bonesLength, vector<pf::boneGeometry> &boneGeometry) {
	for (int i = 0; i < boneGeometry.size(); i++) {
		auto it = bonesLength.find(boneGeometry[i].name);
		if (it != bonesLength.end())
			boneGeometry[i].length = bonesLength.find(boneGeometry[i].name)->second;
	}
}

void ModelHandler::updateBonesRadiusFromMap(map<string, vector<float>> bonesRadius, vector<pf::boneGeometry> &boneGeometry) {
	for (int i = 0; i < boneGeometry.size(); i++) {
		auto it = bonesRadius.find(boneGeometry[i].name);
		if (it != bonesRadius.end()) {
			boneGeometry[i].topRadius1 = bonesRadius.find(boneGeometry[i].name)->second[0];
			boneGeometry[i].topRadius2 = bonesRadius.find(boneGeometry[i].name)->second[1];
			boneGeometry[i].bottomRadius1 = bonesRadius.find(boneGeometry[i].name)->second[2];
			boneGeometry[i].bottomRadius2 = bonesRadius.find(boneGeometry[i].name)->second[3];
		}
	}
}

void ModelHandler::updateBonesDOFMap(map<string, vector<bool>> &bonesDOF, string boneName, vector<bool> dofs) {
	try {
		bonesDOF.at(boneName) = dofs;
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

void ModelHandler::updateDOFFromMap(map<string, vector<bool>> bonesDOF, vector<pf::boneConfig> &bonesConfig) {
	for (int i = 0; i < bonesConfig.size(); i++) {
		auto it = bonesDOF.find(bonesConfig[i].name);
		if (it != bonesDOF.end()) {
			vector<bool> dof;
			dof = bonesDOF.find(bonesConfig[i].name)->second;
			if (dof[0])
				bonesConfig[i].isRotX = true;
			else
				bonesConfig[i].isRotX = false;

			if (dof[1])
				bonesConfig[i].isRotY = true;
			else
				bonesConfig[i].isRotY = false;

			if (dof[2])
				bonesConfig[i].isRotZ = true;
			else
				bonesConfig[i].isRotZ = false;

		}
		//vector<bool>().swap(dof);
	}
}

void ModelHandler::updateLimitsFromMap(map<string, vector<int>> modelLimits, vector<pf::boneConfig> &bonesConfig) {
	for (int i = 0; i < bonesConfig.size(); i++) {
		auto it = modelLimits.find(bonesConfig[i].name);
		if (it != modelLimits.end()) {
			vector<int> limits = modelLimits.find(bonesConfig[i].name)->second;

			bonesConfig[i].minRotX = limits[0];
			bonesConfig[i].maxRotX = limits[1];
			bonesConfig[i].minRotY = limits[2];
			bonesConfig[i].maxRotY = limits[3];
			bonesConfig[i].minRotZ = limits[4];
			bonesConfig[i].maxRotZ = limits[5];
		}
	}
}

void ModelHandler::updateVelocityFromMap(map<string, vector<float>> modelVelocity, vector<pf::boneConfig> &bonesConfig) {
	for (int i = 0; i < bonesConfig.size(); i++) {
		auto it = modelVelocity.find(bonesConfig[i].name);
		if (it != modelVelocity.end()) {
			vector<float> velocity = modelVelocity.find(bonesConfig[i].name)->second;

			bonesConfig[i].vRotX = velocity[0];
			bonesConfig[i].vRotY = velocity[1];
			bonesConfig[i].vRotZ = velocity[2];
		}
	}
}

void ModelHandler::setGoldenRatio(map<string, float> &bonesLength, float height) {
	/*wykorzystywane*/
	float head;// = bonesLength.find("Head")->second;
	float neck;// = bonesLength.find("Neck")->second;
	float spine1_dum1;// = bonesLength.find("Spine1_dum1")->second;
	//float spine1 = bonesLength.find("Spine1")->second;
	float spine;// = bonesLength.find("Spine")->second;
	float leftUpLeg_dum;// = bonesLength.find("LeftUpLeg_dum")->second;
	float leftUpLeg;//= bonesLength.find("LeftUpLeg")->second;
	float leftLeg;//= bonesLength.find("LeftLeg")->second;
	float rightUpLeg_dum;// = bonesLength.find("RightUpLeg_dum")->second;
	float rightUpLeg;// = bonesLength.find("RightUpLeg")->second;
	float rightLeg;// = bonesLength.find("RightLeg")->second;
	float leftShoulder;// = bonesLength.find("LeftShoulder")->second;
	float leftArm;// = bonesLength.find("LeftArm")->second;
	float leftForeArm;// = bonesLength.find("LeftForeArm")->second;
	float leftHand_dum2;// = bonesLength.find("LeftHand_dum2")->second;
	float rightShoulder;// = bonesLength.find("RightShoulder")->second;
	float rightArm;// = bonesLength.find("RightArm")->second;
	float rightForeArm;//= bonesLength.find("RightForeArm")->second;
	float rightHand_dum2;// = bonesLength.find("RightHand_dum2")->second;
	float rightFoot;// = bonesLength.find("RightFoot")->second;
	float leftFoot;//= bonesLength.find("LeftFoot")->second;
	/*puste*/
	float spine_dum;// = bonesLength.find("Spine_dum")->second;
	float leftToeBase;//= bonesLength.find("LeftToeBase")->second;
	float rightToeBase;// = bonesLength.find("RightToeBase")->second;
	float spine1;// = bonesLength.find("Spine1")->second;
	//float spine1_dum1 = bonesLength.find("Spine1_dum1")->second;
	float spine1_dum2;// = bonesLength.find("Spine1_dum2")->second;
	float spine1_dum3;// = bonesLength.find("Spine1_dum3")->second;
	float leftHand;//= bonesLength.find("LeftHand")->second;
	float leftHand_dum1;// = bonesLength.find("LeftHand_dum1")->second;
	float leftHandThumb;// = bonesLength.find("LeftHandThumb")->second;
	float rightHand;// = bonesLength.find("RightHand")->second;
	float rightHand_dum1;// = bonesLength.find("RightHand_dum1")->second;
	float rightHandThumb;// = bonesLength.find("RightHandThumb")->second;

	/*float height, width;

	if ((leftUpLeg + leftLeg) > (rightUpLeg + rightLeg))
		height = leftUpLeg + leftLeg + spine + spine1_dum1 + spine1 + neck + head;
	else 
		height = rightUpLeg + rightLeg + spine + spine1_dum1 + spine1 + neck + head;

	if (rightHand_dum2 + rightForeArm + rightArm + rightShoulder + leftHand_dum2 + leftForeArm + leftArm + leftShoulder > rightUpLeg_dum + leftUpLeg_dum)
		width = rightHand_dum2 + rightForeArm + rightArm + rightShoulder + leftHand_dum2 + leftForeArm + leftArm + leftShoulder;
	else
		width = rightUpLeg_dum + leftUpLeg_dum;
		*/
	/*wysokosc*/
	float mh, eh;
	eh = height / (GOLDEN_RATIO + 1);
	mh = GOLDEN_RATIO * eh; // lub height - eh

	/*dolna czesc modelu*/
	float m1h, m2h, e1h, e2h;
	e1h = mh / (GOLDEN_RATIO + 1);
	m1h = mh - e1h;
	e2h = m1h / (GOLDEN_RATIO + 1);
	m2h = m1h - e2h;

	/*gorna czesc modelu*/
	float m3h, m4h, m5h, e3h, e4h, e5h, e6h;
	e3h = eh / (GOLDEN_RATIO + 1);
	m3h = eh - e3h;
	e4h = e3h / (GOLDEN_RATIO + 1);
	m4h = e3h - e4h;
	e5h = m4h / (GOLDEN_RATIO + 1);
	m5h = m4h - e5h;
	e6h = m5h / (GOLDEN_RATIO + 1);

	/*szerokosc*/
	float m7w, m8w, m9w, e7w, e8w, e9w;
	e7w = m2h / (GOLDEN_RATIO + 1);
	m7w = m2h - e7w;
	e8w = m3h / (GOLDEN_RATIO + 1);
	m8w = m3h - e8w;
	e9w = m8w / (GOLDEN_RATIO + 1);
	m9w = m8w - e9w;

	head = e3h - e6h;
	neck = e6h;
	spine1_dum1 = m3h;

	leftUpLeg = rightUpLeg = e2h + e7w;
	leftLeg = rightLeg = e1h;
	spine = m7w;

	rightHand_dum2 = leftHand_dum2 = leftFoot = rightFoot = e7w;
	leftForeArm = rightForeArm = m7w;
	leftArm = rightArm = m3h;
	leftShoulder = rightShoulder = e8w;
	leftUpLeg_dum = rightUpLeg_dum = m9w;

	//width = rightHand_dum2 + rightForeArm + rightArm + rightShoulder + leftHand_dum2 + leftForeArm + leftArm + leftShoulder;
	//cout << width << endl;
	//cout << endl;
	try {
		/*zapis do mapy*/
		bonesLength.at("Head") = head;
		bonesLength.at("Neck") = neck;
		bonesLength.at("Spine1_dum1") = spine1_dum1;
		bonesLength.at("Spine") = spine;
		bonesLength.at("LeftUpLeg_dum") = leftUpLeg_dum;
		bonesLength.at("LeftUpLeg") = leftUpLeg;
		bonesLength.at("LeftLeg") = leftLeg;
		bonesLength.at("RightUpLeg_dum") = rightUpLeg_dum;
		bonesLength.at("RightUpLeg") = rightUpLeg;
		bonesLength.at("RightLeg") = rightLeg;
		bonesLength.at("LeftShoulder") = leftShoulder;
		bonesLength.at("LeftArm") = leftArm;
		bonesLength.at("LeftForeArm") = leftForeArm;
		bonesLength.at("LeftHand_dum2") = leftHand_dum2;
		bonesLength.at("RightShoulder") = rightShoulder;
		bonesLength.at("RightArm") = rightArm;
		bonesLength.at("RightForeArm") = rightForeArm;
		bonesLength.at("RightHand_dum2") = rightHand_dum2;
		bonesLength.at("LeftFoot") = leftFoot;
		bonesLength.at("RightFoot") = rightFoot;

		bonesLength.at("LeftToeBase") = 0.0;
		bonesLength.at("Spine_dum") = 0.0;
		bonesLength.at("RightToeBase") = 0.0;
		bonesLength.at("Spine1") = 0.0;
		bonesLength.at("Spine1_dum2") = 0.0;
		bonesLength.at("Spine1_dum3") = 0.0;
		bonesLength.at("LeftHand") = 0.0;
		bonesLength.at("LeftHand_dum1") = 0.0;
		bonesLength.at("LeftHandThumb") = 0.0;
		bonesLength.at("RightHand") = 0.0;
		bonesLength.at("RightHand_dum1") = 0.0;
		bonesLength.at("RightHandThumb") = 0.0;
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		QMessageBox::critical(NULL, "Error", "Plik ASF jest niezgodny z plikiem DAT", QMessageBox::Ok);
		exit(-1);
	}
}

float ModelHandler::getModelHeightFromMap(map<string, float> &bonesLength) {
	/*wykorzystywane*/
	float head = bonesLength.find("Head")->second;
	float neck = bonesLength.find("Neck")->second;
	float spine1_dum1 = bonesLength.find("Spine1_dum1")->second;
	//float spine1 = bonesLength.find("Spine1")->second;
	float spine = bonesLength.find("Spine")->second;
	float leftUpLeg_dum = bonesLength.find("LeftUpLeg_dum")->second;
	float leftUpLeg = bonesLength.find("LeftUpLeg")->second;
	float leftLeg = bonesLength.find("LeftLeg")->second;
	float rightUpLeg_dum = bonesLength.find("RightUpLeg_dum")->second;
	float rightUpLeg = bonesLength.find("RightUpLeg")->second;
	float rightLeg = bonesLength.find("RightLeg")->second;
	float leftShoulder = bonesLength.find("LeftShoulder")->second;
	float leftArm = bonesLength.find("LeftArm")->second;
	float leftForeArm = bonesLength.find("LeftForeArm")->second;
	float leftHand_dum2 = bonesLength.find("LeftHand_dum2")->second;
	float rightShoulder = bonesLength.find("RightShoulder")->second;
	float rightArm = bonesLength.find("RightArm")->second;
	float rightForeArm = bonesLength.find("RightForeArm")->second;
	float rightHand_dum2 = bonesLength.find("RightHand_dum2")->second;
	float rightFoot = bonesLength.find("RightFoot")->second;
	float leftFoot = bonesLength.find("LeftFoot")->second;
	/*puste*/
	float spine_dum = bonesLength.find("Spine_dum")->second;
	float leftToeBase = bonesLength.find("LeftToeBase")->second;
	float rightToeBase = bonesLength.find("RightToeBase")->second;
	float spine1 = bonesLength.find("Spine1")->second;
	//float spine1_dum1 = bonesLength.find("Spine1_dum1")->second;
	float spine1_dum2 = bonesLength.find("Spine1_dum2")->second;
	float spine1_dum3 = bonesLength.find("Spine1_dum3")->second;
	float leftHand = bonesLength.find("LeftHand")->second;
	float leftHand_dum1 = bonesLength.find("LeftHand_dum1")->second;
	float leftHandThumb = bonesLength.find("LeftHandThumb")->second;
	float rightHand = bonesLength.find("RightHand")->second;
	float rightHand_dum1 = bonesLength.find("RightHand_dum1")->second;
	float rightHandThumb = bonesLength.find("RightHandThumb")->second;

	float height;

	if ((leftUpLeg + leftLeg) > (rightUpLeg + rightLeg))
		height = leftUpLeg + leftLeg + spine + spine1_dum1 + spine1 + neck + head;
	else
		height = rightUpLeg + rightLeg + spine + spine1_dum1 + spine1 + neck + head;

	return height;
}

float ModelHandler::getModelWidthFromMap(map<string, float> &bonesLength) {
	/*wykorzystywane*/
	float head = bonesLength.find("Head")->second;
	float neck = bonesLength.find("Neck")->second;
	float spine1_dum1 = bonesLength.find("Spine1_dum1")->second;
	//float spine1 = bonesLength.find("Spine1")->second;
	float spine = bonesLength.find("Spine")->second;
	float leftUpLeg_dum = bonesLength.find("LeftUpLeg_dum")->second;
	float leftUpLeg = bonesLength.find("LeftUpLeg")->second;
	float leftLeg = bonesLength.find("LeftLeg")->second;
	float rightUpLeg_dum = bonesLength.find("RightUpLeg_dum")->second;
	float rightUpLeg = bonesLength.find("RightUpLeg")->second;
	float rightLeg = bonesLength.find("RightLeg")->second;
	float leftShoulder = bonesLength.find("LeftShoulder")->second;
	float leftArm = bonesLength.find("LeftArm")->second;
	float leftForeArm = bonesLength.find("LeftForeArm")->second;
	float leftHand_dum2 = bonesLength.find("LeftHand_dum2")->second;
	float rightShoulder = bonesLength.find("RightShoulder")->second;
	float rightArm = bonesLength.find("RightArm")->second;
	float rightForeArm = bonesLength.find("RightForeArm")->second;
	float rightHand_dum2 = bonesLength.find("RightHand_dum2")->second;
	float rightFoot = bonesLength.find("RightFoot")->second;
	float leftFoot = bonesLength.find("LeftFoot")->second;
	/*puste*/
	float spine_dum = bonesLength.find("Spine_dum")->second;
	float leftToeBase = bonesLength.find("LeftToeBase")->second;
	float rightToeBase = bonesLength.find("RightToeBase")->second;
	float spine1 = bonesLength.find("Spine1")->second;
	//float spine1_dum1 = bonesLength.find("Spine1_dum1")->second;
	float spine1_dum2 = bonesLength.find("Spine1_dum2")->second;
	float spine1_dum3 = bonesLength.find("Spine1_dum3")->second;
	float leftHand = bonesLength.find("LeftHand")->second;
	float leftHand_dum1 = bonesLength.find("LeftHand_dum1")->second;
	float leftHandThumb = bonesLength.find("LeftHandThumb")->second;
	float rightHand = bonesLength.find("RightHand")->second;
	float rightHand_dum1 = bonesLength.find("RightHand_dum1")->second;
	float rightHandThumb = bonesLength.find("RightHandThumb")->second;


	float width;

	if (rightHand_dum2 + rightForeArm + rightArm + rightShoulder + leftHand_dum2 + leftForeArm + leftArm + leftShoulder > rightUpLeg_dum + leftUpLeg_dum)
		width = rightHand_dum2 + rightForeArm + rightArm + rightShoulder + leftHand_dum2 + leftForeArm + leftArm + leftShoulder;
	else
		width = rightUpLeg_dum + leftUpLeg_dum;

	return width;
}

float ModelHandler::getBoneGeometryLength(vector<pf::boneGeometry> bonesGeo, string name) {
	float result = 0.0;
	for (int i = 0; i < bonesGeo.size(); i++) {
		if (bonesGeo[i].name == name) {
			result = bonesGeo[i].length;
			break;
		}
	}
	return result;
}