#ifndef MONITORMODEL_H
#define MONITORMODEL_H

#include "vector.h"

#include <QRect>

class MonitorModel
{
public:
    MonitorModel(const Vec2i& size, const Vec2i& position) :
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

    inline const Vec2i& getPosition() const { return mPosition; }

    inline const Vec2i& getSize() const { return mSize; }

    inline void setSize(const Vec2i& size){
        mSize = size;
    }

    inline void move(int dX, int dY){
        mPosition.setX(mPosition.x() + dX);
        mPosition.setY(mPosition.y() + dY);
    }

    inline void setPosition(const Vec2i& pos){
        mPosition = pos;
    }

    inline QRect getRect(double scale = 1) const {
        return QRect(mPosition.x() * scale,
                     mPosition.y() * scale,
                     mSize.x() * scale,
                     mSize.y() * scale);
    }

    inline bool isSelected() const {
        return mIsSelected;
    }

    inline void setSelected(bool select) {
        mIsSelected = select;
    }

private:
    Vec2i mSize;
    Vec2i mPosition;
    bool mIsSelected = false;
};

#endif // MONITORMODEL_H
