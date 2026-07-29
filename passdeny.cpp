#include "passdeny.h"
#include "ui_passdeny.h"

passDeny::passDeny(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::passDeny)
{
    ui->setupUi(this);
}

passDeny::~passDeny()
{
    delete ui;
}
