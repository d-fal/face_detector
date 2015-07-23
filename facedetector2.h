#ifndef FACEDETECTOR2_H
#define FACEDETECTOR2_H

#include <QThread>
#include "mainwindow.h"
#include "mainthread.h"
class faceDetector2 : public QThread
{
    Q_OBJECT
public:
    explicit faceDetector2(QObject *parent = 0);
    void run();
    void detectAndDisplay();
    void recognizeFace(cv::Mat &Frame);
    static cv::Mat crop;
    static std::string box_text;
   private:
    int currentClassID;
    bool face_founded;
    std::string face_cascade_name;
    cv::CascadeClassifier face_cascade;
    std::string window_name = "Capture - Face detection";
    int filenumber; // Number of file to be saved
    int id;
    std::string filename;
    std::string fn_haar;
    std::string fn_csv;

    std::vector<cv::Mat> images;
    std::vector<int> labels;
    bool personnelTaken;

signals:
    void sendImage(QImage img,QString);
    void sendCropped(QString address);
    void sendOwnerName();
    void authentication(bool);
    void isFaceFounded(bool);
    void sysOnline(bool);
public slots:

};

#endif // FACEDETECTOR2_H
