#ifndef UPLOADIMAGE_H
#define UPLOADIMAGE_H

#include <QObject>
#include "mainwindow.h"
class uploadImage : public QObject
{
    Q_OBJECT
public:
    explicit uploadImage(QObject *parent = 0);
    bool uploadFile(std::string filepath , std::string label, int Authencity);
    static bool syncRemoteID();
    static bool take_remote_back(const int remoteid);
    static bool check_interface_authencity(QString code);
signals:

public slots:

};

#endif // UPLOADIMAGE_H
