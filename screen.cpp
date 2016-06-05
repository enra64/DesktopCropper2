#include "screen.h"

Screen::Screen() {
}

Screen::Screen(const Screen &o) {
    /// rectangle currently enclosing all monitors
    mCurrentScreenRect = o.mCurrentScreenRect;

    for(auto it = o.mMonitors.begin(); it != o.mMonitors.end(); it++)
        mMonitors.insert(it.key(), new Monitor(*it.value()));
}

Screen &Screen::operator=(const Screen &) {
    throw 1;
}

Screen::~Screen() {
    for(Monitor* m : mMonitors)
        delete m;
}

const QRect& Screen::getRect() const {
    return mCurrentScreenRect;
}

void Screen::saveCrops(const QFile &path, const QImage& img, double imageScale) {
    for(auto i = mMonitors.begin(); i != mMonitors.end(); i++)
        i.value()->saveCrop(path.fileName() + "_" + i.key(), img, imageScale);
}

const Monitor &Screen::getMonitor(const QString &name) {
    return *mMonitors.find(name).value();
}

QString Screen::getMonitorName(QPoint clickPosition) {
    for(auto it = mMonitors.begin(); it != mMonitors.end(); it++)
        if(it.value()->contains(clickPosition))
            return it.key();
    return QString();
}

bool Screen::removeMonitor(const QString &name) {
    bool found = mMonitors.remove(name) > 0;
    updateRect();
    return found;
}

void Screen::addMonitor(const QString &name, const QSize &size, const QPoint &pos) {
    // note to self: if you construct the element without "new" here, the monitorview dies before it is even used
    mMonitors.insert(name, new Monitor(size, pos));
    updateRect();
}

void Screen::select(const QString &which, bool select) {
    mMonitors.find(which).value()->setSelected(select);
}

void Screen::selectAll(bool select) {
    for(Monitor* m : mMonitors)
        m->setSelected(select);
}

void Screen::draw(QPainter& painter) {
    for(Monitor* m : mMonitors)
        m->draw(painter);
}


bool Screen::scaledMonitorsFitImage(const QImage& img) {
    return img.rect().contains(getRect());
}

void Screen::noCheckScaleBy(const double factor) {
    for(Monitor* m : mMonitors)
        if(m->isSelected())
            m->scaleBy(factor);
    updateRect();
}

void Screen::noCheckSetScale(const double factor) {
    for(Monitor* m : mMonitors)
        if(m->isSelected())
            m->setScale(factor);
    updateRect();
}

void Screen::noCheckMove(int dX, int dY) {
    for(Monitor* m : mMonitors)
        if(m->isSelected())
            m->move(dX, dY);
    updateRect();
}

void Screen::scaleBy(const double factor, const QImage& img) {
    Screen testScreen(*this);
    testScreen.noCheckScaleBy(factor);
    if(testScreen.scaledMonitorsFitImage(img))
        noCheckScaleBy(factor);
}


void Screen::setScale(const double factor, const QImage &img) {
    Screen testScreen(*this);
    testScreen.noCheckSetScale(factor);
    if(testScreen.scaledMonitorsFitImage(img))
        noCheckSetScale(factor);
}

void Screen::moveMonitors(int dX, int dY, const QImage& img) {
    Screen testScreen(*this);
    testScreen.noCheckMove(dX, dY);
    if(testScreen.scaledMonitorsFitImage(img))
        noCheckMove(dX, dY);
}

void Screen::updateRect() {
    int left = getOuterMonitorBorder(Border::LEFT_BORDER);
    int top = getOuterMonitorBorder(Border::TOP_BORDER);

    int width = getOuterMonitorBorder(Border::RIGHT_BORDER) - left;
    int height = getOuterMonitorBorder(Border::BOTTOM_BORDER) - top;

    mCurrentScreenRect = QRect(left, top, 1 + width, 1 + height);
}

int Screen::getOuterMonitorBorder(Border b) {
    bool botOrRightBorder = b == Border::BOTTOM_BORDER || b == Border::RIGHT_BORDER;
    int val = botOrRightBorder ? 0 : INT_MAX;
    for(auto it = mMonitors.begin(); it != mMonitors.end(); it++) {
        int cVal = it.value()->getBorderPosition(b);
        if(botOrRightBorder)
            val = cVal > val ? cVal : val;
        else
            val = cVal < val ? cVal : val;
    }
    return val;
}
