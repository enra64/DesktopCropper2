#ifndef SCREENUNDOWRAPPER_H
#define SCREENUNDOWRAPPER_H

#include "screen.h"
#include <memory>

typedef std::list<std::shared_ptr<Screen>> ScreenDList;

class ScreenUndoWrapper
{
public:
    ScreenUndoWrapper();

    /*
     *
     *
     *
     *
     * ************************************************************************************** UNDO FUNCTIONS
     *
     *
     *
     *
     */

    /**
     * @brief undo undo the last action
     */
    void undo(const QImage &img);

    /**
     * @brief redo undo the last undo
     */
    void redo(const QImage& img);

    /*
     *
     *
     *
     *
     * *************************************************************************************** PUBLIC NON CONST SCREEN FUNCTIONS
     *
     *
     *
     *
     *
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
     *
     *
     *
     *
     * *************************************************************************************** PUBLIC CONST SCREEN FUNCTIONS
     *
     *
     *
     *
     *
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
    const Monitor &getMonitor(const QString& name) const;

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
     * @brief saveState Push a copy of the current object state, which we can then modify. If there are items in between the list start and our current position, erase those, they are undone.
     */
    void saveState();

    bool mSavedBeginningOfScaleBy = false, mSavedBeginningOfSetScale = false, mSavedBeginningOfMove = false;

    std::shared_ptr<Screen> getCurrentScreen() const;

    ScreenDList::iterator mCurrentPosition;

    ScreenDList mScreenList;
};

#endif // SCREENUNDOWRAPPER_H
