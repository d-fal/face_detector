#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QThread>

#include "mainwindow.h"

class mainThread : public QThread
{
    Q_OBJECT
public:
    explicit mainThread(QObject *parent = 0);
    void run();
    static cv::Mat Frame;
    static bool search_for_face;
    static QString barcode;
    static int remoteID;
    bool checkForCode(QString code);
    bool checkForCommandCode(QString code);
    static bool isActive;
    static int waitms;
signals:
    void codeFound(QString code);
    void sendImage(QImage img, QString code);
    void sendVideoImage(QImage img);
    void controlCommandRemote();
    void remoteCodeFound(int);

public slots:

};

#endif // MAINTHREAD_H
