#include "notelist.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NoteList noteList;
    MainWindow mainWindow;
    if (argc > 1) {
        QString filePath = QString::fromLocal8Bit(argv[1]);
        mainWindow.openNote(filePath);
    } else {
        noteList.show();
    }

    return a.exec();
}
