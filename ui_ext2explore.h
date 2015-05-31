/********************************************************************************
** Form generated from reading UI file 'ext2explore.ui'
**
** Created: Sun 31. May 23:37:02 2015
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXT2EXPLORE_H
#define UI_EXT2EXPLORE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Ext2Explore
{
public:
    QAction *action_Rescan_System;
    QAction *action_Exit;
    QAction *action_Save;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTreeView *tree;
    QListView *list;
    QMenuBar *menubar;
    QMenu *menu_File;

    void setupUi(QMainWindow *Ext2Explore)
    {
        if (Ext2Explore->objectName().isEmpty())
            Ext2Explore->setObjectName(QString::fromUtf8("Ext2Explore"));
        Ext2Explore->resize(550, 310);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resource/Icon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Ext2Explore->setWindowIcon(icon);
        action_Rescan_System = new QAction(Ext2Explore);
        action_Rescan_System->setObjectName(QString::fromUtf8("action_Rescan_System"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/resource/Rescan.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Rescan_System->setIcon(icon1);
        action_Exit = new QAction(Ext2Explore);
        action_Exit->setObjectName(QString::fromUtf8("action_Exit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/resource/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Exit->setIcon(icon2);
        action_Save = new QAction(Ext2Explore);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/resource/filesave.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Save->setIcon(icon3);
        centralwidget = new QWidget(Ext2Explore);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        tree = new QTreeView(splitter);
        tree->setObjectName(QString::fromUtf8("tree"));
        tree->setLayoutDirection(Qt::LeftToRight);
        splitter->addWidget(tree);
        list = new QListView(splitter);
        list->setObjectName(QString::fromUtf8("list"));
        splitter->addWidget(list);

        horizontalLayout->addWidget(splitter);

        Ext2Explore->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Ext2Explore);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 550, 20));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        Ext2Explore->setMenuBar(menubar);

        menubar->addAction(menu_File->menuAction());
        menu_File->addSeparator();
        menu_File->addAction(action_Rescan_System);
        menu_File->addSeparator();
        menu_File->addAction(action_Exit);

        retranslateUi(Ext2Explore);
        QObject::connect(action_Exit, SIGNAL(triggered()), Ext2Explore, SLOT(close()));

        QMetaObject::connectSlotsByName(Ext2Explore);
    } // setupUi

    void retranslateUi(QMainWindow *Ext2Explore)
    {
        Ext2Explore->setWindowTitle(QApplication::translate("Ext2Explore", "ExtMounter", 0, QApplication::UnicodeUTF8));
        action_Rescan_System->setText(QApplication::translate("Ext2Explore", "&Rescan System", 0, QApplication::UnicodeUTF8));
        action_Exit->setText(QApplication::translate("Ext2Explore", "&Exit", 0, QApplication::UnicodeUTF8));
        action_Save->setText(QApplication::translate("Ext2Explore", "&\320\227\320\261\320\265\321\200\320\265\320\263\321\202\320\270", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("Ext2Explore", "&File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Ext2Explore: public Ui_Ext2Explore {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXT2EXPLORE_H
