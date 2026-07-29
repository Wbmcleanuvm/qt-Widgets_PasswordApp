#include "login.h"
#include "ui_login.h"
#include "vault.h"
#include <QDebug>

Login::Login(Vault &v, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login),
    vault(v)
{
    ui->setupUi(this);
    setUi();
    // ui->viewPw->setIcon(QIcon(":/icons/icons/eyeOpenIcon.svg"));
    // ui->viewPw->setIconSize(QSize(10, 10));
    // ui->viewPw->setFixedSize(30, 30);
    // ui->viewPw->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}

Login::~Login()
{
    delete ui;
}
void Login::onIconClicked(bool isVisible){
    ui->pwEnter->setEchoMode(isVisible ? QLineEdit::Normal : QLineEdit::Password);
    iconAction->setIcon(QIcon(isVisible ? ":/icons/icons/eyeClosedIcon.svg" : ":/icons/icons/eyeOpenIcon.svg"));
}

void Login::setUi(){
    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("Login");
    ui->pwEnter->setPlaceholderText("Password");
    ui->pwEnter->setAlignment(Qt::AlignCenter);
    ui->pwEnter->setEchoMode(QLineEdit::Password);
    iconAction = new QAction(QIcon(":/icons/icons/eyeOpenIcon.svg"), "View Password", ui->pwEnter);
    iconAction->setCheckable(true);
    ui->pwEnter->addAction(iconAction, QLineEdit::TrailingPosition);
    connect(iconAction, &QAction::toggled, this, &Login::onIconClicked);
}

void Login::on_Submit_clicked()
{
    enteredPw = ui->pwEnter->text();
    if(vault.verifyMaster(enteredPw.toStdString())){
        accept();
    }else if(attempts > 0)
    {
        ui->pwEnter->clear();
        attempts -=1;
        ui->pwIncorrect->setText("Incorrect, " + QString::number(attempts) + " Attempts remaining.");
    }else
    {
        reject();
    }
}


