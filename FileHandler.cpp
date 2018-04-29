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
		msgBox.setText("Plik asf musi mieæ nazwê.");
		msgBox.exec();
	}
	else {
		pf::Model3D model;
		model.saveConfig(datFileName.toUtf8().constData(), bonesConf, bonesGeometry);
	}
}

void FileHandler::saveHelperFile(QString folderPath, QString helperFileName) {
	ofstream plik(helperFileName.toUtf8().constData());

	plik << ":folderPath" << endl;
	plik << "\t" << folderPath.toUtf8().constData() << endl;
	plik << ":glWidgetBackground" << endl;
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

		QMessageBox::information(NULL, QObject::tr("app"),
			QObject::tr("Zapisano pomyœlnie"),
			QMessageBox::Cancel,
			QMessageBox::Cancel);
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
		for (int i = 0; i < asfPaths.size(); i++) {
			vector<vector<float>> statesTMP;
			pf::Model3D model3D = pf::Model3D(pf::Model3D::Cylinder, asfPaths[i].toUtf8().constData(), datPaths[i].toUtf8().constData());
			pf::MotionData::loadStateVectorFromAMC(fileName.toUtf8().constData(), model3D.getNamesMovingBones(), statesTMP);
			states.push_back(statesTMP[i]);
		}
		
		QMessageBox::information(NULL, QObject::tr("app"),
			QObject::tr("Wczytano pomyœlnie"),
			QMessageBox::Cancel,
			QMessageBox::Cancel);
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
						int id = stoi(tmp.substr(tmp.find("(") + 1, tmp.find(")") - tmp.find("(") - 1));
						asf = asf.remove(asf.length() - 7, 7) + "(" + QString::fromStdString(std::to_string(id + 1)) + ").asf"; //tu nie czyta id wekszych niz 9
						dat = dat.remove(dat.length() - 7, 7) + "(" + QString::fromStdString(std::to_string(id + 1)) + ").dat";
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