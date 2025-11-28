#include "notelist.h"
#include "ui_notelist.h"
#include "mainwindow.h"

#include <QCheckBox>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QtQuickWidgets/QtQuickWidgets>
NoteList::NoteList(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NoteList)
{
    ui->setupUi(this);

    MainWindow *mainWindow = new MainWindow();
    connect(this,&NoteList::openNote,mainWindow,&MainWindow::openNote);


    contentWidget = ui->scrollAreaWidgetContents;
    grid = new QGridLayout(contentWidget);
    grid->setSpacing(15);
    grid->setContentsMargins(10, 10, 10, 10);

    contentWidget->setLayout(grid);


    ui->scrollArea->setWidgetResizable(true);



    // Notları yükle
    loadNotes();

}



NoteList::~NoteList()
{
    delete ui;
}







void NoteList::on_actionInfo_triggered()
{

}

void NoteList::loadNotes() {

    QLayoutItem *child;
    while ((child = grid->takeAt(0)) != 0) {
        if (child->widget()) {
            child->widget()->hide();

            child->widget()->deleteLater();
        }
        delete child; // Layout item'ı sil
    }

    QFile file("notes_list.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not open notes_list.txt";

    }

    QTextStream in(&file);
    int index = 0;

    // --- MEVCUT NOTLARI YÜKLE ---
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() < 2) continue;

        QString noteName = parts[0].trimmed();
        QString filePath = parts[1].trimmed();
        QString cardColor = (parts.size() > 2) ? parts[2].trimmed() : "#FFFB7D";

        QQuickWidget *qmlView = new QQuickWidget(contentWidget);
        qmlView->setResizeMode(QQuickWidget::SizeRootObjectToView);
        qmlView->setSource(QUrl("qrc:/qml/qml/CardView.qml"));


        qmlView->setAttribute(Qt::WA_AlwaysStackOnTop);
        qmlView->setClearColor(Qt::transparent);

        QObject *rootObj = qmlView->rootObject();
        rootObj->setProperty("titleText", noteName);
        rootObj->setProperty("filePath", filePath);
        rootObj->setProperty("cardColor", cardColor);


        connect(rootObj, SIGNAL(cardClicked(QString)), this, SLOT(handleCardClick(QString)));
        connect(rootObj, SIGNAL(deleteClicked(QString)), this, SLOT(deleteNoteCardClicked(QString)));
        connect(rootObj, SIGNAL(colorClicked(QString)), this, SLOT(changeCardColor(QString)));
        connect(rootObj, SIGNAL(rightClicked(QString)),this, SLOT(rightClicked(QString)));

        int row = index / 5;
        int col = index % 5;
        grid->addWidget(qmlView, row, col, Qt::AlignTop | Qt::AlignLeft);

        index++;
    }
    file.close();


    QQuickWidget *addQmlView = new QQuickWidget(contentWidget);
    addQmlView->setResizeMode(QQuickWidget::SizeRootObjectToView);
    addQmlView->setSource(QUrl("qrc:/qml/qml/NewNoteCard.qml"));


    addQmlView->setAttribute(Qt::WA_AlwaysStackOnTop);
    addQmlView->setClearColor(Qt::transparent);

    QObject *addRootObj = addQmlView->rootObject();
    if (addRootObj) {
        connect(addRootObj, SIGNAL(createClicked()), this, SLOT(newNoteCardClicked()));
    }

    int row = index / 5;
    int col = index % 5;
    grid->addWidget(addQmlView, row, col, Qt::AlignTop | Qt::AlignLeft);


}




void NoteList::handleCardClick(const QString &filePath)
{
    qDebug() << "QML Kart Tıklaması Yakalandı. Dosya Yolu:" << filePath;


    emit openNote(filePath);


    this->close();
}

void NoteList::newNoteCardClicked()
{

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Add Note");
    msgBox.setText("How would you like to add a note?");
    msgBox.setIcon(QMessageBox::Question);

    QAbstractButton *btnNew = msgBox.addButton("Create New", QMessageBox::YesRole);
    QAbstractButton *btnExisting = msgBox.addButton("Add Existing", QMessageBox::NoRole);
    msgBox.addButton("Cancel", QMessageBox::RejectRole);

    msgBox.exec();

    QString filePath;
    bool createNewFile = false;

    if (msgBox.clickedButton() == btnNew) {

        filePath = QFileDialog::getSaveFileName(this, "Create New Note", "", "BetterNotepad Documents (*.bnpd);;All Files (*)");
        createNewFile = true;
    }
    else if (msgBox.clickedButton() == btnExisting) {

        filePath = QFileDialog::getOpenFileName(this, "Select Existing Note", "", "BetterNotepad Documents (*.bnpd);;All Files (*)");
        createNewFile = false;
    }
    else {
        return;
    }

    if (filePath.isEmpty()) return;


    if (createNewFile && !filePath.endsWith(".bnpd")) {
        filePath += ".bnpd";
    }

    QFile checkFile("notes_list.txt");
    if (checkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&checkFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");

            // Formatımız: NotAdi,DosyaYolu
            // parts[1] dosya yolunu tutar.
            if (parts.size() >= 2) {
                QString existingPath = parts[1].trimmed();
                if (existingPath == filePath) {
                    QMessageBox::warning(this, "Warning", "This file is already in your list!");
                    checkFile.close();
                    return; // Fonksiyondan çık, işlem yapma.
                }
            }
        }
        checkFile.close();
    }

    if (createNewFile) {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "The note file could not be created!");
            return;
        }
        file.close();
    }


    QFileInfo info(filePath);
    QString noteName = info.baseName();

    QFile listFile("notes_list.txt");
    if (listFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&listFile);
        out << noteName << "," << filePath << "\n";
        listFile.close();
    }

    emit openNote(filePath);


    this->close();
}

void NoteList::deleteNoteCardClicked(const QString &filePath)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Delete Note");
    msgBox.setText("Are you sure you want to remove this note?");
    msgBox.setInformativeText("If you check the box below, the file will be moved to the trash.");
    msgBox.setIcon(QMessageBox::Question);

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    QCheckBox *cbDeleteFile = new QCheckBox("Delete file from computer (Move to Trash)", &msgBox);
    msgBox.setCheckBox(cbDeleteFile);

    if (msgBox.exec() != QMessageBox::Yes) {
        return;
    }

    // A) Bilgisayardan Silme (Trash)
    if (cbDeleteFile->isChecked()) {
        if (QFile::exists(filePath)) {
            bool success = QFile::moveToTrash(filePath);
            if (!success) {
                QMessageBox::warning(this, "Error", "Could not move note to trash.");
            }
        }
    }

    // B) Listeden Silme
    QList<QString> linesToKeep;
    QFile listFile("notes_list.txt");

    if (listFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&listFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() >= 2) {
                QString currentPath = parts[1].trimmed();
                if (currentPath != filePath) {
                    linesToKeep.append(line);
                }
            }
        }
        listFile.close();
    }

    if (listFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&listFile);
        for (const QString &line : linesToKeep) {
            out << line << "\n";
        }
        listFile.close();
    }

    loadNotes();
}
void NoteList::changeCardColor(const QString &filePath) {
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose a color");
    if (!color.isValid()) {
        return;
    }


    QString newColorHex = color.name();


    QList<QString> linesToKeep;
    QFile listFile("notes_list.txt");

    if (listFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&listFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");

            if (parts.size() >= 2) {
                QString currentPath = parts[1].trimmed();
                if (currentPath == filePath) {

                    QString noteName = parts[0].trimmed();

                    line = noteName + "," + filePath + "," + newColorHex;
                }
            }
            linesToKeep.append(line);
        }
        listFile.close();
    }


    if (listFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&listFile);
        for (const QString &line : linesToKeep) {
            out << line << "\n";
        }
        listFile.close();
    }


    loadNotes();
}


void NoteList::rightClicked(const QString &filePath)
{

    QMenu contextMenu(this);


    QAction *openLocationAction = contextMenu.addAction("Open File Location");




    QAction *selectedAction = contextMenu.exec(QCursor::pos());

    if (selectedAction == openLocationAction) {

        QFileInfo fileInfo(filePath);
        QString directoryPath = fileInfo.absoluteDir().path();
        QUrl directoryUrl = QUrl::fromLocalFile(directoryPath);

        if (!QDesktopServices::openUrl(directoryUrl)) {
            QMessageBox::warning(this, "Hata", tr("Dosya konumu açılamadı: %1").arg(directoryPath));
        }
    }
}



