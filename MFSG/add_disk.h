#ifndef ADD_DISK_H
#define ADD_DISK_H

#include <QDialog>
#include <QString>
namespace Ui {
    class add_disk;
}

class add_disk : public QDialog
{
    Q_OBJECT

public:
    explicit add_disk(QWidget *parent = 0);
    ~add_disk();
    QString dname;
    QString size;
    QString fe;
    QString te;
    int ok;
private:
    Ui::add_disk *ui;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // ADD_DISK_H
