#include <QtGui/QApplication>
#include "ExtExplorer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Q_INIT_RESOURCE(ExtMounter);
    log_init();
    Ext2Explore w;
    w.show();
    return a.exec();
}
