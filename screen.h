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

    /**
     * @brief select Set a monitor selection state
     * @param which monitor key
     * @param select future selection state
     */
    void select(const QString& which, bool select = true);

    /**
     * @brief select Set all monitors selection state
     * @param which monitor key
     * @param select future selection state
     */
    void selectAll(bool select = true);

    /**
     * @brief moveMonitors Move the screen without checking for validity
     * @param dX x delta
     * @param dY y delta
     * @param img the image the transformed screen must fit into
     */
    void moveMonitors(int dX, int dY, const QImage &img);

    /**
     * @brief setScale Scale the screen, reverting if the transformation result is invalid
     * @param factor scale factor
     * @param img the image the transformed screen must fit into
     */
    void setScale(const double factor, const QImage &img);

    /**
     * @brief scaleBy Scale the screen, reverting if the transformation result is invalid
     * @param factor scale factor
     * @param img the image the transformed screen must fit into
     */
    void scaleBy(const double factor, const QImage &img, Scale which = Scale::BOTH);

    /**
     * @brief removeMonitor
     * @param name name of the monitor to be removed
     * @return whether the monitor was found
     */
    bool removeMonitor(const QString& name);

    /**
     * @brief addMonitor
     * @param name what id to use for the monitor
     * @param size what size does the monitor have
     * @param pos at what position is the monitor
     */
    void addMonitor(const QString& name, const QSize &size, const QPoint &pos);

    /*
     * *************************************************************************************** PUBLIC CONST FUNCTIONS
     */


    /**
     * @brief getRect Get the rectangle representing the screen
     * @return QRect that contains all monitors
     */
    const QRect &getRect() const;

    /**
     * @brief saveCrops Saves all monitor crops from the image
     * @param path where to save to
     */
    void saveCrops(const QFile &path, const QImage &img, double imageScale) const;

    /**
     * @brief getMonitor Retrieves a monitor by name
     * @param name The name as it was given to the monitor
     * @return  Requested monitor reference
     */
    const Monitor& getMonitor(const QString& name) const;

    /**
     * @brief getMonitorName Translates from a clicked location to the monitor at that position
     * @param clickPosition where the concerned monitor was clicked
     * @return name of the clicked monitor
     */
    QString getMonitorName(QPoint clickPosition) const;

    /**
     * @brief isSelected Acquire the current selection state of "which"
     * @param which Name of the monitor the information is requested from
     * @return true if the monitor is selected
     */
    bool isSelected(const QString& which) const;

    /**
     * @brief draw Draw all monitors
     * @param painter QPainter that ist to be drawn on
     */
    void draw(QPainter &painter) const;

    /**
     * @brief getMinScaleFactor Retrieve the smallest current scale factor
     * @return double
     */
    double getMinScaleFactor() const;

    /**
     * @brief getUnscaledRect Get the screen rectangle the monitors would have if the screen was unscaled. copy screen, set scale to 1, return rect, delete copy.
     * @return QRect
     */
    QRect getUnscaledRect() const;

    /**
     * @brief getSizeAsString Retrieve the screen size as a string
     * @return string representation of screen size
     */
    QString getSizeAsString() const;
private:
    /**
     * @brief scaledMonitorsFitImage Check whether this screen fits into an image
     * @param img image that the screen must fit into
     * @return true if the screen fits
     */
    bool scaledMonitorsFitImage(const QImage& img) const;

    /**
     * @brief noCheckScaleBy Scale the screen without checking for validity
     * @param factor scale factor
     * @param which which axis to scale
     */
    void noCheckScaleBy(const double factor, Scale which = Scale::BOTH);

    /**
     * @brief noCheckSetScale Scale the screen without checking for validity
     * @param factor scale factor
     * @param which which axis to scale
     */
    void noCheckSetScale(const double factor, Scale which = Scale::BOTH);

    /**
     * @brief noCheckMove Move the screen without checking for validity
     * @param dX x delta
     * @param dY y delta
     */
    void noCheckMove(int dX, int dY);

    /**
     * @brief updateRect update the cached rectangle enclosing all monitors
     */
    void updateRect();

    /// get outer border of the monitor sitting out the to border b
    /**
     * @brief getOuterMonitorBorder Get border of the screen, so the outermost monitors define the border
     * @param b which border to check
     * @return int position on the x or y axis
     */
    int getOuterMonitorBorder(Border b) const;

    /**
     * @brief mCurrentScreenRect rectangle currently enclosing all monitors
     */
    QRect mCurrentScreenRect;

    /**
     * @brief mMonitors QMap of currently known monitors
     */
    MonitorMap mMonitors;
};

#endif // SCREEN_H
