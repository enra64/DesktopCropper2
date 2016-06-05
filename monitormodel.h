#ifndef MONITORMODEL_H
#define MONITORMODEL_H

#include <QRect>

enum struct Border { TOP_BORDER, RIGHT_BORDER, BOTTOM_BORDER, LEFT_BORDER };

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

    inline int getBorderPosition(const Border& b, double scale = 1) const {
        switch(b){
            case Border::TOP_BORDER:
                return (mPosition.y()) * scale;
            case Border::RIGHT_BORDER:
                return (mPosition.x() + mSize.width()) * scale;
            case Border::BOTTOM_BORDER:
                return (mPosition.y() + mSize.height()) * scale;
            case Border::LEFT_BORDER:
                return mPosition.x() * scale;
        }
        throw "unknown border";
    }

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
