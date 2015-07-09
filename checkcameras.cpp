#include "checkcameras.h"
#include "ui_checkcameras.h"
#include "uploadimage.h"
using namespace std;
using namespace cv;

int checkCameras::scannerCode;
int checkCameras::cameraCode;
checkCameras::checkCameras(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkCameras)
{
    ui->setupUi(this);
    ui->cameraCode->setValue(cameraCode);
    ui->scannerCode->setValue(scannerCode);
    int maxCams=countCameras();
    cout<<"number of active cameras: "<<maxCams<<endl;
    ui->cameraCode->setMaximum(maxCams);
    ui->scannerCode->setMaximum(maxCams);
}

checkCameras::~checkCameras()
{
    delete ui;
}

int checkCameras::countCameras(){
       int maxTested = 4,camCount=0;
       for (int i = 0; i < maxTested; i++){
         cv::VideoCapture temp_camera(i);
         if(temp_camera.isOpened()){
             cout<<"Camera #"<<i<<" is ready."<<endl;
             Mat temp_frame;
             int cnt=0;
             while(true){
                 temp_camera>>temp_frame;
                 if(!temp_frame.empty() && cnt>30){
                 QString label="Camera #"+QString::number(i);
                 //cv::resize(temp_frame,temp_frame,Size(640,480),-1,-1);

                 putText(temp_frame,label.toStdString(),Point(100,100),
                         FONT_HERSHEY_COMPLEX_SMALL, 3, cvScalar(200,200,250), 1, CV_AA);

                 temp_camera.release();
                 QImage img2= QImage((const unsigned char*)(temp_frame.data),
                 temp_frame.cols,temp_frame.rows,QImage::Format_RGB888);
                 switch (i) {
                 case 0:
                     ui->camera_0->setPixmap(
                                 QPixmap::fromImage(img2).scaled(
                                     ui->camera_0->width(),ui->camera_0->height()));
                     break;
                 case 1:
                     ui->camera_1->setPixmap(
                                 QPixmap::fromImage(img2).scaled(
                                     ui->camera_1->width(),ui->camera_1->height()));
                     break;
                 case 2:
                     ui->camera_2->setPixmap(
                                 QPixmap::fromImage(img2).scaled(
                                     ui->camera_2->width(),ui->camera_2->height()));

                     break;
                 default:
                     break;
                 }
                 break;
                 }
                 cnt++;
             }
             camCount++;
           }
       }
       return camCount;
    }

void checkCameras::on_okBtn_clicked()
{
    cameraCode=ui->cameraCode->value();
    scannerCode=ui->scannerCode->value();
    if(cameraCode==scannerCode){
        QMessageBox msgBox;
        msgBox.setText("Error!\nTwo camera IDs cannot be the same!");
        msgBox.exec();
    }else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Save settings?",
                                      "Click yes if you have set the camera IDs right."
                                      "Click no, otherwise.",
                                        QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){
        QSettings settings("Cyber 7", "Face Recognizer");
        settings.setValue("cameraCode",cameraCode);
        settings.setValue("scannerCode",scannerCode);

        this->close();
        }
    }

}
