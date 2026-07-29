#include "confirmdelete.h"
#include "ui_confirmdelete.h"

ConfirmDelete::ConfirmDelete(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfirmDelete)
{
    ui->setupUi(this);
    setWindowTitle("Confrim");

}

ConfirmDelete::~ConfirmDelete()
{
    delete ui;
}

void ConfirmDelete::on_noButton_clicked(bool checked)
{
    reject();
}


void ConfirmDelete::on_yesButton_clicked(bool checked)
{
    accept();
}

