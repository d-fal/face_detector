#ifndef NETWORKMANAGEMENT_H
#define NETWORKMANAGEMENT_H

#include <QDialog>

namespace Ui {
class networkManagement;
}

class networkManagement : public QDialog
{
    Q_OBJECT

public:
    explicit networkManagement(QWidget *parent = 0);
    ~networkManagement();
    static QString systemCode;
    static bool isGenuine;

private slots:
    void on_connectionTest_clicked();

    void on_validateCode_clicked();

private:
    Ui::networkManagement *ui;
};

#endif // NETWORKMANAGEMENT_H
