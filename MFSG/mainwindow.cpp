#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);



    this->music.setCurrent("/home");
    //////// connect signals
    ps.setProcessChannelMode(QProcess::MergedChannels);
    connect(&ps, SIGNAL(readyRead()), this, SLOT(on_cmdExec_data_available()));
    connect(&ps, SIGNAL(finished(int)), this, SLOT(on_cmdExec_finish(int)));
    ////////////////////////
    this->mapabits=0;
    this->mapa="";
    this->pause=0;
    this->page=0;
    this->ui->left->setDisabled(true);
    this->ui->right->setDisabled(true);
    this->ui->STOP->setDisabled(true);
    this->ui->pause->setDisabled(true);
    this->ui->pushButton_exec->setDisabled(true);
    this->ui->tagsong->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_exec_clicked() {
    this->mapabits=0;
    this->ui->pause->setDisabled(false);
    this->ui->STOP->setDisabled(false);
    output = "";
    QString location;
    location=this->r.clocation;
    location.append("mfs.bin");
    ps.setWorkingDirectory(this->r.clocation);
    QStringList argu;
    argu.append("-play");
    argu.append(this->ui->songs->selectedItems().at(0)->text());
    argu.append(this->d.dname);


    ps.start(location,argu);
    this->ui->pushButton_exec->setDisabled(true);
}

void MainWindow::on_cmdExec_finish(int a) {

    this->ui->pushButton_exec->setDisabled(false);
    QString temp;
    temp.setNum(a);
    if(this->mapabits==0){
    ui->textEdit_output->append(output);
    }else{
    this->mapabits=0;
}


}

void MainWindow::on_cmdExec_data_available() {

    output += ps.readAll();

}

void MainWindow::on_actionADD_DIR_triggered()
{
r.exec();
}

void MainWindow::on_actionNEW_DISK_triggered()
{

   d.exec();
   ps.setWorkingDirectory(this->r.clocation);
   if(d.ok==1){
   output = "";

   bool ok;
   int size = d.size.toInt(&ok,10);
   int blockamount = size*1024;
   QString tosize;
   tosize.setNum(blockamount,10);

   QStringList argu;
   argu.append("-c");
   argu.append(tosize);
   argu.append(d.fe);
   argu.append(d.te);
   argu.append(d.dname);

   QString location;
   location=this->r.clocation;
   location.append("mfs.bin");


    ps.start(location,argu);
    ps.waitForFinished();
    QString pg;
    pg.setNum(this->page,10);
    this->ui->pglabel->setText(pg);
    this->mapadebits();
    this->ui->right->setDisabled(false);
    this->ui->pushButton_exec->setDisabled(true);
    }

}

void MainWindow::on_actionADD_SONG_triggered()
{

   s.exec();
   this->ui->tags->clear();
   ps.setWorkingDirectory(this->r.clocation);
   output = "";
   QString location;
   location=this->r.clocation;
   location.append("mfs.bin");
    ui->textEdit_output->append(s.filename);
   QStringList argu;
   argu.append("-a");
   argu.append(s.filename);
   argu.append("archivo");
   argu.append(this->d.dname);
   ps.start(location,argu);
   ps.waitForFinished();
   //this->mapadebits();
   this->printtags();
   this->ui->pushButton_exec->setDisabled(false);
   this->ui->tagsong->setDisabled(false);



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
    this->ui->pushButton_exec->setDisabled(false);
    this->ui->STOP->setDisabled(true);
    this->ui->pause->setDisabled(true);

}

void MainWindow::on_pause_clicked()
{

    if(this->pause==0){

        this->pause=1;
        ps.kill();
        this->ui->pushButton_exec->setDisabled(true);
        this->ui->STOP->setDisabled(true);
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
        this->ui->pushButton_exec->setDisabled(false);
        this->ui->STOP->setDisabled(false);
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





void MainWindow::printtags(){


    output = "";

    QString location;
    location=this->r.clocation;
    location.append("mfs.bin");
    ps.setWorkingDirectory(this->r.clocation);

    QStringList argu;
    argu.append("-l");
    argu.append(this->d.dname);


    ps.start(location, argu);
    ps.waitForFinished();
    QString tag="";
    for(int i=0;i<this->output.size();i++){
        if(this->output.at(i)!=';'){
          tag.append(this->output.at(i));
      }else{
         this->ui->tags->addItem(tag);
         tag="";
      }
    }

}



void MainWindow::on_tags_itemClicked(QListWidgetItem* item)
{

 this->ui->songs->clear();
 this->filename.clear();
 QString tag = item->text();
 output = "";

 QString location;
 location=this->r.clocation;
 location.append("mfs.bin");
 ps.setWorkingDirectory(this->r.clocation);

 QStringList argu;
 argu.append("-s");
 argu.append(tag);
 argu.append(this->d.dname);

 ps.start(location,argu);
 ps.waitForFinished();
 QString inodos = this->output;
 QString ind="";
 for(int i=0;i<inodos.size();i++){
     if(inodos.at(i)!=';'){
       ind.append(inodos.at(i));
   }else{
      this->getinofof(ind);
      ind="";
   }
 }


}
void MainWindow::getinofof(QString inodo){
    output = "";

    QString location;
    location=this->r.clocation;
    location.append("mfs.bin");
    ps.setWorkingDirectory(this->r.clocation);


    QStringList argu;
    argu.append("-qi");
    argu.append(inodo);
    argu.append(this->d.dname);


    ps.start(location,argu);
    ps.waitForFinished();
    QString info=this->output;
    QString cancion="";
    int here=0;

    for(int i=0;i<info.size();i++){
        if(info.at(i)!=';'){
          cancion.append(info.at(i));
      }else{
          here++;
          if(here==1){

              this->ui->songs->addItem(cancion);
        }

          cancion="";
      }
    }

}

void MainWindow::on_tagsong_clicked()
{
    this->ui->tags->clear();
    output = "";

    QString location;
    location=this->r.clocation;
    location.append("mfs.bin");
    ps.setWorkingDirectory(this->r.clocation);


    QStringList argu;
    argu.append("-t");
    argu.append(this->ui->totag->text());
    argu.append(this->ui->songs->selectedItems().at(0)->text());
    argu.append(this->d.dname);



    ps.start(location,argu);
    ps.waitForFinished();
    this->printtags();


}



void MainWindow::on_right_clicked()
{
    bool ok;
    int cantm = this->d.size.toInt(&ok,10);
    int cantb = cantm*1024;
    int max = cantb/100;
    this->page++;
    QString pg;
    pg.setNum(this->page,10);
    this->ui->pglabel->setText(pg);
    this->ui->left->setDisabled(false);
    this->mapadebits();
    if(this->page==max){
        this->ui->right->setDisabled(true);
    }

}

void MainWindow::mapadebits(){
    this->mapabits=1;
    output = "";
    QString location;

    location=this->r.clocation;
    location.append("mfs.bin");
    ps.setWorkingDirectory(this->r.clocation);

    QStringList argu;
    argu.append("-ma");
    argu.append(this->d.dname);


    ps.start(location, argu);
    ps.waitForFinished();
    QString oo;
    oo.setNum(this->page*100,10);
    QString pp;
    pp.setNum((this->page*100)+99,10);
    this->ui->begin->setText(oo);
    this->ui->end->setText(pp);
    this->discovir = new DiscoG();
    bool ok;
    int cantm = this->d.size.toInt(&ok,10);
    int cantb = cantm*1024;
    int fe = this->d.fe.toInt(&ok,10);
    int te = this->d.te.toInt(&ok,10);
    this->discovir->setrefencia(cantb,fe,te,output,this->page);
    this->ui->gridLayout->addWidget(this->discovir,0,0,1,1);
    this->update();
}

void MainWindow::on_left_clicked()
{this->page--;
 QString pg;
 pg.setNum(this->page,10);
 this->ui->pglabel->setText(pg);
 this->ui->right->setDisabled(false);
 this->mapadebits();
 if(this->page==0){
     this->ui->left->setDisabled(true);
 }
}

void MainWindow::on_actionInstall_mpg123_triggered()
{

    output = "";
    QString comando;
    QString location;
    location=this->r.clocation;
    location.append("mfs.bin");
    ps.setWorkingDirectory(this->r.clocation);
    comando="-install";
    ps.start(location,comando.split(" "));


}



void MainWindow::on_borrar_clicked()
{

    output = "";

    QString location;
    location=this->r.clocation;
    location.append("mfs.bin");
    ps.setWorkingDirectory(this->r.clocation);


    QStringList argu;
    argu.append("-d");
    argu.append(this->ui->songs->selectedItems().at(0)->text());
    argu.append(this->d.dname);

    ps.start(location,argu);
    ps.waitForFinished();
    this->ui->tags->clear();
    this->printtags();

}
