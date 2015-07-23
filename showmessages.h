#ifndef SHOWMESSAGES_H
#define SHOWMESSAGES_H
#include <QLabel>
#include <QObject>
#include "mainwindow.h"
class showMessages : public QObject
{
    Q_OBJECT
public:
    explicit showMessages(QObject *parent = 0);
    void updateStats();
    void showRemoteConnection(QString);

    static int screenWidth;
    static int screenHeight;
    QWidget *win;
    QWidget *msgWin;
    QPixmap *pixMsg;
    QPixmap *pix;
    QPainter *paint;
    QPixmap *pixTop;
    QLabel *readiesBoard;
    QLabel *messageIndicator;
    QPainter *painter2;


signals:

public slots:
void close();
};

#endif // SHOWMESSAGES_H
