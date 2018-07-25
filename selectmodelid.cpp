#include "selectmodelid.h"

SelectModelID::SelectModelID(QWidget *parent)
	: QDialog(parent)
{
	QDialog *dialog = new QDialog();
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	dialog->setWindowTitle("Select model configuration");

	okButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(okButton, &QDialogButtonBox::accepted, this, &SelectModelID::okButtonAccepted);
	connect(okButton, &QDialogButtonBox::rejected, this, &SelectModelID::okButtonRejected);

	mainLayout = new QGridLayout(this);
	label = new QLabel("Select model: ");
	comboBox = new QComboBox(this);

	mainLayout->addWidget(label, 0, 0, 1, 1);
	mainLayout->addWidget(comboBox, 0, 1, 1, 2);
	mainLayout->addWidget(okButton, 1, 0, 1, 3, Qt::AlignCenter);
	setLayout(mainLayout);
}

SelectModelID::~SelectModelID()
{
}

void SelectModelID::okButtonAccepted() {
	selected = true;
	if(comboBox->isEnabled())
	id = comboBox->currentText().toInt();
	close();
}

void SelectModelID::okButtonRejected() {
	selected = false;
	close();
}

void SelectModelID::setComboBox(int j) {
	for (int i = 0; i < j; i++) 
		comboBox->addItem(QString::number(i));
}

void SelectModelID::setComboBox(vector<int> ids) {
	for (int i = 0; i < ids.size(); i++)
		comboBox->addItem(QString::number(ids[i]));
}

void SelectModelID::setID(int i) {
	id = i;
}
int SelectModelID::getID() {
	return id;
}

