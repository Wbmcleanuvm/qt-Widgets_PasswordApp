#ifndef CONFIRMDELETE_H
#define CONFIRMDELETE_H

#include <QDialog>

namespace Ui {
class ConfirmDelete;
}

class ConfirmDelete : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDelete(QWidget *parent = nullptr);
    ~ConfirmDelete();

private slots:
    void on_noButton_clicked(bool checked);

    void on_yesButton_clicked(bool checked);

private:
    Ui::ConfirmDelete *ui;
    void buildUi();
};

#endif // CONFIRMDELETE_H
