#pragma once

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QTreeWidget>
#include <QGridLayout>
#include <QDialogButtonBox>
#include "Model3D.h"
#include "MyException.h"
using namespace std;

class BoneManagment : public QDialog
{
	Q_OBJECT

public:
	BoneManagment(QWidget *parent);
	~BoneManagment();

	QTreeWidget *treeWidget;
	QGridLayout *gridLayout;
	QDialogButtonBox *okButton;

	/*lista pozwalajaca na wybor kosci w modelu*/
	void setUsedBones(vector<pf::ASFBone> asfBones, vector<int> usedBones);

	/*indeksy uzytych w modelu kosci*/
	vector<int> getUsedBonesIDs(vector<pf::boneGeometry> bonesGeometry, vector<string> allBones);
	
	/*funkcja zamienia kolejnosc elementow w wektorach, aby pokrywala sie ona z plikiem asf*/
	void matchBonesWithASF(vector<int> &ids, vector<string> &names, vector<pf::ASFBone> asf);

	/*elementy QTreeWidget*/
	vector<QTreeWidgetItem*> treeWidgetsItems;

	/*id wybranych kosci*/
	vector<int> selectedBonesIDs;

	/*nazwy wybranych kosci*/
	vector<string> selectedBonesNames;

private:
	/*wektor zawierajacy id kosci tworzacych QTreeWidget*/
	vector<int> bonesIDs;

	/*wektor zawierajacy nazwy kosci tworzacych QTreeWidget*/
	vector<QString> bonesNames;

	/*dodanie roota do QTreeWidget*/
	void addTreeRoot(QString name, int id);

	/*dodanie potomkow do QTreeWidget*/
	void addTreeChild(QTreeWidgetItem *parent, QString name, int id);

	/*pobranie id wybranego elementu*/
	int getTreeItemID(vector<int> bonesIDs, int parentID);

	/*zaznaczenie checkboxow odpowiadajacych kosci po uruchomieniu okna*/
	void setCheckBoxes(vector<int> usedBonesIDs);

private slots:
	void okButtonAccepted();
	void okButtonRejected();

	/* przynajmniej jedna kosc (oprocz root) musi byc wybrana, aby mozliwe bylo zapisanie pliku .dat */
	void isAtLeastOneChecked(QTreeWidgetItem * item, int i);
};
