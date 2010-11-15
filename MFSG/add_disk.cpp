#include "add_disk.h"
#include "ui_add_disk.h"

add_disk::add_disk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_disk)
{
    ui->setupUi(this);

}

add_disk::~add_disk()
{
    delete ui;
}

void add_disk::on_buttonBox_accepted()
{
    this->dname=this->ui->le_dname->text();
    this->size=this->ui->le_size->text();
    this->te=this->ui->le_te->text();
    this->fe=this->ui->le_fe->text();
}
