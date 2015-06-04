#ifndef EXTEXPLORER_H
#define EXTEXPLORER_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QListView>
#include <QTreeView>
#include <QSplitter>
#include <QTextCodec>
#include <QFileDialog>
#include <fstream>
#include <iostream>

#include "Read.h"
#include "ExtCopy.h"

namespace Ui {
    class ExtExplorer;
}

class ExtExplorer : public QMainWindow {
    Q_OBJECT
public:
    ExtExplorer(QWidget *parent = 0);
    ~ExtExplorer();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ExtExplorer *ui;
    ExtCopy copyfile;

    QItemSelectionModel *selectionModel;
    QStandardItemModel *filemodel;
    QStandardItem *root;
    Read *app;
    QTextCodec *codec;

    void InitFileSystemRoot();
    QString handle_mime(string file, uint16_t mode);
    void Loging();
    void DeleteChildren(QStandardItem *parent);

private slots:
    void on_action_Save_triggered();
    void on_action_Rescan_System_triggered();
    void on_action_Exit_triggered();
    void on_action_item_dbclicked(const QModelIndex &index);
    void ContextMenu(const QPoint &);
};

#endif
