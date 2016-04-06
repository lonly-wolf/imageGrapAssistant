#include "myPen.h"
#include<QLabel>
#include<QPixmap>
#include<QMouseEvent>
#include<QPainter>
#include<QPointF>
#include<QDebug>

myPen::myPen(QLabel *parent) : QLabel(parent)
{
    myPainter=new QPainter;
    linePainter=new QPainter;
    showPicPainter=new QPainter;
    isClicking=false;
    isPen=false;
    isLoadPic=false;
}
myPen::~myPen()
{
    delete myPainter;
    delete linePainter;
    delete showPicPainter;
}
void myPen::mousePressEvent(QMouseEvent *event)
{
    QMouseEvent *mouseEvent=static_cast<QMouseEvent *>(event);
    QLabel::mousePressEvent(event);
    beginPoint.setX(mouseEvent->pos().x());
    beginPoint.setY(mouseEvent->pos().y());
    if(this->isPen){
        beginPointer.setX(mouseEvent->pos().x());
        beginPointer.setY(mouseEvent->pos().y());
    }
}

void myPen::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<"mouse is moving-----------------------";

    mouseEvent2=static_cast<QMouseEvent *>(event);
    this->currentPoint.setX(mouseEvent2->pos().x());
    this->currentPoint.setY(mouseEvent2->pos().y());
    if(!this->isPen){
        this->setGeometry(mouseEvent2->pos().x()-beginPoint.x()+this->pos().x(),mouseEvent2->pos().y()-beginPoint.y()+this->pos().y(),this->width(),this->height());
    }
    else if(this->isPen){
            straightLine[0]=QPointF(beginPointer.x(),beginPointer.y());
            straightLine[1]=QPointF(currentPoint.x(),currentPoint.y());
            pointArray.append(QPointF(beginPointer.x(),beginPointer.y()));
            pointArray.append(QPointF(currentPoint.x(),currentPoint.y()));
            linePainter->begin(&myImage);
            QPen pen(Qt::blue);
            QBrush brush1(Qt::green,Qt::Dense3Pattern);  //用指定的颜色和模式绘制图形
            pen.setBrush(brush1);
            pen.setWidth(2);
            pen.setCapStyle(Qt::RoundCap); //设置线的末端的风格 共三种
            pen.setJoinStyle(Qt::RoundJoin);//设置坐标与连接线之间的风格
            linePainter->setPen(pen);
            QPainterPath path;
            path.moveTo(beginPointer.x(),beginPointer.y());
            path.lineTo(currentPoint.x(),currentPoint.y());
            linePainter->drawPath(path);
            linePainter->end();
            beginPointer.setX( mouseEvent2->x());
            beginPointer.setY(mouseEvent2->y());
            update();
    }
}

void myPen::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->isPen){
        pointArray.append(QPointF(beginPointer.x(),beginPointer.y()));
        pointArray.append(QPointF(currentPoint.x(),currentPoint.y()));

    }
}

//重写label方法
void myPen::paintEvent(QPaintEvent *event)
{
    showPicPainter->begin(this);
    showPicPainter->drawImage(0,0,myImage);
    showPicPainter->end();
}

//获取图片路径
 void  myPen::setImage(QString name)
 {
     this->imageName=name;
     myImage.load(imageName);
     isLoadPic=true;
     update();
 }
 //返回画笔编辑的图片
 QImage myPen::getMyImage()
{
     return myImage;
}
//设置图片
 void myPen::setImageByImage(QImage *image)
 {
     myImage=*image;
     update();
 }
 //设置图片高度
 void myPen::setImageWidth(int width)
 {
     myImage.size().setWidth(width);
 }
//设置图片宽度
 void myPen::setImageHeight(int height)
 {
     myImage.size().setHeight(height);

 }
//设置旋转角度
 void myPen::setImageRotate(QMatrix matrix)
{
     myMatrix=matrix;
     QMatrix matrix2;
     matrix2.rotate(90);
     myImage.transformed(matrix2);
     showPicPainter->begin(this);
     showPicPainter->translate(50,50);
     showPicPainter->rotate(90);
     showPicPainter->drawImage(0,0,myImage);
     update();
}


