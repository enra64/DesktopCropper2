#ifndef MONITORMODEL_H
#define MONITORMODEL_H

#include <QRect>

class MonitorModel
{
public:
    MonitorModel(const QSize& size, const QPoint& position) :
        mSize(size),
        mPosition(position) {
    }

    MonitorModel(const MonitorModel& o) :
        mSize(o.mSize),
        mPosition(o.mPosition),
        mIsSelected(o.mIsSelected){
    }

    MonitorModel& operator=(const MonitorModel& that){
        mSize = that.mSize;
        mPosition = that.mPosition;
        mIsSelected = that.mIsSelected;
        return *this;
    }

    inline int getLeft() const { return mPosition.x(); }
    inline int getRight() const { return mPosition.x() + getWidth(); }
    inline int getTop() const { return mPosition.y(); }
    inline int getBottom() const { return mPosition.x() + getHeight(); }

    inline int getWidth() const { return mSize.width(); }
    inline int getHeight() const { return mSize.height(); }

    inline void setSize(const QSize& size){
        mSize = size;
    }

    inline void move(int dX, int dY){
        mPosition += QPoint(dX, dY);
    }

    inline void setPosition(const QPoint& pos){
        mPosition = pos;
    }

    inline QRect getRect(double scale = 1) const {
        return QRect(mPosition * scale, mSize * scale);
    }

    inline bool isSelected() const {
        return mIsSelected;
    }

    inline void setSelected(bool select) {
        mIsSelected = select;
    }

private:
    QSize mSize;
    QPoint mPosition;
    bool mIsSelected = true;
};

#endif // MONITORMODEL_H
