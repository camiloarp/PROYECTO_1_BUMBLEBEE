#ifndef ADD_DIR_H
#define ADD_DIR_H

#include <QDialog>
#include <QString>

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
    QString dlocation;
    QString mlocation;

private:
    Ui::ADD_DIR *ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // ADD_DIR_H
