#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include "networkmanagement.h"
#include "uploadimage.h"

uploadImage::uploadImage(QObject *parent) :
    QObject(parent)
{
}

void uploadImage::uploadFile(std::string filepath,std::string label, int Authencity){
    std::cout<<"Syncing... "<<Authencity<<std::endl;
    QEventLoop eventLoop;
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart textPart;
    QHttpPart textPart2, textPart3,textPart4,textPart5;
    textPart2.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"attendantID\""));
    QString Label=QString::fromStdString(label);
    textPart2.setBody(Label.toLatin1());


    textPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"submit\""));
    textPart.setBody("Upload image");

    textPart3.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"Authencity\""));
    textPart3.setBody(QByteArray::number(Authencity));

    textPart4.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"systemCode\""));
    textPart4.setBody(networkManagement::systemCode.toUtf8());

    //textPart5.setHeader(QNetworkRequest::ContentDispositionHeader,
    //                    QVariant("form-data; name=\"currentTime\""));
    //QString just="12-03-2015 00:01:29";

    //textPart5.setBody(just.toUtf8());

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"fileToUpload\" ; filename=\""
                                 +QString::fromStdString(filepath)+"\""));
    QFile *file = new QFile(QString::fromStdString(filepath));
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
    multiPart->append(textPart);
    multiPart->append(textPart2);
    multiPart->append(textPart3);
    multiPart->append(imagePart);
    multiPart->append(textPart4);
   // multiPart->append(textPart5);

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString(MainWindow::serverAddress+"/uploader.php") ) );
    QNetworkReply *reply = mgr.post(req,multiPart);


    multiPart->setParent(reply);
      eventLoop.exec(); // blocks stack until "finished()" has been called
    if (reply->error() == QNetworkReply::NoError) {
        //success
        QString strx=reply->readAll();
      qDebug()<<strx;

        QJsonDocument jsonResponse = QJsonDocument::fromJson(strx.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();



     //   qDebug()<<propertyKeys;
        delete reply;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }

}


bool uploadImage::syncRemoteID(){
    QEventLoop eventLoop;
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);


    QHttpPart textPart2;
    textPart2.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"syncRemotes\""));
    QString Label="\""+MainWindow::foundedCode+"\","+QString::number(mainThread::remoteID)+"";
    textPart2.setBody(Label.toLatin1());

    multiPart->append(textPart2);
    qDebug()<<Label;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString(MainWindow::serverAddress+"/syncRemotes.php") ) );
    QNetworkReply *reply = mgr.post(req,multiPart);


    multiPart->setParent(reply);
      eventLoop.exec(); // blocks stack until "finished()" has been called
    if (reply->error() == QNetworkReply::NoError) {
        //success
      QString strx=reply->readAll();
      qDebug()<<strx;

      //   qDebug()<<propertyKeys;
      delete reply;
      return true;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
        return false;
    }


}
bool uploadImage::take_remote_back(const int remoteid){

    QEventLoop eventLoop;
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);


    QHttpPart textPart2,textPart1;
    textPart2.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"take_remote_back\""));
    textPart2.setBody(QByteArray::number(remoteid));
    textPart1.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"systemCode\""));
    textPart1.setBody(networkManagement::systemCode.toUtf8());


    multiPart->append(textPart2);
    multiPart->append(textPart1);

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString(MainWindow::serverAddress+"/syncRemotes.php") ) );
    QNetworkReply *reply = mgr.post(req,multiPart);


    multiPart->setParent(reply);
      eventLoop.exec(); // blocks stack until "finished()" has been called
    if (reply->error() == QNetworkReply::NoError) {
        //success
      QString strx=reply->readAll();
      qDebug()<<strx;

      //   qDebug()<<propertyKeys;
      delete reply;
      return true;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
        return false;
    }

}

bool uploadImage::check_interface_authencity(QString code){
    QEventLoop eventLoop;
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);


    QHttpPart textPart2;
    textPart2.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"isGenuine\""));
    textPart2.setBody(code.toUtf8());

    multiPart->append(textPart2);

    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString(MainWindow::serverAddress+"/recieveVotersData.php") ) );
    QNetworkReply *reply = mgr.post(req,multiPart);

    multiPart->setParent(reply);
      eventLoop.exec(); // blocks stack until "finished()" has been called
    if (reply->error() == QNetworkReply::NoError) {
        //success
      QString strx=reply->readAll();
      QJsonDocument jsonResponse = QJsonDocument::fromJson(strx.toUtf8());
      QJsonObject jsonObject = jsonResponse.object();

      QJsonObject obj=jsonObject["code"].toObject();
      code=obj.value("response").toString();
      bool isGenuineServer=obj.value("validity").toString().toInt()>0;
      if(isGenuineServer){
       networkManagement::systemCode=code;
       networkManagement::isGenuine=true;
      }


        delete reply;

      return true;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
        return false;
    }

}
