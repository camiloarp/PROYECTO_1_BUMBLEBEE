#include "add_song.h"
#include "ui_add_song.h"

add_song::add_song(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_song)
{
    ui->setupUi(this);
}

add_song::~add_song()
{
    delete ui;
}

void add_song::on_buttonBox_accepted()
{
    this->filename=this->ui->le_file->text();

}

void add_song::on_Browse_clicked()
{
    QString directory = QFileDialog::getOpenFileName(this,
                               tr("Find Files"), QDir::currentPath(),"*.mp3");
    this->ui->le_file->setText(directory);

}
