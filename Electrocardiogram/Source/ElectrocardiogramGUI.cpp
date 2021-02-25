#include "ElectrocardiogramGUI.hpp"
#include <QLineSeries>
#include <QScatterSeries>
#include <QAreaSeries>
#include <QLegendMarker>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

#include "ConfigurationCache.hpp"


ElectrocardiogramGUI::ElectrocardiogramGUI(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.btnSave->setEnabled(false);
    ui.btnClear->setEnabled(false);
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
    ui.lblCountVal->setText("-");
    ui.lblSumVal->setText("-");
    ui.lblMinVal->setText("-");
    ui.lblMaxVal->setText("-");
    ui.lblAverageVal->setText("-");
    ui.lblStdDeviationVal->setText("-");
    ui.lblHeartbeatsVal->setText("-");
    ui.lblBpmVal->setText("-");
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
        // Get everything from commandline if possible
        int32_t windowSize = 2;
        ConfigurationCache::instance().tryGetValue("--smoothing-window-size", &windowSize);
        uint32_t samplingIntervalMs = 5;
        ConfigurationCache::instance().tryGetValue("--sampling-interval", &samplingIntervalMs);
        int32_t signalRangeMilliVolt = 4;
        ConfigurationCache::instance().tryGetValue("--signalrange-millivolts", &signalRangeMilliVolt);
        int32_t signalRangeRawMax = std::numeric_limits<uint16_t>::max();
        ConfigurationCache::instance().tryGetValue("--signalrange-max", &signalRangeRawMax);
        int32_t tailLength = 25;
        ConfigurationCache::instance().tryGetValue("--heartbeat-tail-length", &tailLength);

        const std::vector<double_t> loadedData = fileManager.Import(filename.toStdString());
        Signal signal = Signal(loadedData);
        const Signal smoothedSignal = signalProcessor.smoothSignal(signal, windowSize);

        loadedEcg = std::make_unique<EKG>(ecgProcessor.constructEkgFromReadings(signal.readings(), smoothedSignal.readings(), 
            samplingIntervalMs, signalRangeMilliVolt, signalRangeRawMax, tailLength));

        auto series = new QtCharts::QLineSeries();
        double_t time = 0;
        for (auto e : loadedEcg->readings())
        {
            series->append(time, e);
            time = time + loadedEcg->samplingIntervalMs() / 1000.0;
        }
        chart->addSeries(series);
        series->setColor(QColor(Qt::green));
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
        series->setName("ECG Signal");

        QtCharts::QAreaSeries* heartBeatAreaSeries = nullptr;
        for (const auto& beat : loadedEcg->heartbeats())
        {
            auto upperSeries = new QtCharts::QLineSeries();
            upperSeries->append(beat.frameStartMs / 1000.0, loadedEcg->maximum());
            upperSeries->append(beat.frameEndMs / 1000.0, loadedEcg->maximum());
            auto lowerSeries = new QtCharts::QLineSeries();
            lowerSeries->append(beat.frameStartMs / 1000.0, 0);
            lowerSeries->append(beat.frameEndMs / 1000.0, 0);
            heartBeatAreaSeries = new QtCharts::QAreaSeries(upperSeries, lowerSeries);
            chart->addSeries(heartBeatAreaSeries);
            heartBeatAreaSeries->attachAxis(xAxis);
            heartBeatAreaSeries->attachAxis(yAxis);
            heartBeatAreaSeries->setColor(QColor(Qt::darkGray));
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

        // This re-add is done as when the heartbeats were to be added first
        // the chart would incorrectly scale them when the ECG gets added after.
        // When the ECG gets added first, the chart scale is properly set.
        chart->removeSeries(series);
        chart->addSeries(series);

        ui.lblCountVal->setText(QString::number(loadedEcg->count()));
        ui.lblSumVal->setText(QString::number(loadedEcg->sum()));
        ui.lblMinVal->setText(QString::number(loadedEcg->minimum()));
        ui.lblMaxVal->setText(QString::number(loadedEcg->maximum()));
        ui.lblAverageVal->setText(QString::number(loadedEcg->average()));
        ui.lblStdDeviationVal->setText(QString::number(loadedEcg->standardDeviation()));
        ui.lblHeartbeatsVal->setText(QString::number(loadedEcg->heartbeatCount()));
        ui.lblBpmVal->setText(QString::number(loadedEcg->heartbeatsPerMinute()));

        ui.btnSave->setEnabled(true);
        ui.btnClear->setEnabled(true);
        ui.actionUnloadFile->setEnabled(true);
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

void ElectrocardiogramGUI::SaveFile(EKG* ecg, const QString& filename)
{
    qDebug() << filename;
    if (!filename.isEmpty())
    {
        const std::vector<std::string> data = ecgProcessor.exportToCsv(*ecg);
        fileManager.Export(data, filename.toStdString());
    }
}

void ElectrocardiogramGUI::OnClear()
{
    Clear();
}

void ElectrocardiogramGUI::Clear()
{
    loadedEcg = nullptr;
    chart->removeAllSeries();
    chart->zoomReset();
    ui.btnClear->setEnabled(false);
    ui.btnSave->setEnabled(false);
    ui.actionUnloadFile->setEnabled(false);
    ui.lblCountVal->setText("-");
    ui.lblSumVal->setText("-");
    ui.lblMinVal->setText("-");
    ui.lblMaxVal->setText("-");
    ui.lblAverageVal->setText("-");
    ui.lblStdDeviationVal->setText("-");
    ui.lblHeartbeatsVal->setText("-");
    ui.lblBpmVal->setText("-");
}