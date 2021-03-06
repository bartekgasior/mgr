#include "bonemanagment.h"

BoneManagment::BoneManagment(QWidget *parent)
	: QDialog(parent)
{
	QDialog *dialog = new QDialog();
	
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	dialog->setWindowTitle("Zarz�dzaj modelem");
	treeWidget = new QTreeWidget();
	/*nazwa, checkbox*/
	treeWidget->setColumnCount(2);
	treeWidget->setColumnWidth(0, 350);

	gridLayout = new QGridLayout;
	okButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(okButton, &QDialogButtonBox::accepted, this, &BoneManagment::okButtonAccepted);
	connect(okButton, &QDialogButtonBox::rejected, this, &QDialog::reject);

	gridLayout->addWidget(treeWidget, 0, 0, 20, 20);
	gridLayout->addWidget(okButton, 21, 9, 1, 2);
	setLayout(gridLayout);

}

BoneManagment::~BoneManagment()
{
}

void BoneManagment::setUsedBones(vector<pf::ASFBone> asfBones, vector<int> usedBonesIDs) {
	/*petla dla kazdej kosci wczytanej z pliku asf*/
	for (int i = 0; i < asfBones.size(); i++) {

		if (i == 0) {
			addTreeRoot(QString::fromStdString(asfBones[i].name), asfBones[i].id);
		}

		//dla kazdego potomka
		for (int j = 0; j < asfBones[i].idChildren.size(); j++) {
			//sprawdz czy element znajduje sie juz na liscie
			if (!(std::find(bonesIDs.begin(), bonesIDs.end(), asfBones[i].idChildren[j]) != bonesIDs.end())) {
				int tmp = getTreeItemID(bonesIDs, asfBones[i].id);
				addTreeChild(treeWidgetsItems[tmp], QString::fromStdString(asfBones[asfBones[i].idChildren[j]].name), asfBones[i].idChildren[j]);
			}
		}
	}

	setCheckBoxes(usedBonesIDs);
	//for (int i = 0; i < asfBones.size() - 1; i++) {
	connect(treeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(isAtLeastOneChecked(QTreeWidgetItem *, int)));
	//}
}

void BoneManagment::addTreeRoot(QString name,int id) {
	QTreeWidgetItem *treeItem = new QTreeWidgetItem(treeWidget);
	treeItem->setText(0, name);
	treeItem->setCheckState(1, Qt::Checked);
	treeItem->setExpanded(true);
	treeItem->setFlags(Qt::ItemIsEnabled);

	treeWidgetsItems.push_back(treeItem);
	bonesNames.push_back(name);
	bonesIDs.push_back(id);
}

void BoneManagment::addTreeChild(QTreeWidgetItem *parent, QString name, int id) {
	QTreeWidgetItem *treeItem = new QTreeWidgetItem();
	treeItem->setText(0, name);
	treeItem->setCheckState(1, Qt::Unchecked);
	parent->addChild(treeItem);
	treeItem->setExpanded(true);
	//treeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);

	treeWidgetsItems.push_back(treeItem);
	bonesNames.push_back(name);
	bonesIDs.push_back(id);
}

vector<int> BoneManagment::getUsedBonesIDs(vector<pf::boneGeometry> bonesGeometry, vector<string> allBones) {
	vector<int> result;
	for (int i = 0; i < allBones.size(); i++) {
		for (int j = 0; j < bonesGeometry.size(); j++) {
			if (allBones[i] == bonesGeometry[j].name) {
				result.push_back(i);
				break;
			}
		}
	}
	return result;
}

int BoneManagment::getTreeItemID(vector<int> bonesIDs, int parentID) {
	int result = -1;
	try {
		for (int i = 0; i < bonesIDs.size(); i++) {
			if (bonesIDs[i] == parentID) {
				result = i;
				break;
			}
		}
		if (result == -1) {
			throw MyException("Something went wrong while processing ASF file!");
		}
	}
	catch (MyException& e) {
		cerr << e.what() << endl;
		throw MyException("My very own message");
	}
	return result;
}

void BoneManagment::setCheckBoxes(vector<int> usedBonesIDs) {
	
	for (int i = 0; i < bonesIDs.size(); i++) {

		for (int j = 0; j < usedBonesIDs.size(); j++) {
			if (bonesIDs[i] == usedBonesIDs[j]) {
				treeWidgetsItems[i]->setCheckState(1, Qt::Checked);
			}
		}
	}
}

void BoneManagment::isAtLeastOneChecked(QTreeWidgetItem * item, int i) {
	bool anyChecked = false;
	for (int j = 1; j < treeWidgetsItems.size(); j++) {
		if (treeWidgetsItems[j]->checkState(1) == Qt::Checked) {
			anyChecked = true;
			break;
		}
	}
	if (!anyChecked) {
		item->setCheckState(1, Qt::Checked);
	}
}

void BoneManagment::okButtonAccepted() {
	for (int i = 0; i < treeWidgetsItems.size(); i++) {
		if (treeWidgetsItems[i]->checkState(1) == Qt::Checked) {
			selectedBonesNames.push_back(bonesNames[i].toUtf8().constData());
			selectedBonesIDs.push_back(bonesIDs[i]);
		}
	}
	close();
}

void BoneManagment::okButtonRejected() {
	close();
}

void BoneManagment::matchBonesWithASF(vector<int> &ids, vector<string> &names, vector<pf::ASFBone> asf) {
	vector<int> idTMP;
	vector<string> nameTMP;
	for (int i = 0; i < asf.size(); i++) {
		for (int j = 0; j < names.size(); j++) {
			if (asf[i].name == names[j]) {
				idTMP.push_back(ids[j]);
				nameTMP.push_back(names[j]);
				break;
			}
		}
	}
	ids.clear();
	names.clear();
	ids = idTMP;
	names = nameTMP;
}