#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent)
	: QDialog(parent)
{
	QDialog *dialog = new QDialog();
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	dialog->setWindowTitle("Config");

	okButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(okButton, &QDialogButtonBox::accepted, this, &ConfigDialog::okButtonAccepted);
	connect(okButton, &QDialogButtonBox::rejected, this, &ConfigDialog::okButtonRejected);

	mainLayout = new QVBoxLayout;
	jointsRadiusLayout = new QHBoxLayout;
	timerLayout = new QHBoxLayout;
	backgroundResolutionLayout = new QHBoxLayout;

	addWidgetsToJointsLayout();
	addWidgetsToTimerLayout();
	addWidgetsToBackgroundResolutionLayout();

	mainLayout->addWidget(jointsRadiusGroupBox);
	mainLayout->addWidget(timerGroupBox);
	mainLayout->addWidget(backgroundResolutionGroupBox);
	mainLayout->addWidget(okButton);
	mainLayout->setAlignment(Qt::AlignCenter);
	setLayout(mainLayout);
}

ConfigDialog::~ConfigDialog()
{
}

void ConfigDialog::okButtonAccepted() {
	okClicked = true;
	close();
}

void ConfigDialog::okButtonRejected() {
	okClicked = false;
	close();
}

void ConfigDialog::addWidgetsToJointsLayout() {
	jointsRadiusGroupBox = new QGroupBox("Joints", this);
	jointsRadiusCheckBox = new QCheckBox(this);
	jointsRadiusLabel = new QLabel("Joints radius: ",this);
	jointRadiusSpinBox = new QSpinBox(this);
	jointRadiusSpinBox->setMinimum(0.0);

	connect(jointsRadiusCheckBox, SIGNAL(stateChanged(int)), this, SLOT(drawJoints()));
	//jointsRadiusCheckBox->setChecked(true);

	jointsRadiusLayout->addWidget(jointsRadiusCheckBox);
	jointsRadiusLayout->addWidget(jointsRadiusLabel);
	jointsRadiusLayout->addWidget(jointRadiusSpinBox);

	jointsRadiusLayout->setAlignment(Qt::AlignLeft);
	jointsRadiusGroupBox->setLayout(jointsRadiusLayout);
}

void ConfigDialog::addWidgetsToTimerLayout() {
	timerGroupBox = new QGroupBox("Timers", this);
	timerLabel = new QLabel("Set timer ticks:", this);
	timerSpinBox = new QSpinBox(this);
	timerSpinBox->setMinimum(0);
	timerSpinBox->setMaximum(10000);

	timerLayout->addWidget(timerLabel);
	timerLayout->addWidget(timerSpinBox);
	timerLayout->setAlignment(Qt::AlignLeft);

	timerGroupBox->setLayout(timerLayout);
}

void ConfigDialog::addWidgetsToBackgroundResolutionLayout() {
	backgroundResolutionGroupBox = new QGroupBox("Background resolution", this);
	backgroundResolutionCheckBox = new QCheckBox(this);
	backgroundResolutionWidthLabel = new QLabel("Width: ", this);
	backgroundResolutionHeightLabel = new QLabel("Height: ", this);
	backgroundResolutionWidthSpinBox = new QSpinBox(this);
	backgroundResolutionHeightSpinBox = new QSpinBox(this);

	connect(backgroundResolutionCheckBox, SIGNAL(stateChanged(int)), this, SLOT(resizeBackgroundResolution()));
	backgroundResolutionCheckBox->setChecked(false);
	backgroundResolutionHeightSpinBox->setDisabled(true);
	backgroundResolutionWidthSpinBox->setDisabled(true);

	backgroundResolutionWidthSpinBox->setMinimum(0);
	backgroundResolutionWidthSpinBox->setMaximum(5000);

	backgroundResolutionHeightSpinBox->setMinimum(0);
	backgroundResolutionHeightSpinBox->setMaximum(5000);
	backgroundResolutionLayout->addWidget(backgroundResolutionCheckBox);
	backgroundResolutionLayout->addSpacing(20);
	backgroundResolutionLayout->addWidget(backgroundResolutionWidthLabel);
	backgroundResolutionLayout->addWidget(backgroundResolutionWidthSpinBox);
	backgroundResolutionLayout->addWidget(backgroundResolutionHeightLabel);
	backgroundResolutionLayout->addWidget(backgroundResolutionHeightSpinBox);
	backgroundResolutionLayout->setAlignment(Qt::AlignLeft);
	backgroundResolutionGroupBox->setLayout(backgroundResolutionLayout);
}

void ConfigDialog::setJointRadiusSpinBoxValue(int val) {
	jointRadiusSpinBox->setValue(val);
}

void ConfigDialog::drawJoints() {
	if (jointsRadiusCheckBox->isChecked()) {
		drawJointsInGlWidgets = true;
		jointRadiusSpinBox->setDisabled(false);
	}
	else {
		drawJointsInGlWidgets = false;
		jointRadiusSpinBox->setDisabled(true);
	}
}

int ConfigDialog::getJointsRadius() {
	return jointRadiusSpinBox->value();
}

void ConfigDialog::disableJointSpinBox() {
	jointRadiusSpinBox->setDisabled(true);
}

void ConfigDialog::setRadiusCheckBox(bool val) {
	jointsRadiusCheckBox->setChecked(val);
}

void ConfigDialog::setTimerSpinBox(int val) {
	timerSpinBox->setValue(val);
}

int ConfigDialog::getTimerSpinBoxValue() {
	return timerSpinBox->value();
}

void ConfigDialog::setResolutionSpinBoxes(int w, int h) {
	backgroundResolutionWidthSpinBox->setValue(w);
	backgroundResolutionHeightSpinBox->setValue(h);
}

int ConfigDialog::getWidthSpinBox() {
	return backgroundResolutionWidthSpinBox->value();
}

int ConfigDialog::getHeightSpinBox() {
	return backgroundResolutionHeightSpinBox->value();
}

void ConfigDialog::resizeBackgroundResolution() {
	if (backgroundResolutionCheckBox->isChecked()) {
		resizeBackgroundRes = true;
		backgroundResolutionHeightSpinBox->setDisabled(false);
		backgroundResolutionWidthSpinBox->setDisabled(false);
	}
	else {
		resizeBackgroundRes = false;
		backgroundResolutionHeightSpinBox->setDisabled(true);
		backgroundResolutionWidthSpinBox->setDisabled(true);
	}
}

void ConfigDialog::setResolutionCheckBox(bool val) {
	backgroundResolutionCheckBox->setChecked(val);
}