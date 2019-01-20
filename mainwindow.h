#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QVector>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QComboBox>
#include <QSpinBox>
#include <QSpacerItem>
#include <QSignalMapper>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <stdio.h>

#include "fileshandler.h"
#include "glwidget.h"
#include "FileHandler.h"
#include "bonemanagment.h"
#include "modelindialog.h"
#include "camerasconfig.h"
#include "selectmodelid.h"
#include "configdialog.h"
#include "MyException.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
	/*liczba okien*/
    int addFrameButtonCounter = 0;

	/*czy menu kamer zostalo wczesniej uzyte*/
	bool cameraFirstTime = true;

	/*################# config dialog ##################*/
	/*timer ticks*/
	int timerValue = 2000;

	/*czy zmieniac rozmiar tla*/
	bool resizeBackground = false;
	int backgroundWidth = 1900;
	int backgroundHeight = 1200;
	/*###################################*/

	/*czy usunięto okno (klatkę)*/
	bool frameDeleted = true;

	/*sprawdzenie czy po uruchomieniu aplikacji nie zostaly wprowadzone zmiany modelu*/
	bool justLaunched = true;

	/*zmienne do wczytywania modelu*/
	bool amcLoaded = false; //czy zaladowano plik amc
	QString amcPath;
	bool datLoaded = false;
	QString datPath;
	bool asfLoaded = false;
	QString asfPath;
	// Human 3D Model
	pf::Model3D* mainModel;
	//ModelHandler* modelHandler;

	/*#################################################*/
	// zmienne modelu 3D
	vector<pf::range2> limits;// vector zawierający limity rotacji poszczególnych kości
	vector<float> velocity;
	vector<pf::Matrixf> rotations;//rotacje
	vector<string> usedBones;//nazwy wykorzystanych kosci
	vector<string> allBones;//nazwy wszystkich kosci
	vector<vector<float>> modelState;//stany modelu
	
	vector<pf::ASFBone> asfBones;//kosci
	map<string, int> idxBonesMap;
	vector<pf::boneConfig> bonesConf;//dof, limits itp
	vector<pf::boneGeometry> bonesGeometry;//top radius, length itp

	/*mapy przechowuja wartosci wszystkich kosci - nawet tych nie uzywanych w danym momencie*/

	/*dlugosci kosci oraz ich nazwa*/
	map<string, float> bonesLength;

	/*limity obrotow danej kosci
	np. LeftLeg 50, 50, 50, 50, 50, 50 -> odpowiednio limity w osi X, Y, Z
	*/
	map<string, vector<int>> modelLimits;

	/* velocity -> LeftLeg 0, 10, 15*/
	map<string, vector<float>> modelVelocity;

	/* dof -> np. LeftLeg true, true, false */
	map<string, vector<bool>> modelDOF;

	/*radius -> LeftLeg, topRadius1, topRadius2, bottomRadius1, bottomRadius2*/
	map<string, vector<float>> modelRadius;

	/*wektor o rozmiarze modelState, gdy true zapisuje konfiguracje*/
	vector<bool> saveModelState;

	/*obroty wszystkich kosci wraz z nazwa*/
	vector<map<string, pf::Vec3f>> bonesRotations;

	/*przesuniecie modelu*/
	vector<vector<float>> modelTranslation;
	/*#################################################*/

	/*menu*/
	QMenuBar *menuBar;
	QMenu *plikMenu;
	QMenu *saveMenu;
	QMenu *loadMenu;
	QMenu *cameraMenu;

	/*odczyt, zaspis plikow*/
	QAction *saveDatAction;
	QAction *loadDatAction;

	QAction *loadAsfAction;
	QAction *saveAsfAction;

	QAction *loadFromAmcAction;
	QAction *saveAmcSequenceAction;

	/*wczytaj, zapisz projekt*/
	QAction *loadConfigAction;
	QAction *saveConfigAction;

	/*zarzadaj koscmi modelu*/
	QAction *manageModelBonesAction;

	/*to usunac*/
	QAction *setModelAction;
	/*camera*/
	QAction *openCameraConfigAction;
	QAction *saveCamerasAction;
	QAction *loadCamerasAction;

	/*Help - About*/
	QAction *helpAction;

	/*config*/
	QAction *configAction;
    
    QPushButton *addFrameButton;
	
	QHBoxLayout *selectFrameLayout;
	QLabel *selectFrameLabel;
	QComboBox *comboBoxFrameSelector;//lista pozwalajaca wybrac numer klatki, na ktorej model bedzie modyfikowany
	QPushButton *setIteratorButton;//ustawienie numeru klatki wybranego okna dla wszystkich pozostalych
	QPushButton *saveCurrentModelStateToVector;//po wcisnieciu przycisku wartosc elementu wektora przechowujacego zmienne wymagane do zapisu klatek (false - nie zapisuj, true - zapisz) zostaje zmieniona na true
											 
	/*widgety do obslugi glownego slidera*/
	QHBoxLayout *sliderLayout;
	QSlider *globalSlider;
	QPushButton *increaseGlobalSlider;
	QPushButton *reduceGlobalSlider;

    QGridLayout *gridLayout;//głowny layout aplikacji
    QVBoxLayout *vboxRotateLayout; //boczny panel - rotacja modelu
	QVBoxLayout *vboxLengthLayout; //boczny panel - kalbracja dlugosci kosci
	QVBoxLayout *vboxGeometryLayout; //boczny panel - geometry
    QGridLayout *gridChildLayout; // panel poziomy - klatki
	
    QScrollArea *scrollArea;//widget umozliwiajacy scrollowanie głównego layoutu
	QScrollArea *scrollRotateSideArea, *scrollLengthSideArea, *scrollGeometrySideArea;//widget umozliwiajacy scrollowanie bocznego layoutu
    QWidget *scrollWidget;//widget do ktorego dodawane sa elementy dodawanego po wcisnieciu buttona
	QWidget *scrollRotateSideWidget, *scrollLengthSideWidget, *scrollGeometrySideWidget;
    QWidget *windowLayout;//widget do ktorego dodawany jest gridLayout
	QWidget *tabWidget;//boczne menu - length, rotate, geometry
	QTabWidget *sideManuTabWidget; //widget zawierajacy menu poszczegolnych klatek
	
	/*wektor kamer*/
	QVector<pf::Camera*> cameras;
	QVector<bool> useQuat;//0-degrees, 1-quat

	/*golden ratio*/
	QGroupBox *goldenRatioGroupBox;
	QPushButton *setGoldenRatioButton;
	QCheckBox *setGoldenRatioCheckBox;
	QHBoxLayout *goldenRatio1RowLayout;
	QHBoxLayout *goldenRatio2RowLayout;
	QVBoxLayout *goldenRatioLayout;
	QLabel *goldenRatioHeightLabel;
	QDoubleSpinBox *goldenRatioSpinBox;

	//interfejs klatek
    QVector<GLWidget*> glWidgetsVector; //glWidget
	QVector<QHBoxLayout*> glWidgetLayoutVector; //layout glWidgetu - potrzebny do scrollowania
	QVector<QScrollArea*> glWidgetScrollAreaVector; //obszar scrollowania widgetu
	QVector<QWidget*> glWidgetsWidgetVector; //widget przechowujacy 2 powyzsze elementy
	QVector<QWidget*> bordersWidgetsVector; //ramka interfejsu
	QVector<QLabel*> frameIDVector; //indeks okna
	QVector<QHBoxLayout*> horizontalGLWidgetMenuVector; //layout przechowujacy wszystkie elementy menu znajdujacego sie pod obiektem glwidget
	QVector<QWidget*> hMenuWidgetVector; //widget przechowujacy layout menu
    QVector<QPushButton*> minButtonsVector; //przycisk przewijajacy zdjecie do pierwszego elementu
    QVector<QPushButton*> reduceButtonsVector; //przycisk przewijajacy zdjecie do tylu
    QVector<QPushButton*> maxButtonsVector;  //przycisk przewijajacy zdjecie do ostatniego elementu
    QVector<QPushButton*> increaseButtonsVector; //przycisk przewijajacy zdjecie do przodu
	QVector<QPushButton*> playPauseButtonsVector; //przycisk uruchamiajacy automatyczne przewijanie zdjec
	QVector<bool> playPressedVector; //czy przycisk play zostal wcisniety
    QVector<QLabel*> countersVector; //wyswietlenie indeksu tla (np. 1/14)
    QVector<QSlider*> slidersVector; //slider dla kazdego okna
	QVector<QCheckBox*> sliderCheckBoxesVector; //chekcbox laczacy slider z glownym suwakiem aplikacji
    QVector<int> imagesListIterator; //aktualny indeks
	QVector<QPushButton*> menuButtonsVector; //przycisk rozwijajacy menu
	QVector<QHBoxLayout*> menuButtonsLayoutVector; //layout umozliwiajacy dodanie spacerow do przycisku menu

	QVector<QMenu*> frameScrollingMenu; //rozwijane menu
	QVector<QMenu*> selectBackgroundMenuVector; //wybor tla
	QVector<QMenu*> selectCameraMenuVector; //wybor kamery
	QVector<QMenu*> resizeGLWidgetVector; //rozmiar glwidget
	//QVector<QAction*> showModelInDialogVector; //model w osobnym oknie
	QVector<QAction*> selectAviBackgroundVector; //wybor sciezki tla z avi
	QVector<QAction*> selectImagesBackgroundVector; //wybor sciezki tla z folderu zdjec
	//QVector<QAction*> setConfigFromModelVector; //konfiguracja modelu pobrana z wybranego obiektu
	//QVector<QAction*> setConfigToAllModelsVector; //ustawienie konfiguracji modelu do wszystkich pozostalych okien
	QVector<QAction*> deleteFrameMenuVector; //usuniecie okna wybranej klatki
	QVector<QVector<QAction*>> selectCameraActionVector; //dostepne kamery
	QVector<QTimer*> backgroundTimersVector; //timer sluzacy do automatycznego przewijania tla okna
	QVector<QAction*> resizeDownActionVector; //pomniejsz glwidget
	QVector<QAction*> resizeUpActionVector; //powieksz glwidget

	/*qSignalMappers dla przyciskow*/
	/*menu kazdego okna*/
	QVector<QSignalMapper*> increaseButtonMapperVector;
	QVector<QSignalMapper*> reduceButtonMapperVector;
	QVector<QSignalMapper*> minButtonMapperVector;
	QVector<QSignalMapper*> maxButtonMapperVector;
	QVector<QSignalMapper*> playPauseButtonMapperVector;
	QVector<QSignalMapper*> timerMapperVector;
	//QVector<QSignalMapper*> showModelInDialogMapperVector;
	//QVector<QSignalMapper*> setConfigFromModelMapperVector;
	QVector<QSignalMapper*> addImagesBackgroundMapperVector;
	QVector<QSignalMapper*> addAviMapperVector;
	QVector<QSignalMapper*> deleteFrameMapperVector;
	//QVector<QSignalMapper*> setConfigToAllModelsMapperVector;
	QVector<QSignalMapper*> sliderMapperVector;
	QVector<QVector<QSignalMapper*>> selectCameraMapperVector;
	QVector<QSignalMapper*> resizeDownGLWidgetMapperVector;
	QVector<QSignalMapper*> resizeUpGLWidgetMapperVector;

	/*boczne menu*/
	QVector<QSignalMapper*> rotateMapperVector;
	QVector<QSignalMapper*> rotateFromLineEditMapperVector;
	QVector<QSignalMapper*> lengthMapperVector;
	QVector<QSignalMapper*> lengthFromLineEditMapperVector;
	QVector<QSignalMapper*> geometryMapperVector;
	QVector<QSignalMapper*> geometryLengthFromLineEditMapperVector;
	QVector<QSignalMapper*> geometryTopRadiusFromLineEditMapperVector;
	QVector<QSignalMapper*> geometryBottomRadiusFromLineEditMapperVector;
	QVector<QSignalMapper*> translateFromLineEditMapperVector;

	//wektory zawierajace elementy bocznego menu rotacji
	QVector<QLabel*> sideRotateMenuLabelsVector; 
	QVector<QHBoxLayout*> sideRotateMenuHLayoutsVector;
	QVector<QPushButton*> sideRotateMenuButtonsVector;
	QVector<QLineEdit*> sideRotateMenuLineEditsVector;
	QVector<QSpinBox*> sideRotateMenuSpinBoxesVector;

	//wektory zawierajace elementy bocznego menu kalibracji
	QVector<QLabel*> sideLengthMenuLabelsVector;
	QVector<QHBoxLayout*> sideLengthMenuHLayoutsVector;
	QVector<QPushButton*> sideLengthMenuButtonsVector;
	QVector<QLineEdit*> sideLengthMenuLineEditsVector;
	QVector<QSpinBox*> sideLengthMenuSpinBoxesVector;
	QVector<QLabel*> sideLengthMenuRotLabelsVector;
	QVector<QCheckBox*> sideLengthMenuRotCheckBoxesVector;
	QVector<QLabel*> sideLengthMenuLimVelLabelsVector;
	QVector<QSpinBox*> sideLengthMenuLimVelSpinBoxesVector;

	//wektory zawierajace elementy bocznego menu kalibracji - ustawienia kamery, translacja oraz skalowanie modelu
	//QComboBox *sideLengthMenuCameraComboBox;
	//QLabel *sideLengthMenuCameraLabels;
	//QCheckBox *sideLengthMenuCameraCheckBox;

	QVector<QLabel*> sideLengthMenuLabelsExtraVector;
	QVector<QHBoxLayout*> sideLengthMenuHLayoutsExtraVector;
	QVector<QPushButton*> sideLengthMenuButtonsExtraVector;
	QVector<QLineEdit*> sideLengthMenuLineEditsExtraVector;
	QVector<QSpinBox*> sideLengthMenuSpinBoxesExtraVector;

	//wektory zawierajace elementy bocznego menu geometry
	QVector<QLabel*> sideGeometryMenuLabelsVector;
	QVector<QHBoxLayout*> sideGeometryMenuHLayoutsVector;
	QVector<QPushButton*> sideGeometryMenuButtonsVector;
	QVector<QLineEdit*> sideGeometryMenuLineEditsVector;
	QVector<QSpinBox*> sideGeometryMenuSpinBoxesVector;

    Ui::MainWindow *ui;

	/*Menu aplikacji*/
	void prepareMenus();
	void prepareLayouts();
	void prepareMainModel();

	/*ustaw label(wyswietlajacy id) wybranego okna*/
    void setTextIteratorLabel(QLabel *label, int iterator, int listSize);

	/*funkcja mapujaca przyciski kazdego okna*/
	void prepareFrameMenu(int i);

	/*zapsz kamery do pliku*/
	void saveCameras(QString filename);

	/*wczytaj kamery z pliku*/
	void loadCameras(QString fileName);

	/*zapisz projekt*/
	void saveProject(QString fileName, QString asfFilePath, QString amcFilePath, QString datFilePath, QString camerasFilePath);

	/*wczytaj projekt*/
	void loadProject(QString fileName);

	/*Dodanie okna do obslugi klatki w głownym oknie aplikacji*/
    void addFrameMenu(int i);

	/*Dodanie bocznego menu sluzacego do rotacji kosci o wybrany kat*/
	void addGLWidgetRotateMenu();

	/*Dodanie bocznego menu sluzacego do ustawienia dlugosci kosci*/
	void addGLWidgetLengthMenu();

	/*Dodanie bocznego menu sluzacego do ustawienia geometry*/
	void addGLWidgetGeometryMenu();

	/*wyczysczenie wartości zmiennych bocznego menu - length,rotate,geometry*/
	void deleteSideMenus();

	/*dodanie funkcji(akcji) do menu klatki*/
	void addFrameScrollingMenu(QPushButton *button, int id);

	/*ustawienia slidera kazdego okna*/
	void prepareSlider(QSlider *slider, int i);

	/*ustawienie parametrów qspinbox*/
	void setQSpinBoxSettings(QSpinBox * qspinbox, int minRange, int maxRange, int defaultValue);

	/*ustawienie checkboxow isRot*/
	void setRotCheckBoxes();

	/*ustawienie wartosci limits w menu*/
	void setLimitsSpinBoxes();

	/*ustawienie wartosci velocity w menu*/
	void setVelocitySpinBoxes();

	/*ustawienie menu kamer*/
	void setCamerasMenu();

	/*ustawienie menu kamer w sytuacji gdy kamery zostaly wczesniej wczytane i uzytkownik chce dodac kolejne okno*/
	void addCameraMenuToFrame();

	/*wylaczenie przyciskow dla ktorych isRot nie jest wybrane*/
	void disableRotatingButtons();

	/*dodanie obiektow QSignalMapper do odpowiednich wektorow, funkcja wywolywana po dodaniu nowego okna*/
	void addFrameMenuMappers(int id);

	/*dodanie zdjec do przyciskow menu kazdego z okien*/
	void addFrameMenuButtonsImages(QPushButton *minButton, QPushButton *reduceButton, QPushButton *increaseButton, QPushButton *maxButton, QPushButton *playButton);

	/*dodanie zdjec do przyskow bocznego menu*/
	void addSideMenuButtonsImages(QPushButton *substractButton, QPushButton *addButton);

	/*zdjecia do przyciskow slidera*/
	void addGlobalSliderButtonsImages(QPushButton *reduceButton, QPushButton *increaseButton);

	/*po wczytaniu klatek tla do okna nr 0 - ustaw wielkosc wektora modelState na rowna liczbie klatek*/
	void updateModelStateVectorSize(int frames);

	/*odswiezenie modelu na glwidget*/
	void updateGLWidgetDrawning(int i);

	/*ustawienie modelState wybranego okna na 0 -- wykorzystywane gdy iterator przekracza max pierwszego okna*/
	void setGLWidgetDrawningToFirstModelConfigurtion(int i);

	/*sprawdzenie czy indeks przekracza ilosc wczytanych obrazow pierwszego okna*/
	bool checkIteratorRange(int i);

	/*ustaw aktualny modelState okna*/
	void setGLWidgetDrawing(int i);

	/*usuniecie layoutu przechowujacego poszczegolne okna*/
	void deleteGridChildLayout();

	/*rezerwacja pamieci wektorow*/
	void reserveVectorsMemory();

	/*inicjalizacja map kosci*/
	void initializeMaps();

	/*funkcje mapujace przyciski*/
    void mapIncreaseButtonSlot(int i);
    void mapReduceButtonSlot(int i);
    void mapMaxButtonSlot(int i);
    void mapMinButtonSlot(int i);
	void mapPlayPauseButtonSlot(int i);
	void mapManageBonesSlot(int i);
	void mapShowModelInDialog(int i);
	void mapAddImagesToBackground(int i);
	void mapAddAviToBackground(int i);
	void mapSetConfigFromModel(int i);
	void mapSetConfigToAllModels(int i);
	void mapDeleteFrame(int i);
	void mapTimerSlot(int i);
	void mapResizeDownGLWidget(int i);
	void mapResizeUpGLWidget(int i);

	/*zaladowanie tla zdjec po 'wrzuceniu' pliku*/
	void loadImagesBackgroundDropped(int i);
	void loadAviDropped(int i);
	void loadDatDropped(QString str);
	void loadProjectDropped(QString fileName);

	/*wczytanie plikow konfiguracyjnych po wrzuceniue do aplikacji*/
	void loadFromDroppedAMC(QString str);
	void loadFromDroppedASF(QString str);
	void loadCamerasFromDroppedINI(QString str);

	void dragEnterEvent(QDragEnterEvent *e);
	void dragLeaveEvent(QDragLeaveEvent *e);
	void dropEvent(QDropEvent *e);

	/*funkcje pomocnicze do wczytywania plikow*/
	void loadDAT(QString fileName);
	void loadASF(QString filaName);
	void loadAMC(QString fileName);

	/*sprawdzenie czy wczytano kamery czy projekt*/
	int checkINIFile(QString fileName);

private slots:

	/*zapis modelu .dat*/
	void saveDatToFile();

	/*zapis modelu .dat do wybranej sciezki*/
	void saveDatFromQStringToFile(QString fileName);

	/*odczyt modelu*/
	void loadDatFromFile();

	/*zapis sekwencji klatek*/
	void saveAMCToFile();

	/*zapis sekwencji klatek do wybranej sciezki*/
	void saveAMCFromQStringToFile(QString fileName);

	/*Wczytanie pliku amc*/
	void loadFromAmc();

	/*zapisz pliku asf*/
	void saveAsfToFile();

	/*zapisz pliku asf do wybranej sciezki*/
	void saveAsfFromQStringToFile(QString fileName);

	/*wczytanie pliku asf*/
	void loadFromASF();

	/*edycja wyswietlanych kosci modelu*/
	void manageBonesPressed();

	/*konfiguracja kamery*/
	void openCameraConfigPressed();

	/*zapis kamer do pliku*/
	void saveCamerasPressed();

	/*odczyt pliku kamer*/
	void loadCamerasPressed();

	/*zapis stanu aplikacji*/
	void saveProjectPressed();

	/*wczytanie aplikacji*/
	void loadProjectPressed();

	/*config - rodzielczosc tla, timer, rozmiar stawow*/
	void configPressed();

	/*help*/
	void helpPressed();

	/*wcisniecie przycisku powoduje zapis true do wektora sprawdzajacego czy zapisac dana konfiguracje modelu*/
	void saveModelStateToBoolVector();

	/*Operacje na modelu*/
	/*rotacja wybranego elementu modelu
	funkcja przyjmuje string w postaci "boneName;direction;axis;qLineEditID;spinBoxID" np. "root;-1.0f;X;0;0"
	direction = 1.0 || direction = -1.0
	axis = "X" || axis = "Y" || axis = "Z"
	qLineEditID >= 0 && qLineEditID <= bones.size()*3
	qSpinBoxID >= 0 && qSpinBoxID <= bones.size()*3
	*/
	void rotate(QString str);

	/*ustaw rotacje przez zmiane lineEdit
	str:
	boneName,
	axis,
	qLineEditID
	*/
	void setRotate(QString str);

	/*zmiana dlugosci kosci
	string w postaci "boneName;direction;qLineEditID;qSpinBoxID"
	direction = 1.0 || direction = -1.0
	qLineEditID >= 0 && qLineEditID <= bones.size()
	qSpinBoxID >= 0 && qSpinBoxID <= bones.size()
	*/
	void updateBoneLength(QString str);

	/*ustaw dlugosc kosci na podstawie wartosci lineedit o id i
	str:
	boneName,
	qLineEditId
	*/
	void setBoneLengthFromLineEdit(QString str);

	/*zmiana dlugosci kosci
	string w postaci "boneName;direction;qLineEditID;qSpinBoxID"
	direction = 1.0 || direction = -1.0
	qLineEditID >= 0 && qLineEditID <= bones.size()
	qSpinBoxID >= 0 && qSpinBoxID <= bones.size()
	*/
	void updateBoneLengthGeometry(QString str);

	/*ustaw dlugosc kosci na podstawie wartosci lineedit o id i
	str:
	boneName,
	qLineEditId
	*/
	void setBoneLengthGeometryFromLineEdit(QString str);

	/*zmiana topRadius kosci
	string w postaci "boneName;direction;qLineEditID;qSpinBoxID"
	direction = 1.0 || direction = -1.0
	qLineEditID >= 0 && qLineEditID <= bones.size()
	qSpinBoxID >= 0 && qSpinBoxID <= bones.size()
	*/
	void updateBoneTopRadius(QString str);

	/*ustaw topRadius kosci na podstawie wartosci lineedit o id i
	str:
	boneName,
	qLineEditId
	*/
	void setBoneTopRadiusFromLineEdit(QString str);

	/*zmiana bottomRadius kosci
	string w postaci "boneName;direction;qLineEditID;qSpinBoxID"
	direction = 1.0 || direction = -1.0
	qLineEditID >= 0 && qLineEditID <= bones.size()
	qSpinBoxID >= 0 && qSpinBoxID <= bones.size()
	*/
	void updateBoneBottomRadius(QString str);

	/*ustaw bottomRadius kosci na podstawie wartosci lineedit o id i
	str:
	boneName,
	qLineEditId
	*/
	void setBoneBottomRadiusFromLineEdit(QString str);

	/*translacja modelu
	string w postaci "direction;axis;qLineEditID;qSpinBoxID
	*/
	void translate(QString str);

	/*ustaw przesuniecie modelu podstawie wartosci lineedit o id i
	str:
	axis,
	qLineEditId
	*/
	void setTranslate(QString str);

	/*funkcja ustawiajaca kamere dla danego okna*/
	void selectCamera(QString str);

	/*aktualizacja suwakow okien*/
	void updateSliders();

	/*przewijane klatek wybranego okna*/
	void increaseIteratorPressed(int i);
	void reduceIteratorPressed(int i);
	void maxIteratorPressed(int i);
	void minIteratorPressed(int i);
	void playPausePressed(int i);

	//elementy rozwijanego menu klatki
	void selectImagesBackgroundPressed(int i);
	void selectAviBackgroundPressed(int i);
	void resizeDownGLWidgetPressed(int i);
	void resizeUpGLWidgetPressed(int i);

	/*usuniecie okna*/
	void deleteFramePressed(int i);

	/*dodanie okna z modelem postaci*/
	void buttonAddFramePressed();

	/* aktualziacja iteratora klatki*/
	void updateFrameIterator(int i);

	/*zaladowanie bocznego menu po zmianach*/
	void reloadGLWidgetMenu();

	/*zaladowanie menu rotacji*/
	void reloadGLRotateMenu();

	/*odswiezenie wartosci w polach qlineedit po rotacji kosci*/
	void refreshRotateSideMenu();

	/*odswiezenie wartosci w polach qlineedit po edycji dlugosci */
	void refreshLengthSideMenu();

	/*odswiezenie wartosci w polach qlineedit po edycji dlugosci lub wartosci katow*/
	void refreshGeometrySideMenu();

	/*aktualizacja vectora boneConfig po wybraniu odpowiedniego chechboxa isRot*/
	void updateBoneConfigFromCB();

	/*aktualizacja vectora boneConfig po edycji limitow*/
	void updateBoneConfigFromLimitsSB();

	void updateBoneConfigFromVelocitySB();

	/*funkcje wywolywane przy zmianie zakladki bocznego menu*/
	void tabSelected();

	/*wylaczenie limits dla ktorych isRot nie jest wybrane*/
	void disableLimitsSpinBoxes();

	/*przewijanie tla dla wybranego okna*/
	void playBackground(int i);

	/*stosunek dlugosci kosci = golden ratio*/
	void setGoldenRatio();

	/*po zmianie wysokosci modelu - ustaw nowy stosunek golden ratio*/
	void setGoldenRatioFromSpinBox();

	/*ustawienie aktualnej klatki dla pozostalych okien*/
	void setIteratorToFrames();

	/*zmniejsz wartosc glownego slidera o 1*/
	void reduceGlobalSliderPressed();

	/*zwieksz wartosc glownego slidera o 1*/
	void increaseGlobalSliderPressed();
};

#endif // MAINWINDOW_H
