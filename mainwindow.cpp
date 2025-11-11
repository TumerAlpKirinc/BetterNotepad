#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "notelist.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <QFont>
#include <QColorDialog>
#include <QColor>
#include <QInputDialog>
#include <QPrintDialog>
#include <QPrinter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    currentFile.clear();

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::confirmDiscardChanges()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(
        this, "Unsaved Changes",
        "You have unsaved changes. Do you want to discard them?",
        QMessageBox::Yes | QMessageBox::No
        );
    return reply == QMessageBox::Yes;
}

void MainWindow::writeFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << ui->textEdit->toHtml();
        ui->textEdit->document()->setModified(false);
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "Could not save the file.");
    }
    this->show();
}


void MainWindow::on_actionNew_triggered()
{
    if(!ui->textEdit->document()->isModified()|| confirmDiscardChanges()){
        ui->textEdit->clear();
        currentFile.clear();
    }
}


void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open File",
        "",
        "BetterNotepad Documents (*.bnpd);;HTML Files (*.html);;All Files (*)"
        );

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            ui->textEdit->setHtml(in.readAll());
            file.close();
            currentFile = filePath;
            ui->textEdit->document()->setModified(false);
        } else {
            QMessageBox::warning(this, "Error", "Could not open the file.");
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (currentFile.isEmpty()) {
        on_actionSave_As_triggered();
    } else {
        writeFile(currentFile);
    }
}


void MainWindow::on_actionSave_As_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Save File",
        "",
        "BetterNotepad Documents (*.bnpd);;HTML Files (*.html);;All Files (*)"
        );

    if (!filePath.isEmpty()) {
        if (!filePath.endsWith(".bnpd")) {
            filePath += ".bnpd";
        }
        currentFile = filePath;
        writeFile(currentFile);
    }
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionAbout_BetterNotepad_triggered()
{

}


void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,this);
    if (ok){
        ui->textEdit->setCurrentFont(font);
    }
}



void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white,this,"Choose a color");
    if(color.isValid()){
        ui->textEdit->setTextColor(color);
    }
}


void MainWindow::on_actionFind_triggered()
{
    bool ok;
    QString searchText = QInputDialog::getText(this, "Find", "Enter text to find:", QLineEdit::Normal, "", &ok);
    if (ok && !searchText.isEmpty()) {
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor = ui->textEdit->document()->find(searchText, cursor);

        if (!cursor.isNull()) {
            ui->textEdit->setTextCursor(cursor);
        } else {
            QMessageBox::information(this, "Find", "The text was not found.");
        }
    }
    else {
        QMessageBox::warning(this, "Error", "Search line is empty");
    }
}


void MainWindow::on_actionPrint_triggered()
{

    QPrinter printer;
    QPrintDialog dialog(&printer,this);
    if(dialog.exec() == QPrintDialog::Rejected)
        return;
    ui->textEdit->print(&printer);
}
void MainWindow::openNote(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        ui->textEdit->setHtml(in.readAll());
        file.close();
        currentFile = filePath;
        ui->textEdit->document()->setModified(false);
        this->show();
    } else {
        QMessageBox::warning(this, "Error", "Could not open the file.");
    }
}




void MainWindow::on_actionReturn_to_Note_List_triggered()
{
    NoteList *notelist = new NoteList();
    this->close();
    notelist->show();

}

