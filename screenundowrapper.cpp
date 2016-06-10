#include "screenundowrapper.h"

ScreenUndoWrapper::ScreenUndoWrapper() {
    mScreenList.push_front(std::make_shared<Screen>());
    mCurrentPosition = mScreenList.begin();
}

void ScreenUndoWrapper::undo(const QImage& img) {
    auto end = --mScreenList.end();
    // go back to last valid state
    while(mCurrentPosition != end && !(*++mCurrentPosition)->scaledMonitorsFitImage(img));
}

void ScreenUndoWrapper::redo(const QImage& img) {
    auto begin = mScreenList.begin();
    // go forward to next valid state
    while(mCurrentPosition != begin && !(*--mCurrentPosition)->scaledMonitorsFitImage(img));
}

/**
 * @brief saveState Push a copy of the current object state, which we can then modify.
 *                  If there are items in between the list start and our current position, erase those, they are undone.
 */
void ScreenUndoWrapper::saveState() {
    // remove everything newer than current position
    if(mCurrentPosition != mScreenList.begin()){
        //mCurrentPosition;
        mScreenList.erase(mScreenList.begin(), mCurrentPosition);
    }
    // add a new state
    mScreenList.push_front(std::make_shared<Screen>(*mScreenList.front()));
    // point current position to newest state
    mCurrentPosition = mScreenList.begin();
}

std::shared_ptr<Screen> ScreenUndoWrapper::getCurrentScreen() const {
    return *mCurrentPosition;
}


/*
 *
 *
 *
 *
 *
 *
 *
 *
 * SCREEN WRAPPER FUNCTIONS
 * These functions act only as wrappers for the actual screen.
 *
 *
 *
 *
 *
 *
 *
 *
 */
void ScreenUndoWrapper::select(const QString &which, bool select) {
    saveState();
    getCurrentScreen()->select(which, select);
}

void ScreenUndoWrapper::selectAll(bool select) {
    saveState();
    getCurrentScreen()->selectAll(select);
}

bool ScreenUndoWrapper::removeMonitor(const QString &name) {
    saveState();
    return getCurrentScreen()->removeMonitor(name);
}

void ScreenUndoWrapper::addMonitor(const QString &name, const QSize &size, const QPoint &pos) {
    saveState();
    getCurrentScreen()->addMonitor(name, size, pos);
}

/*
 *
 *
 *
 *
 *
 *
 *
 *
 * SCREEN WRAPPER FUNCTIONS
 * These functions require extra attention, because we dont want to save state for every action they receive.
 *
 *
 *
 *
 *
 *
 *
 *
 */
void ScreenUndoWrapper::moveMonitors(int dX, int dY, const QImage &img) {
    // only save state on first scaleByCall
    if(!mSavedBeginningOfMove)
        saveState();
    // dont save state anymore
    mSavedBeginningOfMove = true;
    // reset setScale and moveMonitors, so that they will save state again
    mSavedBeginningOfScaleBy = false;
    mSavedBeginningOfSetScale = false;

    // execute original action
    getCurrentScreen()->moveMonitors(dX, dY, img);
}

void ScreenUndoWrapper::setScale(const double factor, const QImage &img) {
    // only save state on first scaleByCall
    if(!mSavedBeginningOfSetScale)
        saveState();
    // dont save state anymore
    mSavedBeginningOfSetScale = true;
    // reset setScale and moveMonitors, so that they will save state again
    mSavedBeginningOfScaleBy = false;
    mSavedBeginningOfMove = false;

    // execute original action
    getCurrentScreen()->setScale(factor, img);
}

void ScreenUndoWrapper::scaleBy(const double factor, const QImage &img, Scale which) {
    // only save state on first scaleByCall
    if(!mSavedBeginningOfScaleBy)
        saveState();
    // dont save state anymore
    mSavedBeginningOfScaleBy = true;
    // reset setScale and moveMonitors, so that they will save state again
    mSavedBeginningOfMove = false;
    mSavedBeginningOfSetScale = false;

    // execute original action
    getCurrentScreen()->scaleBy(factor, img, which);
}

/*
 *
 *
 *
 *
 *
 *
 *
 *
 * SCREEN WRAPPER FUNCTIONS
 * These functions are also wrappers, but because they are const, we dont even need to save a state for it.
 *
 *
 *
 *
 *
 *
 *
 *
 */

const QRect &ScreenUndoWrapper::getRect() const {
    return getCurrentScreen()->getRect();
}

void ScreenUndoWrapper::saveCrops(const QFile &path, const QImage &img, double imageScale) const {
    getCurrentScreen()->saveCrops(path, img, imageScale);
}

QString ScreenUndoWrapper::getMonitorName(QPoint clickPosition) const {
    return getCurrentScreen()->getMonitorName(clickPosition);
}

bool ScreenUndoWrapper::isSelected(const QString &which) const {
    return getCurrentScreen()->isSelected(which);
}

void ScreenUndoWrapper::draw(QPainter &painter) const {
    getCurrentScreen()->draw(painter);
}

double ScreenUndoWrapper::getMinScaleFactor() const {
    return getCurrentScreen()->getMinScaleFactor();
}

QRect ScreenUndoWrapper::getUnscaledRect() const {
    return getCurrentScreen()->getUnscaledRect();
}

QString ScreenUndoWrapper::getSizeAsString() const {
    return getCurrentScreen()->getSizeAsString();
}
