#ifndef EXTCOPY_H
#define EXTCOPY_H

#include <QDialog>
#include <QFile>
#include <QThread>
#include <QTextCodec>
#include <QMessageBox>
#include <QDir>

#include "ui_Progress.h"
#include "Read.h"

class ExtCopyProcess : public QThread
{
    Q_OBJECT
private:
    QString filename;
    ExtFile *file;
    QFile *filetosave;
    char *buffer;
    int blksize;
    bool cancelOperation;
    QTextCodec *codec;

    bool CopyFolder(QString &path, ExtFile *file);
    bool CopyFile(QString &destfile, ExtFile *srcfile);

protected:
    void run();

public:
    ExtCopyProcess(ExtFile *parent, QString &dest);
    ~ExtCopyProcess();

public slots:
    void slot_cancelprocess();

signals:
    void sig_updateui(QString &file, QString &from, QString &to, int copied, int total);
    void sig_copydone();
};

class ExtCopy : public QDialog
{
    Q_OBJECT

    Ui::ProgressDialog *progress;
    QString filename;
    ExtFile *file;
    bool cancelOperation;
    ExtCopyProcess *proc;

public:
    ExtCopy(ExtFile *parent, QString &dest);
    ExtCopy();
    ~ExtCopy();
    void set_file(ExtFile *sfile) { file = sfile; }
    void set_name(QString &name) { filename = name; }
    void StartCopy();

private:
    bool showMessageBox();

private slots:
    void on_buttonBox_clicked(QAbstractButton* button);

public slots:
    void slot_updateui(QString &file, QString &from, QString &to, int copied, int total);
    void slot_copydone();

signals:
    void signal_cancelprocess();
};

#endif
