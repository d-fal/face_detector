#ifndef CHECKCAMERAS_H
#define CHECKCAMERAS_H
#include "mainwindow.h"
#include <QDialog>

namespace Ui {
class checkCameras;
}

class checkCameras : public QDialog
{
    Q_OBJECT

public:
    explicit checkCameras(QWidget *parent = 0);
    ~checkCameras();
    int countCameras();
    static int scannerCode;
    static int cameraCode;
private slots:
    void on_okBtn_clicked();

private:
    Ui::checkCameras *ui;
};

#endif // CHECKCAMERAS_H
