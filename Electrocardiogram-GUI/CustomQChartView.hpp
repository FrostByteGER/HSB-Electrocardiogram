#pragma once
#include <qchartview.h>

class CustomQChartView : public QtCharts::QChartView
{
protected:
    void mouseReleaseEvent(QMouseEvent* e) override;
};
