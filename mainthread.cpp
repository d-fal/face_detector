#include "mainthread.h"
#include "checkcameras.h"
using namespace cv;
using namespace std;

    QString mainThread::barcode;
    Mat mainThread::Frame;
    bool mainThread::search_for_face;
    int mainThread::remoteID;
    bool mainThread::isActive=false;
    int mainThread::waitms=1;
mainThread::mainThread(QObject *parent) :
    QThread(parent)
    {

    }
void mainThread::run(){

    qrScanner *qr = new qrScanner;
    QString  oldBarcode;

    uploadImage *upload = new uploadImage;
    VideoCapture cap(checkCameras::scannerCode);  // change the number to 1 for an external USB webcam

    while(mainThread::isActive){
          //if(MainWindow::isProcessRunning && !MainWindow::newUser) continue;
         try{
          if(!isActive) this->terminate();

          cap >> Frame;
          if(!Frame.empty()){
            normalize(Frame ,Frame,0,255,NORM_MINMAX, CV_8UC3);
          qr->qrReader(Frame,barcode);

          if((barcode!="" && barcode!=oldBarcode)){
              waitms>>2;
              barCodeThread::waitms=1;
              if(checkForCommandCode(barcode)){
                  emit remoteCodeFound(remoteID);
                  if(MainWindow::handin){
                  upload->syncRemoteID();
                  }else{
                  upload->take_remote_back(remoteID);
                  }
                barcode.clear();
                continue;
              }else{
                  if(waitms<1000) waitms+=50;
              }
              if(barcode.length()<6) continue;

              if(checkForCode(barcode)){
               cout<<"User exists"<<endl;
              } else{
               cout<<"NEW USER"<<endl;
               MainWindow::newUser=true;
              }
              search_for_face=true;
              //cout<<barcode.toStdString()<<endl;
              emit codeFound(barcode);
              QImage img= QImage((const unsigned char*)(barCodeThread::Frame.data),
              barCodeThread::Frame.cols,barCodeThread::Frame.rows,QImage::Format_RGB888);
              emit sendImage(img,barcode);
            }else{
          QImage img2= QImage((const unsigned char*)(barCodeThread::Frame.data),
          barCodeThread::Frame.cols,barCodeThread::Frame.rows,QImage::Format_RGB888);
          emit sendVideoImage(img2);
          }
          if (MainWindow::terminateThread)  return;
          oldBarcode=barcode;
          barcode.clear();
          Sleep(waitms);
          }
      } catch(Exception e){
            // cout<<e.err<<endl;
          }
      }
    cap.release();
}
bool mainThread::checkForCommandCode(QString code){
    string comm=code.left(1).toStdString();
    if(comm.compare("A")==0){
    remoteID=code.right(3).toInt();
return true;
    }
    return false;
}

bool mainThread::checkForCode(QString code){
       QSqlQuery query;
    query.exec("SELECT EXISTS(select * from previously_registered "
                  "where attendantID='"+code+"')");
    query.next();
    if(query.value(0).toInt()==0){

    if(!query.exec("insert or replace into previously_registered "
                  "(attendantID , firstName) VALUES ('"+code+"' ,"
                   " 'unknown')")){
        qDebug()<<query.lastError();
    }
    return false;

    } else{
        return true;
    }

}
