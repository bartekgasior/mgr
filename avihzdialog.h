#pragma once

#include <QObject>
#include <QDialog>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QRadioButton>

class AviHzDialog : public QDialog
{
	Q_OBJECT

public:
	AviHzDialog(QWidget *parent);
	~AviHzDialog();

	

	void setLowerHigherSpinBoxes(int max);
	void setFramesCounter(int i);
	int getHz();
	int getSpinBox2Value();
	int	getSpinBox3Value();
	int mode = 0;
	bool clicked = false;
private:
	QRadioButton *radioAllFrames;
	QRadioButton *radioButtonHz;
	QRadioButton *radioButtonPart;

	QDialogButtonBox *okButton;
	QHBoxLayout *secondRowLayout;
	QHBoxLayout *thirdRowLayout;
	QHBoxLayout *fourthRowLayout;
	QVBoxLayout *mainLayout;

	QLabel *label1;
	QLabel *label2;
	QLabel *label3;
	QLabel *label4;

	QSpinBox *spinBox1;
	QSpinBox *spinBox2;
	QSpinBox *spinBox3;

	void okButtonAccepted();

private slots:
	void disableRows();
	void setSpinMinMax1();
	void setSpinMinMax2();
};
