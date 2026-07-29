#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "vault.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(Vault &v, QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_Submit_clicked();
    //toggles visibility on password
    void onIconClicked(bool isVisible);

private:
    Ui::Login *ui;
    QString enteredPw;
    int attempts = 5;
    Vault& vault;
    QAction *iconAction;
    void setUi();
};

#endif // LOGIN_H
