#include "nopass.h"
#include "ui_nopass.h"

noPass::noPass(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::noPass)
{
    ui->setupUi(this);
}

noPass::~noPass()
{
    delete ui;
}
