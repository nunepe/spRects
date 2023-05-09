#include "sp_rects.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sp_rects w;
    w.show();
    return a.exec();
}
