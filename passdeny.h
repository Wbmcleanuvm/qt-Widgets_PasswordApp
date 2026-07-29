#ifndef PASSDENY_H
#define PASSDENY_H

#include <QDialog>

namespace Ui {
class passDeny;
}

class passDeny : public QDialog
{
    Q_OBJECT

public:
    explicit passDeny(QWidget *parent = nullptr);
    ~passDeny();

private:
    Ui::passDeny *ui;
};

#endif // PASSDENY_H
