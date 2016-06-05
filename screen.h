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

    // selection controls
    void select(const QString& which, bool select = true);
    void selectAll(bool select = true);
    bool isSelected(const QString& which) { return getMonitor(which).isSelected(); }

    // draw all screens onto the painter
    void draw(QPainter &painter);

    /// Return the maximum scale factor currently in use by any monitor
    double getMinScaleFactor();

    // family of possible transformations. If the transformed screen would not fit the img, they revert the action.
    void moveMonitors(int dX, int dY, const QImage &img);
    void setScale(const double factor, const QImage &img);
    void scaleBy(const double factor, const QImage &img, Scale which = Scale::BOTH);
    void scaleAllTo(const QImage& img);

    /// copy screen, set scale to 1, return rect, delete copy.
    QRect getUnscaledRect() const;

    ///
    QString getSizeAsString() const;
private:
    /// Check whether all monitors (scaled) fit into img
    bool scaledMonitorsFitImage(const QImage& img);

    // family of transformations without reverting if not valid
    void noCheckScaleAllTo(const QImage& img);
    void noCheckScaleBy(const double factor, Scale which = Scale::BOTH);
    void noCheckSetScale(const double factor, Scale which = Scale::BOTH);
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
