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
#include "screenundowrapper.h"

class CroppingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CroppingWidget(QWidget *parent = 0);

    ~CroppingWidget();

    /**
     * @brief saveCrops Saves crops from the image
     * @param path where to save to
     */
    void saveCrops(const QFile &path) const;

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

    /**
     * @brief selectAllMonitors set selection state for all monitors
     * @param select future selection state
     */
    void selectAllMonitors(bool select);

    /**
     * @brief loadImage load the image to be cropped
     * @param path where the image is to be found
     * @return load operation success
     */
    bool loadImage(const QFile &path);

    /**
     * @brief setStatusbar saves a pointer to the statusbar so that we can update it
     * @param s status bar reference
     */
    void setStatusbar(QStatusBar *s);

    /**
     * @brief resetMonitors reset each monitor scale to x=y, maximum fit, and 0x0 position
     */
    void resetMonitors();

    /**
     * @brief onUndo undo the last action
     */
    void onUndo();

    /**
     * @brief onRedo redo the last action
     */
    void onRedo();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event);

    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent*e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent*e);

private:
    /**
     * @brief mMouseMoved has the mouse moved since the last click
     */
    bool mMouseMoved = false;

    /**
     * @brief mMousePressBeginPosition Where did the last mouse click occur
     */
    QPoint mMousePressBeginPosition;

    /**
     * @brief mImageLoaded image currently loaded
     */
    bool mImageLoaded = false;

    /**
     * @brief scaleToWindowSize scales the image to widget size, and the monitors by the factor of difference
     */
    void scaleToWindowSize();

    /**
     * @brief fullQualityCropPossible whether the crop can be done without quality loss
     * @return true if the monitors can be cropped without scaling the image > 1
     */
    bool fullQualityCropPossible() const;

    /**
     * @brief getMonitorName Translates from a clicked location to the monitor at that position
     * @param clickPosition where the concerned monitor was clicked
     * @return name of the clicked monitor
     */
    QString getMonitorName(QPoint clickPosition) const;

    /**
     * @brief moveMonitors Move the monitors
     * @param dX x delta
     * @param dY y delta
     */
    void moveMonitors(int dX, int dY);

    /**
     * @brief imageScale calculate the scale from the original to the current image
     * @return currentSize / oldSize
     */
    double imageScale() const ;

    /**
     * @brief mCurrentImage the original image, but with all transformations applied
     */
    QImage mCurrentImage;

    /**
     * @brief mOriginalImage the original image without any applied transformations
     */
    QImage mOriginalImage;

    /**
     * @brief mStatusBarView Status bar for displaying information
     */
    QLabel* mStatusBarView;

    /**
     * @brief mScreen object handling all things monitor
     */
    ScreenUndoWrapper mScreen;

    /**
     * @brief updateStatusBar refresh the informatino shown on the status bar
     */
    void updateStatusBar() const;
};

#endif // CROPPINGWIDGET_H
