#ifndef VIEWSPACE_H
#define VIEWSPACE_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>

class Viewspace : public QGraphicsView
{
    Q_OBJECT
public:
    Viewspace(QWidget *parent = 0);
    ~Viewspace();
    void viewFit();
    void zoomIn();
    void zoomOut();

private:
    void scaleView(qreal scaleFactor);
    bool isResized;
    bool isLandscape;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // VIEWSPACE_H
