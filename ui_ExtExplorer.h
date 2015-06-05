/********************************************************************************
** Form generated from reading UI file 'ExtExplorer.ui'
**
** Created: Thu 4. Jun 22:13:47 2015
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXTEXPLORER_H
#define UI_EXTEXPLORER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExtExplorer
{
public:
    QAction *action_Rescan_System;
    QAction *action_Exit;
    QAction *action_Save;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QSplitter *mainsplitter;
    QSplitter *splitter;
    QTreeView *tree;
    QListView *list;
    QTextEdit *worklog;
    QMenuBar *menubar;
    QMenu *menu_File;

    void setupUi(QMainWindow *ExtExplorer)
    {
        if (ExtExplorer->objectName().isEmpty())
            ExtExplorer->setObjectName(QString::fromUtf8("ExtExplorer"));
        ExtExplorer->resize(550, 500);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resource/Icon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ExtExplorer->setWindowIcon(icon);
        ExtExplorer->setIconSize(QSize(40, 40));
        action_Rescan_System = new QAction(ExtExplorer);
        action_Rescan_System->setObjectName(QString::fromUtf8("action_Rescan_System"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/resource/Rescan.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Rescan_System->setIcon(icon1);
        action_Exit = new QAction(ExtExplorer);
        action_Exit->setObjectName(QString::fromUtf8("action_Exit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/resource/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Exit->setIcon(icon2);
        action_Save = new QAction(ExtExplorer);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/resource/filesave.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Save->setIcon(icon3);
        centralwidget = new QWidget(ExtExplorer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mainsplitter = new QSplitter(centralwidget);
        mainsplitter->setObjectName(QString::fromUtf8("mainsplitter"));
        mainsplitter->setOrientation(Qt::Vertical);
        splitter = new QSplitter(mainsplitter);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        tree = new QTreeView(splitter);
        tree->setObjectName(QString::fromUtf8("tree"));
        tree->setLayoutDirection(Qt::LeftToRight);
        splitter->addWidget(tree);
        list = new QListView(splitter);
        list->setObjectName(QString::fromUtf8("list"));
        splitter->addWidget(list);
        mainsplitter->addWidget(splitter);
        worklog = new QTextEdit(mainsplitter);
        worklog->setObjectName(QString::fromUtf8("worklog"));
        worklog->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        mainsplitter->addWidget(worklog);

        gridLayout->addWidget(mainsplitter, 0, 0, 1, 1);

        ExtExplorer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ExtExplorer);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 550, 20));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        ExtExplorer->setMenuBar(menubar);

        menubar->addAction(menu_File->menuAction());
        menu_File->addSeparator();
        menu_File->addAction(action_Rescan_System);
        menu_File->addSeparator();
        menu_File->addAction(action_Exit);

        retranslateUi(ExtExplorer);
        QObject::connect(action_Exit, SIGNAL(triggered()), ExtExplorer, SLOT(close()));

        QMetaObject::connectSlotsByName(ExtExplorer);
    } // setupUi

    void retranslateUi(QMainWindow *ExtExplorer)
    {
        ExtExplorer->setWindowTitle(QApplication::translate("ExtExplorer", "ExtMounter", 0, QApplication::UnicodeUTF8));
        action_Rescan_System->setText(QApplication::translate("ExtExplorer", "&Rescan System", 0, QApplication::UnicodeUTF8));
        action_Exit->setText(QApplication::translate("ExtExplorer", "&Exit", 0, QApplication::UnicodeUTF8));
        action_Save->setText(QApplication::translate("ExtExplorer", "&\320\227\320\261\320\265\321\200\320\265\320\263\321\202\320\270", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("ExtExplorer", "&File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ExtExplorer: public Ui_ExtExplorer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXTEXPLORER_H
