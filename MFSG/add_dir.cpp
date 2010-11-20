#include "add_dir.h"
#include "ui_add_dir.h"

ADD_DIR::ADD_DIR(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ADD_DIR)
{
    ui->setupUi(this);
    ui->le_clocation->setText("/home/camilo/Documents/OAR/PROYECTO_1_BUMBLEBEE/");

}

ADD_DIR::~ADD_DIR()
{
    delete ui;
}

void ADD_DIR::on_buttonBox_accepted()
{
    this->clocation=this->ui->le_clocation->text();

}
