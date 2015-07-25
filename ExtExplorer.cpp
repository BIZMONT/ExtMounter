#include "ExtExplorer.h"
#include "ExtCopy.h"

#include "ui_extexplorer.h"

ExtExplorer::ExtExplorer(QWidget *parent) : QMainWindow(parent), ui(new Ui::ExtExplorer)
{
    filemodel = new QStandardItemModel(this);
    app = new Read();

    ui->setupUi(this);

    ui->mainsplitter->setStretchFactor(0, 1);
    ui->mainsplitter->setStretchFactor(1, 2);

    ui->splitter->setStretchFactor(0, 2);
    ui->splitter->setStretchFactor(1, 1);

    ui->tree->setModel(filemodel);
    ui->tree->header()->hide();
    ui->tree->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tree->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->list->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->list->setSelectionMode( QAbstractItemView::SingleSelection );
    ui->list->setModel(filemodel);

    root = filemodel->invisibleRootItem();
    selectionModel = ui->list->selectionModel();
    ui->tree->setSelectionModel(selectionModel);

    ui->list->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_action_item_dbclicked(const QModelIndex &)));
    connect(ui->list, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_action_item_dbclicked(const QModelIndex &)));
    connect(ui->list, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ContextMenu(const QPoint &)));

    codec = QTextCodec::codecForName("utf-8");

    InitFileSystemRoot();
    Loging();
}

ExtExplorer::~ExtExplorer()
{    
    delete ui;
    delete filemodel;
    delete app;
}

void ExtExplorer::Loging()
{
    QFile file("ExtMounter.log");
    file.open(QIODevice::ReadOnly);
    QString str = file.readAll();
    ui->log->setText(str);
    file.close();
}

void ExtExplorer::DeleteChildren(QStandardItem *parent)
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

        DeleteChildren(item);
        fileData = item->data(Qt::UserRole);
        file = (ExtFile *) fileData.value<void *>();
        delete file;
    }
    parent->removeRows(0, nrows);
}

void ExtExplorer::InitFileSystemRoot()
{
    Partition *temp;
    list<Partition *> parts;
    list<Partition *>::iterator i;
    QStandardItem *item;
    QStandardItem *rootitem;
    void *ptr;


    parts = app->GetPartitions();
    rootitem = new QStandardItem(QIcon(QString::fromAscii(":/icons/resource/computer.png")),
                             ComputerName());
    rootitem->setEditable(false);
    root->appendRow(rootitem);

    for(i = parts.begin(); i != parts.end(); i++)
    {
        temp = (*i);

        if(temp->onview)
            continue;

        item = new QStandardItem(QIcon(QString::fromAscii(":/icons/resource/disk.png")),
                                 QString::fromStdString(temp->GetLinuxName()));

        if(!temp->GetRoot())
        {
            LOG("Помилка кореневого каталогу для %s!\n", temp->GetLinuxName().c_str());
            delete item;
            continue;
        }

        ptr = temp->GetRoot();
        item->setData(qVariantFromValue(ptr), Qt::UserRole);
        item->setEditable(false);
        rootitem->appendRow(item);

        temp->onview = true;
    }
}

void ExtExplorer::ContextMenu(const QPoint &point)
{
    QModelIndex index = ui->list->indexAt(point);
    if (index.isValid())
    {
        QMenu menu(this);
        menu.addAction(ui->action_Save);
        menu.exec(ui->list->mapToGlobal(point));
    }
}

void ExtExplorer::changeEvent(QEvent *e)
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

void ExtExplorer::on_action_Exit_triggered()
{
    delete ui;
    delete filemodel;
    delete app;
    close();
}

void ExtExplorer::on_action_Rescan_System_triggered()
{
    ui->log->clear();
    DeleteChildren(root);

    app = new Read();
    InitFileSystemRoot();
    QFile file("ExtMounter.log");
    file.open(QIODevice::ReadOnly);
    Loging();
}

void ExtExplorer::on_action_item_dbclicked(const QModelIndex &index)
{
    QStandardItem *children;
    QStandardItem *parentItem;
    ExtFile *parentFile;
    ExtFile *files;
    QVariant fileData;
    EXT2DIRENT *dir;
    Partition *part;
    QString itemType;

    parentItem = filemodel->itemFromIndex(index);

    if(parentItem->text()!= ComputerName())
    {
        fileData = parentItem->data(Qt::UserRole);
        parentFile = (ExtFile *) fileData.value<void *>();

        if(!EXT2_S_ISDIR(parentFile->inode.i_mode))
            return;

        ui->list->setRootIndex(index);
        if(parentFile->onview)
            return;

        part = parentFile->partition;

        dir = part->OpenDirectory(parentFile);
        while((files = part->ReadDirectory(dir)) != NULL)
        {
            if(EXT2_S_ISDIR(files->inode.i_mode))
                itemType = QString::fromAscii(":/icons/resource/folder.png");
            else
                itemType = QString::fromAscii(":/icons/resource/file.png");

            children = new QStandardItem(QIcon(itemType), codec->toUnicode(files->file_name.c_str()));
            children->setData(qVariantFromValue((void *)files), Qt::UserRole);
            children->setEditable(false);
            parentItem->appendRow(children);
            parentFile->onview = true;
        }
        part->CloseDirectory(dir);
    }
}

void ExtExplorer::on_action_Save_triggered()
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
        filename = QFileDialog::getExistingDirectory(this, tr("Зберегти папку в"),
                                QString::fromAscii(""),
                                QFileDialog::ShowDirsOnly);
    }
    else
    {
        filename = QFileDialog::getSaveFileName(this, tr("Зберегти Файл/Папку"),
                                QString::fromStdString(file->file_name),
                                tr("Усі файли (*)"));
    }
    if(filename.isEmpty())
        return;

    copyfile.SetFile(file);
    copyfile.SetName(filename);
    copyfile.StartCopy();
}

QString ExtExplorer::ComputerName()
{
    WCHAR compName[255];
    DWORD size = sizeof(compName);
    GetComputerName(compName, &size);
    return QString::fromWCharArray(compName, -1);
}
