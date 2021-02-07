#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ElectrocardiogramGUI.h"
#include "../Electrocardiogram-Core/Source/EKG.hpp"
#include "../Electrocardiogram-Core/Source/FileManager.hpp"
#include <QValueAxis>

class ElectrocardiogramGUI : public QMainWindow
{
    Q_OBJECT

public:
    ElectrocardiogramGUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::ElectrocardiogramGUIClass ui;
    FileManager fileManager;
    std::unique_ptr<EKG> loadedEcg;
    std::unique_ptr<EKG> smoothedEcg;
    QtCharts::QChart* chart;
    QtCharts::QValueAxis* xAxis;
    QtCharts::QValueAxis* yAxis;

    void Clear();

private slots:
    void OnActionQuitTriggered();
    void OnLoadFile();
    void OnSaveFile();
    void OnSmoothSignal();
    void OnClear();
    void OnAbout() const;
};
