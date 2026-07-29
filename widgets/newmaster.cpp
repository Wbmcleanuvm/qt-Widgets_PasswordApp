#include "newmaster.h"
#include "ui_newmaster.h"
#include <qstyle.h>
#include <string>
#include "vault.h"

NewMaster::NewMaster(Vault &v,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewMaster)
    , vault(v)
{
    ui->setupUi(this);
    setWindowTitle("Welcome");

}

NewMaster::~NewMaster()
{
    delete ui;
}

//returns true if string is >= 8 characters and contains at least 1 special character and 1 uppercase letter
bool NewMaster::checkValidPassword(std::string str)
{
    bool hasUpper = false;
    bool hasChar = false;
    // sets to true if any char in str is special
    for (int i = 0; i < str.length(); i++)
    {
        if (!std::isalnum(str[i])) hasChar = true;
        if (std::isupper(str[i])) hasUpper = true;
    }
    return hasUpper && hasChar && (str.length() >= 8);
}

bool NewMaster::passwordsMatch()
{
    if(ui->firstBox->text() == ui->secBox->text()) return true;
    return false;
}

bool NewMaster::submitValid()
{
    if(passwordsMatch() && checkValidPassword(ui->firstBox->text().toStdString())) return true;
    return false;
}


//slots
void NewMaster::on_firstBox_textChanged(const QString &arg1)
{
    if(!checkValidPassword(arg1.toStdString()) || !passwordsMatch())
    {
        ui->firstBox->setProperty("hasError", true);
        ui->secBox->setProperty("hasError", true);
    }else
    {
        ui->firstBox->setProperty("hasError", false);
        ui->secBox->setProperty("hasError", false);
    }
    ui->firstBox->style()->unpolish(ui->firstBox);
    ui->firstBox->style()->polish(ui->firstBox);
}

void NewMaster::on_secBox_textChanged(const QString &arg1)
{
    if(!passwordsMatch() || !checkValidPassword(arg1.toStdString()))
    {
        ui->firstBox->setProperty("hasError", true);
        ui->secBox->setProperty("hasError", true);
    }else
    {
        ui->firstBox->setProperty("hasError", false);
        ui->secBox->setProperty("hasError", false);
    }

    ui->secBox->style()->unpolish(ui->secBox);
    ui->secBox->style()->polish(ui->secBox);


}

void NewMaster::on_submitButton_clicked()
{
    enteredText = ui->firstBox->text().toStdString();
    if(passwordsMatch() && checkValidPassword(enteredText))
    {
        vault.initializeMaster(enteredText);
        vault.verifyMaster(enteredText);
        enteredText.clear();
        accept();
        ui->firstBox->clear();
        ui->secBox->clear();
        this->close();
    }
    else if(!checkValidPassword(enteredText))
    {
        deny.setMessage("Password must contain: \n One Uppercase letter, \t One special character, \n Be at least 8 characters");
        deny.exec();
    }
    else
    {
        deny.setMessage("Passwords must match!");
        deny.exec();
    }

}

