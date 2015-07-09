#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <windows.h>
#include <QMainWindow>
#include <QProcess>
#include <QProgressDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QPainter>

#include <QImage>
#include <QSound>
#include <QMenu>
#include <QMenuBar>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

#include <QDebug>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QDir>

#include "qrscanner.h"
#include "mainthread.h"
#include "facedetector2.h"
#include "uploadimage.h"
#include "barcodethread.h"
#include "showmessages.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static bool terminateThread;
    static bool newUser;
    static QString foundedCode;
    static QSqlDatabase db;
    static bool isProcessRunning;
    static QString serverAddress;
    static QString imgPath;
    static bool handin;
    static bool takeback;

private:

    Ui::MainWindow *ui;

    private slots:
    void readingFinished();
    void on_pushButton_clicked();
    void showImage(QImage img, QString code);
    void showCropped(QString address);

    void setOwnerName();
    void authenticationStat(bool);
    void isFaceFounded(bool);

    void showVideoImage(QImage img);
    void remoteCodeFound(int id);


    void on_forget_person_clicked();

    void on_pushButton_5_clicked();
    void openCameraSettings();
    void openNetworkSettings();
    void activateThisDistro();
    void openUpCameras();
    void resetDefaultImages();

    void on_pushButton_2_clicked();

    void on_handin_clicked();

    void on_takeback_clicked();

signals:

};

#endif // MAINWINDOW_H
