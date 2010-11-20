#include "discog.h"

DiscoG::DiscoG(QWidget *parent) :
    QWidget(parent)
{
}

void DiscoG::setrefencia(int block,int FS,int TS){
    this->block=block;
    this->FS=FS;
    this->TS=TS;

}

void DiscoG::paintEvent(QPaintEvent *event){

    QPainter painter( this ); // Inicializar el Painter
    painter.setPen( Qt::black ); // La pluma en como vamos a dibujar es negra
    painter.setBrush(Qt::white);
    painter.drawRect(0,0,this->width()-1,this->height()-1);
    for(int i=0;i<17;i++){
        if(i==0){
            painter.setBrush(Qt::cyan);
            painter.drawRect(0,i*(500/16),this->width(),(500/16));

        }
        if(i==1){
            painter.setBrush(Qt::magenta);
            painter.drawRect(0,i*(500/16),this->width(),(500/16));
            int tam_m=(this->block/8)/1024;
            int wi=this->width()/tam_m;
            for(int m=0;m<tam_m;m++){
                painter.drawLine(wi*(m+1),(500/16)*i,wi*(m+1),(500/16)*(i+1));
            }
        }
        if(i==2){
            painter.setBrush(Qt::darkYellow);
            painter.drawRect(0,i*(500/16),this->width(),(500/16));
        }
        if(i==3){
            painter.setBrush(Qt::darkCyan);
            painter.drawRect(0,i*(500/16),this->width(),(500/16));
        }
        if(i>=4){
            painter.setBrush(Qt::darkMagenta);
            painter.drawRect(0,i*(500/16),this->width(),(500/16));
        }
        //painter.drawLine(0,(509/25)*(i+1),this->width(),(509/25)*(i+1));


    }

}
