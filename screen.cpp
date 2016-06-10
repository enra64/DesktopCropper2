#include "screen.h"

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

QRect Screen::getUnscaledRect() const {
    Screen testScreen(*this);
    testScreen.noCheckSetScale(1);
    return testScreen.getRect();
}

QString Screen::getSizeAsString() const {
    QSize s = getUnscaledRect().size();
    return QString("%1x%2").arg(s.width()).arg(s.height());
}

const QRect& Screen::getRect() const {
    return mCurrentScreenRect;
}

void Screen::saveCrops(const QFile &path, const QImage& img, double imageScale) const {
    for(auto i = mMonitors.begin(); i != mMonitors.end(); i++)
        i.value()->saveCrop(path.fileName() + "_" + i.key(), img, imageScale);
}

const Monitor& Screen::getMonitor(const QString &name) const {
    return *mMonitors.find(name).value();
}

QString Screen::getMonitorName(QPoint clickPosition) const {
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

bool Screen::isSelected(const QString &which) const {
    return getMonitor(which).isSelected();
}

void Screen::draw(QPainter& painter) const {
    for(Monitor* m : mMonitors)
        m->draw(painter);
}

double Screen::getMinScaleFactor() const {
    double minScale = INT_MAX;
    for(Monitor* m : mMonitors)
        minScale = std::min(minScale, m->getMinScale());
    return minScale;
}

bool Screen::scaledMonitorsFitImage(const QImage& img) const {
    return img.rect().contains(getRect());
}

void Screen::noCheckScaleBy(const double factor, Scale which) {
    for(Monitor* m : mMonitors)
        if(m->isSelected())
            m->scaleBy(factor, which);
    updateRect();
}

void Screen::noCheckSetScale(const double factor, Scale which) {
    for(Monitor* m : mMonitors)
        if(m->isSelected())
            m->setScale(factor, which);
    updateRect();
}

void Screen::noCheckMove(int dX, int dY) {
    for(Monitor* m : mMonitors)
        if(m->isSelected())
            m->move(dX, dY);
    updateRect();
}

void Screen::scaleBy(const double factor, const QImage& img, Scale which) {
    Screen testScreen(*this);
    testScreen.noCheckScaleBy(factor, which);
    if(testScreen.scaledMonitorsFitImage(img))
        noCheckScaleBy(factor, which);
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
    QPoint topLeft(getOuterMonitorBorder(Border::LEFT_BORDER), getOuterMonitorBorder(Border::TOP_BORDER));
    QPoint bottomRight(getOuterMonitorBorder(Border::RIGHT_BORDER), getOuterMonitorBorder(Border::BOTTOM_BORDER));

    mCurrentScreenRect = QRect(topLeft, bottomRight);
}

int Screen::getOuterMonitorBorder(Border b) const {
    bool botOrRightBorder = b == Border::BOTTOM_BORDER || b == Border::RIGHT_BORDER;
    int val = botOrRightBorder ? 0 : INT_MAX;
    for(auto it = mMonitors.begin(); it != mMonitors.end(); it++) {
        int cVal = it.value()->getBorderPosition(b);
        val = botOrRightBorder ? std::max(cVal, val) : std::min(cVal, val);
    }
    return val;
}
