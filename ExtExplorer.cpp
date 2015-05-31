#include <QFileDialog>

#include "ExtExplorer.h"
#include "ExtCopy.h"

#include "ui_ext2explore.h"

Ext2Explore::Ext2Explore(QWidget *parent) : QMainWindow(parent), ui(new Ui::Ext2Explore)
{
    filemodel = new QStandardItemModel(this);
    app = new Read();

    ui->setupUi(this);

    ui->tree->setModel(filemodel);
    ui->tree->header()->hide();
    ui->tree->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tree->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->list->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->list->setSelectionMode( QAbstractItemView::SingleSelection );
    ui->list->setModel(filemodel);
    ui->list->setViewMode(QListView::IconMode);
    ui->list->setIconSize(QSize(50,60));
    ui->list->setMovement(QListView::Static);
    ui->list->setWordWrap(true);
    ui->list->setWrapping(true);

    root = filemodel->invisibleRootItem();
    selectionModel = ui->list->selectionModel();
    ui->tree->setSelectionModel(selectionModel);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_action_item_dbclicked(const QModelIndex &)));
    connect(ui->list, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_action_item_dbclicked(const QModelIndex &)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Context_menu(const QPoint &)));

    codec = QTextCodec::codecForName("utf-8");

    init_root_fs();
}

Ext2Explore::~Ext2Explore()
{    
    delete ui;
    delete filemodel;
    delete app;
}

void Ext2Explore::delete_children(QStandardItem *parent)
{
    int nrows;
    QVariant fileData;
    QStandardItem *item;
    ExtFile *file;

    if(!parent->hasChildren())
        return;

    nrows = parent->rowCount();
    for(int i = 0; i < nrows; i++)
    {
        item = parent->child(i);
        if(!item)
            continue;

        delete_children(item);
        fileData = item->data(Qt::UserRole);
        file = (ExtFile *) fileData.value<void *>();
        delete file;
    }
    parent->removeRows(0, nrows);
}

void Ext2Explore::init_root_fs()//‘ормуванн€ коренневого каталогу дерева
{
    Partition *temp;
    list<Partition *> parts;
    list<Partition *>::iterator i;
    QStandardItem *item;
    void *ptr;

    parts = app->get_partitions();
    for(i = parts.begin(); i != parts.end(); i++)
    {
        temp = (*i);

        // check if it is already in the view
        if(temp->onview)
            continue;

        item = new QStandardItem(QIcon(QString::fromAscii(":/icons/resource/disk.png")),
                                 QString::fromStdString(temp->get_linux_name()));
        if(!temp->get_root())
        {
            LOG("ѕомилка кореневого каталогу дл€ %s!\n", temp->get_linux_name().c_str());
            delete item;
            continue;
        }

        ptr = temp->get_root();
        item->setData(qVariantFromValue(ptr), Qt::UserRole);
        item->setEditable(false);
        root->appendRow(item);

        temp->onview = true;
    }
}

QString Ext2Explore::handle_mime(string file, uint16_t mode)
{
    QString str;

    if(EXT2_S_ISDIR(mode))
        str = QString::fromAscii(":/icons/resource/file_folder.png");
    else
        str = QString::fromAscii(":/icons/resource/file_unknown.png");

    return str;
}

void Ext2Explore::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Ext2Explore::on_action_Exit_triggered()
{
    delete ui;
    delete filemodel;
    delete app;

    close();
}

void Ext2Explore::on_action_Rescan_System_triggered()
{
    delete_children(root);
    delete app;

    app = new Read();
    init_root_fs();
}

void Ext2Explore::on_action_item_dbclicked(const QModelIndex &index)
{
    QStandardItem *children;
    QStandardItem *parentItem;
    ExtFile *parentFile;
    ExtFile *files;
    QVariant fileData;
    EXT2DIRENT *dir;
    Partition *part;

    parentItem = filemodel->itemFromIndex(index);
    fileData = parentItem->data(Qt::UserRole);
    parentFile = (ExtFile *) fileData.value<void *>();

    if(!EXT2_S_ISDIR(parentFile->inode.i_mode))
        return;

    ui->list->setRootIndex(index);
    if(parentFile->onview)
        return;

    part = parentFile->partition;

    dir = part->open_dir(parentFile);
    while((files = part->read_dir(dir)) != NULL)
    {
        children = new QStandardItem(QIcon(handle_mime(files->file_name, files->inode.i_mode)),
                                     codec->toUnicode(files->file_name.c_str()));
        children->setData(qVariantFromValue((void *)files), Qt::UserRole);
        children->setEditable(false);
        parentItem->appendRow(children);
        parentFile->onview = true;
    }

    part->close_dir(dir);
}

void Ext2Explore::Context_menu(const QPoint &point)
{
    QMenu menu(ui->splitter);
    menu.addAction(ui->action_Save);
    menu.exec(this->mapToGlobal(point));
}

void Ext2Explore::on_action_Save_triggered()
{
    QString filename;
    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;
    QStandardItem *item;
    QVariant fileData;
    ExtFile *file;

    if(indexes.length() <= 0)
        return;

    index = indexes[0];
    item = filemodel->itemFromIndex(index);
    fileData = item->data(Qt::UserRole);
    file = (ExtFile *) fileData.value<void *>();

    if(EXT2_S_ISDIR(file->inode.i_mode))
    {
        filename = QFileDialog::getExistingDirectory(this, tr("Save folder in"),
                                QString::fromAscii(""),
                                QFileDialog::ShowDirsOnly);
    }
    else
    {
        filename = QFileDialog::getSaveFileName(this, tr("Save File/Folder"),
                                QString::fromStdString(file->file_name),
                                tr("All Files (*)"));
    }
    if(filename.isEmpty())
        return;

    copyfile.set_file(file);
    copyfile.set_name(filename);
    copyfile.start_copy();
}
