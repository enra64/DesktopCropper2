#ifndef SCREEN_H
#define SCREEN_H

#include <QMap>
#include <QRect>
#include <QSize>
#include <QFile>
#include <QPoint>

#include <iostream>

#include "monitor.h"

typedef QMap<QString, Monitor*> MonitorMap;

class Screen {
public:
    Screen();
    Screen(const Screen& o);
    Screen& operator=(const Screen& o);
    ~Screen();

    const QRect &getRect() const;

    void saveCrops(const QFile &path, const QImage &img, double imageScale);

    const Monitor& getMonitor(const QString& name);

    QString getMonitorName(QPoint clickPosition);

    bool removeMonitor(const QString& name);

    void addMonitor(const QString& name, const QSize &size, const QPoint &pos);

    void select(const QString& which, bool select = true);

    void selectAll(bool select = true);

    bool isSelected(const QString& which) { return getMonitor(which).isSelected(); }

    void draw(QPainter &painter);

    // family of possible transformations. If the transformed screen would not fit the img, they revert the action.
    void moveMonitors(int dX, int dY, const QImage &img);
    void setScale(const double factor, const QImage &img);
    void scaleBy(const double factor, const QImage &img);
private:
    /// Check whether all monitors (scaled) fit into img
    bool scaledMonitorsFitImage(const QImage& img);

    // family of transformations without reverting if not valid
    void noCheckScaleBy(const double factor);
    void noCheckSetScale(const double factor);
    void noCheckMove(int dX, int dY);

    /// update rectangle enclosing all monitors
    void updateRect();

    /// get outer border of the monitor sitting out the to border b
    int getOuterMonitorBorder(Border b);

    /// rectangle currently enclosing all monitors
    QRect mCurrentScreenRect;

    /// QMap of currently known monitors
    MonitorMap mMonitors;
};

#endif // SCREEN_H
