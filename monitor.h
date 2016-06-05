#ifndef MONITORVIEW_H
#define MONITORVIEW_H

#include <QPainter>
#include <QColor>

enum struct Border { TOP_BORDER, RIGHT_BORDER, BOTTOM_BORDER, LEFT_BORDER };

class Monitor {
public:
    Monitor(int x, int y, int w, int h) : Monitor(QSize(w, h), QPoint(x, y)) {
    }

    Monitor(const QSize& size, const QPoint& position) {
        mMonitorSize = QRect(position, size);
    }

    inline void move(int dX, int dY) {
        mMonitorSize.adjust(dX, dY, dX, dY);
    }

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

    void draw(QPainter& painter) {
        const QRect modelRECT = scaledRect();
        QColor color = mIsSelected ? QColor::fromRgb(0, 255, 0) : QColor::fromRgb(255, 255, 255);
        painter.setPen(color);
        painter.drawRect(modelRECT);
    }

    bool saveCrop(const QString& p, const QImage& originalImage, double imageScale) {
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

        //
        return cropped.save(extendedPath, "jpg", 100);
    }

    bool contains(const QPoint& pos) const {
        return scaledRect().contains(pos);
    }

    void setScale(double factor) {
        mXScale = factor;
        mYScale = factor;
    }

    void scaleBy(double factor) {
        mXScale *= factor;
        mYScale *= factor;
    }

    inline bool isSelected() const {
        return mIsSelected;
    }

    inline void setSelected(bool select) {
        mIsSelected = select;
    }

private:
    /// Returns the rectangle of this monitor, scaled by the saved scales
    inline QRect scaledRect() const {
        return scaledRect(mXScale, mYScale);
    }

    /// Returns the rectangle of this monitor, scaled by the given factor
    inline QRect scaledRect(double xScale, double yScale) const {
        return QRect(
                   mMonitorSize.left() * xScale,
                   mMonitorSize.top() * yScale,
                   mMonitorSize.width() * xScale,
                   mMonitorSize.height() * yScale);
    }

    QRect mMonitorSize;
    double mXScale = 1, mYScale = 1;
    bool mIsSelected = true;
};

#endif // MONITORVIEW_H
