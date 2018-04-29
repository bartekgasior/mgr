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

#include "asf.h"
#include "fileshandler.h"
#include "glwidget.h"
#include "FileHandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int globalListIterator = 1;
    int addFrameButtonCounter = 0;

	//sciezka do folderu ze zdjeciami
	QString *loadedImagesFolderPath;
	QMenuBar *menuBar;
	QMenu *plikMenu;
	QMenu *saveMenu;
	QAction *loadImagesAction;
	QAction *saveAmcAction;
	QAction *saveAmcSequenceAction;
	QAction *loadFromAmcAction;

    QFileInfoList list;
    QPixmap pix;
    QSlider *globalSlider;
    QPushButton *addFrameButton;

    QGridLayout *gridLayout;//głowny layout aplikacji
    QVBoxLayout *vboxLayout; //boczny panel - pionowy
    QGridLayout *gridChildLayout; // panel poziomy

    QScrollArea *scrollArea;//widget umozliwiajacy scrollowanie głównego layoutu
	QScrollArea *scrollSideArea;//widget umozliwiajacy scrollowanie bocznego layoutu
    QWidget *scrollWidget;//widget do ktorego dodawane sa elementy dodawanego po wcisnieciu buttona
	QWidget *scrollSideWidget;
    QWidget *windowLayout;//nowy widget dodawany  do gridLayout

	//lista pozwalajaca wybrac numer klatki, na ktorej model bedzie modyfikowany
	QComboBox *comboBoxFrameSelector;

    QVector<GLWidget*> glWidgetsVector;
    QVector<QPushButton*> minButtonsVector;
    QVector<QPushButton*> reduceButtonsVector;
    QVector<QPushButton*> maxButtonsVector;
    QVector<QPushButton*> increaseButtonsVector;
    QVector<QLabel*> countersVector;
    QVector<QCheckBox*> checkBoxesVector;
    QVector<QPixmap*> pixVector;
    QVector<QLabel*> picLabelsVector;
    QVector<int> imagesListIterator;

	//wektory zawierajace elementy bocznego menu
	QVector<QLabel*> sideMenuLabelsVector;
	QVector<QHBoxLayout*> sideMenuHLayoutsVector;
	QVector<QPushButton*> sideMenuButtonsVector;
	QVector<QLineEdit*> sideMenuLineEditsVector;

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

	/*dodanie okna z modelem postaci*/
	void buttonAddFramePressed();
	void updateGlobalListIterator(int value);

	/*odswiezenie wartosci w polach qlineedit po rotacji kosci*/
	void reloadGLWidgetMenu();

private:
    Ui::MainWindow *ui;

	/*Menu aplikacji*/
	void prepareMenus();
	void prepareLayouts();
    void prepareGlobalSlider(QSlider *slider);
    void setTextIteratorLabel(QLabel *label, int iterator, int listSize);
    void setImage1Label(QLabel *label, QString file);

	void prepareFrameMenu(int i);
    void addFrameMenu(int i);
	void addGLWidgetMenu();

    void mapIncreaseButtonSlot(int i);
    void mapReduceButtonSlot(int i);
    void mapMaxButtonSlot(int i);
    void mapMinButtonSlot(int i);
};

#endif // MAINWINDOW_H
