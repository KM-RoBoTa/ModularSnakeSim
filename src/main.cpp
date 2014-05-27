#include <QApplication>
#include "melosimula.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Melosimula w;
    w.show();
    
    return a.exec();
}
