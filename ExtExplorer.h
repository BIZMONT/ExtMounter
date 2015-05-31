#ifndef EXT2EXPLORE_H
#define EXT2EXPLORE_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QListView>
#include <QTreeView>
#include <QSplitter>
#include <QTextCodec>

#include "Read.h"
#include "ExtCopy.h"

namespace Ui {
    class Ext2Explore;
}

class Ext2Explore : public QMainWindow {
    Q_OBJECT
public:
    Ext2Explore(QWidget *parent = 0);
    ~Ext2Explore();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Ext2Explore *ui;
    ExtCopy copyfile;

    QItemSelectionModel *selectionModel;
    QStandardItemModel *filemodel;
    QStandardItem *root;
    Read *app;
    QTextCodec *codec;

    void init_root_fs();
    QString handle_mime(string file, uint16_t mode);
    void delete_children(QStandardItem *parent);
    void delete_file_tree();

private slots:
    void on_action_Save_triggered();
    void on_action_Rescan_System_triggered();
    void on_action_Exit_triggered();
    void on_action_item_dbclicked(const QModelIndex &index);
    void Context_menu(const QPoint &);
};

#endif
