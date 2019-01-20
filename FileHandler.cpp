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

vector<vector<float>> FileHandler::getAMCFile(pf::Model3D * model, QString amcPath) {
	vector<vector<float>> modelState;
	pf::MotionData::loadStateVectorFromAMC(amcPath.toUtf8().constData(), model->getNamesMovingBones(), modelState);
	return modelState;
}

void FileHandler::saveAMCToFile(vector<float> modelState, vector<string> allBones, vector<string> usedBones, QString amcFileName) {
	
	if (amcFileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("File must have a name.");
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
			if (!flag) {}
				//throw MyException(";
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
		msgBox.setText("asf file must have name.");
		msgBox.exec();
	}else
		pf::MotionData::saveASF(asfFileName.toUtf8().constData(), bones);
}

void FileHandler::saveDATToFile(vector<pf::boneConfig> bonesConf, vector<pf::boneGeometry> bonesGeometry, QString datFileName) {
	if (datFileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("dat file must have name.");
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

void FileHandler::saveHelperFileFromAvi(QString aviPath, QString helperFileName, int frameNumber, int hz) {
	ofstream plik(helperFileName.toUtf8().constData());

	plik << ":glWidgetBackground" << endl;
	plik << "begin" << endl;
	plik << "\tid " << 0 << endl;
	plik << "\ttype " << "avi" << endl;
	plik << "\tpath " << aviPath.toUtf8().constData() << endl;
	plik << "\tframe " << frameNumber << endl;
	plik << "\thz " << hz << endl;
	plik << "end" << endl;
	plik.close();
}

void FileHandler::saveHelperFileFromImagesFolder(QString imgPath, QString helperFileName) {
	ofstream plik(helperFileName.toUtf8().constData());

	plik << ":glWidgetBackground" << endl;
	plik << "begin" << endl;
	plik << "\tid " << 0 << endl;
	plik << "\ttype " << "img" << endl;
	plik << "\tpath " << imgPath.toUtf8().constData() << endl;
	plik << "end" << endl;

	plik.close();
}

void FileHandler::saveHelperFileWithoutBackground(QString helperFileName) {

	ofstream plik(helperFileName.toUtf8().constData());

	plik << ":glWidgetBackground" << endl;
	plik << "begin" << endl;
	plik << "\tid " << 0 << endl;
	plik << "\ttype " << "empty" << endl;
	plik << "\tpath " << "emptyPath" << endl;
	plik << "end" << endl;

	plik.close();
}

void FileHandler::saveHelperFile(QString helperFileName, vector<int> frameID, vector<vector<string>> imagesPath) {
	ofstream plik(helperFileName.toUtf8().constData());

	/*zapis informacji o kolejnych klatkach*/
	plik << ":glWidgetBackground" << endl;
	for (int i = 0; i < frameID.size(); i++) {
		if (imagesPath[i][0] == "img") {
			plik << "begin" << endl;
			plik << "\tid " << frameID[i] << endl;
			plik << "\ttype " << imagesPath[i][0] << endl;
			plik << "\tpath " << imagesPath[i][1] << endl;
			plik << "end" << endl;
		}
		else if(imagesPath[i][0] == "avi") {
			plik << "begin" << endl;
			plik << "\tid " << frameID[i] << endl;
			plik << "\ttype " << imagesPath[i][0] << endl;
			plik << "\tpath " << imagesPath[i][1] << endl;
			plik << "\tframe " << imagesPath[i][2] << endl;
			plik << "\thz " << imagesPath[i][3] << endl;
			plik << "end" << endl;
		}
		else {
			plik << "begin" << endl;
			plik << "\tid " << frameID[i] << endl;
			plik << "\ttype " << imagesPath[i][0] << endl;
			plik << "\tpath " << imagesPath[i][1] << endl;
			plik << "end" << endl;
		}
	}
	
	plik.close();
}

void FileHandler::saveAMCSeq(vector<vector<float>> modelState, vector<pf::ASFBone> allbones, vector<string> usedBones, QString fileName, vector<bool> usedModelState) {
	if (fileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("File must have name.");
		msgBox.exec();
	}
	else {
		ofstream plik(fileName.toUtf8().constData());
		if (modelState.size() == usedModelState.size()) {
			plik << ":FULLY-SPECIFIED" << endl;
			plik << ":DEGREES" << endl;
			for (int i = 0; i < modelState.size(); i++) {
				if (usedModelState[i]) {
					plik << i + 1 << endl;
					plik << allbones[0].name << " " << modelState[i][0] << " "
						<< modelState[i][1] << " " << modelState[i][2] << " "
						<< modelState[i][3] << " " << modelState[i][4] << " "
						<< modelState[i][5] << endl;

					for (int k = 1; k < allbones.size(); k++) {
						bool flag = false;
						for (int j = 1; j < usedBones.size(); j++) {
							if (allbones[k].name == usedBones[j]) {
								plik << allbones[k].name << " " << modelState[i][(j + 1) * 3] << " " << modelState[i][(j + 1) * 3 + 1] << " " << modelState[i][(j + 1) * 3 + 2] << endl;
								flag = true;
								break;
							}
						}
						if (!flag) {}
						//plik << allbones[k].name << " " << 0 << " " << 0 << " " << 0 << endl;
					}
				}
			}
		}
		else {
			cout << "modelState i wektor sprawdzajacy czy zapisac dana konfiguracje maja rozne dlugosci" << endl;
		}
		plik.close();
	}
	//pf::MotionData::saveStateVectorToAMC(fileName.toUtf8().constData(), allbones, usedBones, modelState);
}

vector<vector<float>> FileHandler::loadAmcFromFile(QString fileName, vector<string> allBones, vector<bool> &usedFrames, vector<string> &bones) {
	vector<vector<float>> states;
	vector<int> usedFramesTMP;

	if (fileName.isEmpty()) {
		QMessageBox::warning(NULL, QObject::tr("app"),
			QObject::tr("Cannot open file."),
			QMessageBox::Cancel,
			QMessageBox::Cancel);
	}

	else {
		if (QFileInfo(fileName).exists()) {
			bones.clear();

			//vector<float> tmpModelState;
			char string[100];
			std::string str;
			char inputFilename[100];

			strcpy(inputFilename, fileName.toUtf8().constData());

			ifstream inputFile;
			inputFile.open(inputFilename);
			if (!inputFile.is_open())
				cout << "Cannot open file: " << inputFilename << endl;

			while (getline(inputFile, str)) {
				if (str.at(0) != '#' && str.at(0) != ':'){
					if (isdigit(str.at(0))) {
						int id = 0;
						for (int j = 0; j < str.length(); j++) {
							id = str.at(j) - '0' + 10 * id;
						}
						usedFramesTMP.push_back(id);
					}
					else {
						
						int strpos = str.find(" ");
						std::string bone = str.substr(0, strpos);
						bones.push_back(bone);
						continue;
					}
				}
			}
			if (usedFramesTMP.size() > 0) {
				if (usedFramesTMP.size() > 1) {
					int bonesCount = bones.size() / usedFramesTMP.size();
					bones.erase(bones.begin(), bones.begin() + (bonesCount * (usedFramesTMP.size()-1)));
				}

				pf::MotionData::loadStateVectorFromAMC(fileName.toUtf8().constData(), bones, states);
				int mainGLWidgetWindowFrames = usedFrames.size();
				usedFrames.clear();
				int lastID = usedFramesTMP[usedFramesTMP.size() - 1];

				for (int i = 1; i < mainGLWidgetWindowFrames + 1; i++) {
					if (std::find(usedFramesTMP.begin(), usedFramesTMP.end(), i) != usedFramesTMP.end()) {
						usedFrames.push_back(true);
					}
					else {
						usedFrames.push_back(false);
					}
				}
			}

			inputFile.close();

		}
		else {
			string tmp = "Cannot find amc file!!\n";
			QMessageBox::warning(NULL, QObject::tr("app"),
				QObject::tr(tmp.c_str()),
				QMessageBox::Cancel,
				QMessageBox::Cancel);
			
			states.clear();
		}
	}

	return states;
}

void FileHandler::loadDatFromFile(pf::Model3D *&model, string datFileName, vector<pf::boneConfig> &bonesConf, vector<pf::boneGeometry> &bonesGeometry) {
//	pf::Model3D model;
	model = new pf::Model3D(pf::Model3D::Cylinder, ASF_TEMPLATE_PATH, datFileName);
	model->loadConfig(datFileName, bonesConf, bonesGeometry);
}

void FileHandler::loadDatFromFile(pf::Model3D *&model, string datFileName, vector<pf::boneConfig> &bonesConf, vector<pf::boneGeometry> &bonesGeometry, QString asfFile) {
	model = new pf::Model3D(pf::Model3D::Cylinder, asfFile.toUtf8().constData(), datFileName);
	model->loadConfig(datFileName, bonesConf, bonesGeometry);
}

QString FileHandler::getImagesFolderPath(QString helperFileName) {
	string str;
	char inputFilename[100];

	strcpy(inputFilename, helperFileName.toUtf8().constData());

	ifstream inputFile;
	inputFile.open(inputFilename);
	if (!inputFile.is_open())
		cout << "Cannot open file: " << inputFilename << endl;
	getline(inputFile, str);
	getline(inputFile,str);
	inputFile.close();

	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());

	return QString::fromStdString(str);
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
		cout << "Cannot open file: " << inputFilename << endl;

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

vector<string> FileHandler::getGlWidgetBackgroudTypeFromFile(QString helperFileName) {
	vector<string> types;

	char string[100];
	std::string str;
	char inputFilename[100];

	strcpy(inputFilename, helperFileName.toUtf8().constData());

	ifstream inputFile;
	inputFile.open(inputFilename);
	if (!inputFile.is_open())
		cout << "Cannot open file: " << inputFilename << endl;

	while (strcmp(string, ":glWidgetBackground")) {
		inputFile >> string;
	}

	while (inputFile >> string) {
		while (inputFile >> string && strcmp(string, "end")) {
			if (strcmp(string, "type") == 0) {
				getline(inputFile, str);
				types.push_back(str.substr(str.find(" ") + 1, str.length() - str.find(" ")));
			}
		}
	}

	inputFile.close();

	return types;
}

void FileHandler::getGlWidgetBackgroudAviInfoFromFile(QString helperFileName, vector<int> &framesID, vector<int> &aviHz) {
	char string[100];
	std::string str;
	char inputFilename[100];

	strcpy(inputFilename, helperFileName.toUtf8().constData());

	ifstream inputFile;
	inputFile.open(inputFilename);
	if (!inputFile.is_open())
		cout << "Cannot open file: " << inputFilename << endl;

	while (strcmp(string, ":glWidgetBackground")) {
		inputFile >> string;
	}

	while (inputFile >> string) {
		while (inputFile >> string && strcmp(string, "end")) {
			if (strcmp(string, "frame") == 0) {
				getline(inputFile, str);
				framesID.push_back(std::stoi(str));
			}
			else if (strcmp(string, "hz") == 0) {
				getline(inputFile, str);
				aviHz.push_back(std::stoi(str));
			}
		}
	}

	inputFile.close();
}

void FileHandler::getFilesAsQStrings(QString amcFilePath, QString &asfFilePath, QString &datFilePath, QString &imgFolderPath) {
	QString fileName = QFileInfo(amcFilePath).baseName().toUtf8().constData();
	QString folderPath = QFileInfo(amcFilePath).absolutePath();
	asfFilePath = folderPath + "/" + fileName + ".asf";
	datFilePath = folderPath + "/" + fileName + ".dat";
	QString helperFilePath = folderPath + "/helperFile.dat";
	imgFolderPath = getImagesFolderPath(helperFilePath);
}

void FileHandler::getFilesAsQStrings(QString amcFilePath, vector<QString> &asfFilesPaths, vector<QString> &datFilesPaths) {
	QString fileName = QFileInfo(amcFilePath).baseName().toUtf8().constData();
	QString folderPath = QFileInfo(amcFilePath).absolutePath();
	QString helperFilePath = folderPath + "/helperFile.dat";

	vector<QString> glWidgetsBackground = getGlWidgetBackgroudFromFile(helperFilePath);

	if (!glWidgetsBackground.empty()) {
		for (int i = 0; i < glWidgetsBackground.size(); i++) {
			QString tmpFileName;
			QString tmpASF;
			QString tmpDAT;

			if (glWidgetsBackground[i] == "emptyBckg") {
				tmpFileName = "emptyBckg";
				tmpASF = folderPath + "/" + tmpFileName;
				tmpDAT = folderPath + "/" + tmpFileName;
			}
			else {
				/*nazwa pliku bez rozszerzenia*/
				tmpFileName = QFileInfo(glWidgetsBackground[i]).baseName();
				tmpASF = folderPath + "/" + tmpFileName;
				tmpDAT = folderPath + "/" + tmpFileName;	
			}

			checkExistingFiles(asfFilesPaths, datFilesPaths, tmpASF, tmpDAT);	
		}
	}
	else{
		asfFilesPaths.push_back(folderPath + "/" + fileName + ".asf");
		datFilesPaths.push_back(folderPath + "/" + fileName + ".dat");
	}
	//imgFolderPath = getImagesFolderPath(helperFilePath);	
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
						asf = asf.remove(asf.length() - removeChars, removeChars) + "(" + QString::fromStdString(std::to_string(id + 1)) + ").asf"; 
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
			QObject::tr("Number of .asf and .dat files is not equal!"),
			QMessageBox::Cancel,
			QMessageBox::Cancel);
	}
}

int FileHandler::getImageID(QFileInfoList allImagesList, QString loadedImage) {
	int result;

	for (int j = 0; j < allImagesList.size(); j++) {
		if (allImagesList[j] == loadedImage) {
			result = j + 1;
			break;
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

void FileHandler::reloadParams(GLWidget *&glWidget, vector<pf::boneConfig> bonesConf, vector<pf::boneGeometry> bonesGeometry, QString asfFile) {
	saveDATToFile(bonesConf, bonesGeometry, "tmpDatFile.dat");

	try {
		glWidget->model = new pf::Model3D(pf::Model3D::Cylinder, asfFile.toUtf8().constData(), "tmpDatFile.dat");
	}
	catch (std::exception& e) {
		QMessageBox::critical(NULL, "Error", e.what(), QMessageBox::Ok);
		cerr << e.what() << endl;
		exit(-1);
	}
	catch (pf::Exception& e) {
		
		cerr << e.what() << endl;
		QMessageBox::critical(NULL, "Error", e.what(), QMessageBox::Ok);
		exit(-1);
	}

	std::remove("tmpDatFile.dat");
}