#include "avihzdialog.h"

AviHzDialog::AviHzDialog(QWidget *parent)
	: QDialog(parent)
{
	QDialog *dialog = new QDialog();
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	dialog->setWindowTitle("Frames");

	okButton = new QDialogButtonBox(QDialogButtonBox::Ok);

	connect(okButton, &QDialogButtonBox::accepted, this, &AviHzDialog::okButtonAccepted);

	mainLayout = new QVBoxLayout;
	secondRowLayout = new QHBoxLayout;

	spinBox = new QSpinBox;
	spinBox->setSuffix("Hz");

	label1 = new QLabel;
	label2 = new QLabel("Czestotliwosc klatek: ");

	spinBox->setValue(10);
	spinBox->setMinimum(1);
	spinBox->setMaximum(20);

	secondRowLayout->addWidget(label2);
	secondRowLayout->addWidget(spinBox);

	mainLayout->addWidget(label1);
	mainLayout->addLayout(secondRowLayout);
	mainLayout->addWidget(okButton);
	setLayout(mainLayout);
}

AviHzDialog::~AviHzDialog()
{
}

void AviHzDialog::setFramesCounter(int i) {
	QString str = "Wczytany plik .avi ma " + QString::number(i) + " klatek.";
	label1->setText(str);
}

int AviHzDialog::getHz() {
	return spinBox->value();
}

void AviHzDialog::okButtonAccepted() {
	close();
}