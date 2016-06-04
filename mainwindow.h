#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_2_triggered();

    void on_actionSave_As_2_triggered();

    void on_actionSelect_None_triggered();

    void on_actionSelect_All_triggered();

private:
    QString showFileOpenDialog();
    void showFileSaveDialog();
    Ui::MainWindow *ui;
    QString mFilePath;
    bool mSelectAllToggle = true;
};

#endif // MAINWINDOW_H
