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
#include "sidemenubuilder.h"

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

private slots:
    void loadFiles();
    void increaseIteratorPressed(int i);
    void reduceIteratorPressed(int i);
    void maxIteratorPressed(int i);
    void minIteratorPressed(int i);
    void buttonAddFramePressed();
    void updateGlobalListIterator(int value);

	/*Operacje na modelu*/
	void rotateModelXUp();
	void rotateModelXDown();
	void rotateModelYLeft();
	void rotateModelYRight();

	void rotateLeftUpLegX1();
	void rotateLeftUpLegX2();
	void rotateLeftUpLegY1();
	void rotateLeftUpLegY2();
	void rotateLeftUpLegZ1();
	void rotateLeftUpLegZ2();

	void rotateLeftLegX1();
	void rotateLeftLegX2();
	void rotateLeftLegY1();
	void rotateLeftLegY2();
	void rotateLeftLegZ1();
	void rotateLeftLegZ2();

	void rotateRightUpLegX1();
	void rotateRightUpLegX2();
	void rotateRightUpLegY1();
	void rotateRightUpLegY2();
	void rotateRightUpLegZ1();
	void rotateRightUpLegZ2();

	void rotateRightLegX1();
	void rotateRightLegX2();
	void rotateRightLegY1();
	void rotateRightLegY2();
	void rotateRightLegZ1();
	void rotateRightLegZ2();
private:
    Ui::MainWindow *ui;

    void prepareGlobalSlider(QSlider *slider);
    void setTextIteratorLabel(QLabel *label, int iterator, int listSize);
    void setImage1Label(QLabel *label, QString file);

    void addFrameMenu(int i);
	void addGLWidgetMenu();
    void prepareFrameMenu(int i);

    void mapIncreaseButtonSlot(int i);
    void mapReduceButtonSlot(int i);
    void mapMaxButtonSlot(int i);
    void mapMinButtonSlot(int i);
};

#endif // MAINWINDOW_H
