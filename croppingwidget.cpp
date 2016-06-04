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

const MonitorView& CroppingWidget::getMonitor(const QString &name) {
    return mScreen.getMonitor(name);
}

QString CroppingWidget::getMonitorName(QPoint clickPosition) {
    return mScreen.getMonitorName(clickPosition);
}

void CroppingWidget::selectAllMonitors(bool select)
{
    mScreen.selectAll(select);
}

void CroppingWidget::saveCrops(const QFile &path) {
    mScreen.saveCrops(path, mOriginalImage, mImageScale, mScreen.getMonitorScale());
}

void CroppingWidget::paintEvent(QPaintEvent * /* event */) {
    QPainter painter(this);
    painter.drawImage(0, 0, mImage);

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
    mScreen.scaleBy(e->angleDelta().y() > 0 ? 0.98 : 1.02);
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
    // clicked
    if(!mMouseMoved && mImageLoaded){
        QString clickedMonitorName = getMonitorName(e->pos());
        if(e->modifiers() & Qt::ControlModifier){
            // get selected monitor
            // toggle selection state
            mScreen.select(clickedMonitorName, !mScreen.isSelected(clickedMonitorName));
        }
        // control not pressed
        else{
            // deselect all
            mScreen.selectAll(false);
            mScreen.select(clickedMonitorName);
        }
        update();
    }
}

void CroppingWidget::scale() {
    mImage = mOriginalImage.scaled(size(), Qt::KeepAspectRatio);
    mImageScale = (double) mImage.width() / (double)mOriginalImage.width();
    mScreen.scaleBy((double) mImage.width() / (double) mScreen.getRect().width());
    updateStatusBar();
}

void CroppingWidget::updateStatusBar(){
    const double oldMonitorScale = mScreen.getMonitorScale();
    mScreen.setScale(1);
    QString text = QString("Scales: Image: %1, Monitors: %2; Sizes: Image: %3, Screen: %4 -> %5")
    .arg(mImageScale)
    .arg(oldMonitorScale)
    .arg(QString("%1x%2").arg(mOriginalImage.width()).arg(mOriginalImage.height()))
    .arg(QString("%1x%2").arg(mScreen.getRect().width()).arg(mScreen.getRect().height()))
    .arg(mOriginalImage.rect().contains(mScreen.getRect()) ? "ok" : "too small");
    mStatusBarView->setText(text);
    mScreen.setScale(oldMonitorScale);
}

void CroppingWidget::moveMonitors(int dX, int dY) {
    mScreen.moveMonitors(dX, dY, mImage);
    update();
}

bool CroppingWidget::loadImage(const QFile &path) {
    bool success = mImage.load(path.fileName());
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
