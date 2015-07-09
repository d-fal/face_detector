#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "checkcameras.h"
#include "networkmanagement.h"
#include <QInputDialog>
using namespace std;
using namespace cv;
    QString MainWindow::imgPath;
    QString MainWindow::serverAddress;
    QSqlDatabase MainWindow::db;
    QString MainWindow::foundedCode;
    bool MainWindow::terminateThread=false;
    bool MainWindow::newUser;
    bool MainWindow::isProcessRunning=false;
    bool MainWindow::handin=true;
    bool MainWindow::takeback;
    QSettings settings("Cyber 7", "Face Recognizer");
    showMessages msgs;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
newUser=false;
ui->setupUi(this);

    QMenu *fileMenu = menuBar()->addMenu(tr("&Camera Management"));
    QMenu *networkMenu = menuBar()->addMenu(tr("&Network Management"));

    fileMenu->addAction("Camera/Scanner settings",this,
                    SLOT(openCameraSettings()),Qt::CTRL+Qt::Key_N);
    fileMenu->addSeparator();
    networkMenu->addAction("Network Settings",this,
                       SLOT(openNetworkSettings()),Qt::CTRL+Qt::Key_T);
    string pPath;
    pPath =getenv("USERPROFILE");

    imgPath=QString::fromUtf8(pPath.c_str())+"\\appData\\Local\\cyber7";
    if(!QDir(imgPath).exists()) QDir().mkdir(imgPath);

    MainWindow::db =QSqlDatabase::addDatabase("QSQLITE");
    MainWindow::db.setDatabaseName(imgPath+"\\faceRecognition.db");
    if(!MainWindow::db.open()) {
        qDebug()<<MainWindow::db.lastError();
    }else{
        cout<<"DB is openned!"<<endl;
    }
     QSqlQuery query;
     if(query.exec("create table if not exists previously_registered (id int , "
                         "classID INTEGER PRIMARY KEY AUTOINCREMENT,attendantID varchar(30), "
                         "firstName varchar(30), "
                         " lastName varchar(30), registrationDate datetime,remoteID int)")){
               cout<<"DB created"<<endl;
           }else{
               qDebug()<<query.lastError();

           }


    checkCameras::cameraCode=settings.value("cameraCode").toInt();
    checkCameras::scannerCode=settings.value("scannerCode").toInt();
    serverAddress=settings.value("server").toString();

    activateThisDistro();
    resetDefaultImages();



}

void MainWindow::openCameraSettings(){
    checkCameras *checkCams = new checkCameras(this);
    checkCams->show();
}


MainWindow::~MainWindow()
{


    db.close();
    terminateThread=true;
    barCodeThread::isActive=false;
    delete ui;

}

void MainWindow::remoteCodeFound(int id){
    ui->remoteFound->setText("remote ID: "+QString::number(id)+" <--> "+
                             MainWindow::foundedCode);
    QSqlQuery query;
    if(handin){
    query.exec("update previously_registered set remoteID="+QString::number(id)+" where "
               "attendantID="+MainWindow::foundedCode);
    }
}

void MainWindow::readingFinished(){
cout<<"Ended"<<endl;
}

void MainWindow::on_pushButton_clicked()
{
  openUpCameras();
}

void MainWindow::authenticationStat(bool stat){
    if(stat) ui->label_4->setStyleSheet("QLabel{ color:green;}");
        else ui->label_4->setStyleSheet("QLabel{ color:red;}");
}

void MainWindow::showImage(QImage img, QString code){
    if(!MainWindow::isProcessRunning ){
    QSound::play(imgPath+"\\sounds\\"+"\\notify.wav");

    MainWindow::isProcessRunning=true;
    foundedCode=code;
    faceDetector2 *fd=new faceDetector2;
    connect(fd,SIGNAL(authentication(bool)),SLOT(authenticationStat(bool)));
    connect(fd,SIGNAL(sendImage(QImage,QString)),this,SLOT(showImage(QImage,QString)));
    connect(fd,SIGNAL(sendCropped(QString)),this,SLOT(showCropped(QString)));
    connect(fd,SIGNAL(sendOwnerName()),this,SLOT(setOwnerName()));
    connect(fd,SIGNAL(isFaceFounded(bool)),this,SLOT(isFaceFounded(bool)));


    fd->start();
    QString address=MainWindow::imgPath+"\\"+code+"\\1.png";
    Mat imageToBe=imread(address.toStdString());
    QImage img2= QImage((const unsigned char*)(imageToBe.data),
    imageToBe.cols,imageToBe.rows,QImage::Format_RGB888);
    ui->label_5->setPixmap(QPixmap::fromImage(img2).scaled(ui->label_5->width(),ui->label_5->height()));
        //ui->label->resize(ui->label->pixmap()->size());
    }
    ui->label->setPixmap(QPixmap::fromImage(img));
    if(!newUser)
    msgs.updateStats();
}
void MainWindow::isFaceFounded(bool isFace){
    ui->label_2->setText("NO FACE");
    ui->label_2->setStyleSheet("QLabel{color: red;}");
    //ui->label_2->setFont(QFont::setPointSize(25));
}

void MainWindow::showCropped(QString address){
     QSound::play(imgPath+"\\sounds"+"\\shutter.wav");

    QPixmap image(address);
    ui->label_2->setPixmap(image.scaled(ui->label_2->width(),ui->label_2->height()));
    //ui->label->resize(ui->label_2->pixmap()->size());
    //imshow("Face",faceDetector2::crop);

}
void MainWindow::showVideoImage(QImage img){
    if(!MainWindow::isProcessRunning){

    ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->width(),ui->label->height()));
    }
}

void MainWindow::setOwnerName(){
    ui->label_4->setText(QString::fromStdString(faceDetector2::box_text));
}


void MainWindow::openUpCameras(){

    if(!mainThread::isActive){
        mainThread *thread = new mainThread;
        mainThread::isActive=true;
        connect(thread,SIGNAL(codeFound(QString)),ui->textBrowser,SLOT(setText(QString)));
        connect(thread,SIGNAL(sendImage(QImage,QString)),this,SLOT(showImage(QImage,QString)));
        connect(thread,SIGNAL(sendVideoImage(QImage)),this,SLOT(showVideoImage(QImage)));
        connect(thread,SIGNAL(remoteCodeFound(int)),this,SLOT(remoteCodeFound(int)));


        thread->start();
        Sleep(1000);
    }
    if(!barCodeThread::isActive){
        barCodeThread *barcode = new barCodeThread;

        barCodeThread::isActive=true;
        barcode->start();
    }

}

void MainWindow::on_forget_person_clicked()
{
    resetDefaultImages();
    QSqlQuery query;
    query.exec("delete from previously_registered where attendantID='"+MainWindow::foundedCode+"'");
    QDir dir(MainWindow::foundedCode);
    if (!dir.exists()){
        cout<<"File does not exist!"<<endl;

    }else{

       QDir().remove(imgPath+"\\"+MainWindow::foundedCode);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    //db.close();
    mainThread::isActive=false;
    barCodeThread::isActive=false;
    //Sleep(1000);
    //this->close();
    resetDefaultImages();

}

void MainWindow::openNetworkSettings(){
    networkManagement *netManager = new networkManagement(this);
    netManager->show();
}


void MainWindow::activateThisDistro(){
    QMessageBox msgBox;
    networkManagement::isGenuine=settings.value("genuine").toString().toInt()>0;
    if(!networkManagement::isGenuine){
        bool ok;
            QString text = QInputDialog::getText(this, tr("Activate the program"),
                                                 tr("This distribution is not registered yet\n"
                                                    "Some features such as networking would not be able"
                                                    " in trial mode.\n"
                                                    " Activation code should have been sent "
                                                    "to you when you purchased the cyber7 system.\n"
                                                    " If you can not find the code call +98(21)xxxxx .\n"
                                                    "Enter your activation code:"), QLineEdit::Normal,
                                                 "Activation code", &ok);
            if (ok && !text.isEmpty()){
            if(!uploadImage::check_interface_authencity(text)){

                msgBox.setText("Error!\nThere is something wrong with your network connection.\n"
                               "Check the server address in \"Network settings\" and make sure "
                               "the address is ok. If the problem persists, "
                               "contact cyber 7 experts.");
                msgBox.exec();

            } else{
                if(networkManagement::isGenuine){
                    msgBox.setText("Congratulations!\nYour software is now active and working "
                                   "with full functionality.");
                    msgBox.exec();
                    settings.setValue("genuine",1);
                    settings.setValue("systemCode",networkManagement::systemCode);

                }else{
                    msgBox.setText("Sorry!\nThe entered code is not valid.\n"
                                   "You will use this sotware in trial mode. The networking "
                                   "is disabled in trial mode.");
                    msgBox.exec();

                }
            }
            }
            else{
                qDebug()<<"This distro is not active";
            }
    }else{
    networkManagement::systemCode=settings.value("systemCode").toString();
    }
}

void MainWindow::resetDefaultImages(){
    QPixmap pix_male(imgPath+"/Blank-Face-Person-Icon.png");
    QPixmap pix_female(imgPath+"/female-face.png");
    QPixmap pix_unknown(imgPath+"/face1.png");
    ui->label->setPixmap(pix_male.scaled(ui->label->width(),ui->label->height()));
    ui->label_2->setPixmap(pix_female.scaled(ui->label_2->width(),ui->label_2->height()));
    ui->label_5->setPixmap(pix_unknown.scaled(ui->label_5->width(),ui->label_5->height()));


}

void MainWindow::on_pushButton_2_clicked()
{
    if(foundedCode!=""){
    mainThread::search_for_face=true;
    QImage img= QImage((const unsigned char*)(barCodeThread::Frame.data),
    barCodeThread::Frame.cols,barCodeThread::Frame.rows,QImage::Format_RGB888);
    showImage(img,foundedCode);
    }
}

void MainWindow::on_handin_clicked()
{
    handin=true;
    takeback=false;
}

void MainWindow::on_takeback_clicked()
{
    handin=false;
    takeback=true;
}
