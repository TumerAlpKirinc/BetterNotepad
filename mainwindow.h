#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void openNote(const QString &filePath);

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionRedo_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionAbout_BetterNotepad_triggered();

    bool confirmDiscardChanges();

    void writeFile(const QString &filePath);

    void on_actionFont_triggered();

    void on_actionColor_triggered();

    void on_actionFind_triggered();

    void on_actionPrint_triggered();

    void on_actionReturn_to_Note_List_triggered();

    void setDarkMode(bool enabled);

    void applySavedTheme();

private:
    Ui::MainWindow *ui;
    QString currentFile;
};
#endif // MAINWINDOW_H
