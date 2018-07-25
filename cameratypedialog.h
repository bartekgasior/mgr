#pragma once

#include <QObject>
#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QHBoxLayout>

class CameraTypeDialog : public QDialog
{
	Q_OBJECT

public:
	CameraTypeDialog(QWidget *parent);
	~CameraTypeDialog();

	int id = 0;
	bool okClicked = false;
private:
	QVBoxLayout *mainLayout;
	QLabel *label;
	QDialogButtonBox *okButton;
	QComboBox *comboBox;

	void okButtonAccepted();
	void okButtonRejected();

private slots:
	void getID();
};
