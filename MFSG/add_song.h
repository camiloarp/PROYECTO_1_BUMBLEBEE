#ifndef ADD_SONG_H
#define ADD_SONG_H

#include <QDialog>
#include <QString>
#include <QFileDialog>
#include <QDir>

namespace Ui {
    class add_song;
}

class add_song : public QDialog
{
    Q_OBJECT

public:
    explicit add_song(QWidget *parent = 0);
    ~add_song();
    QString filename;
    QString disk;
    QDir download;


private:
    Ui::add_song *ui;

private slots:
    void on_Browse_clicked();
    void on_buttonBox_accepted();
};

#endif // ADD_SONG_H
