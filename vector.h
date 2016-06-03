#ifndef VECTOR_H
#define VECTOR_H

/*
template <typename T>
class Vector
{
public:
    Vector(){
        mX = 0;
        mY = 0;
    }

    Vector(T x, T y) {
        mX = x;
        mY = y;
    }

    inline T x() const {
        return mX;
    }

    inline T y() const {
        return mY;
    }

    inline void setX(T x) {
        mX = x;
    }

    inline void setY(T y) {
        mY = y;
    }

    Vector<T> getScaled(double scale){
        return Vector<T>(mX * scale, mY * scale);
    }

private:
    T mX;
    T mY;
};

typedef Vector<int> Vec2i;*/

class Vec2i {
public:
    Vec2i() {
        mX = 0;
        mY = 0;
    }

    Vec2i(const Vec2i& o) {
        mX = o.mX;
        mY = o.mY;
    }

    Vec2i& operator= (const Vec2i& o) {
        mX = o.mX;
        mY = o.mY;
        return *this;
    }

    ~Vec2i() { }

    Vec2i(int x, int y) {
        mX = x;
        mY = y;
    }

    inline int x() const {
        return mX;
    }

    inline int y() const {
        return mY;
    }

    inline void setX(int x) {
        mX = x;
    }

    inline void setY(int y) {
        mY = y;
    }

private:
    int mX;
    int mY;
};

#endif // VECTOR_H
