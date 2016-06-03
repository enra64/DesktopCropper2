#include "croppingwidget.h"

CroppingWidget::CroppingWidget(QWidget *parent) : QWidget(parent) {
    mStatusBarView = new QLabel("Scales: Image: 1, Monitors: 1");
    mStatusBarView->setAlignment(Qt::AlignRight);
}

CroppingWidget::~CroppingWidget()
{
    delete mStatusBarView;
    for(MonitorView* m : mMonitors)
        delete m;
}

void CroppingWidget::addMonitor(const QString &name, const Vec2i& size, const Vec2i& pos) {
    // note to self: if you construct the element without "new" here, the monitorview dies before it is even used
    mMonitors.insert(name, new MonitorView(size, pos));
    update();
}

const MonitorView& CroppingWidget::getMonitor(const QString &name)
{
    return *mMonitors.find(name).value();
}

QString CroppingWidget::getMonitorName(Vec2i) { return QString(); }

void CroppingWidget::saveCrops(const QFile &path) {
    for(auto i = mMonitors.begin(); i != mMonitors.end(); i++)
        i.value()->saveCrop(path.fileName() + "_" + i.key(), mOriginalImage, mImageScale, mMonitorScale);
}

void CroppingWidget::selectMonitor(const QString &name, bool select = true) {
    mMonitors.find(name).value()->setSelected(select);
    update();
}

void CroppingWidget::paintEvent(QPaintEvent * /* event */) {
    std::cout << "ok" << std::endl;
    QPainter painter(this);
    painter.drawImage(0, 0, mImage);

    for(MonitorView* m : mMonitors)
        m->draw(painter, mMonitorScale);
}

void CroppingWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(mOriginalImage.isNull())
        return;

    scale(event->size());
}

void CroppingWidget::wheelEvent(QWheelEvent *e)
{
    mMonitorScale *= e->angleDelta().y() > 0 ? 0.98 : 1.02;
    update();
}

void CroppingWidget::mousePressEvent(QMouseEvent *e)
{
    mousePressed = true;
    mMousePressBeginPosition = e->pos();
}

void CroppingWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint positionDelta = e->pos() - mMousePressBeginPosition;
    mMousePressBeginPosition = e->pos();
    moveMonitors(positionDelta.x(), positionDelta.y());
}

void CroppingWidget::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void CroppingWidget::scale(const QSize &size)
{
    mImage = mOriginalImage.scaled(size, Qt::KeepAspectRatio);
    mImageScale = (double) mImage.width() / (double)mOriginalImage.width();
    mMonitorScale = (double) size.width() / (double) getMonitorOverallWidth();
    mStatusBarView->setText(QString("Scales: Image: %1, Monitors: %2").arg(mImageScale).arg(mMonitorScale));
}

void CroppingWidget::moveMonitors(int dX, int dY)
{
    for(MonitorView* m : mMonitors)
        m->move(dX, dY);
    update();
}

int CroppingWidget::getMonitorOverallWidth()
{
    int w = 0;
    for(MonitorView* m : mMonitors)
        w += m->getModel().getSize().x();
    return w;
}

bool CroppingWidget::loadImage(const QFile &path) {
    bool success = mImage.load(path.fileName());
    mOriginalImage.load(path.fileName());
    scale(size());
    update();
    return success;
}

void CroppingWidget::setStatusbar(QStatusBar *s)
{
    s->addWidget(mStatusBarView);
}
