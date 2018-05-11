#include "FileHandler.h"

FileHandler::FileHandler()
{
}


FileHandler::~FileHandler()
{
}

vector<vector<float>> FileHandler::getAMCTemplate(pf::Model3D * model) {
	vector<vector<float>> modelState;
	pf::MotionData::loadStateVectorFromAMC("ETC/modelTemplate.amc", model->getNamesMovingBones(), modelState);
	return modelState;
}

void FileHandler::saveAMCToFile(vector<float> modelState, vector<string> allBones, vector<string> usedBones, QString amcFileName) {
	
	if (amcFileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("Plik musi mieæ nazwê.");
		msgBox.exec();
	}
	else {
		ofstream plik(amcFileName.toUtf8().constData());

		plik << ":FULLY-SPECIFIED" << endl;
		plik << ":DEGREES" << endl;
		plik << 1 << endl;
		plik << allBones[0] << " " << modelState[0] << " " << modelState[1] << " " << modelState[2] << " " << modelState[3] << " " << modelState[4] << " " << modelState[5] << endl;

		for (int i = 1; i < allBones.size(); i++) {
			bool flag = false;
			for (int j = 1; j < usedBones.size(); j++) {
				if (allBones[i] == usedBones[j]) {
					plik << allBones[i] << " " << modelState[(j + 1) * 3] << " " << modelState[(j + 1) * 3 + 1] << " " << modelState[(j + 1) * 3 + 2] << endl;
					flag = true;
					break;
				}
			}
			if (!flag)
				plik << allBones[i] << " " << 0 << " " << 0 << " " << 0 << endl;
		}

		plik.close();

		/*QMessageBox::information(NULL, QObject::tr("app"),
			QObject::tr("Zapisano pomyœlnie"),
			QMessageBox::Cancel,
			QMessageBox::Cancel);*/
	}
}

void FileHandler::saveASFToFile(vector<pf::ASFBone> bones, QString asfFileName) {
	if (asfFileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("Plik asf musi mieæ nazwê.");
		msgBox.exec();
	}else
		pf::MotionData::saveASF(asfFileName.toUtf8().constData(), bones);
}

void FileHandler::saveDATToFile(vector<pf::boneConfig> bonesConf, vector<pf::boneGeometry> bonesGeometry, QString datFileName) {
	if (datFileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("Plik dat musi mieæ nazwê.");
		msgBox.exec();
	}
	else {
		pf::Model3D model;
		model.saveConfig(datFileName.toUtf8().constData(), bonesConf, bonesGeometry);
		//saveDATGeometry(bonesGeometry, datFileName);
	}
}

void FileHandler::saveDATGeometry(vector<pf::boneGeometry> bonesGeometry, QString datFileName) {
	/*iterator bonesGeometry*/
	int i = 0;
	string fileName = datFileName.toUtf8().constData();

	char string[100];
	std::string str;
	char inputFilename[100];

	strcpy(inputFilename, datFileName.toUtf8().constData());

	ifstream plikIn;
	plikIn.open(inputFilename);

	ofstream plikOut(fileName);

	if (!plikIn || !plikOut){
		cout << "Error opening files!" << endl;
	}
	else {
		while (strcmp(string, ":geometry")) {
			plikIn >> string;
		}
		while (plikIn >> string) {
			while (plikIn >> string && strcmp(string, "end")) {
				if (strcmp(string, bonesGeometry[i].name.c_str()) == 0) {
					getline(plikIn, str);
					str = std::to_string(int(bonesGeometry[i].topRadius1)) + " " +
						std::to_string(int(bonesGeometry[i].topRadius2)) + " " +
						std::to_string(int(bonesGeometry[i].bottomRadius1)) + " " +
						std::to_string(int(bonesGeometry[i].bottomRadius2)) + " " +
						std::to_string(int(bonesGeometry[i].length)) + " " +
						std::to_string(int(bonesGeometry[i].label.r)) + " " +
						std::to_string(int(bonesGeometry[i].label.g)) + " " +
						std::to_string(int(bonesGeometry[i].label.b)) + "\n" ;
					plikOut << str;
				}
				i++;
			}
		}
	}

	plikIn.close();
	plikOut.close();

}

void FileHandler::saveHelperFile(QString folderPath, QString helperFileName, QString imagePath) {
	ofstream plik(helperFileName.toUtf8().constData());

	plik << ":folderPath" << endl;
	plik << "\t" << folderPath.toUtf8().constData() << endl;
	plik << ":glWidgetBackground" << endl;
	plik << "\tid " << 0 << endl;
	plik << "\tpath " << imagePath.toUtf8().constData() << endl;
	plik.close();
}

void FileHandler::saveHelperFile(QString folderPath, QString helperFileName, vector<int> frameID, vector<QString> imagesPath) {
	ofstream plik(helperFileName.toUtf8().constData());

	plik << ":folderPath" << endl;
	plik << "\t" << folderPath.toUtf8().constData() << endl;
	/*zapis informacji o kolejnych klatkach*/
	plik << ":glWidgetBackground" << endl;
	for (int i = 0; i < frameID.size(); i++) {
		plik << "begin" << endl;
		plik << "\tid " << frameID[i] << endl;
		plik << "\tpath " << imagesPath[i].toUtf8().constData() << endl;
		plik << "end" << endl;
	}
	
	plik.close();
}

void FileHandler::saveAMCSeq(vector<vector<float>> modelState, vector<string> allBones, vector<string> usedBones, QString fileName) {
	if (fileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("Plik musi mieæ nazwê.");
		msgBox.exec();
	}
	else {
		ofstream plik(fileName.toUtf8().constData());

		plik << ":FULLY-SPECIFIED" << endl;
		plik << ":DEGREES" << endl;

		for (int i = 0; i < modelState.size(); i++) {
			plik << i + 1 << endl;
			plik << allBones[0] << " " << modelState[i][0] << " "
				<< modelState[i][1] << " " << modelState[i][2] << " "
				<< modelState[i][3] << " " << modelState[i][4] << " " 
				<< modelState[i][5] << endl;

			for (int k = 1; k < allBones.size(); k++) {
				bool flag = false;
				for (int j = 1; j < usedBones.size(); j++) {
					if (allBones[k] == usedBones[j]) {
						plik << allBones[k] << " " << modelState[i][(j + 1) * 3] << " " << modelState[i][(j + 1) * 3 + 1] << " " << modelState[i][(j + 1) * 3 + 2] << endl;
						flag = true;
						break;
					}
				}
				if (!flag)
					plik << allBones[k] << " " << 0 << " " << 0 << " " << 0 << endl;
			}
		}

		plik.close();
		//pf::MotionData::saveStateVectorToAMC(fileName.toUtf8().constData(), allBones, usedBones, modelState);
	}
}

vector<vector<float>> FileHandler::loadAmcFromFile(QString fileName, vector<QString> asfPaths, vector<QString> datPaths) {
	vector<vector<float>> states;

	if (fileName.isEmpty()) {
		QMessageBox::warning(NULL, QObject::tr("app"),
			QObject::tr("Nie mo¿na otworzyæ pliku."),
			QMessageBox::Cancel,
			QMessageBox::Cancel);
	}

	else {
		if (asfPaths.size() != datPaths.size()) {
			QMessageBox::warning(NULL, QObject::tr("app"),
				QObject::tr("Niepoprawna iloœæ plików asf lub dat."),
				QMessageBox::Cancel,
				QMessageBox::Cancel);
		}
		else {

			for (int i = 0; i < asfPaths.size(); i++) {
				if (QFileInfo(asfPaths[i]).exists() && QFileInfo(datPaths[i]).exists()) {
					vector<vector<float>> statesTMP;
					pf::Model3D model3D = pf::Model3D(pf::Model3D::Cylinder, asfPaths[i].toUtf8().constData(), datPaths[i].toUtf8().constData());
					pf::MotionData::loadStateVectorFromAMC(fileName.toUtf8().constData(), model3D.getNamesMovingBones(), statesTMP);
					states.push_back(statesTMP[i]);
				}
				else {
					string tmp = "Brak podanych plików dat lub asf!\n" + asfPaths[i].toStdString() + "\n" + datPaths[i].toStdString();
					QMessageBox::warning(NULL, QObject::tr("app"),
						QObject::tr(tmp.c_str()),
						QMessageBox::Cancel,
						QMessageBox::Cancel);

					states.clear();
					break;
				}
			}
			if(!states.empty())
			QMessageBox::information(NULL, QObject::tr("app"),
				QObject::tr("Wczytano pomyœlnie"),
				QMessageBox::Cancel,
				QMessageBox::Cancel);
		}
	}

	return states;
}

void FileHandler::loadDatFromFile(string datFileName, vector<pf::boneConfig> &bonesConf, vector<pf::boneGeometry> &bonesGeometry) {
	pf::Model3D model;
	model.loadConfig(datFileName, bonesConf, bonesGeometry);
}

QString FileHandler::getImagesFolderPath(QString helperFileName) {
	char string[100];
	char inputFilename[100];

	strcpy(inputFilename, helperFileName.toUtf8().constData());

	ifstream inputFile;
	inputFile.open(inputFilename);
	if (!inputFile.is_open())
		cout << "B³¹d otwarcia pliku: " << inputFilename << endl;
	inputFile >> string;
	inputFile >> string;
	inputFile.close();

	return QString::fromStdString(string);
}

vector<QString> FileHandler::getGlWidgetBackgroudFromFile(QString helperFileName) {
	vector<QString> filesPath;

	char string[100];
	std::string str;
	char inputFilename[100];

	strcpy(inputFilename, helperFileName.toUtf8().constData());

	ifstream inputFile;
	inputFile.open(inputFilename);
	if (!inputFile.is_open())
		cout << "B³¹d otwarcia pliku: " << inputFilename << endl;

	while (strcmp(string, ":glWidgetBackground")) {
		inputFile >> string;
	}

	while (inputFile >> string) {
		while (inputFile >> string && strcmp(string, "end")) {
			if (strcmp(string, "path") == 0) {
				getline(inputFile, str);
				filesPath.push_back(QString::fromStdString(str.substr(str.find(" ")+1, str.length() - str.find(" "))));
			}
		}
	}

	inputFile.close();

	return filesPath;
}

void FileHandler::getFilesAsQStrings(QString amcFilePath, QString &asfFilePath, QString &datFilePath, QString &imgFolderPath) {
	QString fileName = QFileInfo(amcFilePath).baseName().toUtf8().constData();
	QString folderPath = QFileInfo(amcFilePath).absolutePath();
	asfFilePath = folderPath + "/" + fileName + ".asf";
	datFilePath = folderPath + "/" + fileName + ".dat";
	QString helperFilePath = folderPath + "/helperFile.dat";
	imgFolderPath = getImagesFolderPath(helperFilePath);
}

void FileHandler::getFilesAsQStrings(QString amcFilePath, vector<QString> &asfFilesPaths, vector<QString> &datFilesPaths, QString &imgFolderPath) {
	QString fileName = QFileInfo(amcFilePath).baseName().toUtf8().constData();
	QString folderPath = QFileInfo(amcFilePath).absolutePath();
	QString helperFilePath = folderPath + "/helperFile.dat";

	vector<QString> glWidgetsBackground = getGlWidgetBackgroudFromFile(helperFilePath);
	if (!glWidgetsBackground.empty()) {
		for (int i = 0; i < glWidgetsBackground.size(); i++) {
			/*nazwa pliku bez rozszerzenia*/
			QString tmpFileName = QFileInfo(glWidgetsBackground[i]).baseName();
			QString tmpASF = folderPath + "/" + tmpFileName;
			QString tmpDAT = folderPath + "/" + tmpFileName;

			checkExistingFiles(asfFilesPaths, datFilesPaths, tmpASF, tmpDAT);	
		}
	}
	else{
		asfFilesPaths.push_back(folderPath + "/" + fileName + ".asf");
		datFilesPaths.push_back(folderPath + "/" + fileName + ".dat");
	}
	imgFolderPath = getImagesFolderPath(helperFilePath);	
}

void FileHandler::checkExistingFiles(vector<QString> &asfFiles, vector<QString> &datFiles, QString asf, QString dat) {
	if (asfFiles.size() == datFiles.size()) {
		bool asfCopyFound = false;
		if (asfFiles.size() == 0) {
			asf = asf + ".asf";
			dat = dat + ".dat";
			asfFiles.push_back(asf);
			datFiles.push_back(dat);
		}
		else {
			for (int i = 0; i < asfFiles.size(); i++) {
				QString asfTmp, datTmp;
				if (!asfCopyFound) {
					asfTmp = asf + ".asf";
					datTmp = dat + ".dat";
				}
				else {
					asfTmp = asf;
					datTmp = dat;
				}

				if (asfTmp == asfFiles[i]) {
					string tmp = asfFiles[i].toUtf8().constData();
					if (tmp.find("(") != string::npos && tmp.find(")") != string::npos) {
						/* ilosc cyfr w nawiasie */
						int length = tmp.find(")") - tmp.find("(") - 1;
						/* wartosc w nawiasie */
						int id = stoi(tmp.substr(tmp.find("(") + 1, length));
						/* ilosc znakow do usuniecia - ().asf */
						int removeChars = 6 + length;
						asf = asf.remove(asf.length() - removeChars, removeChars) + "(" + QString::fromStdString(std::to_string(id + 1)) + ").asf"; //tu nie czyta id wekszych niz 9
						dat = dat.remove(dat.length() - removeChars, removeChars) + "(" + QString::fromStdString(std::to_string(id + 1)) + ").dat";
					}
					else {
						asf = asf + "(1).asf";
						dat = dat + "(1).dat";
					}
					asfCopyFound = true;

				}
			}
			if (!asfCopyFound) {
				asf = asf + ".asf";
				dat = dat + ".dat";
			}
			asfFiles.push_back(asf);
			datFiles.push_back(dat);
		}
	}
	else {
		QMessageBox::warning(NULL, QObject::tr("app"),
			QObject::tr("Liczba plików .asf oraz .dat nie jest równa!"),
			QMessageBox::Cancel,
			QMessageBox::Cancel);
	}
}

vector<int> FileHandler::getImagesIDs(QFileInfoList allImagesList, vector<QString> loadedImages) {
	vector<int> result;

	for (int i = 0; i < loadedImages.size(); i++) {
		for (int j = 0; j < allImagesList.size(); j++) {
			if (allImagesList[j] == loadedImages[i]) {
				result.push_back(j+1);
				break;
			}
		}
	}

	return result;
}

void FileHandler::addBoneConf(vector<pf::boneConfig> &bonesConf, map<string, int> idxBonesMap, vector<pf::ASFBone> bones, int i) {
	istringstream iss(bones[i].dof);
	string s;
	vector <string> boneDOF;
	while (getline(iss, s, ' ')) {
		boneDOF.push_back(s);
	}

	pf::boneConfig bConf;

	bConf.name = bones[i].name;

	if (std::find(boneDOF.begin(), boneDOF.end(), "tx") != boneDOF.end())
		bConf.isTransX = true;
	else
		bConf.isTransX = false;

	if (std::find(boneDOF.begin(), boneDOF.end(), "ty") != boneDOF.end())
		bConf.isTransY = true;
	else
		bConf.isTransY = false;

	if (std::find(boneDOF.begin(), boneDOF.end(), "tz") != boneDOF.end())
		bConf.isTransZ = true;
	else
		bConf.isTransZ = false;

	if (std::find(boneDOF.begin(), boneDOF.end(), "rx") != boneDOF.end())
		bConf.isRotX = true;
	else
		bConf.isRotX = false;

	if (std::find(boneDOF.begin(), boneDOF.end(), "ry") != boneDOF.end())
		bConf.isRotY = true;
	else
		bConf.isRotY = false;

	if (std::find(boneDOF.begin(), boneDOF.end(), "rz") != boneDOF.end())
		bConf.isRotZ = true;
	else
		bConf.isRotZ = false;

	bConf.rotX = 0.0;
	bConf.rotY = 0.0;
	bConf.rotZ = 0.0;
	bConf.maxRotX = 360.0;
	bConf.minRotX = -360.0;
	bConf.maxRotY = 360.0;
	bConf.minRotY = -360.0;
	bConf.maxRotZ = 360.0;
	bConf.minRotZ = -360.0;

	bConf.vRotX = 0.0;
	bConf.vRotY = 0.0;
	bConf.vRotZ = 0.0;
	bConf.vTransX = 0.0;
	bConf.vTransY = 0.0;
	bConf.vTransZ = 0.0;

	bonesConf.push_back(bConf);
	
}

void FileHandler::addBoneGeo(vector<pf::boneGeometry> &bonesGeometry, map<string, int> idxBonesMap, vector<pf::ASFBone> bones, int i) {
	if (bones[i].name != "root") {
		pf::boneGeometry bGeo;
		bGeo.name = bones[i].name;

		if (bGeo.name == "LeftUpLeg" || bGeo.name == "RightUpLeg" || bGeo.name == "LeftArm" || bGeo.name == "RightArm")
			bGeo.length = round(bones[i].length) - 4;
		else if (bGeo.name == "RightForeArm" || bGeo.name == "LeftForeArm")
			bGeo.length = round(bones[i].length) - 25;
		else
			bGeo.length = round(bones[i].length);

		boneGeometryLabels(bGeo);
		boneGeometryRadius(bGeo);

		bonesGeometry.push_back(bGeo);
	}
}

void FileHandler::boneGeometryLabels(pf::boneGeometry &bGeo) {
	if ((bGeo.name.find("Left") != std::string::npos) && ((bGeo.name.find("Foot") != std::string::npos) || (bGeo.name.find("Toe") != std::string::npos) || (bGeo.name.find("Leg") != std::string::npos))) {
		//bGeo.label.a = 0.0;
		bGeo.label.b = LEFT_LEG_LABEL;
		bGeo.label.g = LEFT_LEG_LABEL;
		bGeo.label.r = LEFT_LEG_LABEL;
	}

	else if ((bGeo.name.find("Right") != std::string::npos) && ((bGeo.name.find("Foot") != std::string::npos) || (bGeo.name.find("Toe") != std::string::npos) || (bGeo.name.find("Leg") != std::string::npos))) {
		//bGeo.label.a = 0.0;
		bGeo.label.b = RIGHT_LEG_LABEL;
		bGeo.label.g = RIGHT_LEG_LABEL;
		bGeo.label.r = RIGHT_LEG_LABEL;
	}

	else if ((bGeo.name.find("Left") != std::string::npos) && ((bGeo.name.find("Arm") != std::string::npos) || (bGeo.name.find("Hand") != std::string::npos) || (bGeo.name.find("Shoulder") != std::string::npos))) {
		bGeo.label.b = LEFT_ARM_LABEL;
		bGeo.label.g = LEFT_ARM_LABEL;
		bGeo.label.r = LEFT_ARM_LABEL;
	}

	else if ((bGeo.name.find("Right") != std::string::npos) && ((bGeo.name.find("Arm") != std::string::npos) || (bGeo.name.find("Hand") != std::string::npos) || (bGeo.name.find("Shoulder") != std::string::npos))) {
		bGeo.label.b = RIGHT_ARM_LABEL;
		bGeo.label.g = RIGHT_ARM_LABEL;
		bGeo.label.r = RIGHT_ARM_LABEL;
	}

	else {
		bGeo.label.b = 255;
		bGeo.label.g = 255;
		bGeo.label.r = 255;
	}
}

void FileHandler::boneGeometryRadius(pf::boneGeometry &bGeo) {
	bGeo.topRadius1 = 0.0;
	bGeo.topRadius2 = 0.0;
	bGeo.bottomRadius1 = 0.0;
	bGeo.bottomRadius2 = 0.0;
}

int FileHandler::findBoneIDByName(string name, vector<pf::ASFBone> asfBones) {
	int res = 0;
	for (int i = 0; i < asfBones.size(); i++) {
		
		if (name == asfBones[i].name) {
			res = asfBones[i].id;
			break;
		}
	}
	return res;
}

bool FileHandler::isBoneChecked(string name, vector<string> allBonesNames) {
	for (int i = 0; i < allBonesNames.size(); i++) {
		if (allBonesNames[i] == name)
			return true;
	}
	return false;
}