#include "PurchaseManager.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PurchaseManager w;
    w.show();

    return a.exec();
}
