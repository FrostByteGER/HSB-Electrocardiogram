#include "ElectrocardiogramGUI.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElectrocardiogramGUI w;
    w.show();
    return QApplication::exec();
}
