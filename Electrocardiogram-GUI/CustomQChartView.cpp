#include "CustomQChartView.hpp"

void CustomQChartView::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton)
    {
        chart()->zoomReset();
        return;
    }
    QChartView::mouseReleaseEvent(e);
}