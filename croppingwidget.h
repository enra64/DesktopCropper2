#ifndef CROPPINGWIDGET_H
#define CROPPINGWIDGET_H

#include <QResizeEvent>
#include <QLabel>
#include <QWidget>
#include <QImage>
#include <QMap>
#include <QPainter>
#include <QFile>
#include <QStatusBar>

#include <iostream>

#include "monitor.h"
#include "screen.h"

class CroppingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CroppingWidget(QWidget *parent = 0);

    ~CroppingWidget();

    bool removeMonitor(const QString& name);

    void addMonitor(const QString& name, const QSize &size, const QPoint &pos);

    const Monitor& getMonitor(const QString& name);

    QString getMonitorName(QPoint clickPosition);

    void selectAllMonitors(bool select);

    void saveCrops(const QFile &path);

    bool loadImage(const QFile &path);

    void setStatusbar(QStatusBar *s);

    bool fullQualityCropPossible();

    void resetMonitors();
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event);
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent*e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent*e);

private:
    // mouse stuff
    bool mMousePressed = false;
    bool mMouseMoved = false;
    QPoint mMousePressBeginPosition;

    /// is there an image loaded?
    bool mImageLoaded = false;

    void scale();

    /// Moves monitors by delta
    void moveMonitors(int dX, int dY);

    /// calculate the scale between the original and the current image
    double imageScale();

    /// image data
    QImage mCurrentImage, mOriginalImage;

    /// pointer to the status bar view available for showing some relevant data
    QLabel* mStatusBarView;

    Screen mScreen;

    /// update the status bar information
    void updateStatusBar();
};

#endif // CROPPINGWIDGET_H
