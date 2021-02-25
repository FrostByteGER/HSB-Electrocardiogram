#pragma once

#include "ui_ElectrocardiogramGUI.h"
#include "EKG.hpp"
#include "FileManager.hpp"
#include <QValueAxis>

#include "EKGProcessor.hpp"
#include "SignalProcessor.hpp"

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
