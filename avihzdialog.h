#pragma once

#include <QObject>
#include <QDialog>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QDialogButtonBox>

class AviHzDialog : public QDialog
{
	Q_OBJECT

public:
	AviHzDialog(QWidget *parent);
	~AviHzDialog();

	QSpinBox *spinBox;

	void setFramesCounter(int i);
	int getHz();
private:
	QDialogButtonBox *okButton;
	QHBoxLayout *secondRowLayout;
	QVBoxLayout *mainLayout;
	QLabel *label1;
	QLabel *label2;

	void okButtonAccepted();
};
