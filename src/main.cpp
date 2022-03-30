#include "PurchaseManager.h"

#include "Settings.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Settings settings;

    PurchaseManager w;
    w.show();

    return a.exec();
}
