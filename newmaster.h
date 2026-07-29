#ifndef NEWMASTER_H
#define NEWMASTER_H

#include <QDialog>
#include <string>
#include "vault.h"
#include "denypass.h"


namespace Ui {
class NewMaster;
}

class NewMaster : public QDialog
{
    Q_OBJECT

public:
    explicit NewMaster(Vault &v, QWidget *parent = nullptr);
    ~NewMaster();

    bool checkValidPassword(std::string str);
    bool passwordsMatch();
    bool submitValid();

private slots:

    void on_firstBox_textChanged(const QString &arg1);

    void on_secBox_textChanged(const QString &arg1);

    void on_submitButton_clicked();

private:
    std::string enteredText;
    Ui::NewMaster *ui;
    Vault& vault;
    denyPass deny;

};

#endif // NEWMASTER_H
