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

#include "asf.h"
#include "fileshandler.h"
#include "glwidget.h"
#include "FileHandler.h"
#include "bonemanagment.h"

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

	//sciezka do folderu ze zdjeciami
	QString *loadedImagesFolderPath;

	/*menu*/
	QMenuBar *menuBar;
	QMenu *plikMenu;
	QMenu *saveMenu;
	QAction *loadImagesAction;
	QAction *saveAmcAction;
	QAction *saveAmcSequenceAction;
	QAction *loadFromAmcAction;

    QFileInfoList list;
    QSlider *globalSlider;
    QPushButton *addFrameButton;
	QLabel *selectFrameLabel;

    QGridLayout *gridLayout;//głowny layout aplikacji
    QVBoxLayout *vboxLayout; //boczny panel - rotacja modelu
    QGridLayout *gridChildLayout; // panel poziomy - klatki
	QHBoxLayout *selectFrameLayout;

    QScrollArea *scrollArea;//widget umozliwiajacy scrollowanie głównego layoutu
	QScrollArea *scrollSideArea;//widget umozliwiajacy scrollowanie bocznego layoutu
    QWidget *scrollWidget;//widget do ktorego dodawane sa elementy dodawanego po wcisnieciu buttona
	QWidget *scrollSideWidget;
    QWidget *windowLayout;//widget do ktorego dodawany jest gridLayout
	QWidget *tabWidget;
	QTabWidget *sideManuTabWidget; //widget zawierajacy menu poszczegolnych klatek

	//lista pozwalajaca wybrac numer klatki, na ktorej model bedzie modyfikowany
	QComboBox *comboBoxFrameSelector;

	//interfejs klatek
    QVector<GLWidget*> glWidgetsVector; //glWidget
    QVector<QPushButton*> minButtonsVector; //przycisk przewijajacy zdjecie pierwszego elementu
    QVector<QPushButton*> reduceButtonsVector; //przycisk przewijajacy zdjecie do tylu
    QVector<QPushButton*> maxButtonsVector;  //przycisk przewijajacy zdjecie do przodu
    QVector<QPushButton*> increaseButtonsVector; //przycisk przewijajacy zdjecie ostatniego elementu
    QVector<QLabel*> countersVector; //wyswietlenie indeksu tla np. 1/14
    QVector<QCheckBox*> checkBoxesVector; //checkbox zapewniajacy polaczenie z globalSlider
    QVector<int> imagesListIterator; //aktualny indeks
	QVector<QPushButton*> menuButtonsVector; //przycisk rozwijajacy menu
	QVector<QMenu*> frameScrollingMenu; //rozwijane menu
	QVector<QAction*> manageBonesVector; //edycja wykorzystywanytch kosci przy budowie modelu

	//wektory zawierajace elementy bocznego menu
	QVector<QLabel*> sideMenuLabelsVector; 
	QVector<QHBoxLayout*> sideMenuHLayoutsVector;
	QVector<QPushButton*> sideMenuButtonsVector;
	QVector<QLineEdit*> sideMenuLineEditsVector;
	QVector<QSpinBox*> sideMenuSpinBoxesVector;

    Ui::MainWindow *ui;
	BoneManagment *boneManagmentWindow;
	/*Menu aplikacji*/
	void prepareMenus();
	void prepareLayouts();
    void prepareGlobalSlider(QSlider *slider);
    void setTextIteratorLabel(QLabel *label, int iterator, int listSize);

	void prepareFrameMenu(int i);
    void addFrameMenu(int i);
	void addGLWidgetMenu();

	/*wyczysczenie wartości zmiennych*/
	void resetVariables();

	/*ukrycie elementów głownego layoutu*/
	void hideLayouts();
	/*odkrycie elementów głownego layoutu*/
	void showLayouts();

	/*dodanie funkcji do menu klatki*/
	void addFrameScrollingMenu(QPushButton *button, int id);

	/*ustawienie parametrów qspinbox*/
	void setQSpinBoxSettings(QSpinBox * qspinbox);

    void mapIncreaseButtonSlot(int i);
    void mapReduceButtonSlot(int i);
    void mapMaxButtonSlot(int i);
    void mapMinButtonSlot(int i);
	void mapManageBonesSlot(int i);

private slots:
	/*wczytanie plików z wybranej ścieżki*/
	void loadFiles();

	/*zapis jednej klatki*/
	void saveOneFrameToFile();

	/*zapis sekwencji klatek*/
	void saveSequenceToFile();

	/*Wczytanie pliku amc*/
	void loadFromAmc();

	/*Operacje na modelu*/
	/*rotacja wybranego elementu modelu
	funkcja przyjmuje string w postaci "boneName;direction;axis;qLineEditID" np. "root;-1.0f;X;0"
	direction = 1.0 || direction = -1.0
	axis = "X" || axis = "Y" || axis = "Z"
	qLineEditID >= 0 && qLineEditID <= bones.size()*3
	*/
	void rotate(QString str);

	/*przewijane klatek wybranego okna*/
	void increaseIteratorPressed(int i);
	void reduceIteratorPressed(int i);
	void maxIteratorPressed(int i);
	void minIteratorPressed(int i);

	//elementy rozwijanego menu
	void manageBonesPressed(int i);

	/*dodanie okna z modelem postaci*/
	void buttonAddFramePressed();
	void updateGlobalListIterator(int value);

	/*zaladowanie bocznego menu po zmianach*/
	void reloadGLWidgetMenu();

	/*odswiezenie wartosci w polach qlineedit po rotacji kosci*/
	void refreshSideMenu();

	/*funkcja dezaktywuje przyciski do obrotu kosci jesli limit(0;0);*/
	bool disableRotatingButtons(vector<pf::range2> limits, int id);
};

#endif // MAINWINDOW_H
