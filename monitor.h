#ifndef MONITORVIEW_H
#define MONITORVIEW_H

#include <QPainter>
#include <QColor>

enum struct Border { TOP_BORDER, RIGHT_BORDER, BOTTOM_BORDER, LEFT_BORDER };
enum struct Scale { BOTH, Y, X };

class Monitor {
public:
    /**
     * @brief Monitor Constructor for creating a monitor with a given size and position. Used scale should be 1.
     * @param x x position
     * @param y y position
     * @param w width
     * @param h height
     */
    Monitor(int x, int y, int w, int h) : Monitor(QSize(w, h), QPoint(x, y)) {
    }

    /**
     * @brief Monitor Constructor for creating a monitor with a given size and position. Used scale should be 1.
     * @param size size
     * @param position position
     */
    Monitor(const QSize& size, const QPoint& position) {
        mUnscaledSize = QRect(position, size);
    }

    /**
     * @brief move Move the monitor
     * @param dX x delta
     * @param dY y delta
     */
    inline void move(int dX, int dY) {
        mUnscaledSize.adjust(dX, dY, dX, dY);
    }

    /**
     * @brief getBorderPosition Get the monitor border x/y position
     * @param b top/left/bottom/right border
     * @return position
     */
    int getBorderPosition(const Border& b) const {
        switch(b) {
        case Border::TOP_BORDER:
            return scaledRect().top();
        case Border::RIGHT_BORDER:
            return scaledRect().right();
        case Border::BOTTOM_BORDER:
            return scaledRect().bottom();
        case Border::LEFT_BORDER:
            return scaledRect().left();
        }
        throw "unknown border";
    }

    /**
     * @brief draw Draw this monitor onto the painter
     * @param painter QPainter that should be drawn on
     */
    void draw(QPainter& painter) const {
        const QRect modelRECT = scaledRect();
        QColor color = mIsSelected ? QColor::fromRgb(0, 255, 0) : QColor::fromRgb(255, 255, 255);
        painter.setPen(color);
        painter.drawRect(modelRECT);
    }

    /**
     * @brief saveCrop Crop this monitor out of the given image
     * @param p path to the save location
     * @param originalImage crop image source
     * @param imageScale scale the image is currently shown at
     * @return true if the image could be saved successfully
     */
    bool saveCrop(const QString& p, const QImage& originalImage, double imageScale) const {
        // scale must be 1, or the size would not be for example 1920x1080
        QRect cropRect = scaledRect(1, 1);

        // get unaltered image size
        QSize origImageSize = originalImage.size();

        // append jpg, the background setting script dies on everything else
        QString extendedPath = QString(p).append(".jpg");

        // scale independently so different monitor heights can be accounted for
        int xTargetSize = origImageSize.width() * imageScale * (1 / mXScale);
        int yTargetSize = origImageSize.height() * imageScale * (1 / mYScale);
        QSize targetSize(xTargetSize, yTargetSize);

        // scale the image so that we can crop the full-size monitors out of it.
        // ignore the ratio, because the scale ratios are calculated keeping the same ratios if possible
        QImage scaled = originalImage.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        // copy our monitor out of the image
        QImage cropped = scaled.copy(cropRect);

        // save
        return cropped.save(extendedPath, "jpg", 100);
    }

    /**
     * @brief contains Function to check whether a click is in the monitor
     * @param pos click position
     * @return true if the given position is within the monitor
     */
    bool contains(const QPoint& pos) const {
        return scaledRect().contains(pos);
    }

    /**
     * @brief scaleBy Set this monitor scale to factor
     * @param factor scale factor
     * @param which what axis the transformation should be applied to, either Scale::BOTH, Scale::X, or Scale::Y
     */
    void setScale(double factor, Scale which = Scale::BOTH) {
        if(which == Scale::BOTH || which == Scale::X)
            mXScale = factor;
        if(which == Scale::BOTH || which == Scale::Y)
            mYScale = factor;
    }

    /**
     * @brief scaleBy Scale this monitor by factor
     * @param factor scale factor
     * @param which what axis the transformation should be applied to, either Scale::BOTH, Scale::X, or Scale::Y
     */
    void scaleBy(double factor, Scale which = Scale::BOTH) {
        if(which == Scale::BOTH || which == Scale::X)
            mXScale *= factor;
        if(which == Scale::BOTH || which == Scale::Y)
            mYScale *= factor;
    }

    /**
     * @brief getMinScale Acquire the smaller scale factor of x and y
     * @return the smaller factor double
     */
    double getMinScale() const { return std::min(mYScale, mXScale); }


    /**
     * @brief isSelected Acquire the current selection state
     * @return true if the monitor is selected
     */
    inline bool isSelected() const { return mIsSelected; }

    /**
     * @brief setSelected set the selection state for this monitor
     * @param select future selection state
     */
    inline void setSelected(bool select) { mIsSelected = select; }

private:

    /**
     * @brief scaledRect Get the scaled rect for this monitor using saved factors
     * @return rectangle representing this monitor
     */
    inline QRect scaledRect() const {
        return scaledRect(mXScale, mYScale);
    }

    /**
     * @brief scaledRect Get the scaled rect for this monitor
     * @param xScale x axis scale factor
     * @param yScale y axis scale factor
     * @return rectangle representing this monitor
     */
    inline QRect scaledRect(double xScale, double yScale) const {
        return QRect(
                   mUnscaledSize.left() * xScale,
                   mUnscaledSize.top() * yScale,
                   mUnscaledSize.width() * xScale,
                   mUnscaledSize.height() * yScale);
    }

    /**
     * @brief mMonitorSize rectangle representing this monitors unscaled size
     */
    QRect mUnscaledSize;

    /**
     * @brief mXScale X axis scale
     */
    double mXScale = 1, mYScale = 1;

    /**
     * @brief mIsSelected Y axis scale
     */
    bool mIsSelected = true;
};

#endif // MONITORVIEW_H
