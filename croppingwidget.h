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

#include "monitorview.h"

class CroppingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CroppingWidget(QWidget *parent = 0);

    ~CroppingWidget();

    bool removeMonitor(const QString& name);

    void addMonitor(const QString& name, const Vec2i &size, const Vec2i &pos);

    const MonitorView& getMonitor(const QString& name);

    QString getMonitorName(Vec2i clickPosition);

    void selectMonitor(const QString& name, bool select);

    void saveCrops(const QFile &path);

    bool loadImage(const QFile &path);

    inline void setImageScale(double scale){ mImageScale = scale; }
    inline void setMonitorScale(double scale){ mMonitorScale = scale; }

    void setStatusbar(QStatusBar *s);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event);
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent*e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent*);

private:
    bool mousePressed = false;
    QPoint mMousePressBeginPosition;
    QMap<QString, MonitorView*> mMonitors;
    void scale(const QSize& size);
    void moveMonitors(int dX, int dY);
    int getMonitorOverallWidth();
    double mMonitorScale = 1;
    double mImageScale = 1;
    QImage mImage;
    QLabel* mStatusBarView;
    QImage mOriginalImage;
};

#endif // CROPPINGWIDGET_H
