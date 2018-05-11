#pragma once
#include "Model3D.h"
#include "Types.h"

#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>

using namespace std;

class FileHandler
{
public:
	FileHandler();
	~FileHandler();

	/*wczytanie startowego pliku amc, wartosci rotacji i translacji równe 0*/
	vector<vector<float>> getAMCTemplate(pf::Model3D * model);

	/*zapis pliku amc - stan jednego modelu, nazwy wszystkich kosci,wykorzystane kosci, nazwa pliku*/
	void saveAMCToFile(vector<float> modelState, vector<string> allBones, vector<string> usedBones, QString amcFileName);

	/*zapis pliku asf - kosci, nazwa pliku*/
	void saveASFToFile(vector<pf::ASFBone> bones, QString asfFileName);

	/*zapis pliku dat */
	void saveDATToFile(vector<pf::boneConfig> bonesConf, vector<pf::boneGeometry> bonesGeometry, QString datFileName);

	/*zapis pliku pomocniczego - wykorzystywany przy zapisie jednek klatki
	sceizka do folderu ze zdjeciami
	*/
	void saveHelperFile(QString folderPath, QString helperFileName, QString imagePath);

	/*zapis pliku pomocniczego - wykorzystywany przy zapisie sekwencji klatek
	sciezka do folderu ze zdjeciami
	nazwa pliku
	vector zawierajacy id glWidget
	sciezki do plikow tla odpowiadajacych klatek
	*/
	void saveHelperFile(QString folderPath, QString helperFileName, vector<int> frameID, vector<QString> imagesPath);

	/*zapis sekwencji klatek do pliku - wektor stanow modelu, nazwy wszystkich kosci,wykorzystane kosci, nazwa pliku*/
	void saveAMCSeq(vector<vector<float>> modelState, vector<string> allBones, vector<string> usedBones, QString fileName);

	/*Wczytanie pliku .amc do wektora stanów*/
	vector<vector<float>> loadAmcFromFile(QString fileName, vector<QString> asfPath, vector<QString> datPath);

	/*wczytywanie pliku .dat*/
	/*! \brief Load model configuration.
	*
	* @param datFileName[in] - Path to model configuration file
	* @param bonesConf[out] - vector with bones configuration
	* @param bonesGeometry[out] - vector with bones geometry configuration
	*/
	void loadDatFromFile(string datFileName, vector<pf::boneConfig> &bonesConf, vector<pf::boneGeometry> &bonesGeometry);

	/*Sciezka do folderu zdjec tla, zapisana w danym pliku - helperFile.dat*/
	QString getImagesFolderPath(QString helperFileName);

	/*funkcja zwracaj¹ca scie¿ki do obrazów tla kolejnych klatek*/
	vector<QString> getGlWidgetBackgroudFromFile(QString helperFileName);

	/*wczytanie nazw plikow konfiguracyjnych
	asfFilePath - sciezka do pliku asf
	datFilePath - sciezka do pliku dat
	imgFolderPath - sciezka do folderu ze zdjeciami 
	*/
	void getFilesAsQStrings(QString amcFilePath, QString &asfFilePath, QString &datFilesPaths, QString &imgFolderPath);

	/*wczytanie nazw plikow konfiguracyjnych
	asfFilePath - sciezki do plikow asf
	datFilePath - sciezki do plikow dat
	imgFolderPath - sciezka do folderu ze zdjeciami
	*/
	void getFilesAsQStrings(QString amcFilePath, vector<QString> &asfFilesPaths, vector<QString> &datFilePath, QString &imgFolderPath);
	
	/*funkcja sprawdzaj¹ca czy dane zdjecie tla zostalo wykorzystane wiecej niz raz, jesli tak do nazwy plikow .asf oraz .dat dodaje indeks np. asf(1).asf*/
	void checkExistingFiles(vector<QString> &asfFiles, vector<QString> &datFiles, QString asf, QString dat);

	/*funkcja zwraca wektor indeksow zdjec wykorzystanych do tworzenia modelu - wykorzystywana do wczytywania wczesniej zapisanej sekwencji*/
	vector<int> getImagesIDs(QFileInfoList allImagesList, vector<QString> loadedImages);

	/*dodanie kosci do modelu oraz pliku dat
	funkcja przyjmuje za parametry zmienne wybranego obiektu QOpenGlWidget oraz indeks kosci z pliku .asf
	*/
	void addBoneConf(vector<pf::boneConfig> &bonesConf, map<string, int> idxBonesMap, vector<pf::ASFBone> bones, int i);

	/*dodanie :geometry do pliku dat
	funkcja przyjmuje za parametry zmienne wybranego obiektu QOpenGlWidget oraz indeks kosci z pliku .asf
	*/
	void addBoneGeo(vector<pf::boneGeometry> &bonesGeometry, map<string, int> idxBonesMap, vector<pf::ASFBone> bones, int i);

	int findBoneIDByName(string name, vector<pf::ASFBone> asfBones);

	bool isBoneChecked(string name, vector<string> allBonesNames);

private:
	/*zapis wartosci label w pliku .dat*/
	void boneGeometryLabels(pf::boneGeometry &bGeo);

	/*zapis wartosci radius w pliku .dat*/
	void boneGeometryRadius(pf::boneGeometry &bGeo);

	/*zapis :geometry w pliku dat*/
	void saveDATGeometry(vector<pf::boneGeometry> bonesGeometry, QString datFileName);
};

