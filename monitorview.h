#ifndef MONITORVIEW_H
#define MONITORVIEW_H

#include "monitormodel.h"

#include <QPainter>
#include <QColor>

class MonitorView
{
public:
    MonitorView(int x, int y, int w, int h) : MonitorView(QSize(w, h), QPoint(x, y)){
    }

    MonitorView(const QSize& size, const QPoint& position){
        mModel = new MonitorModel(size, position);
    }

    ~MonitorView() {
        delete mModel;
    }

    inline const MonitorModel& getModel() const {
        return *mModel;
    }

    inline void move(int dX, int dY) {
        mModel->move(dX, dY);
    }

    void draw(QPainter& painter, double scale){
        const QRect modelRECT = mModel->getRect(scale);
        QColor color = mModel->isSelected() ? QColor::fromRgb(0, 255, 0) : QColor::fromRgb(255, 255, 255);
        painter.setPen(color);
        painter.drawRect(modelRECT);
    }

    bool saveCrop(const QString& p, const QImage& originalImage, double imageScale, double monitorScale){
        // scale must be 1, or the size would not be for example 1920x1080
        QRect cropRect = mModel->getRect(1);
        QSize origImageSize = originalImage.size();
        QString extendedPath = QString(p).append(".jpg");

        QSize targetSize = origImageSize * imageScale * (1 / monitorScale);

        QImage scaled = originalImage.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QImage cropped = scaled.copy(cropRect);
        return cropped.save(extendedPath, "jpg", 100);
    }

    inline bool isSelected() const {
        return mModel->isSelected();
    }

    inline void setSelected(bool select) {
        mModel->setSelected(select);
    }

private:
    MonitorModel* mModel = nullptr;
};

#endif // MONITORVIEW_H
