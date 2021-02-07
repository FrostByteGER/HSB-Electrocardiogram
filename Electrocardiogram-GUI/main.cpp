#include "ElectrocardiogramGUI.hpp"
#include <QtWidgets/QApplication>


#include "../Electrocardiogram-Core/Source/FileManager.hpp"
#include "../Electrocardiogram-Core/Source/Signal.hpp"

int main(int argc, char *argv[])
{
    const FileManager fileManager;
    const std::vector<uint16_t> data = { 600, 470, 170, 430, 300 }; // fileManager.Import("Data/ecg.dat");  //
    const Signal sig{ data };
    QApplication a(argc, argv);
    ElectrocardiogramGUI w;
    w.show();
    return QApplication::exec();
}
