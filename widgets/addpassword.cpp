#include "addpassword.h"
#include "ui_addpassword.h"
#include <string>
#include <QString>
#include <QStringList>

AddPassword::AddPassword(Vault &v, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddPassword)
    , vault(v)
{
    ui->setupUi(this);
    ui->denyText->hide();
    setWindowTitle("Add Password");

}

AddPassword::~AddPassword()
{
    delete ui;
}

void AddPassword::on_submitButton_clicked()
{
    bool submit = true;
    ui->denyText->hide();
    QString site = ui->siteEnter->text();
    QString username = ui->usernameEnter->text();
    QStringList siteLst = vault.getSiteNames();
    QStringList userLst = vault.getUsernames();
    for(int i = 0; i < siteLst.size();i++){
        if(site == siteLst[i] && username == userLst[i]){
            submit = false;
            ui->denyText->show();
        }
    }
    std::string password = ui->pwEnter->text().toStdString();
    if(submit){
        vault.addPassword(site.toStdString(),username.toStdString(),password);
        ui->siteEnter->clear();
        ui->usernameEnter->clear();
        ui->pwEnter->clear();
        accept();
    }

}