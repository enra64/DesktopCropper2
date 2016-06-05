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

    inline void setImageScale(double scale){ mImageScale = scale; }

    void setStatusbar(QStatusBar *s);
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
    bool mMousePressed = false;
    bool mMouseMoved = false;
    QPoint mMousePressBeginPosition;

    bool mImageLoaded = false;
    void scale();
    void moveMonitors(int dX, int dY);
    //double mMonitorScale = 1;
    double mImageScale = 1;
    QImage mImage;
    QLabel* mStatusBarView;
    QImage mOriginalImage;

    Screen mScreen;
    void updateStatusBar();
};

#endif // CROPPINGWIDGET_H
