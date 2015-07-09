#ifndef SHOWMESSAGES_H
#define SHOWMESSAGES_H

#include <QObject>
#include "mainwindow.h"
class showMessages : public QObject
{
    Q_OBJECT
public:
    explicit showMessages(QObject *parent = 0);
    void updateStats();
    static int screenWidth;
    static int screenHeight;
    QWidget *win;
    QPixmap *pix;
    QPainter *paint;
    QPixmap *pixTop;
    QLabel *readiesBoard;


signals:

public slots:
void close();
};

#endif // SHOWMESSAGES_H
