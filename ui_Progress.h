/********************************************************************************
** Form generated from reading UI file 'Progress.ui'
**
** Created: Sun 14. Jun 00:50:35 2015
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESS_H
#define UI_PROGRESS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_ProgressDialog
{
public:
    QDialogButtonBox *buttonBox;
    QProgressBar *progressBar;
    QLabel *FileLabel;
    QLabel *ToLabel;
    QLabel *Fromlabel;

    void setupUi(QDialog *ProgressDialog)
    {
        if (ProgressDialog->objectName().isEmpty())
            ProgressDialog->setObjectName(QString::fromUtf8("ProgressDialog"));
        ProgressDialog->resize(411, 162);
        ProgressDialog->setMinimumSize(QSize(411, 162));
        ProgressDialog->setMaximumSize(QSize(411, 162));
        ProgressDialog->setModal(true);
        buttonBox = new QDialogButtonBox(ProgressDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(170, 130, 81, 32));
        buttonBox->setLayoutDirection(Qt::RightToLeft);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
        progressBar = new QProgressBar(ProgressDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(30, 100, 371, 23));
        progressBar->setValue(24);
        FileLabel = new QLabel(ProgressDialog);
        FileLabel->setObjectName(QString::fromUtf8("FileLabel"));
        FileLabel->setGeometry(QRect(30, 30, 321, 16));
        ToLabel = new QLabel(ProgressDialog);
        ToLabel->setObjectName(QString::fromUtf8("ToLabel"));
        ToLabel->setGeometry(QRect(30, 50, 321, 16));
        Fromlabel = new QLabel(ProgressDialog);
        Fromlabel->setObjectName(QString::fromUtf8("Fromlabel"));
        Fromlabel->setGeometry(QRect(30, 70, 321, 16));

        retranslateUi(ProgressDialog);

        QMetaObject::connectSlotsByName(ProgressDialog);
    } // setupUi

    void retranslateUi(QDialog *ProgressDialog)
    {
        ProgressDialog->setWindowTitle(QApplication::translate("ProgressDialog", "\320\232\320\276\320\277\321\226\321\216\320\262\320\260\320\275\320\275\321\217 ...", 0, QApplication::UnicodeUTF8));
        FileLabel->setText(QApplication::translate("ProgressDialog", "0", 0, QApplication::UnicodeUTF8));
        ToLabel->setText(QApplication::translate("ProgressDialog", "0", 0, QApplication::UnicodeUTF8));
        Fromlabel->setText(QApplication::translate("ProgressDialog", "0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProgressDialog: public Ui_ProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESS_H
