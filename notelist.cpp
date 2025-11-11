#include "notelist.h"
#include "ui_notelist.h"
#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

NoteList::NoteList(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NoteList)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Note Name" << "File Path");
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 400);
    loadNotes();
    MainWindow *mainWindow = new MainWindow();
    connect(this,&NoteList::openNote,mainWindow,&MainWindow::openNote);



}



NoteList::~NoteList()
{
    delete ui;
}

void NoteList::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select Note File", "", "BetterNotepad Documents (*.bnpd);;All Files (*)");
    if (!filePath.isEmpty()) {
        // Yeni satır ekle
        int newRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(newRow);

        // Dosya adı ve yolu
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();

        // Tabloya ekleme
        QTableWidgetItem *nameItem = new QTableWidgetItem(fileName);
        QTableWidgetItem *pathItem = new QTableWidgetItem(filePath);

        // Dosya yolunu düzenlenemez yap
        pathItem->setFlags(pathItem->flags() & ~Qt::ItemIsEditable);

        // Sütunlara yerleştir
        ui->tableWidget->setItem(newRow, 0, nameItem);
        ui->tableWidget->setItem(newRow, 1, pathItem);


    }
}





void NoteList::on_actionInfo_triggered()
{

}
void NoteList::saveNotes() {
    QFile file("notes_list.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            QTableWidgetItem *nameItem = ui->tableWidget->item(row, 0);
            QTableWidgetItem *pathItem = ui->tableWidget->item(row, 1);

            if (nameItem && pathItem) {
                out << nameItem->text() << "," << pathItem->text() << "\n";
            }
        }
        file.close();
    }
}
void NoteList::closeEvent(QCloseEvent *event) {
    saveNotes();
    event->accept();
}
void NoteList::loadNotes() {
    QFile file("notes_list.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");

            if (parts.size() == 2) {
                int newRow = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(newRow);

                QTableWidgetItem *nameItem = new QTableWidgetItem(parts[0].trimmed());
                QTableWidgetItem *pathItem = new QTableWidgetItem(parts[1].trimmed());

                if (!nameItem || !pathItem) {
                    qDebug() << "Error: Table items could not be created!";
                    continue;
                }

                pathItem->setFlags(pathItem->flags() & ~Qt::ItemIsEditable);

                ui->tableWidget->setItem(newRow, 0, nameItem);
                ui->tableWidget->setItem(newRow, 1, pathItem);
            } else {
                qDebug() << "Error: Malformed line in notes file -> " << line;
            }
        }
        file.close();
    } else {
        qDebug() << "Error: Could not open notes_list.txt";
    }
}



void NoteList::on_pushButton_2_clicked()
{
    int selectedRow = ui->tableWidget->currentRow(); // Kullanıcının seçtiği satır

    if (selectedRow >= 0) { // Eğer geçerli bir satır seçildiyse
        ui->tableWidget->removeRow(selectedRow); // Satırı sil
    } else {
        QMessageBox::warning(this, "Warning", "Please select a note to delete.");
    }
}


void NoteList::on_pushButton_3_clicked()
{
    int selectedRow = ui->tableWidget->currentRow(); // Seçili satırı al

    if (selectedRow >= 0) {
        QString filePath = ui->tableWidget->item(selectedRow, 1)->text(); // Dosya yolunu al

        // Kullanıcıya dosyayı silmek istediğini onaylat
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Note",
                                      "Are you sure you want to delete this note permanently?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Dosyayı bilgisayardan sil
            if (QFile::remove(filePath)) {
                ui->tableWidget->removeRow(selectedRow); // Başarıyla silindiyse tablodan da kaldır
                saveNotes(); // Güncellenmiş listeyi kaydet
            } else {
                QMessageBox::warning(this, "Error", "Failed to delete the file.");
            }
        }
    } else {
        QMessageBox::warning(this, "Warning", "Please select a note to delete.");
    }
}


void NoteList::on_pushButton_4_clicked()
{
    int row = ui->tableWidget->currentRow();
    QString filePath = ui->tableWidget->item(row,1)->text();
    emit openNote(filePath);
    close();
}


void NoteList::on_tableWidget_cellDoubleClicked(int row, int column)
{
    QString filePath = ui->tableWidget->item(row,1)->text();
    emit openNote(filePath);
    close();

}

