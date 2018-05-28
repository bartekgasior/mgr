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
#include <opencv2/opencv.hpp>
#include <stdio.h>

#include "asf.h"
#include "fileshandler.h"
#include "glwidget.h"
#include "FileHandler.h"
#include "bonemanagment.h"
#include "modelindialog.h"

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
    int globalListIterator = 1;
    int addFrameButtonCounter = 0;
	bool isAvi = false;

	// Human 3D Model
	pf::Model3D* mainModel;

	/*#################################################*/
	// zmienne modelu 3D
	// vector zawierający limity rotacji poszczególnych kości
	vector<pf::range2> limits;

	//rotacje
	vector<pf::Matrixf> rotations;

	//nazwy wykorzystanych kosci
	vector<string> usedBones;

	//nazwy wszystkich kosci
	vector<string> allBones;

	//stany modelu
	vector<vector<float>> modelState;

	//kosci
	vector<pf::ASFBone> asfBones;
	map<string, int> idxBonesMap;

	//
	vector<pf::boneConfig> bonesConf;
	vector<pf::boneGeometry> bonesGeometry;

	/*#################################################*/

	//sciezka do folderu ze zdjeciami
	QString *loadedImagesFolderPath;
	QVector<cv::Mat> aviFrames;

	/*menu*/
	QMenuBar *menuBar;
	QMenu *plikMenu;
	QMenu *saveMenu;
	QMenu *loadMenu;
	//QAction *loadImagesAction;
	//QAction *loadAviAction;
	QAction *loadFromAmcAction;
	QAction *saveAmcAction;
	QAction *saveAmcSequenceAction;
	QAction *manageModelBonesAction;
	
	/*signalmapper dla przyciskow obslugujacych iteracje tla kazdej z klatek*/
	QSignalMapper *increaseButtonSignalMapper;

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

	//lista pozwalajaca wybrac numer klatki, na ktorej model bedzie modyfikowany
	QComboBox *comboBoxFrameSelector;

	//interfejs klatek
    QVector<GLWidget*> glWidgetsVector; //glWidget
	QVector<QHBoxLayout*> horizontalGLWidgetMenuVector; //layout przechowujacy wszystkie elementy menu znajdujacego sie pod obiektem glwidget
	QVector<QWidget*> hMenuWidgetVector; //widget przechowujacy layout menu, wykorzystywany do metod show() i hide()
    QVector<QPushButton*> minButtonsVector; //przycisk przewijajacy zdjecie pierwszego elementu
    QVector<QPushButton*> reduceButtonsVector; //przycisk przewijajacy zdjecie do tylu
    QVector<QPushButton*> maxButtonsVector;  //przycisk przewijajacy zdjecie do przodu
    QVector<QPushButton*> increaseButtonsVector; //przycisk przewijajacy zdjecie ostatniego elementu
    QVector<QLabel*> countersVector; //wyswietlenie indeksu tla np. 1/14
    QVector<QSlider*> slidersVector; //slider dla kazdego okna
    QVector<int> imagesListIterator; //aktualny indeks
	QVector<QPushButton*> menuButtonsVector; //przycisk rozwijajacy menu
	QVector<QMenu*> frameScrollingMenu; //rozwijane menu
	QVector<QMenu*> selectBackgroundMenuVector; //wybor tla
	//QVector<QAction*> manageBonesVector; //edycja wykorzystywanytch kosci przy budowie modelu
	QVector<QAction*> showModelInDialogVector; //model w osobnym oknie
	QVector<QAction*> selectAviBackgroundVector; //wybor sciezki tla z avi
	QVector<QAction*> selectImagesBackgroundVector; //wybor sciezki tla z folderu zdjec

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

	//wektory zawierajace elementy bocznego menu kalibracji - translacja oraz skalowanie modelu
	QVector<QLabel*> sideLengthMenuLabelsExtraVector;
	QVector<QHBoxLayout*> sideLengthMenuHLayoutsExtraVector;
	QVector<QPushButton*> sideLengthMenuButtonsExtraVector;
	QVector<QLineEdit*> sideLengthMenuLineEditsExtraVector;
	QVector<QSpinBox*> sideLengthMenuSpinBoxesExtraVector;

    Ui::MainWindow *ui;
	BoneManagment *boneManagmentWindow;
	ModelInDialog *modelInDialog;

	/*Menu aplikacji*/
	void prepareMenus();
	void prepareLayouts();
	void prepareMainModel();
    void setTextIteratorLabel(QLabel *label, int iterator, int listSize);

	void prepareFrameMenu(int i);

	/*Dodanie okna do obslugi klatki w głownym oknie aplikacji*/
    void addFrameMenu(int i);

	/*Dodanie bocznego menu sluzacego do rotacji kosci o wybrany kat*/
	void addGLWidgetRotateMenu();

	/*Dodanie bocznego menu sluzacego do ustawienia dlugosci kosci*/
	void addGLWidgetLengthMenu();

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
	void setRotCheckBoxes(int id);

	void setLimitsSpinBoxes(int id);

	void setVelocitySpinBoxes(int id);

	/*wylaczenie przyciskow dla ktorych isRot nie jest wybrane*/
	void disableRotatingButtons();

    void mapIncreaseButtonSlot(int i);
    void mapReduceButtonSlot(int i);
    void mapMaxButtonSlot(int i);
    void mapMinButtonSlot(int i);
	void mapManageBonesSlot(int i);
	void mapShowModelInDialog(int i);
	void mapAddImagesToBackground(int i);
	void mapAddAviToBackground(int i);

private slots:
	/*zapis jednej klatki*/
	void saveOneFrameToFile();

	/*zapis sekwencji klatek*/
	void saveSequenceToFile();

	/*Wczytanie pliku amc*/
	void loadFromAmc();

	/*edycja kosci modelu*/
	void manageBonesPressed();

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

	/*translacja modelu
	string w postaci "direction;axis;qLineEditID;qSpinBoxID
	*/
	void translate(QString str);

	/*skalowanie modelu
	string w postaci "direction;qSpinBoxID
	*/
	void scale(QString str);

	/*przewijane klatek wybranego okna*/
	void increaseIteratorPressed(int i);
	void reduceIteratorPressed(int i);
	void maxIteratorPressed(int i);
	void minIteratorPressed(int i);

	//elementy rozwijanego menu klatki
	void showModelInDialogPressed(int i);
	void selectImagesBackgroundPressed(int i);
	void selectAviBackgroundPressed(int i);

	/*dodanie okna z modelem postaci*/
	void buttonAddFramePressed();

	/*slider*/
	void updateGlobalListIterator(int value);

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

	/*aktualizacja vectora boneConfig po wybraniu odpowiedniego chechboxa isRot*/
	void updateBoneConfigFromCB();

	/*aktualizacja vectora boneConfig po edycji limitow*/
	void updateBoneConfigFromLimitsSB();

	void updateBoneConfigFromVelocitySB();

	void tabSelected();
};

#endif // MAINWINDOW_H
