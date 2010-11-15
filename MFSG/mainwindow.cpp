#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //////// connect signals
    ps.setProcessChannelMode(QProcess::MergedChannels);
    connect(&ps, SIGNAL(readyRead()), this, SLOT(on_cmdExec_data_available()));
    connect(&ps, SIGNAL(finished(int)), this, SLOT(on_cmdExec_finish(int)));
    ////////////////////////
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_exec_clicked() {

    output = "";
    QString comando;
    comando="-c 204800 7 32 ";
    comando.append(this->r.dlocation);
    ps.start(this->r.clocation, comando.split(" "));
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
   output = "";
   QString comando;
   comando="-c ";
   comando.append(d.size);
   comando.append(" ");
   comando.append(d.fe);
   comando.append(" ");
   comando.append(d.te);
   comando.append(" ");
   comando.append(this->r.dlocation);
   comando.append(d.dname);
   ps.start(this->r.clocation, comando.split(" "));


}

void MainWindow::on_actionADD_SONG_triggered()
{
   s.exec();
   output = "";
   QString comando;
   comando="-a ";
   comando.append(s.filename);
   comando.append(" ");
   comando.append(this->r.dlocation);
   comando.append(this->s.disk);
   ps.start(this->r.clocation, comando.split(" "));

}
