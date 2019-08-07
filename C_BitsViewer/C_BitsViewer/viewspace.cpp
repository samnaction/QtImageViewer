#include "viewspace.h"

Viewspace::Viewspace(QWidget *parent)
    : QGraphicsView(parent)
{
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::HighQualityAntialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    isResized = false;
    isLandscape = false;

    setDragMode(QGraphicsView::ScrollHandDrag);
}

Viewspace::~Viewspace()
{
}

void Viewspace::zoomIn()
{
    scaleView(qreal(1.2));
}

void Viewspace::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void Viewspace::viewFit()
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
    isResized = true;

    if (sceneRect().width() > sceneRect().height())
        isLandscape = true;
    else
        isLandscape = false;
}

void Viewspace::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        if (event->delta() > 0) zoomIn();
        else zoomOut();
    }
    else if (event->modifiers() == Qt::ShiftModifier)
    {
        QWheelEvent fakeEvent(event->pos(), event->delta(), event->buttons(), Qt::NoModifier, Qt::Horizontal);
        QGraphicsView::wheelEvent(&fakeEvent);
    }
    else if (event->modifiers() == Qt::NoModifier)
    {
        QGraphicsView::wheelEvent(event);
    }
}

void Viewspace::scaleView(qreal scaleFactor)
{
    if(sceneRect().isEmpty())
        return;

    QRectF expectedRect = transform().scale(scaleFactor, scaleFactor).mapRect(sceneRect());
    qreal expRectLength;
    int viewportLength;
    int imgLength;

    if (isLandscape)
    {
        expRectLength = expectedRect.width();
        viewportLength = viewport()->rect().width();
        imgLength = sceneRect().width();
    }
    else
    {
        expRectLength = expectedRect.height();
        viewportLength = viewport()->rect().height();
        imgLength = sceneRect().height();
    }

    if (expRectLength < viewportLength / 2) // minimum zoom : half of viewport
    {
        if (!isResized || scaleFactor < 1)
            return;
    }
    else if (expRectLength > imgLength * 10) // maximum zoom : x10
    {
        if (!isResized || scaleFactor > 1)
            return;
    }
    else
    {
        isResized = false;
    }

    scale(scaleFactor, scaleFactor);
}

void Viewspace::resizeEvent(QResizeEvent *event)
{
    isResized = true;
    QGraphicsView::resizeEvent(event);
}
