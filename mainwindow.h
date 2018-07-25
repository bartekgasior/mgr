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

#include "asf.h"
#include "fileshandler.h"
#include "glwidget.h"
#include "FileHandler.h"
#include "bonemanagment.h"
#include "modelindialog.h"
#include "camerasconfig.h"
#include "selectmodelid.h"

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
    int addFrameButtonCounter = 0;
	bool isAvi = false;
	bool cameraFirstTime = true;
	/*czy usunięto okno (klatkę)*/
	bool frameDeleted = true;
	/*sprawdzenei czy po uruchomieniu aplikacji nie zostaly wprowadzone zmiany modelu*/
	bool justLaunched = true;
	// Human 3D Model
	pf::Model3D* mainModel;
	ModelHandler modelHandler;

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
	vector<pf::boneConfig> bonesConf;
	vector<pf::boneGeometry> bonesGeometry;
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

	QAction *loadFromAmcAction;
	QAction *saveAllAmcSequenceAction;
	QAction *saveAmcSequenceAction;
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
	/*signalmapper dla przyciskow obslugujacych iteracje tla kazdej z klatek*/
	//QSignalMapper *increaseButtonSignalMapper;

    QFileInfoList list;
    QSlider *globalSlider;
    QPushButton *addFrameButton;
	QLabel *selectFrameLabel;

    QGridLayout *gridLayout;//głowny layout aplikacji
    QVBoxLayout *vboxRotateLayout; //boczny panel - rotacja modelu
	QVBoxLayout *vboxLengthLayout; //boczny panel - kalbracja dlugosci kosci
	QVBoxLayout *vboxGeometryLayout; //boczny panel - geometry
    QGridLayout *gridChildLayout; // panel poziomy - klatki
	QHBoxLayout *selectFrameLayout;

    QScrollArea *scrollArea;//widget umozliwiajacy scrollowanie głównego layoutu
	QScrollArea *scrollRotateSideArea, *scrollLengthSideArea, *scrollGeometrySideArea;//widget umozliwiajacy scrollowanie bocznego layoutu
    QWidget *scrollWidget;//widget do ktorego dodawane sa elementy dodawanego po wcisnieciu buttona
	QWidget *scrollRotateSideWidget, *scrollLengthSideWidget, *scrollGeometrySideWidget;
    QWidget *windowLayout;//widget do ktorego dodawany jest gridLayout
	QWidget *tabWidget;
	QTabWidget *sideManuTabWidget; //widget zawierajacy menu poszczegolnych klatek
	QPushButton *saveCurrentModelStateToVector;//po wcisnieciu przycisku wartosc elementu wektora przechowujacego zmienne wymagane do zapisu klatek (false - nie zapisuj, true - zapisz) zostaje zmieniona na true
	//lista pozwalajaca wybrac numer klatki, na ktorej model bedzie modyfikowany
	QComboBox *comboBoxFrameSelector;

	/*wektor kamer*/
	QVector<pf::Camera*> cameras;
	ModelInDialog *modelInDialog;

	/*golden ratio*/
	QPushButton *setGoldenRatioButton;
	QCheckBox *setGoldenRatioCheckBox;
	QHBoxLayout *goldenRatioLayout;

	//interfejs klatek
    QVector<GLWidget*> glWidgetsVector; //glWidget
	QVector<QWidget*> bordersWidgetsVector; //ramka interfejsu
	QVector<QLabel*> frameIDVector; //indeks okna
	QVector<QHBoxLayout*> horizontalGLWidgetMenuVector; //layout przechowujacy wszystkie elementy menu znajdujacego sie pod obiektem glwidget
	QVector<QWidget*> hMenuWidgetVector; //widget przechowujacy layout menu
    QVector<QPushButton*> minButtonsVector; //przycisk przewijajacy zdjecie pierwszego elementu
    QVector<QPushButton*> reduceButtonsVector; //przycisk przewijajacy zdjecie do tylu
    QVector<QPushButton*> maxButtonsVector;  //przycisk przewijajacy zdjecie ostatniego elementu
    QVector<QPushButton*> increaseButtonsVector; //przycisk przewijajacy zdjecie do przodu
	QVector<QPushButton*> playPauseButtonsVector; //przycisk uruchamiajacy automatyczne przewijanie zdjec
	QVector<bool> playPressedVector; //czy przycisk play zostal wcisniety
    QVector<QLabel*> countersVector; //wyswietlenie indeksu tla np. 1/14
    QVector<QSlider*> slidersVector; //slider dla kazdego okna
	QVector<QCheckBox*> sliderCheckBoxesVector; //chekcbox laczacy slider z glownym suwakiem aplikacji
    QVector<int> imagesListIterator; //aktualny indeks
	QVector<QPushButton*> menuButtonsVector; //przycisk rozwijajacy menu
	QVector<QHBoxLayout*> menuButtonsLayoutVector; //layout umozliwiajacy dodanie spacerow do przycisku
	QVector<QMenu*> frameScrollingMenu; //rozwijane menu
	QVector<QMenu*> selectBackgroundMenuVector; //wybor tla
	QVector<QMenu*> selectCameraMenuVector; //wybor kamery
	QVector<QAction*> showModelInDialogVector; //model w osobnym oknie
	QVector<QAction*> selectAviBackgroundVector; //wybor sciezki tla z avi
	QVector<QAction*> selectImagesBackgroundVector; //wybor sciezki tla z folderu zdjec
	QVector<QAction*> setConfigFromModelVector; //konfiguracja modelu pobrana z wybranego obiektu
	QVector<QAction*> setConfigToAllModelsVector; //ustawienie konfiguracji modelu do wszystkich pozostalych okien
	QVector<QAction*> deleteFrameMenuVector; //usuniecie okna wybranej klatki
	QVector<QVector<QAction*>> selectCameraActionVector; //dostepne kamery
	QVector<QTimer*> backgroundTimersVector; //timer sluzacy do automatycznego przewijania tla okna

	/*qSignalMappers dla przyciskow*/
	/*menu kazdego okna*/
	QVector<QSignalMapper*> increaseButtonMapperVector;
	QVector<QSignalMapper*> reduceButtonMapperVector;
	QVector<QSignalMapper*> minButtonMapperVector;
	QVector<QSignalMapper*> maxButtonMapperVector;
	QVector<QSignalMapper*> playPauseButtonMapperVector;
	QVector<QSignalMapper*> timerMapperVector;
	QVector<QSignalMapper*> showModelInDialogMapperVector;
	QVector<QSignalMapper*> setConfigFromModelMapperVector;
	QVector<QSignalMapper*> addImagesBackgroundMapperVector;
	QVector<QSignalMapper*> addAviMapperVector;
	QVector<QSignalMapper*> deleteFrameMapperVector;
	QVector<QSignalMapper*> setConfigToAllModelsMapperVector;
	QVector<QSignalMapper*> sliderMapperVector;
	QVector<QVector<QSignalMapper*>> selectCameraMapperVector;
	/*boczne menu*/
	QVector<QSignalMapper*> rotateMapperVector;
	QVector<QSignalMapper*> lengthMapperVector;
	QVector<QSignalMapper*> geometryMapperVector;

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
	QComboBox *sideLengthMenuCameraComboBox;
	QLabel *sideLengthMenuCameraLabels;
	QCheckBox *sideLengthMenuCameraCheckBox;

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
    void setTextIteratorLabel(QLabel *label, int iterator, int listSize);

	void prepareFrameMenu(int i);

	void saveCameras(QString filename);

	void loadCameras(QString fileName);

	/*Dodanie okna do obslugi klatki w głownym oknie aplikacji*/
    void addFrameMenu(int i);

	/*Dodanie bocznego menu sluzacego do rotacji kosci o wybrany kat*/
	void addGLWidgetRotateMenu();

	/*Dodanie bocznego menu sluzacego do ustawienia dlugosci kosci*/
	void addGLWidgetLengthMenu();

	/*Dodanie bocznego menu sluzacego do ustawienia geometry*/
	void addGLWidgetGeometryMenu();

	/*wyczysczenie wartości zmiennych*/
	void resetVariables();

	/*ukrycie elementów głownego layoutu*/
	void hideLayouts();

	/*odkrycie elementów głownego layoutu*/
	void showLayouts();

	/*dodanie funkcji do menu klatki*/
	void addFrameScrollingMenu(QPushButton *button, int id);

	/*ustawienia slidera kazdego okna*/
	void prepareSlider(QSlider *slider, int i);

	/*ustawienie parametrów qspinbox*/
	void setQSpinBoxSettings(QSpinBox * qspinbox, int minRange, int maxRange, int defaultValue);

	/*ustawienie checkboxow isRot*/
	void setRotCheckBoxes();

	void setLimitsSpinBoxes();

	void setVelocitySpinBoxes();

	/*ustawienie menu kamer*/
	void setCamerasMenu();

	/*ustawienie menu kamer w sytuacji gdy kamery zostaly wczesniej wczytane i uzytkownik chce dodac kolejne okno*/
	void addCameraMenuToFrame();

	/*wylaczenie przyciskow dla ktorych isRot nie jest wybrane*/
	void disableRotatingButtons();

	/*odswiezenie layoutu po usunieciu jednego okna (katki)*/
	void refreshGridChildLayout();

	/*dodanie obiektow QSignalMapper do odpowiednich wektorow, funkcja wywolywana po dodaniu nowego okna*/
	void addFrameMenuMappers(int id);

	/*dodanie zdjec do przyciskow menu kazdego z okien*/
	void addFrameMenuButtonsImages(QPushButton *minButton, QPushButton *reduceButton, QPushButton *increaseButton, QPushButton *maxButton, QPushButton *playButton);

	/*dodanie zdjec do przyskow bocznego menu*/
	void addSideMenuButtonsImages(QPushButton *substractButton, QPushButton *addButton);

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

private slots:

	/*zapis modelu .dat*/
	void saveDatToFile();

	/*odczyt modelu*/
	void loadDatFromFile();

	/*zapis wszystkich sekwencji klatek*/
	void saveAllFramesSequenceToFile();

	/*zapis sekwencji klatek*/
	void saveOneFrameSequenceToFile();

	/*Wczytanie pliku amc*/
	void loadFromAmc();

	/*edycja kosci modelu*/
	void manageBonesPressed();

	/*konfiguracja kamery*/
	void openCameraConfigPressed();

	/*zapis kamer do pliku*/
	void saveCamerasPressed();

	/*odczyt pliku kamer*/
	void loadCamerasPressed();

	/*kalibracja glownego modelu aplikacji*/
	void setModelPressed();

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

	/*zmiana dlugosci kosci
	string w postaci "boneName;direction;qLineEditID;qSpinBoxID"
	direction = 1.0 || direction = -1.0
	qLineEditID >= 0 && qLineEditID <= bones.size()
	qSpinBoxID >= 0 && qSpinBoxID <= bones.size()
	*/
	void updateBoneLength(QString str);

	void updateBoneLengthGeometry(QString str);

	void updateBoneTopRadius(QString str);

	void updateBoneBottomRadius(QString str);

	/*translacja modelu
	string w postaci "direction;axis;qLineEditID;qSpinBoxID
	*/
	void translate(QString str);

	/*skalowanie modelu
	string w postaci "direction;qSpinBoxID
	*/
	void scale(QString str);

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
	void showModelInDialogPressed(int i);
	void selectImagesBackgroundPressed(int i);
	void selectAviBackgroundPressed(int i);
	void setConfigFromModelPressed(int i);
	void setConfigToAllModelsPressed(int i);

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

	/*odswiez model po zmianie rodzaju kamery*/
	void updateModelAfterCameraComboBoxChanged();

	/*dla zakladki geometry rysuj prostopadlosciany*/
	void tabSelected();

	/*wylaczenie limits dla ktorych isRot nie jest wybrane*/
	void disableLimitsSpinBoxes();

	/*instrukcje po wylaczeniu okna konfiguracji modelu w osobnym oknie*/
	void deleteModelInDialog();

	/*przewijanie tla dla wybranego okna*/
	void playBackground(int i);

	/*stosunek dlugosci kosci = golden ratio*/
	void setGoldenRatio();
};

#endif // MAINWINDOW_H
