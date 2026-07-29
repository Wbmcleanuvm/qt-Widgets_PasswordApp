#ifndef DENYPASS_H
#define DENYPASS_H

#include <QDialog>

namespace Ui {
class denyPass;
}

class denyPass : public QDialog
{
    Q_OBJECT

public:
    explicit denyPass(QWidget *parent = nullptr);
    ~denyPass();
    void setMessage(QString msg);

private slots:
    void on_pushButton_clicked();

private:
    Ui::denyPass *ui;
};

#endif // DENYPASS_H
