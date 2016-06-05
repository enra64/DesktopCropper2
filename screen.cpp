#include "screen.h"

Screen::Screen() {}

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

double Screen::getMinScaleFactor() {
    double minScale = INT_MAX;
    for(Monitor* m : mMonitors)
        minScale = std::min(minScale, m->getMinScale());
    return minScale;
}

// TODO: add ctrl+z -.-.

bool Screen::scaledMonitorsFitImage(const QImage& img) {
    return img.rect().contains(getRect());
}

void Screen::noCheckScaleAllTo(const QImage& img) {
    // what scale for the monitors to fit the image
    QSize currentSize = mCurrentScreenRect.size();
    QPoint currentPosition = mCurrentScreenRect.topLeft();

    // add the position, otherwise, if pos != 0,0, we scale out of the image
    currentSize += QSize(currentPosition.x(), currentPosition.y());

    // scale to fit within the image
    QSize targetSize = currentSize.scaled(img.size(), Qt::KeepAspectRatio);

    // calculate scaled factor.
    double factor = (double) targetSize.width() / (double) currentSize.width();

    // scale all monitors
    for(Monitor* m : mMonitors)
        m->scaleBy(factor);

    // update rect
    updateRect();
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

void Screen::scaleAllTo(const QImage& img) {
    noCheckScaleAllTo(img);
    return;
    Screen testScreen(*this);
    testScreen.noCheckScaleAllTo(img);
    if(testScreen.scaledMonitorsFitImage(img))
        noCheckScaleAllTo(img);
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

int Screen::getOuterMonitorBorder(Border b) {
    bool botOrRightBorder = b == Border::BOTTOM_BORDER || b == Border::RIGHT_BORDER;
    int val = botOrRightBorder ? 0 : INT_MAX;
    for(auto it = mMonitors.begin(); it != mMonitors.end(); it++) {
        int cVal = it.value()->getBorderPosition(b);
        val = botOrRightBorder ? std::max(cVal, val) : std::min(cVal, val);
    }
    return val;
}
