#include "config.h"
#include "ui_config.h"
#include <iostream>
#include <vector>
#include <QtGui>
#include <QMessageBox>
#include <QSettings>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

const Size CommonResolutions[] = {
    Size(220,176),
    Size(160,256),
    Size(208,208),
    Size(256,192),
    Size(280,192),
    Size(432,128),
    Size(240,240),
    Size(320,192),
    Size(320,200),
    Size(256,256),
    Size(320,208),
    Size(320,224),
    Size(320,240),
    Size(320,256),
    Size(376,240),
    Size(272,340),
    Size(400,240),
    Size(320,320),
    Size(432,240),
    Size(560,192),
    Size(400,270),
    Size(480,234),
    Size(400,300),
    Size(480,250),
    Size(312,390),
    Size(640,200),
    Size(480,272),
    Size(512,256),
    Size(416,352),
    Size(480,320),
    Size(640,240),
    Size(640,256),
    Size(512,342),
    Size(800,240),
    Size(512,384),
    Size(640,320),
    Size(640,350),
    Size(640,360),
    Size(480,500),
    Size(720,348),
    Size(720,350),
    Size(640,400),
    Size(720,364),
    Size(800,352),
    Size(600,480),
    Size(640,480),
    Size(640,512),
    Size(768,480),
    Size(800,480),
    Size(848,480),
    Size(854,480),
    Size(800,600),
    Size(960,540),
    Size(832,624),
    Size(960,544),
    Size(1024,576),
    Size(960,640),
    Size(1024,600),
    Size(1024,640),
    Size(960,720),
    Size(1136,640),
    Size(1024,768),
    Size(1024,800),
    Size(1152,720),
    Size(1152,768),
    Size(1280,720),
    Size(1120,832),
    Size(1280,768),
    Size(1152,864),
    Size(1334,750),
    Size(1280,800),
    Size(1152,900),
    Size(1024,1024),
    Size(1366,768),
    Size(1280,854),
    Size(1280,960),
    Size(1600,768),
    Size(1440,900),
    Size(1280,1024),
    Size(1440,960),
    Size(1600,900),
    Size(1400,1050),
    Size(1440,1024),
    Size(1440,1080),
    Size(1600,1024),
    Size(1680,1050),
    Size(1600,1200),
    Size(1920,1080),
    Size(1920,1200),
    Size(2048,1152),
    Size(1792,1344),
    Size(1856,1392),
    Size(1800,1440),
    Size(2880,900),
    Size(2048,1280),
    Size(1920,1400),
    Size(2538,1080),
    Size(1920,1440),
    Size(2160,1440),
    Size(2048,1536),
    Size(2304,1440)
};

//******************Save settings

void config::SaveSettings(QString value, QString key, QString varible)
{
QSettings settings("JoshConwaySoft", "uWho");
settings.beginGroup(value);
settings.setValue(key, varible);
settings.endGroup();
}

//******************Resolution Query per webcam

QVector<int> config::getSupportedVideoResolutions(int camera)
{
    VideoCapture cameraDevice = VideoCapture(camera);
    QVector<int> supportedVideoResolutions;  // This refers to the index of CommonResolutions
    for (int i = 0; i < 100 ; i++){  //There are 101 elements. -1 for index, that leaves 100
        waitKey(100);
        Size test = CommonResolutions[i];
        cameraDevice.set(CV_CAP_PROP_FRAME_WIDTH, test.width);
        cameraDevice.set(CV_CAP_PROP_FRAME_HEIGHT, test.height);
        double width = cameraDevice.get(CV_CAP_PROP_FRAME_WIDTH);
        double height = cameraDevice.get(CV_CAP_PROP_FRAME_HEIGHT);
        if (test.width == width && test.height == height) {
            qDebug() << "PASS" << QString::number(test.width) << QString::number(test.height) ;
            supportedVideoResolutions.append( i );
        }
    }
    return supportedVideoResolutions;
}

//******************Webcam Query

QVector<int> getSupportedWebcams(void)
{
    QVector<int> webcamNumbers ;
    for (int i = 0 ; i < 10 ; i++){
        VideoCapture camera(i) ;
        waitKey(100); // We wait to let the system catch up. Elsewise, V4L errors. :/
        if(camera.isOpened()){
           webcamNumbers.append(i);
           qDebug() << "webcam#" << i ;
        }else{
            return webcamNumbers;
        }
    }
    return webcamNumbers;
}



//******************main

config::config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::config)
{
    ui->setupUi(this);

    QVector<int> webcamIndexList = getSupportedWebcams() ;
    if (webcamIndexList.size() == 0){
        QMessageBox errorBox;
        errorBox.setText("There are no valid cameras on this system. Please plug one in and try again.");
        errorBox.exec();
    }else {
        config::SaveSettings( "Video", "Device", "0" );
        for(int i = 0; i < webcamIndexList.size(); i++ ){
            ui->cameraCombobox->addItem( QString::number(webcamIndexList.at(i)) ); //Populates camera combobox with camera #s
            QVector<int> resolutions = config::getSupportedVideoResolutions(i);
            for(int j = 0; j < resolutions.size(); j++ ){
                QString resolutionString = QString::number(CommonResolutions[resolutions.at(j)].width) + "X" + QString::number(CommonResolutions[resolutions.at(j)].height);
                qDebug() << resolutionString ;
                ui->resolutionCombobox->addItem(resolutionString);
            }
        }
    }
}

//******************Saving logic

void config::on_cameraCombobox_currentIndexChanged(int index)
{
    config::SaveSettings( "Video", "Device", QString::number(index) );
}

void config::on_resolutionCombobox_currentIndexChanged(const QString &arg1)
{
    QString resolution = arg1 ;
    QString resX = resolution.section('X', 0, 0); // Because handling the resolutions directly is so much easier.
    QString resY = resolution.section('X', 1, 1); // Wish I could pass a Size() . No handling needed then.

    config::SaveSettings( "Video", "Resolution_X", resX );
    config::SaveSettings( "Video", "Resolution_Y", resY );
}


//******************config destructor
config::~config()
{
    delete ui;
}
