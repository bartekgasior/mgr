#include "selectmodelid.h"

SelectModelID::SelectModelID(QWidget *parent)
	: QDialog(parent)
{
	QDialog *dialog = new QDialog();
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	dialog->setWindowTitle("Wybierz konfiguracje modelu");

	okButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(okButton, &QDialogButtonBox::accepted, this, &SelectModelID::okButtonAccepted);
	connect(okButton, &QDialogButtonBox::rejected, this, &SelectModelID::okButtonRejected);

	mainLayout = new QGridLayout(this);
	label = new QLabel("Wybierz model: ");
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
	id = comboBox->currentText().toInt();
	close();
}

void SelectModelID::okButtonRejected() {
	close();
}

void SelectModelID::setComboBox(vector<int> ids, int id) {
	for (int i = 0; i < ids.size(); i++) {
		if (ids[i] != id) {
			comboBox->addItem(QString::number(ids[i]), ids[i]);
		}
	}
}
void SelectModelID::setID(int i) {
	id = i;
}
int SelectModelID::getID() {
	return id;
}