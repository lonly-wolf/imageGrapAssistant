#include "mylabel.h"
#include<QDebug>
#include<QMouseEvent>
#include<QPainter>

myLabel::myLabel(QLabel *parent) : QLabel(parent)
{
nowMousePoint.setX(15);
nowMousePoint.setY(233);
RTPPoint.setX(15);
RTPPoint.setY(233 );
}
 myLabel::~myLabel()
{

}

void myLabel::mouseMoveEvent(QMouseEvent *event)
{
    QMouseEvent *mouseEvent=static_cast<QMouseEvent *>(event);
     nowMousePoint=mouseEvent->pos();
    if(mouseEvent->pos().x()<=15){
        nowMousePoint.setX(15);
    }
    else if(mouseEvent->pos().x()>=225){
        nowMousePoint.setX(225);

    }
    if(mouseEvent->pos().y()<=30){
        nowMousePoint.setY(30);

    }
    else if(mouseEvent->pos().y()>=230){
        nowMousePoint.setY(230);

    }
    if((mouseEvent->pos().x()-15)/2.08>=0 && (mouseEvent->pos().x()-15)/2.08<=100){
        relativeMousePoint.setX((mouseEvent->pos().x()-15)/2.08);

    }
    if(115-mouseEvent->pos().y()/2>=0 && 115-mouseEvent->pos().y()/2<=100){
        relativeMousePoint.setY(115-mouseEvent->pos().y()/2);

    }
    qDebug("relative mouse position : %d,%d",relativeMousePoint.x(),relativeMousePoint.y());
   // qDebug("real mouse position : %d,%d",mouseEvent->pos().x(), mouseEvent->pos().y() );

    update();

}

void myLabel::paintEvent(QPaintEvent *event)
{
    QPainter pointPainter;

    pointPainter.begin(this);
    QPen pen(Qt::red);
    QBrush brush1(Qt::blue,Qt::SolidPattern);  //用指定的颜色和模式绘制图形
    pen.setBrush(brush1);
    pen.setWidth(10);
    pen.setCapStyle(Qt::RoundCap); //设置线的末端的风格 共三种
    pen.setJoinStyle(Qt::RoundJoin);//设置坐标与连接线之间的风格
    pointPainter.setPen(pen);
    pointPainter.drawImage(0,0,QImage(":x-y.png"));
   // pointPainter.drawPoint(nowMousePoint);  //跟随鼠标实时移动
    pointPainter.drawPoint(RTPPoint);

    pointPainter.end();
    qDebug("paintEvent");


}

QPoint myLabel::getCurrentPoint()
{
    return nowMousePoint;
}
QPoint myLabel::getrelativePoint()
{
    return relativeMousePoint;
}
 //设置当前位置坐标
void myLabel::setCurrentPoint(QPoint *point)
{
    RTPPoint=*point;
    //需要在此添加缩放级别
    RTPPoint.setX(point->x()*2.08+15);
    RTPPoint.setY((115-point->y())*2);
    update();

}

