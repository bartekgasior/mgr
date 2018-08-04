#pragma once

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QScrollArea>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStackedWidget>
#include <QGroupBox>
#include <QCheckBox>

#include "Camera.h"
#include "cameratypedialog.h"

using namespace std;

class CamerasConfig : public QDialog
{
	Q_OBJECT

public:
	CamerasConfig(QWidget *parent);
	~CamerasConfig();

	/*wektor zawierajacy wykrzystywane kamery*/
	QVector<pf::Camera*> cameras;

	/*wybrany typ kamery*/
	QVector<int> cameraType;

	QVector<bool> useQuat;
	/*czy zaakceptowano zmiany*/
	bool okClicked = false;
	bool firstTime = true;
	/*flaga aktualizujaca cameraComboBox po dodaniu kamery po wcisnieciu przycisku */
	bool wasDialogOpen = false;
	/*zwrocenie kamer z wybranymi parametrami*/
	void returnCameras(QVector<pf::Camera*> &dest);

	void disableWidgets();
	void enableWidgets();
	/*pobierz kamery wykorzystywane w glownym oknie aplikacji*/
	void getCamerasFromMainWindow(QVector<pf::Camera*> cams);

private:
	QDialogButtonBox *okButton;
	/*glowny layout*/
	QVBoxLayout *mainLayout;
	QHBoxLayout *hBoxLayout; //combo box + button do dodawania kamer
	QHBoxLayout *hBoxWidthLayout; //width label i spin box 
	QHBoxLayout *hBoxHeightLayout; //height label i spin box 
	QWidget *tabWidget;
	QTabWidget *cameraTabWidget; 
	QStackedWidget *cameraStackedWidget;
	QWidget *scrollTsai1Widget;
	QWidget *scrollTsai2Widget;
	QWidget *scrollPerspectiveWidget;
	QWidget *scrollPerspectiveDisortionsWidget;
	QWidget *scrollOtherParamsWidget;

	QScrollArea *scrollTsai1Area;
	QScrollArea *scrollTsai2Area;
	QScrollArea *scrollPerspectiveArea;
	QScrollArea *scrollPerspectiveDisortionsArea;
	QScrollArea *scrollOtherParamsArea;

	QVBoxLayout *vboxTsai1Layout;
	QVBoxLayout *vboxTsai2Layout;
	QVBoxLayout *vboxPerspectiveLayout;
	QVBoxLayout *vboxPerspectiveDisortionsLayout;
	QVBoxLayout *vboxOtherParamsLayout;

	/*layout zawierajacy parametry kamer*/
	QHBoxLayout *parametersLayout;
	/*kazda wiersz layoutu*/
	QVector<QVBoxLayout*> hLayouts;

	QVector<QLabel*> tsai1Labels;
	QVector<QDoubleSpinBox*> tsai1SpinBoxes;
	QVector<QHBoxLayout*> tsai1HLayout;
	QVector<QGroupBox*> tsai1GroupBoxes;

	QVector<QLabel*> tsai2Labels;
	QVector<QDoubleSpinBox*> tsai2SpinBoxes;
	QVector<QHBoxLayout*> tsai2HLayout;
	QVector<QGroupBox*> tsai2GroupBoxes;

	QVector<QLabel*> perspectiveLabels;
	QVector<QDoubleSpinBox*> perspectiveSpinBoxes;
	QVector<QHBoxLayout*> perspectiveHLayout;
	QVector<QGroupBox*> perspectiveGroupBoxes;
	QCheckBox *perspectiveQuatCB;
	QCheckBox *perspectiveRadCB;

	QVector<QLabel*> perspectiveDisortionsLabels;
	QVector<QDoubleSpinBox*> perspectiveDisortionsSpinBoxes;
	QVector<QHBoxLayout*> perspectiveDisortionsHLayout;
	QVector<QGroupBox*> perspectiveDisortionsGroupBoxes;
	QCheckBox *perspectiveDisortionQuatCB;
	QCheckBox *perspectiveDisortionRadCB;

	//QLabel *widthLabel, *heightLabel;
	//QSpinBox *widthSpinBox, *heightSpinBox;
	QComboBox *cameraComboBox;
	/*combobox do wyboru typu kamery*/
	QComboBox *cameraTypeComboBox;
	/*dodanie nowej kamery*/
	QPushButton *addNewCamera;

	void prepareLayout();
	void addTsai1Tab();
	void addTsai2Tab();
	void addPerspectiveTab();
	void addPerspectiveDisortionsTab();

private slots:
	
	void okButtonAccepted();
	void okButtonRejected();
	void buttonAddCameraPressed();
	void setCameraParametersToSpinBoxes();
	/*po zmianie wartosci spinbox funkcja wprowadza zmiany do odpowiedniego obiektu*/
	void saveCameraParametersFromSB();
	/*typ kamery - wartosc combobox*/
	void setCameraType();

	void hidePerspectiveRadLayout();
	void hidePerspectiveQuatLayout();
	void hidePerspectiveDisortionQuatLayout();
	void hidePerspectiveDisortionRadLayout();
};
