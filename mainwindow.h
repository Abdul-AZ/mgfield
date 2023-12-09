#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector3D>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateCameraLocationStatus(QVector3D newPos);
    void showExportedImageStatus(QString path);
    void showAboutWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
