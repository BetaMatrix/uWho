#include "webget.h"
#include "ui_webget.h"
#include <QSettings>
#include <QDebug>
#include <QtConcurrent/qtconcurrentrun.h>
#include <QThread>
#include <QProcess>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

Mat myRunFunction(QString url);

webget::webget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::webget)
{
    ui->setupUi(this);

    QSettings settings("JoshConwaySoft", "uWho");

    ui->url1->setPlaceholderText( settings.value("Webget/url1").toString() );
    ui->url2->setPlaceholderText( settings.value("Webget/url2").toString() );
    ui->url3->setPlaceholderText( settings.value("Webget/url3").toString() );
    ui->url4->setPlaceholderText( settings.value("Webget/url4").toString() );
    ui->url5->setPlaceholderText( settings.value("Webget/url5").toString() );
    ui->url6->setPlaceholderText( settings.value("Webget/url6").toString() );
    ui->url7->setPlaceholderText( settings.value("Webget/url7").toString() );
    ui->url8->setPlaceholderText( settings.value("Webget/url8").toString() );
    ui->url9->setPlaceholderText( settings.value("Webget/url9").toString() );
    ui->url10->setPlaceholderText( settings.value("Webget/url10").toString() );
}

webget::~webget()
{
    delete ui;
}

//*************Thread spawner*******************

extern Mat myRunFunction(QString url){
    if(url.size() < 5){
        Mat zero = Mat(1,1, CV_64F, Scalar(0.) );
        return zero;
    }
    QProcess curl;
    curl.start("curl", QStringList() << " " << url << " -f" << " -k" << " -N");

    QByteArray result = curl.readAllStandardOutput();

    QImage cueImage ;
    cueImage = QImage::fromData(result);
    qDebug() << "Attempting to convert QImage to Mat for " << url;
    Mat frame = Mat( cueImage.height(), cueImage.width(), CV_8UC3, const_cast<uchar*>(cueImage.bits()), cueImage.bytesPerLine() ).clone();

    qDebug() << "Operation " << url << " succeeded. Sending back to detection system.";


    return frame;
}


void webget::on_pushButton_clicked()
{
QSettings settings("JoshConwaySoft", "uWho");

QString url = settings.value("Webget/url1").toString();
QFuture<Mat> future1 = QtConcurrent::run(myRunFunction, url);
Mat result1 = future1.result();


url = settings.value("Webget/url2").toString();
QFuture<Mat> future2 = QtConcurrent::run(myRunFunction, url);


url = settings.value("Webget/url3").toString();
QFuture<Mat> future3 = QtConcurrent::run(myRunFunction, url);


url = settings.value("Webget/url4").toString();
QFuture<Mat> future4 = QtConcurrent::run(myRunFunction, url);


url = settings.value("Webget/url5").toString();
QFuture<Mat> future5 = QtConcurrent::run(myRunFunction, url);


url = settings.value("Webget/url6").toString();
QFuture<Mat> future6 = QtConcurrent::run(myRunFunction, url);


url = settings.value("Webget/url7").toString();
QFuture<Mat> future7 = QtConcurrent::run(myRunFunction, url);


url = settings.value("Webget/url8").toString();
QFuture<Mat> future8 = QtConcurrent::run(myRunFunction, url);


url = settings.value("Webget/url9").toString();
QFuture<Mat> future9 = QtConcurrent::run(myRunFunction, url);


url = settings.value("Webget/url10").toString();
QFuture<Mat> future10 = QtConcurrent::run(myRunFunction, url);

future1.waitForFinished();

namedWindow("VidWindow", CV_WINDOW_AUTOSIZE );
imshow("VidWindow" ,future1.result() );
waitKey(0);

}

//**************Saving logic below***************

void webget::SaveSettings(QString value, QString key, QString varible)
{
QSettings settings("JoshConwaySoft", "uWho");
settings.beginGroup(value);
settings.setValue(key, varible);
settings.endGroup();
}


void webget::on_url1_editingFinished()
{
    SaveSettings("Webget", "url1", ui->url1->text());
}

void webget::on_url2_editingFinished()
{
    SaveSettings("Webget", "url2", ui->url2->text());
}

void webget::on_url3_editingFinished()
{
    SaveSettings("Webget", "url3", ui->url3->text());
}

void webget::on_url4_editingFinished()
{
    SaveSettings("Webget", "url4", ui->url4->text());
}

void webget::on_url5_editingFinished()
{
    SaveSettings("Webget", "url5", ui->url5->text());
}

void webget::on_url6_editingFinished()
{
    SaveSettings("Webget", "url6", ui->url6->text());
}

void webget::on_url7_editingFinished()
{
    SaveSettings("Webget", "url7", ui->url7->text());
}

void webget::on_url8_editingFinished()
{
    SaveSettings("Webget", "url8", ui->url8->text());
}

void webget::on_url9_editingFinished()
{
    SaveSettings("Webget", "url9", ui->url9->text());
}

void webget::on_url10_editingFinished()
{
    SaveSettings("Webget", "url10", ui->url10->text());
}

void webget::on_millis1_editingFinished()
{
    SaveSettings("Webget", "millis1", QString::number(ui->millis1->value()));
}

void webget::on_millis2_editingFinished()
{
    SaveSettings("Webget", "millis2", QString::number(ui->millis2->value()));
}

void webget::on_millis3_editingFinished()
{
    SaveSettings("Webget", "millis3", QString::number(ui->millis3->value()));
}

void webget::on_millis4_editingFinished()
{
    SaveSettings("Webget", "millis4", QString::number(ui->millis4->value()));
}

void webget::on_millis5_editingFinished()
{
    SaveSettings("Webget", "millis5", QString::number(ui->millis5->value()));
}

void webget::on_millis6_editingFinished()
{
    SaveSettings("Webget", "millis6", QString::number(ui->millis6->value()));
}

void webget::on_millis7_editingFinished()
{
    SaveSettings("Webget", "millis7", QString::number(ui->millis7->value()));
}

void webget::on_millis8_editingFinished()
{
    SaveSettings("Webget", "millis8", QString::number(ui->millis8->value()));
}

void webget::on_millis9_editingFinished()
{
    SaveSettings("Webget", "millis9", QString::number(ui->millis9->value()));
}

void webget::on_millis10_editingFinished()
{
    SaveSettings("Webget", "millis10", QString::number(ui->millis10->value()));
}


