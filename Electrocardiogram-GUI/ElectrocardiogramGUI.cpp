#include "ElectrocardiogramGUI.hpp"
#include <QLineSeries>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>



ElectrocardiogramGUI::ElectrocardiogramGUI(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.btnSave->setEnabled(false);
    ui.btnSmooth->setEnabled(false);
    ui.btnClear->setEnabled(false);
    ui.actionUnloadFile->setEnabled(false);
    ui.chart->setRenderHints(QPainter::Antialiasing);
    chart = ui.chart->chart();
    xAxis = new QtCharts::QValueAxis();
    xAxis->setLabelFormat("%i");
    xAxis->setTitleText(QString("Seconds"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    yAxis = new QtCharts::QValueAxis();
    yAxis->setLabelFormat("%i");
    yAxis->setTitleText(QString("mV"));
    yAxis->setRange(-4, 4);
    chart->addAxis(yAxis, Qt::AlignLeft);
    ui.chart->setRubberBand(QtCharts::QChartView::HorizontalRubberBand);
}

void ElectrocardiogramGUI::OnActionQuitTriggered()
{
    QApplication::instance()->quit();
}

void ElectrocardiogramGUI::OnAbout() const
{
    QMessageBox::about(this->centralWidget(), QString("Electrocardiogram v1.0.0"), QString("Made by Kevin Kuegler for C++ Course 2020/2021"));
}

void ElectrocardiogramGUI::OnLoadFile()
{
    const QString filename = QFileDialog::getOpenFileName(this->centralWidget(), QString("Open ECG"), QString(), QString("ECG(.dat) Files (*.dat);;All Files (*)"));
    qDebug() << filename;
    if(filename.isEmpty())
    {
        return;
    }

    // Clear only if we loaded another file!
    Clear();

    try
    {
        std::vector<uint16_t> loadedData = fileManager.Import(filename.toStdString());
        loadedEcg = std::make_unique<EKG>(loadedData);
        xAxis->setRange(0, loadedData.size() * loadedEcg.get()->SamplingIntervalMs() / 1000);
        ui.btnSave->setEnabled(true);
        ui.btnSmooth->setEnabled(true);
        ui.btnClear->setEnabled(true);
        ui.actionUnloadFile->setEnabled(true);

        auto series = new QtCharts::QLineSeries();
        uint64_t time = 0;
        for (auto e : loadedData)
        {
            series->append(time, e);
            time += loadedEcg.get()->SamplingIntervalMs();
        }
        series->setColor(QColor(Qt::green));
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
        chart->addSeries(series);
    }catch(const std::exception& e)
    {
        qDebug() << "Exception occured while loading ECG file " << filename << ": " << e.what();
    }
}

void ElectrocardiogramGUI::OnSaveFile()
{
    const QString filename = QFileDialog::getSaveFileName(this->centralWidget(), QString("Save ECG"), QString(), QString("ECG(.dat) Files (*.dat);;All Files (*)"));
    qDebug() << filename;
    if(!filename.isEmpty())
    {
        const std::vector<std::string> data = loadedEcg.get()->Export();
        fileManager.Export(data, filename.toStdString());
    }
}

void ElectrocardiogramGUI::OnSmoothSignal()
{
    //TODO: Does it really work?
    auto smoothedReadings = loadedEcg.get()->SmoothReadings(5);
    smoothedEcg = std::make_unique<EKG>(smoothedReadings);
    ui.btnSmooth->setEnabled(false);
    auto series = new QtCharts::QLineSeries();
    uint64_t time = 0;
    for (auto e : smoothedReadings)
    {
        series->append(time, e);
        time += loadedEcg.get()->SamplingIntervalMs();
    }
    series->setColor(QColor(Qt::blue));
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);
    chart->addSeries(series);
}

void ElectrocardiogramGUI::OnClear()
{
    Clear();
}

void ElectrocardiogramGUI::Clear()
{
    loadedEcg = nullptr;
    smoothedEcg = nullptr;
    chart->removeAllSeries();
    ui.btnClear->setEnabled(false);
    ui.btnSave->setEnabled(false);
    ui.btnSmooth->setEnabled(false);
    ui.actionUnloadFile->setEnabled(false);
}