#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cropper->setStatusbar(ui->statusBar);
    ui->cropper->addMonitor("l", QSize(1024, 1280), QPoint(0, 320));
    ui->cropper->addMonitor("m", QSize(1920, 1080), QPoint(1024, 420));
    ui->cropper->addMonitor("r", QSize(1080, 1920), QPoint(2944, 0));
    setWindowTitle(QString("Desktop Cropper 2"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QFile image(showFileOpenDialog());
    if(!image.exists()){
        ui->statusBar->showMessage("file not found", 1500);
        return;
    }
    if(ui->cropper->loadImage(image))
        ui->statusBar->showMessage("load ok", 1500);
    else
        ui->statusBar->showMessage("load failed", 1500);
    mFilePath = "";
    setWindowTitle(QString("Desktop Cropper 2: %1").arg(QFileInfo(image.fileName()).fileName()));
}

void MainWindow::on_actionSave_2_triggered()
{
    if(mFilePath.isNull() || mFilePath.isEmpty())
        showFileSaveDialog();
    if(!mFilePath.isNull() && !mFilePath.isEmpty()){
        ui->statusBar->showMessage("saving", 1000);
        ui->cropper->saveCrops(mFilePath);
    }
    else
        ui->statusBar->showMessage("could not save", 1000);
}

void MainWindow::on_actionSave_As_2_triggered()
{
    showFileSaveDialog();
    if(!mFilePath.isNull() && !mFilePath.isEmpty())
        ui->cropper->saveCrops(mFilePath);
}

QString MainWindow::showFileOpenDialog()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory("/home/arne/Downloads");
    dialog.setFilter(QDir::Files);
    dialog.setWindowTitle("Open File");
    dialog.setNameFilter("Images(*.jpg *.png)");
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    QDialog::DialogCode result = (QDialog::DialogCode)dialog.exec();

    if(result == QDialog::DialogCode::Accepted)
        return dialog.selectedFiles().first();
    else
        return QString();
}

void MainWindow::showFileSaveDialog()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setDirectory("/home/arne/Documents/Pictures/backgrounds");
    dialog.setFilter(QDir::Files);
    dialog.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
    dialog.setWindowTitle("Save File");
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    dialog.exec();
    mFilePath = dialog.selectedFiles().first();
}

void MainWindow::on_actionSelect_None_triggered()
{
    ui->cropper->selectAllMonitors(false);
}

void MainWindow::on_actionSelect_All_triggered()
{
    ui->cropper->selectAllMonitors(true);
}
