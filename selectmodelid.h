#pragma once

#include <QObject>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <iostream>
using namespace std;

class SelectModelID : public QDialog
{
	Q_OBJECT

public:
	SelectModelID(QWidget *parent);
	~SelectModelID();

	void setComboBox(vector<int> ids, int i);
	void setID(int i);
	int getID();

private:
	QDialogButtonBox *okButton;
	QComboBox *comboBox;
	QLabel *label;
	QGridLayout *mainLayout;
	
	int id;

private slots:
	void okButtonAccepted();
	void okButtonRejected();
};
