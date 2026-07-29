#include "denypass.h"
#include "ui_denypass.h"
#include "QLabel"

denyPass::denyPass(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::denyPass)
{
    ui->setupUi(this);
    ui->message->setAlignment(Qt::AlignCenter);
}

denyPass::~denyPass()
{
    delete ui;
}

void denyPass::setMessage(QString msg){
    ui->message->setText(msg);
}


void denyPass::on_pushButton_clicked()
{
    this->close();
}

