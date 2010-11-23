#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->discovir = new DiscoG();
    this->discovir->setrefencia(204800,0,0);
    this->ui->gridLayout->addWidget(this->discovir,0,0,1,1);
    //////// connect signals
    ps.setProcessChannelMode(QProcess::MergedChannels);
    connect(&ps, SIGNAL(readyRead()), this, SLOT(on_cmdExec_data_available()));
    connect(&ps, SIGNAL(finished(int)), this, SLOT(on_cmdExec_finish(int)));
    this->pause=0;
    ////////////////////////
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_exec_clicked() {

    output = "";
    QString comando;
    QString location;
    location=this->r.clocation;
    location.append("mfs.bin");
    ps.setWorkingDirectory(this->r.clocation);
    comando="-play cancion.mp3 disco.dat";
    ps.start(location, comando.split(" "));
    this->ui->pushButton_exec->setDisabled(true);
}

void MainWindow::on_cmdExec_finish(int a) {

    QString temp;
    temp.setNum(a); // Rogger: Solo para ver el exit Code
    ui->textEdit_output->append(output + "Fin:"+temp);

    ui->pushButton_exec->setEnabled(true);
}

void MainWindow::on_cmdExec_data_available() {
    output += ps.readAll() + "\n";
}

void MainWindow::on_actionADD_DIR_triggered()
{
r.exec();
}

void MainWindow::on_actionNEW_DISK_triggered()
{
   d.exec();
   ps.setWorkingDirectory(this->r.clocation);
   output = "";
   QString comando;
   comando="-c ";
   comando.append(d.size);
   comando.append(" ");
   comando.append(d.fe);
   comando.append(" ");
   comando.append(d.te);
   comando.append(" ");
   comando.append(d.dname);
   QString location;
   location=this->r.clocation;
   location.append("mfs.bin");
   ps.start(location, comando.split(" "));


}

void MainWindow::on_actionADD_SONG_triggered()
{
   s.exec();
   output = "";
   QString comando;
   comando="-a ";
   comando.append(s.filename);
   comando.append(" ");
   comando.append(this->s.disk);
   ps.start(this->r.clocation, comando.split(" "));


}

void MainWindow::on_STOP_clicked()
{
    ps.kill();
    output = "";
    QString comando;
    QString location;
    location=this->r.clocation;
    location.append("mfs.bin");
    ps.setWorkingDirectory(this->r.clocation);
    comando="-kill";
    ps.execute(location, comando.split(" "));

}

void MainWindow::on_pause_clicked()
{
    if(this->pause==0){
        this->pause=1;
        ps.kill();
        output = "";
        QString comando;
        QString location;
        location=this->r.clocation;
        location.append("mfs.bin");
        ps.setWorkingDirectory(this->r.clocation);
        comando="-pause";
        ps.execute(location, comando.split(" "));
    }else{
        this->pause=0;
        ps.kill();
        output = "";
        QString comando;
        QString location;
        location=this->r.clocation;
        location.append("mfs.bin");
        ps.setWorkingDirectory(this->r.clocation);
        comando="-conti";
        ps.execute(location, comando.split(" "));
    }

}
