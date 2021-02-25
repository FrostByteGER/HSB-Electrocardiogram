#include "ElectrocardiogramGUI.hpp"
#include <QLineSeries>
#include <QScatterSeries>
#include <QAreaSeries>
#include <QLegendMarker>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>



ElectrocardiogramGUI::ElectrocardiogramGUI(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.btnSave->setEnabled(false);
    ui.btnSmooth->setEnabled(false);
    ui.btnClear->setEnabled(false);
    ui.btnSaveSmoothed->setEnabled(false);
    ui.actionUnloadFile->setEnabled(false);
    ui.chart->setRenderHints(QPainter::Antialiasing);
    chart = ui.chart->chart();
    xAxis = new QtCharts::QValueAxis();
    xAxis->setLabelFormat("%i");
    xAxis->setTitleText(QString("Seconds"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    yAxis = new QtCharts::QValueAxis();
    yAxis->setLabelFormat("%d");
    yAxis->setTitleText(QString("mV"));
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
    const QString filename = QFileDialog::getOpenFileName(this->centralWidget(), QString("Open ECG"), QString(), QString("ECG Files (*.dat);;All Files (*)"));
    qDebug() << filename;
    if(filename.isEmpty())
    {
        return;
    }

    // Clear only if we loaded another file!
    Clear();

    try
    {
        std::vector<double_t> loadedData = fileManager.Import(filename.toStdString());
        loadedEcg = std::make_unique<EKG>(loadedData, 5, 4, std::numeric_limits<uint16_t>::max());
        ui.btnSave->setEnabled(true);
        ui.btnSmooth->setEnabled(true);
        ui.btnClear->setEnabled(true);
        ui.actionUnloadFile->setEnabled(true);

        auto series = new QtCharts::QLineSeries();
        uint32_t time = 0;
        for (auto e : loadedData)
        {
            series->append(time, e);
            time += loadedEcg.get()->samplingIntervalMs();
        }
        chart->addSeries(series);
        series->setColor(QColor(Qt::green));
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
        
    }catch(const std::exception& e)
    {
        qDebug() << "Exception occured while loading ECG file " << filename << ": " << e.what();
    }
}

void ElectrocardiogramGUI::OnSaveEcg()
{
    const QString filename = QFileDialog::getSaveFileName(this->centralWidget(), QString("Save ECG"), QString(), QString("ECG Files (*.dat);;All Files (*)"));
    SaveFile(loadedEcg.get(), filename);
}

void ElectrocardiogramGUI::OnSaveEcgSmoothed()
{
    const QString filename = QFileDialog::getSaveFileName(this->centralWidget(), QString("Save Smoothed ECG"), QString(), QString("ECG Files (*.dat);;All Files (*)"));
    SaveFile(smoothedEcg.get(), filename);
}

void ElectrocardiogramGUI::SaveFile(EKG* ecg, const QString& filename)
{
    qDebug() << filename;
    if (!filename.isEmpty())
    {
        const std::vector<std::string> data = ecg->exportToCsv();
        fileManager.Export(data, filename.toStdString());
    }
}

void ElectrocardiogramGUI::OnSmoothSignal()
{
    auto smoothedReadings = loadedEcg.get()->smoothReadings(2);
    smoothedEcg = std::make_unique<EKG>(smoothedReadings);

    ui.btnSmooth->setEnabled(false);
    ui.btnSaveSmoothed->setEnabled(true);
    auto series = new QtCharts::QLineSeries();
    uint64_t time = 0;

    for (auto e : smoothedReadings)
    {
        series->append(time, e);
        time += loadedEcg.get()->samplingIntervalMs();
    }

    auto seriesDeriv = new QtCharts::QLineSeries();
    std::vector<double_t> derivatedReadings;
    derivatedReadings.reserve(smoothedReadings.size());
    time = 0;
    for (size_t idx = 0; idx < smoothedReadings.size(); ++idx)
    {
        const int32_t prevIdx = std::max(0, static_cast<int32_t>(idx - 1));
        const int32_t nextIdx = std::min(smoothedReadings.size() - 1, idx + 1);
        const double_t previous = smoothedReadings[prevIdx];
        const double_t next = smoothedReadings[nextIdx];

        const double_t result = (next - previous) / (2.0 * smoothedEcg->samplingIntervalMs());
        derivatedReadings.push_back(result);
        seriesDeriv->append(time, result);
        time += smoothedEcg->samplingIntervalMs();
    }

    seriesDeriv->setColor(QColor(Qt::red));



    auto meanSeries = new QtCharts::QLineSeries();
    chart->addSeries(meanSeries);
    meanSeries->attachAxis(xAxis);
    meanSeries->attachAxis(yAxis);
    meanSeries->append(0, smoothedEcg->average());
    meanSeries->append(smoothedReadings.size() * smoothedEcg->samplingIntervalMs(), smoothedEcg->average());

    auto stdDeviationTopSeries = new QtCharts::QLineSeries();
    chart->addSeries(stdDeviationTopSeries);
    stdDeviationTopSeries->attachAxis(xAxis);
    stdDeviationTopSeries->attachAxis(yAxis);
    stdDeviationTopSeries->append(0, smoothedEcg->average() + smoothedEcg->standardDeviation());
    stdDeviationTopSeries->append(smoothedReadings.size() * smoothedEcg->samplingIntervalMs(), smoothedEcg->average() + smoothedEcg->standardDeviation());

    auto stdDeviationBottomSeries = new QtCharts::QLineSeries();
    chart->addSeries(stdDeviationBottomSeries);
    stdDeviationBottomSeries->attachAxis(xAxis);
    stdDeviationBottomSeries->attachAxis(yAxis);
    stdDeviationBottomSeries->append(0, smoothedEcg->average() - smoothedEcg->standardDeviation());
    stdDeviationBottomSeries->append(smoothedReadings.size() * smoothedEcg->samplingIntervalMs(), smoothedEcg->average() - smoothedEcg->standardDeviation());
    
    const int32_t tailLength = 25;
    smoothedEcg->detectHeartbeats(smoothedReadings, tailLength);


    QtCharts::QAreaSeries* heartBeatAreaSeries = nullptr;
    for (const auto& beat : smoothedEcg->heartbeats())
    {
        auto upperSeries = new QtCharts::QLineSeries();
        upperSeries->append(beat.frameStartMs, loadedEcg->maximum());
        upperSeries->append(beat.frameEndMs, loadedEcg->maximum());
        auto lowerSeries = new QtCharts::QLineSeries();
        lowerSeries->append(beat.frameStartMs, 0);
        lowerSeries->append(beat.frameEndMs, 0);
        heartBeatAreaSeries = new QtCharts::QAreaSeries(upperSeries, lowerSeries);
        chart->addSeries(heartBeatAreaSeries);
        heartBeatAreaSeries->attachAxis(xAxis);
        heartBeatAreaSeries->attachAxis(yAxis);
        heartBeatAreaSeries->setColor(QColor(Qt::cyan));
        heartBeatAreaSeries->setName("Heartbeats");

        // Hide legends by default!
        for (auto legend : chart->legend()->markers(heartBeatAreaSeries))
        {
            legend->setVisible(false);
        }
    }

    // Enable for the final one so we have at least one legend title
    for (auto legend : chart->legend()->markers(heartBeatAreaSeries))
    {
        legend->setVisible(true);
    }
    chart->addSeries(series);
    chart->addSeries(seriesDeriv);
    series->setColor(QColor(Qt::blue));
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);

    /*
    auto heartbeatSeries = new QtCharts::QScatterSeries();
    heartbeatSeries->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeCircle);
    heartbeatSeries->setMarkerSize(15);
    heartbeatSeries->setColor(QColor(Qt::darkRed));
    for (const auto& beat : hearbeats)
    {
        heartbeatSeries->append(beat.frameEndMs, beat.frameStartMs);
    }
    chart->addSeries(heartbeatSeries);
    heartbeatSeries->attachAxis(xAxis);
    heartbeatSeries->attachAxis(yAxis);
    */
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
    ui.btnSaveSmoothed->setEnabled(false);
    ui.actionUnloadFile->setEnabled(false);
}