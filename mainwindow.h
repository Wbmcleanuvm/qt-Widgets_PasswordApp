#pragma once

#include <QWidget>
#include <QStringList>
#include "vault.h"
#include "addpassword.h"
#include "confirmdelete.h"

class QListView;
class QStandardItemModel;
class QLineEdit;
class QPushButton;
class QModelIndex;


enum ConnectionRole
{
    UsernameRole = Qt::UserRole + 1,
    PasswordRole
};

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(Vault &v, QWidget *parent = nullptr);

    void loadData();

private slots:
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void togglePasswordVisibility(bool show);
    void onDeleteClicked();
    void onAddClicked();

private:
    void buildUi();

    QListView *m_listView;
    QStandardItemModel *m_model;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_showPasswordBtn;
    QPushButton *m_deletePasswordBtn;
    QPushButton *m_addPasswordBtn;
    Vault& vault;
    AddPassword pass;
    ConfirmDelete del;


};