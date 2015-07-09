#include "qrscanner.h"

using namespace std;
using namespace cv;

qrScanner::qrScanner(QObject *parent) :
    QObject(parent)
{
    myProcess = new QProcess(this);
}

void qrScanner::qrReader(Mat const &img, QString &str){
    QString fileAddress=MainWindow::imgPath+"\\tmp.png";
    imwrite(fileAddress.toStdString(),img);
    QStringList arguments;
    QString program="zbarimg";

    arguments<<"-q"<<"--raw"<<fileAddress;
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(program,arguments);
    myProcess->waitForFinished(); // didn't work without this

    QTextStream stream(myProcess);
    while (!stream.atEnd()) {
    str=stream.readLine();
        }
}
