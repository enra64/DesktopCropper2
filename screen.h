#ifndef SCREEN_H
#define SCREEN_H

#include <QMap>
#include <QRect>
#include <QSize>
#include <QFile>
#include <QPoint>

#include <iostream>

#include "monitorview.h"

typedef QMap<QString, MonitorView*> MonitorMap;

class Screen {
public:
    Screen();
    Screen(const Screen& o);
    Screen& operator=(const Screen& o);
    ~Screen();
    const QRect &getRect() const;

    void saveCrops(const QFile &path, const QImage &img, double imageScale, double monitorScale);

    const MonitorView& getMonitor(const QString& name);
    QString getMonitorName(QPoint clickPosition);
    bool removeMonitor(const QString& name);
    void addMonitor(const QString& name, const QSize &size, const QPoint &pos);

    void moveMonitors(int dX, int dY, const QImage &img);

    void select(const QString& which, bool select = true);
    void selectAll(bool select = true);
    bool isSelected(const QString& which) { return getMonitor(which).isSelected(); }

    void draw(QPainter &painter);

    void setScale(const double factor) {
        mMonitorScale = factor;
        update();
    }

    void scaleBy(const double factor) {
        mMonitorScale *= factor;
        update();
    }

    inline const double& getMonitorScale() const {
        return mMonitorScale;
    }

private:
    QRect mCurrentScreenRect;
    void update();

    MonitorMap::iterator getRightMostMonitor();
    MonitorMap::iterator getLeftMostMonitor();
    MonitorMap::iterator getTopMostMonitor();
    MonitorMap::iterator getBottomMostMonitor();

    MonitorMap mMonitors;
    double mMonitorScale = 1;
};

#endif // SCREEN_H
