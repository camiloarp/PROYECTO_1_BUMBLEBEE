#ifndef DISCOG_H
#define DISCOG_H

#include <QWidget>
#include <QPainter>

class DiscoG : public QWidget
{
    Q_OBJECT
public:
    DiscoG(QWidget *parent = 0);
    void setrefencia(int,int,int,QString,int);

private:
    int block,TS,FS,page;
    QString mapa;
signals:

public slots:

protected:
      void paintEvent(QPaintEvent *event);

};

#endif // DISCOG_H
