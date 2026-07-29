#ifndef NOPASS_H
#define NOPASS_H

#include <QDialog>

namespace Ui {
class noPass;
}

class noPass : public QDialog
{
    Q_OBJECT

public:
    explicit noPass(QWidget *parent = nullptr);
    ~noPass();

private:
    Ui::noPass *ui;
};

#endif // NOPASS_H
