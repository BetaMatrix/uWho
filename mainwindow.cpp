#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "webget.h"
#include "config.h"

#include <QtCore>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QDataStream>
#include <ctime>
#include <sstream>
#include <string>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/face.hpp>

using namespace cv;
using namespace face;
using namespace std;

/** Global variables **/

string face_file = (QDir::homePath() + QString("/.uWho/face.xml")).toUtf8().constData();
QFile faceconf(QDir::homePath() + QString("/.uWho/face.xml") );
QDir uWhoconfig( QDir::homePath() + QString("/.uWho/") );
double lbphThreshold = 60.0 ;
Ptr<LBPHFaceRecognizer> model = face::createLBPHFaceRecognizer(1,8,8,8, lbphThreshold);
QSettings settings("JoshConwaySoft", "uWho");
QString appPath = qApp->applicationDirPath() ;


CascadeClassifier faceCascade;
CascadeClassifier eyesCascade;
string faceCascadeName = (appPath + "lbpcascade_frontalface.xml").toUtf8().constData();
string eyesCascadeName = (appPath + "haarcascade_eye_tree_eyeglasses.xml").toUtf8().constData();


/** end of global variables **/



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap webcam(":/icons/webcam.png");
    QPixmap videofile(":/icons/videofile.png");
    QPixmap dirpicfile(":/icons/dirpics.png");
    QPixmap ipfile(":/icons/internet-cloud-icon.jpg");
    QPixmap configurefile(":/icons/configure-gears.png");
    ui->webcamButton->setIcon(webcam);
    ui->videofileButton->setIcon(videofile);
    ui->dirpicButton->setIcon(dirpicfile);
    ui->ipButton->setIcon(ipfile);
    ui->configureButton->setIcon(configurefile);
}


MainWindow::~MainWindow()
{
    delete ui;
}

//****************************************************************


// Initalizes classifier that was created in upper part of code. Does internal check to verify consistence
// This function takes nothing in, and outputs a true/false for success.

bool MainWindow::checkClassifier(void)
{
    if (faceconf.exists()){
        model->load(face_file);
        qDebug() << "Loaded model." ;
    }else{
        qDebug() << "Generating starting model..." ;
        vector<Mat> images (10);
        vector<int> labels (10);

        QString josh1 = appPath + "josh1.png" ;
        images[0] = (imread( josh1.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        labels[0] = 0;

        QString josh2 = appPath + "josh2.png" ;
        images[1] = (imread( josh2.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        labels[1] = 0;

        QString josh3 = appPath + "josh3.png" ;
        images[2] = (imread( josh3.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        labels[2] = 0;

        QString josh4 = appPath + "josh4.png" ;
        images[3] = (imread( josh4.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        labels[3] = 0;

        QString josh5 = appPath + "josh5.png" ;
        images[4] = (imread( josh5.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        labels[4] = 0;

        QString josh6 = appPath + "josh6.png" ;
        images[5] = (imread( josh6.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        labels[5] = 0;

        QString josh7 = appPath + "josh7.png" ;
        images[6] = (imread( josh7.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        labels[6] = 0;

        QString josh8 = appPath + "josh8.png" ;
        images[7] = (imread( josh8.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        labels[7] = 0;

        QString josh9 = appPath + "josh9.png" ;
        images[8] = (imread( josh9.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        labels[8] = 0;

        QString josh10 = appPath + "josh10.png" ;
        images[9] = (imread( josh10.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        labels[9] = 0;

        qDebug() << "Done loading images in vectors";
        model->train(images, labels);
        qDebug() << "Training successful";

        SaveSettings(QString::number(0), QString::number(10));
        QString josh11 = appPath + "josh11.png" ;
        Mat testingImage = (imread( josh11.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE));
        int predicted = -1;  // Sanity check. We throw a face I know is mine to the predictor.
        double confidence ;
        model->predict(testingImage, predicted, confidence);
        qDebug() << "Testing predicted/confidence: " << predicted << confidence ;
        if (predicted==0){return (1);}else{return (0);}
    }
}


//****************************************************************

vector<Rect> MainWindow::faceFinder(Mat grayImage){
    // This function takes in the image from whatever source you provide and processes it for all valid faces.
    // It does so by doing an error prone face finder, and then sending those faces to an eye finder.
    // If the eye finder confirms the face, then this function will add the face as a Mat in a vector
    // Function returns all faces in picture as a vector of cv::Rects

    vector<Rect> partialFaces;
    vector<Rect> faces (0);


    faceCascade.detectMultiScale(grayImage, partialFaces, 1.1, 3, CV_HAAR_SCALE_IMAGE, Size(28,28));
    for(int i = 0; i < partialFaces.size(); i++){
        vector<Rect> eyes;
        eyesCascade.detectMultiScale( grayImage(partialFaces[i]), eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        if (eyes.size() != 0){
            // add partialFaces[i] to faces[i]
            faces.resize(i+1);
            faces.push_back(partialFaces[i]);
        }
    }


    return faces;
}



//****************************************************************


void MainWindow::on_videofileButton_clicked()
{
    // Below is the open file dialog, and saves said file selected to videoFileNames
    QFileDialog videoFile(this);
    videoFile.setFileMode(QFileDialog::AnyFile);
    videoFile.setNameFilter(tr("Images (*.avi *.mpg *.mpeg *.mkv *.webm *.ogv *.mp4)"));
    videoFile.setViewMode(QFileDialog::List);
    QStringList vFN;
    if (videoFile.exec()){
        vFN = videoFile.selectedFiles();
    }

    // Check for valid file name. If you cancel, it returns instead of crashes
    QString videoFileName;
    if(vFN.size()!=NULL){
        videoFileName = vFN.at(0);
    }else{
        return;
    }

    // Attempt to get total frames in video. This may not be accurate, according to OpenCV devs
    qDebug() << ( videoFileName.toUtf8().constData() ) ;
    VideoCapture cap = VideoCapture(( videoFileName.toUtf8().constData() ));
    qDebug() << "Video loaded" ;
    long int videoFrameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);


    // Starts the Classifier, and then tests said classifier with my default data
    srand(time(NULL));

    // Sets up classifier and proceeds to run a test against it for sanity reasons. BIG chunk of code
    MainWindow::checkClassifier();

    namedWindow("VidWindow");
    Mat frame;
    unsigned long frameNumber = 0;
    do{
        cap.read(frame);
        if(!frame.empty()){
            frameNumber = frameNumber + 1 ;
            ostringstream a;
            a << frameNumber ;
            putText(frame, a.str(), Point(0,20),FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1,8, false);
            blur(frame, frame, Size(3,3));
            faceCascade.load(faceCascadeName);
            eyesCascade.load(eyesCascadeName);
            vector<Rect> faces;
            Mat frame_gray;
            cvtColor(frame, frame_gray, CV_BGR2GRAY);
            faceCascade.detectMultiScale(frame_gray, faces, 1.1, 3, CV_HAAR_SCALE_IMAGE, Size(28,28));
            for(int i = 0; i < faces.size(); i++){
                vector<Mat> facePicture (1);
                vector<int> faceIndex (1);
                vector<Rect> eyes;
                facePicture[0] = frame(faces[i]);   // Gets the face only as the variable facePicture

                rectangle(frame, faces[i], Scalar(255,0,255), 1, 8, 0); // Draws rectangles on webcam video
                string faceString = static_cast<ostringstream*>( &(ostringstream() << i) )->str();
                putText(frame, faceString, Point(faces[i].x, (faces[i].y+10)),FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1,8, false);

                eyesCascade.detectMultiScale( facePicture[0], eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
                long int timeEncode = time(NULL);
                if (eyes.size() != 0){
                    cvtColor(facePicture[0], facePicture[0], CV_BGR2GRAY); // colorspace change to gray
                    int predicted = -1;
                    double confidence ;
                    model->predict(facePicture[0], predicted, confidence); // Check the machine learner and ask if it's seen this face before
                    string predictString = static_cast<ostringstream*>( &(ostringstream() << predicted) )->str();
                    putText(frame, predictString, Point((faces[i].x + faces[i].width - 40), (faces[i].y + 10)),FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1,8, false);
                    if (predicted == -1){
                        faceIndex[0] = rand()%30000;
                        model->update(facePicture,faceIndex); // If its not in the FaceRecognizer, add it
                    }else{
                        faceIndex[0] = predicted;
                        model->update(facePicture,faceIndex);  // if the face is already in, add this as another data point
                    }
                    qDebug() << "face ID#" << predicted << "confidence :"<< confidence;
                }
            }
        }
        ostringstream b;
        b << videoFrameCount ;
        putText(frame, b.str(), Point((cap.get(CV_CAP_PROP_FRAME_WIDTH)-61),20),FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1,8, false);
        imshow("VidWindow" ,frame);
    }while((waitKey(1)<30) || frame.empty() );
    qDebug() << (QDir::homePath() + QString("/.uWho/face.xml")) ;
    model->save(face_file);
    destroyWindow("VidWindow");
}


//****************************************************************


void MainWindow::on_webcamButton_clicked()
{

    // Sets up classifier and proceeds to run a test against it for sanity reasons. BIG chunk of code
    MainWindow::checkClassifier();

    // Sets up a file that keeps track of users and when they are in sight of the camera.
    QSettings users("JoshConwaySoft", "uWhoUsers");
    QFile facetime(QDir::homePath() + QString("/.uWho/facetime.xml"));
    facetime.open( QIODevice::WriteOnly );
    QTextStream facetimeoutput(&facetime);



    namedWindow("VidWindow");
    VideoCapture cap = VideoCapture(settings.value("Video/Device").toString().toInt() );
    cap.set(CV_CAP_PROP_FRAME_WIDTH, settings.value("Video/Resolution_X").toString().toInt() );
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, settings.value("Video/Resolution_Y").toString().toInt() );
    Mat frame;
    faceCascade.load(faceCascadeName);
    eyesCascade.load(eyesCascadeName);

    // faceID enumerates from the ~/.config/JoshConwaySoft/uWhoUsers.conf and sets faceID to the next unused value
    // This is so we can sequentially add new detected users to the log, rather than a messy rand()%30000 junk I previously had
    long long faceID ;
    for (faceID = 1; users.contains(QString::number(faceID)); faceID++){
        qDebug() << QString::number(faceID);
    };

    qDebug() << "faceID is" << faceID ;

    do{
        cap >> frame;
        if(!frame.empty()){
            //unsigned long int seconds= time(NULL);
            QDateTime currentTime = QDateTime::currentDateTime();
            QString dateTime = currentTime.toString("MMddyyyy hh:mm:ss:zzz");
            //blur(frame, frame, Size(3,3));
            vector<Rect> faces;
            Mat frame_gray;
            cvtColor(frame, frame_gray, CV_BGR2GRAY);
            faceCascade.detectMultiScale(frame_gray, faces, 1.1, 3, CV_HAAR_SCALE_IMAGE, Size(30,30));
            vector<Mat> facePicture (1);
            vector<int> faceIndex (1);
            vector<Rect> eyes;

            for(int i = 0; i < faces.size(); i++){
                facePicture[0] = frame(faces[i]);   // Gets the face only as the variable facePicture
                rectangle(frame, faces[i], Scalar(255,0,255), 1, 8, 0); // Draws rectangles on webcam video
                string faceString = static_cast<ostringstream*>( &(ostringstream() << i) )->str();
                putText(frame, faceString, Point(faces[i].x, (faces[i].y+30)),FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1,8, false);
                eyesCascade.detectMultiScale( facePicture[0], eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
                if (eyes.size() != 0){
                    cvtColor(facePicture[0], facePicture[0], CV_BGR2GRAY); // colorspace change to gray
                    int predicted = -1;
                    double confidence ;
                    model->predict(facePicture[0], predicted, confidence); // Check the machine learner and ask if it's seen this face before
                    string predictString = static_cast<ostringstream*>( &(ostringstream() << predicted) )->str();
                    putText(frame, predictString, Point((faces[i].x + faces[i].width - 40), (faces[i].y + 30)),FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1,8, false);
                    if (predicted == -1){
                        faceIndex[0] = faceID;
                        SaveSettings(QString::number(faceID), QString::number(1));
                        model->update(facePicture,faceIndex); // If its not in the FaceRecognizer, add it
                        facetimeoutput << faceIndex[0] << "," << dateTime << "\n" ;
                        qDebug() << "New face detected #" << faceIndex[0] ;
                        faceID = faceID + 1;
                    }else{
                        faceIndex[0] = predicted;
                        long long faceCount = users.value(QString::number(predicted)).toLongLong();
                        SaveSettings(QString::number(predicted), QString::number((faceCount+1)) );
                        if (faceCount < 50){
                            model->update(facePicture,faceIndex);  // if the face is in DB and under 50 samples, add it
                            qDebug() << "Face#" << predicted << " sample added";
                        }
                        facetimeoutput << faceIndex[0] << "," << dateTime << "\n" ;
                    }
                    qDebug() << "face # " << predicted << confidence << dateTime   ;
                }
            }
            imshow("VidWindow" ,frame);}
    }while(waitKey(30)<30);
    facetime.close();
    model->save(face_file);
    qDebug() << "Model saved" ;
    destroyWindow("VidWindow");
}


//***********************************************************************


void MainWindow::on_dirpicButton_clicked()
{
    QSettings users("JoshConwaySoft", "uWhoUsers");

    // Sets up classifier and proceeds to run a test against it for sanity reasons. BIG chunk of code
    MainWindow::checkClassifier();

    QFileDialog folder(this);
    folder.setFileMode(QFileDialog::DirectoryOnly);
    folder.setViewMode(QFileDialog::List);
    QStringList fileNames;
    if (folder.exec()){
        fileNames = folder.selectedFiles();
    }

    if(fileNames.count() < 1){
        return;
    }

    Mat frame;

    QDir dir(fileNames.at(0));
    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();

    qDebug() << "listing directory contents";
    for (int i = 0; i < list.size(); i++){
        qDebug() << (list.at(i)).absoluteFilePath();
    }
    qDebug() << "done listing dir contents";

    // faceID enumerates from the ~/.config/JoshConwaySoft/uWhoUsers.conf and sets faceID to the next unused value
    // This is so we can sequentially add new detected users to the log, rather than a messy rand()%30000 junk I previously had
    long long faceID ;
    for (faceID = 1; users.contains(QString::number(faceID)); faceID++){
        qDebug() << QString::number(faceID);
    };

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        VideoCapture cap = VideoCapture( fileInfo.absoluteFilePath().toUtf8().constData() );
        qDebug() << "Processing" << fileInfo.absoluteFilePath() ;
        cap >> frame;
        blur(frame, frame, Size(3,3));
        faceCascade.load(faceCascadeName);
        eyesCascade.load(eyesCascadeName);
        vector<Rect> faces;
        Mat frame_gray;
        cvtColor(frame, frame_gray, CV_BGR2GRAY);

        faceCascade.detectMultiScale(frame_gray, faces, 1.1, 3, CV_HAAR_SCALE_IMAGE, Size(50,50));
        for(int j = 0; j < faces.size(); j++){
            vector<Mat> facePicture (1);
            vector<int> faceIndex (1);
            vector<Rect> eyes;
            facePicture[0] = frame(faces[j]);   // Gets the face only as the variable facePicture

            rectangle(frame, faces[j], Scalar(255,0,255), 1, 8, 0); // Draws rectangles on webcam video
            string faceString = static_cast<ostringstream*>( &(ostringstream() << j) )->str();
            putText(frame, faceString, Point(faces[j].x, (faces[j].y+30)),FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1,8, false);

            eyesCascade.detectMultiScale( facePicture[0], eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

            if (eyes.size() != 0){
                cvtColor(facePicture[0], facePicture[0], CV_BGR2GRAY); // colorspace change to gray
                int predicted = -1;
                double confidence ;
                model->predict(facePicture[0], predicted, confidence); // Check the machine learner and ask if it's seen this face before
                string predictString = static_cast<ostringstream*>( &(ostringstream() << predicted) )->str();
                putText(frame, predictString, Point((faces[j].x + faces[j].width - 40), (faces[j].y + 30)),FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1,8, false);
                if (predicted == -1){
                    faceIndex[0] = faceID;
                    SaveSettings(QString::number(faceID), QString::number(1));
                    model->update(facePicture,faceIndex); // If its not in the FaceRecognizer, add it
                    faceID = faceID + 1;
                }else{
                    faceIndex[0] = predicted;
                    model->update(facePicture,faceIndex);  // if the face is already in, add this as another data point
                }
                qDebug() << predicted << confidence;
            }
        }
    }
    qDebug() << "Done processing files";
    qDebug() << (QDir::homePath() + QString("/.uWho/face.xml")) ;
    model->save(face_file);
    destroyWindow("VidWindow");
}

void MainWindow::on_ipButton_clicked()
{
    webget webgetwindow;
    webgetwindow.setModal(true);
    webgetwindow.exec();
}

void MainWindow::on_configureButton_clicked()
{
    config configwindow;
    configwindow.setModal(true);
    configwindow.exec();
}


//******************Save settings


void MainWindow::SaveSettings(QString key, QString varible)
{
QSettings settings("JoshConwaySoft", "uWhoUsers");
settings.setValue(key, varible);
}
