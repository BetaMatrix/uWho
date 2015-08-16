#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool checkClassifier();
    vector<Rect> faceFinder(Mat grayImage);

private slots:
    void on_videofileButton_clicked();

    void on_webcamButton_clicked();

    void on_dirpicButton_clicked();

    void on_ipButton_clicked();

    void on_configureButton_clicked();

private:
    Ui::MainWindow *ui;
    void SaveSettings(QString, QString);
};

#endif // MAINWINDOW_H
