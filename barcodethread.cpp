#include "barcodethread.h"
#include "checkcameras.h"
using namespace cv;
using namespace std;

bool barCodeThread::isActive=false;
int barCodeThread::waitms=1;
cv::Mat barCodeThread::Frame;
barCodeThread::barCodeThread(QObject *parent) :
    QThread(parent)
{
}

void barCodeThread::run(){

      VideoCapture cap(checkCameras::cameraCode);  // change the number to 1 for an external USB webcam

      while(barCodeThread::isActive)
      {
          if(waitms<=1024) waitms*=2;
          if(!isActive) this->terminate();
          //if(MainWindow::isProcessRunning && !MainWindow::newUser) continue;
         try{

          cap >> Frame;

          if(!Frame.empty()){
            normalize(Frame ,Frame,0,255,NORM_MINMAX, CV_8UC3);
            //cvtColor(Frame,Frame,CV_BGR2RGB);
          //qr->qrReader(Frame,barcode);
  //cout<<barcode.toStdString()<<endl;

          }
         Sleep(waitms);
      } catch(Exception e){
              //cout<<e.err<<endl;
          }
      }
      cap.release();

}
