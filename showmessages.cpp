#include "showmessages.h"
#include <QApplication>

int showMessages::screenHeight;
int showMessages::screenWidth;
using namespace std;
showMessages::showMessages(QObject *parent) :
    QObject(parent)
{

}

void showMessages::updateStats(){
    screenWidth = QApplication::desktop()->geometry().width();
    screenHeight = QApplication::desktop()->geometry().height()*.98;
    QImage img(MainWindow::imgPath+"\\"+MainWindow::foundedCode+"\\personnel.png");
    int width=img.width();
    int height=img.height();
    if(width==0 || height==0) return;
    cout<<width<<" x "<<height<<endl;

    pix = new QPixmap(width,height);
    paint = new QPainter(pix);

    QGridLayout *layout = new QGridLayout;
    readiesBoard= new QLabel;

    win= new QWidget;
    paint->fillRect(QRect(0,0,width,height),QColor(220,215,255));
    //
    QFont fontTop=paint->font();
    fontTop.setPointSize(24);
    paint->setPen(QPen(QColor(0,255,255),2));
    paint->setFont(fontTop);

    img.scaled(readiesBoard->width(),readiesBoard->height());
    paint->drawImage(QPointF(0,0),img);
    paint->drawText(QRectF(0,height-50,width,50),Qt::AlignCenter,MainWindow::foundedCode);

    //

    readiesBoard->setPixmap(*pix);
    layout->addWidget(readiesBoard,0,0,-1,-1,Qt::AlignCenter);
    win->setLayout(layout);
    win->setWindowFlags(Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint);

    win->setStyleSheet("background-color:#ffe;");
    win->setGeometry(screenWidth-width,screenHeight-height,width,height);

    win->show();
    QTimer *timer4 = new QTimer;
    timer4->start(2000);
    connect(timer4,SIGNAL(timeout()),win,SLOT(close()));

}
void showMessages::close(){
    win->close();

}

void showMessages::showRemoteConnection(QString id){

     screenWidth = QApplication::desktop()->geometry().width();
     screenHeight = QApplication::desktop()->geometry().height()*.98;

     int width=600;
     int height=200;

     pixMsg   = new QPixmap(width,height);
     painter2 = new QPainter(pixMsg);

     QGridLayout *layout = new QGridLayout;
     messageIndicator= new QLabel;

     msgWin= new QWidget;
     painter2->fillRect(QRect(0,0,width,height),QColor(250,215,255));

     QFont fontTop=painter2->font();
     fontTop.setPointSize(24);

     painter2->setPen(QPen(QColor(0,122,100),2));
     painter2->setFont(fontTop);



     painter2->drawText(QRectF(0,0,width,height),Qt::AlignCenter,id);

     messageIndicator->setPixmap(*pixMsg);
     layout->addWidget(messageIndicator,0,0,-1,-1,Qt::AlignCenter);

     msgWin->setLayout(layout);
     msgWin->setWindowFlags(Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint);

     msgWin->setStyleSheet("background-color:#ffe;");
     msgWin->setGeometry(screenWidth/2-width/2,screenHeight/2-height/2,width,height);

     msgWin->show();
     QTimer *timer4 = new QTimer;
     timer4->start(2000);
     connect(timer4,SIGNAL(timeout()),msgWin,SLOT(close()));
}
