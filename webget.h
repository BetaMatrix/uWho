#ifndef WEBGET_H
#define WEBGET_H

#include <QDialog>
#include <opencv2/highgui/highgui.hpp>

namespace Ui {
class webget;
}

class webget : public QDialog
{
    Q_OBJECT

public:
    explicit webget(QWidget *parent = 0);
    ~webget();

private slots:
    void on_url1_editingFinished();

    void on_url2_editingFinished();

    void on_url3_editingFinished();

    void on_url4_editingFinished();

    void on_url5_editingFinished();

    void on_url6_editingFinished();

    void on_url7_editingFinished();

    void on_url8_editingFinished();

    void on_url9_editingFinished();

    void on_url10_editingFinished();

    void on_millis1_editingFinished();

    void on_millis2_editingFinished();

    void on_millis3_editingFinished();

    void on_millis4_editingFinished();

    void on_millis5_editingFinished();

    void on_millis6_editingFinished();

    void on_millis7_editingFinished();

    void on_millis8_editingFinished();

    void on_millis9_editingFinished();

    void on_millis10_editingFinished();

    void on_pushButton_clicked();

private:
    Ui::webget *ui;
    cv::Mat webgetImage(QString) ;
    void SaveSettings(QString, QString, QString);
};

#endif // WEBGET_H
