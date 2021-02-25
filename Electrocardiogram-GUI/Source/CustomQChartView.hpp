#pragma once
#include <qchartview.h>

class CustomQChartView : public QtCharts::QChartView
{
public:
    CustomQChartView(QWidget* parent = nullptr) : QChartView(parent){}

    CustomQChartView(QtCharts::QChart* chart, QWidget* parent)
        : QChartView(chart, parent)
    {
    }

protected:
    void mouseReleaseEvent(QMouseEvent* e) override;
};
