#ifndef QRSCANNER_H
#define QRSCANNER_H

#include <QObject>
#include "mainwindow.h"
#include <QDebug>
#include <iostream>
#include <QStringList>
class qrScanner : public QObject
{
    Q_OBJECT
public:
    explicit qrScanner(QObject *parent = 0);

private:
    QProcess *myProcess;

signals:

public slots:
    void qrReader(const cv::Mat &img, QString &str);
};

#endif // QRSCANNER_H
