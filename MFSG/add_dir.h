#ifndef ADD_DIR_H
#define ADD_DIR_H

#include <QDialog>
#include <QString>
#include <QFileDialog>
#include <QDir>

namespace Ui {
    class ADD_DIR;
}

class ADD_DIR : public QDialog
{
    Q_OBJECT

public:
    explicit ADD_DIR(QWidget *parent = 0);
    ~ADD_DIR();
    QString clocation;

private:
    Ui::ADD_DIR *ui;
        QDir download;

private slots:
    void on_browse_clicked();
    void on_buttonBox_accepted();
};

#endif // ADD_DIR_H
