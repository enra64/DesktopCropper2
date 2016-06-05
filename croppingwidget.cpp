#include "croppingwidget.h"

CroppingWidget::CroppingWidget(QWidget *parent) : QWidget(parent) {
    mStatusBarView = new QLabel("Scales: Image: 1, Monitors: 1");
    mStatusBarView->setAlignment(Qt::AlignRight);
}

CroppingWidget::~CroppingWidget() {
    delete mStatusBarView;
}

void CroppingWidget::addMonitor(const QString &name, const QSize& size, const QPoint& pos) {
    // note to self: if you construct the element without "new" here, the monitorview dies before it is even used
    mScreen.addMonitor(name, size, pos);
    update();
}

const Monitor& CroppingWidget::getMonitor(const QString &name) {
    return mScreen.getMonitor(name);
}

QString CroppingWidget::getMonitorName(QPoint clickPosition) {
    return mScreen.getMonitorName(clickPosition);
}

void CroppingWidget::selectAllMonitors(bool select)
{
    mScreen.selectAll(select);
    update();
}

void CroppingWidget::saveCrops(const QFile &path) {
    mScreen.saveCrops(path, mOriginalImage, imageScale());
}

void CroppingWidget::paintEvent(QPaintEvent * /* event */) {
    QPainter painter(this);
    painter.drawImage(0, 0, mCurrentImage);

    if(mImageLoaded)
        mScreen.draw(painter);
}

void CroppingWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if(mOriginalImage.isNull())
        return;

    scale();
}

void CroppingWidget::wheelEvent(QWheelEvent *e) {
    Scale s = Scale::BOTH;
    if(e->modifiers() & Qt::ControlModifier)
        s = Scale::Y;
    else if (e->modifiers() & Qt::ShiftModifier)
        s = Scale::X;
    mScreen.scaleBy(e->angleDelta().y() > 0 ? 0.98 : 1.02, mCurrentImage, s);
    updateStatusBar();
    update();
}

void CroppingWidget::mousePressEvent(QMouseEvent *e) {
    mMousePressed = true;
    mMousePressBeginPosition = e->pos();
    mMouseMoved = false;
}

void CroppingWidget::mouseMoveEvent(QMouseEvent *e) {
    QPoint positionDelta = e->pos() - mMousePressBeginPosition;
    mMousePressBeginPosition = e->pos();
    mMouseMoved = true;
    moveMonitors(positionDelta.x(), positionDelta.y());
}

void CroppingWidget::mouseReleaseEvent(QMouseEvent *e) {
    mMousePressed = false;

    // clicked?
    if(mMouseMoved || !mImageLoaded)
        return;

    // get clicked monitor
    QString clickedMonitorName = getMonitorName(e->pos());

    // valid result?
    if(clickedMonitorName.isEmpty())
        return;

    // toggle single monitor
    if(e->modifiers() & Qt::ControlModifier || e->modifiers() & Qt::ShiftModifier)
        mScreen.select(clickedMonitorName, !mScreen.isSelected(clickedMonitorName));
    // select single monitor
    else{
        mScreen.selectAll(false);
        mScreen.select(clickedMonitorName);
    }
    update();
}

bool CroppingWidget::fullQualityCropPossible() const {
    double maxScreenScaleFactor = mScreen.getMinScaleFactor();
    // because of this: scale = (imageScale / mXScale) in Monitor::crop()
    if(maxScreenScaleFactor < imageScale())
        return false;
    // because the user cannot make the monitors bigger than the start, the monitors must either fit or be scaled down, so
    // above condition hits
    return true;
}

void CroppingWidget::resetMonitors()
{
    update();
    updateStatusBar();
}

void CroppingWidget::scale() {
    double prevScale = imageScale();
    mCurrentImage = mOriginalImage.scaled(size(), Qt::KeepAspectRatio);

    mScreen.scaleBy(imageScale() / prevScale, mCurrentImage);
    updateStatusBar();
}

void CroppingWidget::updateStatusBar() const {
    QString text = QString("Scales: Image: %1, Monitors: %2; Sizes: Image: %3, Screen: %4 -> %5")
    .arg(imageScale())
    .arg(mScreen.getMinScaleFactor())
    .arg(QString("%1x%2").arg(mOriginalImage.width()).arg(mOriginalImage.height()))
    .arg(mScreen.getSizeAsString())
    .arg(fullQualityCropPossible() ? "ok" : "too small");
    mStatusBarView->setText(text);
}

void CroppingWidget::moveMonitors(int dX, int dY) {
    mScreen.moveMonitors(dX, dY, mCurrentImage);
    update();
}

double CroppingWidget::imageScale() const {
    return (double) mCurrentImage.width() / (double)mOriginalImage.width();
}

bool CroppingWidget::loadImage(const QFile &path) {
    bool success = mCurrentImage.load(path.fileName());
    if(success)
        mImageLoaded = true;
    mOriginalImage.load(path.fileName());
    scale();
    update();
    return success;
}

void CroppingWidget::setStatusbar(QStatusBar *s) {
    s->addWidget(mStatusBarView);
}
