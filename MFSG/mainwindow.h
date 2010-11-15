#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include "add_dir.h"
#include "add_disk.h"
#include "add_song.h"

namespace Ui {
    class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QProcess ps;
    QString output;
    ADD_DIR r;
    add_disk d;
    add_song s;

private slots:
    void on_actionADD_SONG_triggered();
    void on_actionNEW_DISK_triggered();
    void on_actionADD_DIR_triggered();
    void on_pushButton_exec_clicked();
    void on_cmdExec_finish(int a);
    void on_cmdExec_data_available();
};

#endif // MAINWINDOW_H
