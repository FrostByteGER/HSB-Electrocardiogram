#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ElectrocardiogramGUI.h"

class ElectrocardiogramGUI : public QMainWindow
{
    Q_OBJECT

public:
    ElectrocardiogramGUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::ElectrocardiogramGUIClass ui;
};
