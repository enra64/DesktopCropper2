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

class Screen
{
public:
    Screen();
    Screen(const Screen& o);
    Screen& operator=(const Screen& o);
    ~Screen();
    const QRect &getRect() const;

    void saveCrops(const QFile &path, const QImage &img, double imageScale, double monitorScale);

    const MonitorView& getMonitor(const QString& name);
    QString getMonitorName(Vec2i clickPosition, double mMonitorScale);
    bool removeMonitor(const QString& name);
    void addMonitor(const QString& name, const Vec2i &size, const Vec2i &pos);

    void moveMonitors(int dX, int dY, const QImage &img);

    void draw(QPainter &painter);

    inline double& getMonitorScale() { return mMonitorScale; }
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
