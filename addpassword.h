#ifndef ADDPASSWORD_H
#define ADDPASSWORD_H

#include <QDialog>
#include "vault.h"

namespace Ui {
class AddPassword;
}

class AddPassword : public QDialog
{
    Q_OBJECT

public:
    explicit AddPassword(Vault &v, QWidget *parent = nullptr);
    ~AddPassword();

private slots:
    void on_submitButton_clicked();

private:
    Ui::AddPassword *ui;
    Vault vault;
};

#endif // ADDPASSWORD_H
