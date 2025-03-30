#ifndef NOTELIST_H
#define NOTELIST_H

#include <QMainWindow>
#include <QCloseEvent>

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
    void on_pushButton_clicked();

    void on_actionInfo_triggered();

    void loadNotes();

    void closeEvent(QCloseEvent *event);

    void saveNotes();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

signals:
    void openNote(const QString &filePath);


private:
    Ui::NoteList *ui;
};

#endif // NOTELIST_H
