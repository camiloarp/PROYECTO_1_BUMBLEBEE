#include "discog.h"

DiscoG::DiscoG(QWidget *parent) :
    QWidget(parent)
{
}

void DiscoG::setrefencia(int block,int FS,int TS,QString mapa,int page){
    this->block=block;
    this->FS=FS;
    this->TS=TS;
    this->mapa=mapa;
    this->page=page;

}

void DiscoG::paintEvent(QPaintEvent *event){

    QPainter painter( this ); // Inicializar el Painter
    painter.setPen( Qt::black ); // La pluma en como vamos a dibujar es negra
    painter.setBrush(Qt::white);
    painter.drawRect(0,0,this->width()-1,this->height()-1);

    for(int i=0;i<100;i++){
        if(this->mapa.size()>0){
            if(this->page==0){
                if(i==0){
                    painter.setBrush(Qt::magenta);
                }
                int poraqui=1+(this->block/8)/1024;
                if(i<poraqui&&i!=0){
                    painter.setBrush(Qt::yellow);
                }
                int por1 = poraqui + this->FS;
                if(i<por1&&i>poraqui){
                    painter.setBrush(Qt::red);
                }
                int por2 = por1+this->TS;
                if(i<por2&&i>por1){
                    painter.setBrush(Qt::darkMagenta);
                }
                if(i>por2){
                    if(this->mapa.at(i+(this->page*100))=='1'){
                         painter.setBrush(Qt::cyan);
                            }else{
                                 painter.setBrush(Qt::green);
                            }
                }
            }else{
                 if(this->mapa.at(i+(this->page*100))=='1'){
                      painter.setBrush(Qt::cyan);
                         }else{
                              painter.setBrush(Qt::green);
                         }
                 }
            }
        painter.drawRect(i*7,0,7,this->height()-1);
    }
    /*int w = (100/this->block)+1;
    int header = (w * this->width())/100;

   painter.drawRect(0,0,2,this->height()-1);
   painter.setBrush(Qt::magenta);
    int tam_m=((this->block*1024)/8)/1024;
    int ww = ((tam_m*100)/this->block)+1;
    int mb= (ww*this->width())/100;
    painter.drawRect(header+1,0,mb,this->height()-1);
    painter.setBrush(Qt::yellow);

    int www = ((this->FS*100)/this->block)+1;
    int fsb= (www*this->width())/100;
    painter.drawRect(header+mb+1,0,fsb,this->height()-1);

    painter.setBrush(Qt::blue);
   int q = ((this->TS*100)/this->block)+1;
    int tsb= (q*this->width())/100;
    painter.drawRect(header+mb+fsb+1,0,tsb,this->height()-1);
    painter.setBrush(Qt::green);
    painter.drawRect(header+mb+fsb+tsb+1,0,this->width()-(header+mb+fsb+tsb+2),this->height()-1);*/


}
