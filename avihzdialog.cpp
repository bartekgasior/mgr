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
	thirdRowLayout = new QHBoxLayout;
	fourthRowLayout = new QHBoxLayout;

	secondRowLayout->setAlignment(Qt::AlignLeft);
	thirdRowLayout->setAlignment(Qt::AlignLeft);
	fourthRowLayout->setAlignment(Qt::AlignLeft);

	radioAllFrames = new QRadioButton(this);
	radioButtonHz = new QRadioButton(this);
	radioButtonPart = new QRadioButton(this);

	connect(radioAllFrames, SIGNAL(clicked()), this, SLOT(disableRows()));
	connect(radioButtonHz, SIGNAL(clicked()), this, SLOT(disableRows()));
	connect(radioButtonPart, SIGNAL(clicked()), this, SLOT(disableRows()));

	radioAllFrames->setChecked(true);

	spinBox1 = new QSpinBox(this);
	spinBox1->setSuffix("Hz");

	spinBox1->setValue(10);
	spinBox1->setMinimum(1);
	spinBox1->setMaximum(20);

	spinBox2 = new QSpinBox(this);
	spinBox3 = new QSpinBox(this);

	connect(spinBox2, SIGNAL(valueChanged(int)), this, SLOT(setSpinMinMax1()));
	connect(spinBox3, SIGNAL(valueChanged(int)), this, SLOT(setSpinMinMax2()));

	label1 = new QLabel(this);
	label2 = new QLabel("All frames", this);
	label3 = new QLabel("Avi Hz: ", this);
	label4 = new QLabel("Select period: ", this);

	secondRowLayout->addWidget(radioAllFrames);
	secondRowLayout->addWidget(label2);

	thirdRowLayout->addWidget(radioButtonHz);
	thirdRowLayout->addWidget(label3);
	thirdRowLayout->addWidget(spinBox1);

	fourthRowLayout->addWidget(radioButtonPart);
	fourthRowLayout->addWidget(label4);
	fourthRowLayout->addWidget(spinBox2);
	fourthRowLayout->addWidget(spinBox3);

	mainLayout->addWidget(label1);
	mainLayout->addLayout(secondRowLayout);
	mainLayout->addLayout(thirdRowLayout);
	mainLayout->addLayout(fourthRowLayout);
	mainLayout->addWidget(okButton);
	setLayout(mainLayout);

	radioAllFrames->click();
}

AviHzDialog::~AviHzDialog()
{
}

void AviHzDialog::setFramesCounter(int i) {
	QString str = "Loaded .avi file has " + QString::number(i) + " frames.";
	label1->setText(str);
}

int AviHzDialog::getHz() {
	return spinBox1->value();
}

void AviHzDialog::okButtonAccepted() {
	clicked = true;
	close();
}

void AviHzDialog::disableRows() {
	if (radioAllFrames->isChecked()) {
		mode = 0;
		label2->setEnabled(true);

		label3->setEnabled(false);
		spinBox1->setEnabled(false);

		label4->setEnabled(false);
		spinBox2->setEnabled(false);
		spinBox3->setEnabled(false);
	}
	else if (radioButtonHz->isChecked()) {
		mode = 1;
		label2->setEnabled(false);

		label3->setEnabled(true);
		spinBox1->setEnabled(true);

		label4->setEnabled(false);
		spinBox2->setEnabled(false);
		spinBox3->setEnabled(false);
	}
	else {
		mode = 2;
		label2->setEnabled(false);

		label3->setEnabled(false);
		spinBox1->setEnabled(false);

		label4->setEnabled(true);
		spinBox2->setEnabled(true);
		spinBox3->setEnabled(true);

	}
}

void AviHzDialog::setLowerHigherSpinBoxes(int max) {
	spinBox2->setMinimum(0);
	spinBox3->setMinimum(0);

	spinBox2->setMaximum(max);
	spinBox3->setMaximum(max);
}

void AviHzDialog::setSpinMinMax1() {
	if (spinBox2->value() > spinBox3->value()) {
		spinBox2->setValue(spinBox3->value());
	}
}

void AviHzDialog::setSpinMinMax2() {
	if (spinBox2->value() > spinBox3->value()) {
		spinBox3->setValue(spinBox2->value());
	}
}

int AviHzDialog::getSpinBox2Value() {
	return spinBox2->value();
}

int AviHzDialog::getSpinBox3Value() {
	return spinBox3->value();
}