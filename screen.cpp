#include "screen.h"

Screen::Screen(){
}

Screen::Screen(const Screen &)
{
    throw 1;
}

Screen &Screen::operator=(const Screen &)
{
    throw 1;
}

Screen::~Screen()
{
    for(MonitorView* m : mMonitors)
        delete m;
}

const QRect& Screen::getRect() const
{
    return mCurrentScreenRect;
}

void Screen::saveCrops(const QFile &path, const QImage& img, double imageScale, double monitorScale)
{
    for(auto i = mMonitors.begin(); i != mMonitors.end(); i++)
        i.value()->saveCrop(path.fileName() + "_" + i.key(), img, imageScale, monitorScale);
}

const MonitorView &Screen::getMonitor(const QString &name)
{
    return *mMonitors.find(name).value();
}

QString Screen::getMonitorName(Vec2i clickPosition, double mMonitorScale)
{
    auto it = mMonitors.begin();
    for(; it != mMonitors.end(); it++){
        if(it.value()->getModel().getRect(mMonitorScale).contains(clickPosition.x(), clickPosition.y()))
            return it.key();
    }
    return QString();
}

bool Screen::removeMonitor(const QString &name)
{
    bool found = mMonitors.remove(name) > 0;
    update();
    return found;
}

void Screen::addMonitor(const QString &name, const Vec2i &size, const Vec2i &pos)
{
    // note to self: if you construct the element without "new" here, the monitorview dies before it is even used
    mMonitors.insert(name, new MonitorView(size, pos));
    update();
}

void Screen::moveMonitors(int dX, int dY, const QImage& img)
{
    for(MonitorView* m : mMonitors)
        m->move(dX, dY);
    update();
    QRect newRect = getRect();
    // revert if out of bounds
    if(!img.rect().contains(newRect)){
        for(MonitorView* m : mMonitors)
            m->move(-dX, -dY);
        std::cout << "not contained" << std::endl;
    }
    update();
}

void Screen::draw(QPainter& painter)
{
    for(MonitorView* m : mMonitors)
        m->draw(painter, mMonitorScale);
}

void Screen::update()
{
    MonitorView* leftMost = getLeftMostMonitor().value();
    MonitorView* rightMost = getRightMostMonitor().value();
    MonitorView* topMost = getTopMostMonitor().value();
    MonitorView* bottomMost = getBottomMostMonitor().value();

    mCurrentScreenRect = QRect(
                leftMost->getModel().getRect(mMonitorScale).left(),
                topMost->getModel().getRect(mMonitorScale).top(),
                rightMost->getModel().getRect(mMonitorScale).right() - leftMost->getModel().getRect(mMonitorScale).left(),
                bottomMost->getModel().getRect(mMonitorScale).bottom() - topMost->getModel().getRect(mMonitorScale).top());
}

QMap<QString, MonitorView*>::iterator Screen::getRightMostMonitor()
{
    int val = 0;
    QMap<QString, MonitorView*>::iterator pos;
    auto it = mMonitors.begin();
    for(; it != mMonitors.end(); it++){
        int cVal = it.value()->getModel().getPosition().x() + it.value()->getModel().getSize().x();
        if(cVal > val){
            val = cVal;
            pos = it;
        }
    }
    return pos;
}

QMap<QString, MonitorView*>::iterator Screen::getLeftMostMonitor()
{
    int val = INT_MAX;
    QMap<QString, MonitorView*>::iterator pos;
    auto it = mMonitors.begin();
    for(; it != mMonitors.end(); it++){
        int cVal = it.value()->getModel().getPosition().x();
        if(cVal < val){
            val = cVal;
            pos = it;
        }
    }
    return pos;
}

QMap<QString, MonitorView*>::iterator Screen::getTopMostMonitor()
{
    int val = INT_MAX;
    QMap<QString, MonitorView*>::iterator pos;
    auto it = mMonitors.begin();
    for(; it != mMonitors.end(); it++){
        int cVal = it.value()->getModel().getPosition().y();
        if(cVal < val){
            val = cVal;
            pos = it;
        }
    }
    return pos;
}

QMap<QString, MonitorView*>::iterator Screen::getBottomMostMonitor()
{
    int val = 0;
    QMap<QString, MonitorView*>::iterator pos;
    auto it = mMonitors.begin();
    for(; it != mMonitors.end(); it++){
        int cVal = it.value()->getModel().getPosition().y() + it.value()->getModel().getSize().y();
        if(cVal > val){
            val = cVal;
            pos = it;
        }
    }
    return pos;
}
