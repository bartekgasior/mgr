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

	void setComboBox(int j);
	void setComboBox(vector<int> ids);

	void setID(int i);
	int getID();
	bool selected = false;
	QComboBox *comboBox;

private:
	QDialogButtonBox *okButton;
	QLabel *label;
	QGridLayout *mainLayout;
	
	int id = -1;

private slots:
	void okButtonAccepted();
	void okButtonRejected();
};
