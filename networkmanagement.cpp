#include "networkmanagement.h"
#include "ui_networkmanagement.h"
#include "mainwindow.h"
#include "uploadimage.h"
#include <QInputDialog>
QString networkManagement::systemCode;
bool networkManagement::isGenuine;


networkManagement::networkManagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::networkManagement)
{
    ui->setupUi(this);
    ui->serverAddress->setText(MainWindow::serverAddress);

    if(networkManagement::isGenuine){
        ui->activationCode->setEnabled(false);
        ui->validateCode->setEnabled(false);
    }else{
        ui->activationCode->setEnabled(true);
        ui->validateCode->setEnabled(true);
    }
}

networkManagement::~networkManagement()
{
    delete ui;
}

void networkManagement::on_connectionTest_clicked()
{
    MainWindow::serverAddress=ui->serverAddress->text();
    if(uploadImage::check_interface_authencity("Test")){
        QMessageBox msgBox;
        msgBox.setText("This distribution is connected to server.");
        msgBox.exec();
        QSettings settings("Cyber 7", "Face Recognizer");
        settings.setValue("Server",MainWindow::serverAddress);

    }

}

void networkManagement::on_validateCode_clicked()
{
    QSettings settings("Cyber 7", "Face Recognizer");
    QMessageBox msgBox;
    if(!uploadImage::check_interface_authencity(ui->activationCode->text())){

        msgBox.setText("Error!\nThere is something wrong with your network connection.\n"
                       "Check the server address in \"Network settings\" and make sure "
                       "the address is ok. If the problem persists, "
                       "contact cyber 7 experts.");
        msgBox.exec();

    } else{
        if(networkManagement::isGenuine){
            msgBox.setText("Congratulations!\nYour software is now active and working "
                           "with full functionality.");
            msgBox.exec();
            settings.setValue("genuine",1);
            settings.setValue("systemCode",networkManagement::systemCode);
            this->close();

        }else{
            msgBox.setText("Sorry!\nThe entered code is not valid.\n"
                           "You will use this sotware in trial mode. The networking "
                           "is disabled in trial mode.");
            msgBox.exec();

        }
    }
}
