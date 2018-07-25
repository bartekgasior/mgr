#include "cameratypedialog.h"

CameraTypeDialog::CameraTypeDialog(QWidget *parent)
	: QDialog(parent)
{
	QDialog *dialog = new QDialog();
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	dialog->setWindowTitle("Add camera type");
	
	okButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(okButton, &QDialogButtonBox::accepted, this, &CameraTypeDialog::okButtonAccepted);
	connect(okButton, &QDialogButtonBox::rejected, this, &CameraTypeDialog::okButtonRejected);

	mainLayout = new QVBoxLayout;
	comboBox = new QComboBox(this);
	connect(comboBox, SIGNAL(activated(int)), this, SLOT(getID()));

	comboBox->addItem("TSAI1");
	comboBox->addItem("TSAI2");
	comboBox->addItem("Perspective");
	comboBox->addItem("Disortion");
	comboBox->setCurrentIndex(0);

	mainLayout->addWidget(comboBox);
	mainLayout->addWidget(okButton);
	mainLayout->setAlignment(Qt::AlignCenter);
	setLayout(mainLayout);
}

CameraTypeDialog::~CameraTypeDialog()
{
}

void CameraTypeDialog::okButtonAccepted() {
	okClicked = true;
	close();
}

void CameraTypeDialog::okButtonRejected() {
	okClicked = false;
	close();
}

void CameraTypeDialog::getID() {
	id = comboBox->currentIndex();
}