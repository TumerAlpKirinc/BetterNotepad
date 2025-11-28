#ifndef NOTELIST_H
#define NOTELIST_H


#include <QMainWindow>
#include <QCloseEvent>
#include <QtQuickWidgets/QtQuickWidgets>


namespace Ui {
class NoteList;
}

class NoteList : public QMainWindow
{
    Q_OBJECT

public:
    explicit NoteList(QWidget *parent = nullptr);
    ~NoteList();



private slots:

    void on_actionInfo_triggered();

    void loadNotes();

    void handleCardClick(const QString &filePath);

    void newNoteCardClicked();

    void deleteNoteCardClicked(const QString &filePath);

    void changeCardColor(const QString &filePath);

    void rightClicked(const QString &filePath);

signals:
    void openNote(const QString &filePath);



private:
    Ui::NoteList *ui;
    QWidget *contentWidget;
    QGridLayout *grid;
};

#endif // NOTELIST_H
