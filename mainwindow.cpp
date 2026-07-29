#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include "vault.h"
#include <QStringList>
#include <QStringListModel>
#include "mainwindow.h"
#include <QListView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "addpassword.h"

MainWindow::MainWindow(Vault &v, QWidget *parent) : QWidget(parent)
    ,vault(v), pass(v)
{
    setWindowTitle("PassBase");
    buildUi();
    loadData();


    // Whenever the selection changes, refresh the form on the right.
    connect(m_listView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onCurrentChanged);

    if (m_model->rowCount() > 0)
        m_listView->setCurrentIndex(m_model->index(0, 0));
}
void MainWindow::loadData()
{
    const QStringList passwords = vault.getPasswords();
    const QStringList siteNames = vault.getSiteNames();
    const QStringList usernames = vault.getUsernames();
    m_model->removeRows(0, m_model->rowCount());
    for (int i = 0; i < siteNames.size(); ++i) {
        auto *item = new QStandardItem(siteNames[i]);
        item->setData(usernames.value(i), UsernameRole);
        item->setData(passwords.value(i), PasswordRole);
        item->setEditable(false);
        m_model->appendRow(item);
    }
    m_model->sort(0,Qt::AscendingOrder);
}

void MainWindow::onCurrentChanged(const QModelIndex &current, const QModelIndex & previous)
{
    if (!current.isValid()) {
        m_usernameEdit->clear();
        m_passwordEdit->clear();
        return;
    }
    m_usernameEdit->setText(current.data(UsernameRole).toString());
    m_passwordEdit->setText(current.data(PasswordRole).toString());
}

void MainWindow::togglePasswordVisibility(bool show)
{
    m_passwordEdit->setEchoMode(show ? QLineEdit::Normal : QLineEdit::Password);
    m_showPasswordBtn->setText(show ? "Hide" : "Show");
}
void MainWindow::onDeleteClicked()
{
    std::string user = m_usernameEdit->text().toStdString();
    QModelIndex idx = m_listView->currentIndex();
    QString site = idx.data(Qt::DisplayRole).toString();
    del.show();
    if (del.exec() == QDialog::Accepted){
        vault.deletePassword(site.toStdString(), user);
        loadData();
    }
}
void MainWindow::onAddClicked(){
    pass.show();
    if(pass.exec() == QDialog::Accepted) loadData();
}


void MainWindow::buildUi()
{
    // ---- Left column: list of site names ----
    m_model = new QStandardItemModel(this);
    m_listView = new QListView(this);
    m_listView->setModel(m_model);
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_deletePasswordBtn = new QPushButton("Delete");
    m_deletePasswordBtn->setFixedWidth(50);
    connect(m_deletePasswordBtn, &QPushButton::clicked,
            this, &MainWindow::onDeleteClicked);

    m_addPasswordBtn = new QPushButton("New");
    m_addPasswordBtn->setCheckable(true);
    m_addPasswordBtn->setFixedWidth(50);
    connect(m_addPasswordBtn, &QPushButton::clicked,
            this, &MainWindow::onAddClicked);

    auto *listButtonsRow = new QHBoxLayout;
    listButtonsRow->addWidget(m_addPasswordBtn);
    listButtonsRow->addWidget(m_deletePasswordBtn);
    listButtonsRow->addStretch();

    auto *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(listButtonsRow);
    leftLayout->addWidget(m_listView);

    // ---- Right column: username / password ----
    m_usernameEdit = new QLineEdit;
    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setReadOnly(true);
    m_showPasswordBtn = new QPushButton("Show");
    m_showPasswordBtn->setCheckable(true);
    m_showPasswordBtn->setFixedWidth(50);
    connect(m_showPasswordBtn, &QPushButton::toggled,
            this, &MainWindow::togglePasswordVisibility);

    auto *passwordRow = new QHBoxLayout;
    passwordRow->addWidget(m_passwordEdit);
    passwordRow->addWidget(m_showPasswordBtn);

    auto *form = new QFormLayout;
    form->addRow("Username", m_usernameEdit);
    form->addRow("Password", passwordRow);

    auto *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(form);
    rightLayout->addStretch();

    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 2);
}



