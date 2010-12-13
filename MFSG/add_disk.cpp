#include "add_disk.h"
#include "ui_add_disk.h"

add_disk::add_disk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_disk)
{
    ui->setupUi(this);
    this->ui->le_dname->setText("disco.dat");
    this->ui->le_size->setText("200");
    this->ui->le_te->setText("7");
    this->ui->le_fe->setText("32");
    this->ok=0;
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
    this->ok=1;
}

void add_disk::on_buttonBox_rejected()
{
  this->ok=0;
}
