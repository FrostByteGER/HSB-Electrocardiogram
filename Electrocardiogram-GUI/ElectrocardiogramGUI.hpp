#pragma once

#include "ui_ElectrocardiogramGUI.h"
#include "../Electrocardiogram-Core/Source/EKG.hpp"
#include "../Electrocardiogram-Core/Source/FileManager.hpp"
#include <QValueAxis>

#include "../Electrocardiogram-Core/Source/EKGProcessor.hpp"
#include "../Electrocardiogram-Core/Source/SignalProcessor.hpp"

class ElectrocardiogramGUI : public QMainWindow
{
    Q_OBJECT

public:
    ElectrocardiogramGUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::ElectrocardiogramGUIClass ui;
    FileManager fileManager;
    SignalProcessor signalProcessor;
    EKGProcessor ecgProcessor;
    
    std::unique_ptr<EKG> loadedEcg;
    QtCharts::QChart* chart;
    QtCharts::QValueAxis* xAxis;
    QtCharts::QValueAxis* yAxis;

    void Clear();
    void SaveFile(EKG* ecg, const QString& filename);

private slots:
    void OnActionQuitTriggered();
    void OnLoadFile();
    void OnSaveEcg();
    void OnClear();
    void OnAbout() const;
};
