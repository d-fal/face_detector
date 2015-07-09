#ifndef BARCODETHREAD_H
#define BARCODETHREAD_H

#include <QThread>
#include "mainthread.h"

class barCodeThread : public QThread
{
    Q_OBJECT
public:
    explicit barCodeThread(QObject *parent = 0);
    void run();
    static cv::Mat Frame;
    static bool isActive;
    static int waitms;
signals:

public slots:

};

#endif // BARCODETHREAD_H
