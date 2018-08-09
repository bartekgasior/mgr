/********************************************************************************
** Form generated from reading UI file 'bonemanagment.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BONEMANAGMENT_H
#define UI_BONEMANAGMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BoneManagment
{
public:

    void setupUi(QWidget *BoneManagment)
    {
        if (BoneManagment->objectName().isEmpty())
            BoneManagment->setObjectName(QStringLiteral("BoneManagment"));
        BoneManagment->resize(400, 300);

        retranslateUi(BoneManagment);

        QMetaObject::connectSlotsByName(BoneManagment);
    } // setupUi

    void retranslateUi(QWidget *BoneManagment)
    {
        BoneManagment->setWindowTitle(QApplication::translate("BoneManagment", "BoneManagment", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BoneManagment: public Ui_BoneManagment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BONEMANAGMENT_H
