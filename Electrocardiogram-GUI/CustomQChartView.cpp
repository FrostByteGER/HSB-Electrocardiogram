#include "CustomQChartView.hpp"

void CustomQChartView::mouseReleaseEvent(QMouseEvent* e)
{
if (e->button() == Qt::RightButton)
{
    chart()->zoomReset();
    return; //event doesn't go further
}
QChartView::mouseReleaseEvent(e);//any other event
    }