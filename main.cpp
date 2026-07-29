#include "mainwindow.h"
#include "newmaster.h"
#include "src/vault.h"
#include "login.h"
#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QCoreApplication>
#include <QDir>


int main(int argc, char *argv[])

{
    QApplication a(argc, argv);
    QString dbPath = QDir(QCoreApplication::applicationDirPath()).filePath("database");
    Vault vault(dbPath.toStdString());
    QFontDatabase::addApplicationFont(":/f/fonts/IBMPlexMono-SemiBold.ttf");
    QFontDatabase::addApplicationFont(":/f/fonts/OxygenMono-Regular.ttf");

    a.setStyleSheet(R"(
    QDialog, QWidget { background: qlineargradient(x1: 0, y1: 1, x2: 1, y2: 0, stop:0 #1a2e1b, stop:1 #1c1a1f); } /* Generic Label text */ QLabel { font-family: "IBM Plex Mono SemiBold"; background: transparent; font-size: 15px; } /* Input Fields (Line Edits, Text Edits) */ QLineEdit, QTextEdit, QPlainTextEdit {font-family:"Oxygen Mono"; background-color: #000000; color: #ffffff; border: 1px solid #ffffff; border-radius: 4px; padding: 6px 10px; selection-background-color: #007acc; } QLineEdit:focus, QTextEdit:focus { border: 1px solid #007acc; background-color: #ffffff; color: #000000 } /* Buttons (Standard Actions) */ QPushButton { font-family: "Oxygen Mono";background-color: #000000; color: #ffffff;  border: 1px solid #ffffff; border-radius: 4px; padding: 6px 16px; min-width: 80px; } QPushButton:hover { background-color: #ffffff; color: #000000 } QPushButton:pressed { background-color: #2a2a34; } /* Accent Button (e.g., Save, Submit, OK) */ QPushButton#okButton, QPushButton[primary="true"] { background-color: #007acc; } QPushButton#okButton:hover, QPushButton[primary="true"]:hover { background-color: #1c94e0; } /* Combo Boxes (Dropdowns) */ QComboBox { background-color: #2a2a32; border: 1px solid #3d3d4c; border-radius: 4px; padding: 5px 10px; color: #ffffff; min-width: 6em; } QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 20px; border-left-width: 0px; } QComboBox QAbstractItemView { background-color: #2a2a32; border: 1px solid #3d3d4c; selection-background-color: #007acc; color: #ffffff; font-family: "IBM Plex Mono SemiBold" } /* Checkboxes and Radio Buttons */ QCheckBox, QRadioButton { spacing: 8px; color: #e0e0e0; } QCheckBox::indicator, QRadioButton::indicator { width: 16px; height: 16px; border-radius: 3px; border: 1px solid #3d3d4c; background-color: #2a2a32; } QCheckBox::indicator:checked { background-color: #007acc; border: 1px solid #007acc; /* Tip: You can supply an image asset path here if needed */ /* image: url(:/icons/checkmark.png); */ } QListView { background-color: transparent; color: #ffffff; border: 1px solid #ffffff; border-radius: 4px; outline: none; font-family: "Oxygen Mono"; } QListView::item {} QListView::item:hover { background-color: #5c5c61; } QListView::item:selected { background-color: #000000; color: #ffffff; border-radius: 4px; outline: #ffffff} QListView::item:selected:active { background-color: #000000; } QListView::item:selected:!active { background-color: #3d3d4c; }
     )");
    if(!vault.hasMasterPassword())
    {
        NewMaster masterPw(vault);
        if (masterPw.exec() != QDialog::Accepted) return 0;
    }else
    {
        Login login(vault);
        if(login.exec() != QDialog::Accepted) return 0;
    }
    MainWindow w(vault);
    w.show();
    return QApplication::exec();
}
