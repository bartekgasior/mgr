#pragma once

#include "Model3D.h"
#include "glWidget.h"
#include "MyException.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;

class ModelHandler
{
public:
	ModelHandler();
	~ModelHandler();

	void initializeBonesRotationsMap(map<string, pf::Vec3f> &bonesRotations, vector<pf::ASFBone> asfBones);

	void initializeBonesRadiusMap(map<string, vector<float>> &modelRadius, vector<pf::ASFBone> asfBones, vector<pf::boneGeometry> bonesGeometry);

	void initializeBonesDOFMap(map<string, vector<bool>> &modelDOF, vector<pf::ASFBone> asfBones);

	void initializeBonesLimitsMap(map<string, vector<int>> &modelLimits, vector<pf::ASFBone> asfBones);

	void initializeBonesVelocityMap(map<string, vector<float>> &modelVelocity, vector<pf::ASFBone> asfBones);

	void initializeBonesLengthMap(map<string, float> &bonesLength, vector<pf::ASFBone> asfBones);

	/*wyzerowanie wartosci mapy*/
	void clearBonesLengthMap(map<string, float> &bonesLength);

	/*funkcja aktualizuje limity obrotow wybranych kosci*/
	void setLimitsVector(vector<pf::range2> &limits, vector<pf::boneConfig> bones, vector<pf::boneGeometry> bonesGeometry);

	void setVelocityVector(vector<float> &velocity, vector<pf::boneConfig> bones, vector<pf::boneGeometry> bonesGeometry);

	/*aktualizacja dlugosci kosci*/
	void updateLength(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float direction, float value);

	void updateLength(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float value);

	void updateTopRadius(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float direction, float value);

	void updateTopRadius(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float value);

	void updateBottomRadius(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float direction, float value);

	void updateBottomRadius(pf::Model3D *model, vector<pf::boneGeometry> &bonesGeometry, string boneName, float value);

	/*pobranie dlugosci kosci*/
	float getBoneLength(vector<pf::boneGeometry> bonesGeometry, string boneName);

	float getBoneTopRadius(vector<pf::boneGeometry> bonesGeometry, string boneName);

	float getBoneBottomRadius(vector<pf::boneGeometry> bonesGeometry, string boneName);

	/*funkcja zwraca id wybranej kosci w wektorze modelState*/
	int getModelStateID(vector <string> usedBonesNames, string boneName, pf::Vec3 vect);

	/*zapis dlugosci kosci do mapy bonesLength*/
	void saveBonesLengthToMap(map<string, float> &bonesLength, vector<pf::boneGeometry> boneGeometry);

	void saveModelStateToMap(map<string, pf::Vec3f> &bonesRotations, vector<float> mState, vector<pf::boneConfig> bonesConfig);

	void saveModelStateToMap(map<string, pf::Vec3f> &bonesRotations, vector<float> mState, vector<string> allBones);

	void saveRadiusToMap(map<string, vector<float>> &bonesRadius, vector<pf::boneGeometry> boneGeometry);

	void saveDOFToMap(map<string, vector<bool>> &bonesDOF, vector<pf::boneConfig> boneConfig);

	void saveLimitsToMap(map<string, vector<int>> &modelLimits, vector<pf::boneConfig> boneConfig);

	void saveVelocityToMap(map<string, vector<float>> &modelVelocity, vector<pf::boneConfig> boneConfig);
	
	/*aktualizacja map po edycji kosci*/
	void updateModelStateFromMap(vector<vector<float>> &mState, vector<map<string, pf::Vec3f>> bonesRotations, vector<pf::boneConfig> bonesConfig, vector<vector<float>> modelTranslation);

	void updateBonesLengthFromMap(map<string, float> bonesLength, vector<pf::boneGeometry> &boneGeometry);

	void updateBonesLengthMap(map<string, float> &bonesLength, string boneName, float value);

	void updateBonesRadiusFromMap(map<string, vector<float>> bonesRadius, vector<pf::boneGeometry> &boneGeometry);

	void updateBonesDOFMap(map<string, vector<bool>> &bonesDOF, string boneName, vector<bool> dofs);

	void updateDOFFromMap(map<string, vector<bool>> bonesDOF, vector<pf::boneConfig> &bonesConfig);

	void updateLimitsFromMap(map<string, vector<int>> modelLimits, vector<pf::boneConfig> &boneConfig);

	void updateVelocityFromMap(map<string, vector<float>> modelVelocity, vector<pf::boneConfig> &bonesConfig);

	/*ustawienie zlotego stosunku dla kosci*/
	void setGoldenRatio(map<string, float> &bonesLength, float height);

	/*pobranie wysokosci modelu z mapy*/
	float getModelHeightFromMap(map<string, float> &bonesLength);

	/*pobranie szerokosci modelu*/
	float getModelWidthFromMap(map<string, float> &bonesLength);

	float getBoneGeometryLength(vector<pf::boneGeometry> bonesGeo, string name);
};

