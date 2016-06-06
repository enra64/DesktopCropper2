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

    /*
     * *************************************************************************************** PUBLIC NON CONST FUNCTIONS
     */

    // selection controls
    void select(const QString& which, bool select = true);
    void selectAll(bool select = true);

    // family of possible transformations. If the transformed screen would not fit the img, they revert the action.
    void moveMonitors(int dX, int dY, const QImage &img);
    void setScale(const double factor, const QImage &img);
    void scaleBy(const double factor, const QImage &img, Scale which = Scale::BOTH);

    // monitor control
    bool removeMonitor(const QString& name);
    void addMonitor(const QString& name, const QSize &size, const QPoint &pos);

    /*
     * *************************************************************************************** PUBLIC CONST FUNCTIONS
     */


    /// Returns the rectangle representing the screen
    const QRect &getRect() const;

    /// Save all monitor crops
    void saveCrops(const QFile &path, const QImage &img, double imageScale) const;

    // monitor retrieval
    const Monitor& getMonitor(const QString& name) const;
    QString getMonitorName(QPoint clickPosition) const;

    /// Return if "which" is selected
    bool isSelected(const QString& which) const;

    // draw all screens onto the painter
    void draw(QPainter &painter) const;

    /// Return the maximum scale factor currently in use by any monitor
    double getMinScaleFactor() const;

    /// copy screen, set scale to 1, return rect, delete copy.
    QRect getUnscaledRect() const;

    /// convert the rect size to string
    QString getSizeAsString() const;
private:
    /// Check whether all monitors (scaled) fit into img
    bool scaledMonitorsFitImage(const QImage& img) const;

    // family of transformations without reverting if not valid
    void noCheckScaleBy(const double factor, Scale which = Scale::BOTH);
    void noCheckSetScale(const double factor, Scale which = Scale::BOTH);
    void noCheckMove(int dX, int dY);

    /// update rectangle enclosing all monitors
    void updateRect();

    /// get outer border of the monitor sitting out the to border b
    int getOuterMonitorBorder(Border b) const;

    /// rectangle currently enclosing all monitors
    QRect mCurrentScreenRect;

    /// QMap of currently known monitors
    MonitorMap mMonitors;
};

#endif // SCREEN_H
