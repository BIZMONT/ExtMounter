#include "ExtCopy.h"

ExtCopy::ExtCopy(ExtFile *parent, QString &dest)
{
    filename = dest;
    file = parent;
    ExtCopy();
}

ExtCopy::ExtCopy()
{
    cancelOperation = false;

    progress = new Ui::ProgressDialog();
    progress->setupUi(this);
    progress->progressBar->setMaximum(100);
    progress->progressBar->setMinimum(0);
}

ExtCopy::~ExtCopy()
{
    delete progress;
}

bool ExtCopy::StartCopy()
{
    if(!EXT2_S_ISREG(file->inode.i_mode) &&
       !EXT2_S_ISDIR(file->inode.i_mode))
        return false;

    proc = new ExtCopyProcess(file, filename);
    qRegisterMetaType<QString>("QString&");
    QObject::connect(proc, SIGNAL(sig_updateui(QString&,QString&,QString&,int,int)),
            this, SLOT(slot_updateui(QString&,QString&,QString&,int,int)));
    QObject::connect(proc, SIGNAL(sig_copydone()), this, SLOT(slot_copydone()));
    QObject::connect(this, SIGNAL(signal_cancelprocess()), proc, SLOT(slot_cancelprocess()));
    QObject::connect(this, SIGNAL(accepted()), proc, SLOT(slot_cancelprocess()));
    QObject::connect(this, SIGNAL(rejected()), proc, SLOT(slot_cancelprocess()));

    this->show();
    proc->start();
    return true;
}

bool ExtCopy::showMessageBox()
{
    QMessageBox msgBox;
    msgBox.setText(QString::fromAscii("�� ���������� ������� ���������"));
    msgBox.setInformativeText(QString::fromAscii("�� ��������, �� ������ ������� ���������?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch(ret) {
    case QMessageBox::Yes:
        return true;
    case QMessageBox::No:
        return false;
    }

    return false;
}

void ExtCopy::slot_updateui(QString &file, QString &from, QString &to, int copied, int total)
{
    int value;

    if(!file.isEmpty())
        progress->FileLabel->setText(file);
    if(!from.isEmpty())
        progress->Fromlabel->setText(from);
    if(!to.isEmpty())
        progress->ToLabel->setText(to);

    value = (copied * 100) / total;
    progress->progressBar->setValue(value);
}

void ExtCopy::slot_copydone()
{
    delete proc;
    this->hide();
}

ExtCopyProcess::ExtCopyProcess(ExtFile *parent, QString &dest): QThread()
{
    filename = dest;
    file = parent;
    blksize = parent->partition->GetBlockSize();
    buffer = new char [blksize];
    filetosave = NULL;
    cancelOperation = false;
    codec = QTextCodec::codecForName("utf-8");
}

ExtCopyProcess::~ExtCopyProcess()
{
    delete [] buffer;
    if(filetosave)
        delete filetosave;
}

void ExtCopyProcess::slot_cancelprocess()
{
    cancelOperation = true;
}

void ExtCopyProcess::run()
{
    if(EXT2_S_ISDIR(file->inode.i_mode))
    {
        CopyFolder(filename, file);
        emit sig_copydone();
        return ;
    }
    else if(!EXT2_S_ISREG(file->inode.i_mode))
        return ;

    CopyFile(filename, file);
    emit sig_copydone();
}

bool ExtCopyProcess::CopyFile(QString &destfile, ExtFile *srcfile)
{
    uint64_t blocks, blkindex;
    QString qsrc;
    int extra;
    int ret;

    filetosave = new QFile(destfile);
    if (!filetosave->open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        LOG("������� ��������� ����� %s!\n", srcfile->file_name.c_str());
        return false;
    }

    qsrc = codec->toUnicode(srcfile->file_name.c_str());
    blocks = srcfile->file_size / blksize;
    for(blkindex = 0; blkindex < blocks; blkindex++)
    {
        if(cancelOperation)
        {
            return false;
        }
        ret = srcfile->partition->ReadDataBlock(&srcfile->inode, blkindex, buffer);
        if(ret < 0)
        {
            filetosave->close();
            return false;
        }
        filetosave->write(buffer, blksize);
        if((blkindex % 10) == 0)
            emit sig_updateui(qsrc, destfile, filename, (int)blkindex, (int)blocks);
    }

    extra = srcfile->file_size % blksize;
    if(extra)
    {
        ret = srcfile->partition->ReadDataBlock(&srcfile->inode, blkindex, buffer);
        if(ret < 0)
        {
            filetosave->close();
            return false;
        }
        filetosave->write(buffer, extra);
    }
    filetosave->close();
    LOG("���� %s ���������\n", srcfile->file_name.c_str());
    return true;
}

bool ExtCopyProcess::CopyFolder(QString &path, ExtFile *parent)
{
    QDir dir(path);
    QString filetosave;
    QString rootname = path;
    EXT2DIRENT *dirent;
    Partition *part = parent->partition;
    ExtFile *child;
    bool ret;


    if(!EXT2_S_ISDIR(parent->inode.i_mode))
        return false;

    dir.mkdir(codec->toUnicode(parent->file_name.c_str()));

    dirent = part->OpenDirectory(parent);
    while((child = part->ReadDirectory(dirent)) != NULL)
    {
        filetosave = rootname;
        filetosave.append(QString::fromAscii("/"));
        filetosave.append(codec->toUnicode(parent->file_name.c_str()));
        if(EXT2_S_ISDIR(child->inode.i_mode))
        {

            ret = CopyFolder(filetosave, child);
            if((ret == false) && (cancelOperation == true))
            {
                part->CloseDirectory(dirent);
                return false;
            }
            continue;
        }
        else if(!EXT2_S_ISREG(child->inode.i_mode))
        {
            continue;
        }

        filetosave.append(QString::fromAscii("/"));
        filetosave.append(codec->toUnicode(child->file_name.c_str()));
        ret = CopyFile(filetosave, child);
        if((ret == false) && (cancelOperation == true))
        {
            part->CloseDirectory(dirent);
            return false;
        }
    }
    LOG("������� %s � ���� ���� ���������\n", parent->file_name.c_str());
    return true;
}

void ExtCopy::on_buttonBox_clicked(QAbstractButton* button)
{
     cancelOperation = showMessageBox();
     if(cancelOperation)
         emit signal_cancelprocess();
}
