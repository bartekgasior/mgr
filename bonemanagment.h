#pragma once

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QTreeWidget>
#include <QGridLayout>
#include <QDialogButtonBox>
#include "Model3D.h"

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
	void getUsedBones(vector<pf::ASFBone> asfBones, vector<int> usedBones);
	/*indeksy uzytych kosci*/
	vector<int> getUsedBonesIDs(vector<pf::boneConfig> bonesConf, vector<string> allBones);
	
	vector<QTreeWidgetItem*> treeWidgetsItems;

	vector<int> selectedBonesIDs;
	vector<string> selectedBonesNames;

private:
	/*wektor zawierajacy id kosci tworzacych QTreeWidget*/
	vector<int> bonesIDs;
	/*wektor zawierajacy nazwy kosci tworzacych QTreeWidget*/
	vector<QString> bonesNames;

	void addTreeRoot(QString name, int id);
	void addTreeChild(QTreeWidgetItem *parent, QString name, int id);
	int getTreeItemID(vector<int> bonesIDs, int parentID);
	void setCheckBoxes(vector<int> usedBonesIDs);

private slots:
	void okButtonAccepted();
	void okButtonRejected();
};
