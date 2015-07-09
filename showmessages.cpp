#include "showmessages.h"
#include <QApplication>
#include <QLabel>
int showMessages::screenHeight;
int showMessages::screenWidth;
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
    pix = new QPixmap(width,height);
    paint = new QPainter(pix);
    pixTop = new QPixmap(width,height);

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
