#include <QtGui/QApplication>
#include "ExtExplorer.h"
#include "log.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *utfcodec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(utfcodec);
    QTextCodec::setCodecForCStrings(utfcodec);

    Q_INIT_RESOURCE(ExtMounter);
    log_init();
    ExtExplorer w;
    w.show();
    return a.exec();
}
