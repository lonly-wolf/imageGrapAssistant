#include "picedit.h"
#include "ui_picedit.h"
#include<QPainter>
#include<QDebug>
#include<QLabel>
#include<QFileDialog>
#include<QMessageBox>
#include<QMouseEvent>
#include<QLinkedList>
#include"myPen.h"
#include<QGraphicsView>
#include<QGraphicsScene>


picEdit::picEdit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::picEdit)
{
    ui->setupUi(this);
    myImage2=new QImage;
    painter=new QPainter;
    drawLabel=new myPen;
    drawLabel->setStyleSheet("background-color:green");
    drawLabel->setAutoFillBackground(true);
    drawLabel->setAcceptDrops(true);
    drawLabel->setAlignment(Qt::AlignCenter);
    ui->scrollArea->setAlignment(Qt::AlignCenter);
    isPen=false;
    //设置scrollerArea的滚动条策略
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setWidget(drawLabel);
    picSize=1;
    picPosition=0;
    hasTrans=false;
   // lastPosition=ui->imageRotate->value();
    picSizeCount=0;
    picSliderCount=0;
  //  lastPoint.setX(labelWidget->pos().x());
   // lastPoint.setY(labelWidget->pos().y());
}

picEdit::~picEdit()
{
    delete ui;
    delete painter;
    delete  myImage2;
    delete drawLabel;
}

void picEdit::drawPaint()
{
    painter->begin(drawLabel);
    QPen pen(Qt::blue);
    QBrush brush1(Qt::blue,Qt::Dense3Pattern);  //用指定的颜色和模式绘制图形
    pen.setBrush(brush1);
    pen.setWidth(6);
    pen.setCapStyle(Qt::RoundCap); //设置线的末端的风格 共三种
    pen.setJoinStyle(Qt::RoundJoin);//设置坐标与连接线之间的风格
    painter->setPen(pen);
    painter->drawLine(10,50,100,40);
    painter->drawRect(120,50,80,80);
    QRect rect(230,50,80,80);
    painter->drawRoundedRect(rect,10,10); //绘制圆角矩形

    QPointF p1[3]={

         QPointF(120,140),
         QPointF(150,170),
         QPointF(120,190)
     };
    QPointF p2[3]={

         QPointF(180,140),
         QPointF(270,170),
         QPointF(180,320)
     };

    painter->drawPolyline(p1,3); //通过线绘制指针支点
    painter->drawPolygon(p2,3);//以指针支点绘制多边形

    painter->setFont(QFont("Times",20)); //设置字体
    QRect fontRect(10,150,220,180);

    painter->drawText(fontRect,Qt::AlignCenter,QStringLiteral("我的世界"));
    painter->drawRect(200,200,200,300);
    //设置渐变模式
    QRadialGradient gradient(100,100,50,120,120);
    gradient.setColorAt(0,Qt::black);
    gradient.setColorAt(1,Qt::red);
   // gradient.setSpread(QGradient::RepeatSpread);
    gradient.setSpread(QGradient::RepeatSpread);
    QBrush brush3(gradient);
    painter->setBrush(brush3);
    painter->drawRect(50,50,300,300);

    painter->end();

}
//转换图像
void picEdit::transformImage()
{   QPainter myPainter;
    QImage image(":525897.png");
    myPainter.begin(&image);
    QPen pen(Qt::blue,1,Qt::DashLine);
    myPainter.setPen(pen);
    myPainter.drawRect(50,50,100,100);
   QTransform transform;
   transform.translate(50,50); //改变坐标
   transform.rotate(45);       //旋转
   transform.scale(0.5,0.5);   //放大


   myPainter.setTransform(transform);
   myPainter.drawImage(0,0,image);
   myPainter.end();

   //ui->label->resize(QSize(image.width()/3,image.height()/3));
   //ui->label->setPixmap(QPixmap::fromImage(image));
   //在scrollArea添加label图片
   // painter->begin(&myImage);
   //transform.rotate(position);
   // painter->setTransform(transform);
   // painter->translate(centerX,centerY);
   //QRectF rect(ui->scrollArea->width()/2,ui->scrollArea->height()/2,myImage.width(),myImage.height());
   // labelWidget->clear();
   // labelWidget->show();
   // painter->drawImage(0,0,myImage);
   // labelWidget->resize(myImage.width(),myImage.height());
   //  painter->end();

}

void picEdit::paintEvent(QPaintEvent *event)
{

   // drawLabel->size().setWidth(ui->scrollArea->width());
  //  drawLabel->size().setHeight(ui->scrollArea->height());
}


//打开图片函数
void picEdit::on_actionOpen_triggered()
{
  imageName=QFileDialog::getOpenFileName(this,QStringLiteral("Please open a picture"),"","Images (*.png *.bmp *.jpg *.tif *.GIF)");
  if(!imageName.isEmpty()){
      if(!(myImage.load(imageName))){
          QMessageBox::warning(this,"Falied to open the picture","Fail to open the picture",QMessageBox::Ok);
          return;
      }
      else{
          initWidth=myImage.size().width();
          initHeight=myImage.size().height();
          drawLabel->setImage(imageName);
          drawLabel->resize(initWidth,initHeight);
          //labelWidget->setPixmap(QPixmap::fromImage(myImage));
        // drawLabel->setPixmap(QPixmap::fromImage(myImage));
          qDebug()<<"myImage loaded2";
         // labelWidget->setContentsMargins(ui->scrollArea->width()/2- myImage2->width()/2,ui->scrollArea->height()/2-myImage2->height()/2,ui->scrollArea->width()/2- myImage2->width()/2,ui->scrollArea->height()/2-myImage2->height()/2);
          qDebug()<<"myImage loaded3";

          ui->scrollArea->setWidget(drawLabel);
          return;
      }
  }
  else{
      QMessageBox::warning(this,"Falied to open the picture",QStringLiteral("no picture has been selected!"),QMessageBox::Ok);
      return;
  }
}

//旋转图片
void picEdit::on_imageRotate_sliderMoved(int position)
{
    picPosition=position;
    if(!myImage.isNull()){
        qDebug()<<"transformed-----------------";
        hasTrans=true;
        qDebug("position is %d,last position is %d",position,lastPosition);
        ui->label->setText("rotate:" + QString::number(position,10));
        if(position>=lastPosition){
           matrix.rotate((position-lastPosition)/3);
           lastPosition=position;
        }
        else if((position<lastPosition)){
            matrix.rotate((position-lastPosition)/3);
            lastPosition=position;
        }
        *myImage2=myImage.transformed(matrix,Qt::SmoothTransformation);
        drawLabel->setImageByImage(myImage2);
        drawLabel->resize(myImage2->width(),myImage2->height());
    }
    else{
        return;
    }
}

//放大图片
void picEdit::on_pushButton_clicked()
{
    QPoint tempPoint=drawLabel->pos();

if(!myImage.isNull()){

     if(myImage2->width()<=8192 && (myImage2->height())<=8192 && picSizeCount<=7 ){
         if(hasTrans){
              matrix.rotate(0);
              myImage=myImage.transformed(matrix);
         }
         *myImage2=myImage.scaled(myImage2->width()/0.8,myImage2->height()/0.8,Qt::KeepAspectRatio,Qt::SmoothTransformation);
         int scaleWidth=drawLabel->getMyImage().width()/0.8;
         int scaleHeight=drawLabel->getMyImage().height()/0.8;
         drawLabel->setImageWidth(scaleWidth);
         drawLabel->setImageHeight(scaleHeight);
         drawLabel->setImageByImage(&drawLabel->getMyImage().scaled(scaleWidth,scaleHeight,Qt::KeepAspectRatio,Qt::SmoothTransformation));
         drawLabel->resize(scaleWidth,scaleHeight);
         //drawLabel->setPixmap(QPixmap::fromImage(drawLabel->getMyImage().scaled(10,10,Qt::KeepAspectRatio)));
         qDebug()<<"scaling now....................";
         update();
         imageWidth=scaleWidth;
         imageHeight=scaleHeight;
         hasTrans=false;
         picSizeCount+=1;
         ui->horizontalSlider->setValue(picSizeCount);
         picSliderCount=ui->horizontalSlider->value();
         qDebug()<<picSizeCount;

     }
}
drawLabel->move(tempPoint);
}

// 缩小图片
void picEdit::on_pushButton_2_clicked()
{
     QPoint tempPoint=drawLabel->pos();
    if(!myImage.isNull() && picSizeCount>=0 ){
        if(drawLabel->getMyImage().size().width()>initWidth){
            if(hasTrans){
                 matrix.rotate(0);
                 myImage=myImage.transformed(matrix);
            }

            int scaleWidth=drawLabel->getMyImage().width()*0.8;
            int scaleHeight=drawLabel->getMyImage().height()*0.8;
            drawLabel->setImageWidth(scaleWidth);
            drawLabel->setImageHeight(scaleHeight);
            drawLabel->setImageByImage(&drawLabel->getMyImage().scaled(scaleWidth,scaleHeight,Qt::KeepAspectRatio,Qt::SmoothTransformation));
            drawLabel->resize(scaleWidth,scaleHeight);
            qDebug()<<"scaling decrease  now....................";
            imageWidth=scaleWidth;
            imageHeight=scaleHeight;
            hasTrans=false;
            picSizeCount-=1;
            ui->horizontalSlider->setValue(picSizeCount);
            picSliderCount=ui->horizontalSlider->value();
            qDebug()<<picSizeCount;
    }
}
drawLabel->move(tempPoint);
}

//水平滚动放大条
void picEdit::on_horizontalSlider_sliderMoved(int position)
{

    if(position>=picSliderCount){
        on_pushButton_clicked();
    }
    else if(position<picSliderCount){
        on_pushButton_2_clicked();
    }

}

//鼠标移事件
void picEdit::mouseMoveEvent(QMouseEvent *event){

}

//鼠标双击事件
void picEdit::mouseDoubleClickEvent(QMouseEvent *event){

}

//画笔点击事件
void picEdit::on_penButton_clicked()
{
    qDebug("width:%d,height:%d",this->width(),this->height());
    if(ui->penButton->text()==QStringLiteral("画笔")){
    drawLabel->isPen=true;
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    ui->imageRotate->setEnabled(false);
        ui->penButton->setText(QStringLiteral("鼠标"));
    }
    else{
        drawLabel->isPen=false;
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
        ui->imageRotate->setEnabled(true);
        myImage=drawLabel->getMyImage();
        ui->penButton->setText(QStringLiteral("画笔"));
    }

}
//保存图片
void picEdit::on_actionSave_triggered()
{
    QString saveName=QFileDialog::getSaveFileName(this,QStringLiteral("Please open a picture"),"","Images (*.png *.bmp *.jpg *.tif *.GIF)");
    qDebug()<<saveName;
    if(!saveName.isNull()){
        drawLabel->getMyImage().save(saveName);
        QMessageBox::warning(this,"save","save successful");
    }
}

//灰度调节
QImage * picEdit::greyScale(QImage *image)
{
    QImage * newImage=new QImage(image->width(),image->height(),QImage::Format_ARGB32);
    QRgb *lineRgb;
    for(int y=0;y<newImage->height();y++){
        lineRgb=(QRgb *)image->scanLine(y); //按行扫描
        for(int x=0;x<newImage->width();x++){
            int average=(qRed(lineRgb[x])+qGreen(lineRgb[x])+qRed(lineRgb[x]))/3;
            newImage->setPixel(x,y,qRgb(average,average,average));

        }
    }
    return newImage;
}
//灰度调节槽
   void picEdit::on_grayAction_triggered()
   {
       drawLabel->setImageByImage(greyScale(&drawLabel->getMyImage()));
   }
   //亮度调节
   QImage * picEdit::lightScale(QImage *image,int lightSize)
   {
       QImage * newImage=new QImage(image->width(),image->height(),QImage::Format_ARGB32);
       QColor oldColor;
       QRgb *lineRgb;
       for(int y=0;y<newImage->height();y++){
           lineRgb=(QRgb *)image->scanLine(y);
           for(int x=0;x<newImage->width();x++){
               newImage->setPixel(x,y,qRgb(qRed(lineRgb[x])+lightSize/10,qGreen(lineRgb[x])+lightSize/10,qBlue(lineRgb[x])+lightSize/10));
           }
       }


return newImage;
   }
//亮度调节
void picEdit::on_lightAdd15_triggered()
{
    drawLabel->setImageByImage(lightScale(&drawLabel->getMyImage(),15));

}
//亮度调节
 void picEdit::on_lightAdd30_triggered()
{
     drawLabel->setImageByImage(lightScale(&drawLabel->getMyImage(),30));

}
//亮度调节
void picEdit::on_lightAdd45_triggered()
{
    drawLabel->setImageByImage(lightScale(&drawLabel->getMyImage(),45));

}
//亮度调节
void picEdit::on_lightAdd65_triggered()
{
    drawLabel->setImageByImage(lightScale(&drawLabel->getMyImage(),65));

}
 //亮度调节
void picEdit::on_lightAdd85_triggered()
{
    drawLabel->setImageByImage(lightScale(&drawLabel->getMyImage(),85));

}
//亮度调节
void picEdit::on_lightAdd100_triggered()
{
    drawLabel->setImageByImage(lightScale(&drawLabel->getMyImage(),100));

}

//暖色调节
void picEdit::on_warmAction2_triggered()
{
    drawLabel->setImageByImage(warmScale(&drawLabel->getMyImage(),30));

}
//冷色调节
QImage * picEdit::warmScale(QImage *image,int coolSize)
{
    QImage * newImage=new QImage(image->width(),image->height(),QImage::Format_ARGB32);
    QColor oldColor;
    QRgb *lineRgb;
    for(int y=0;y<newImage->height();y++){
        lineRgb=(QRgb *)image->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            newImage->setPixel(x,y,qRgb(qRed(lineRgb[x]),qGreen(lineRgb[x]),qBlue(lineRgb[x])+coolSize));
        }
    }


return newImage;

}

//模糊化
void picEdit::on_mohuAction_triggered()
{
    drawLabel->setImageByImage(mohuScale(&drawLabel->getMyImage()));


}
//模糊化
QImage * picEdit::mohuScale(QImage *image)
{

    QImage * newImage = new QImage(*image);

     int kernel [5][5]= {{0,0,1,0,0},
               {0,1,3,1,0},
               {1,3,7,3,1},
               {0,1,3,1,0},
               {0,0,1,0,0}};
     int kernelSize = 5;
     int sumKernel = 27;
     int r,g,b;
     QColor color;
     QRgb *lineRgb;
     r = 0;
     g = 0;
     b = 0;
     for(int y=kernelSize/2; y<newImage->height()-(kernelSize/2); y++){
         lineRgb=(QRgb *)image->scanLine(y);
       for(int x=kernelSize/2; x<newImage->height()-(kernelSize/2); x++){
         for(int i = -kernelSize/2; i<= kernelSize/2; i++){
           for(int j = -kernelSize/2; j<= kernelSize/2; j++){
             color = QColor(image->pixel(x+i, y+j));
             r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];
             g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];
             b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];
           }
         }
         //

         r = qBound(0, r/sumKernel, 255);
         g = qBound(0, g/sumKernel, 255);
         b = qBound(0, b/sumKernel, 255);

         newImage->setPixel(x,y, qRgb(r,g,b));

       }
     }
     return newImage;
}

//锐化调节
void picEdit::on_sharpAction_triggered()
{
    drawLabel->setImageByImage(sharpScale(&drawLabel->getMyImage()));

}
//锐化调节
QImage * picEdit::sharpScale(QImage *image)
{
    QImage * newImage = new QImage(*image);

     int kernel [3][3]= {{0,-1,0},
               {-1,5,-1},
               {0,-1,0}};
     int kernelSize = 3;
     int sumKernel = 1;
     int r,g,b;
     QColor color;

     for(int x=kernelSize/2; x<newImage->width()-(kernelSize/2); x++){
       for(int y=kernelSize/2; y<newImage->height()-(kernelSize/2); y++){

         r = 0;
         g = 0;
         b = 0;

         for(int i = -kernelSize/2; i<= kernelSize/2; i++){
           for(int j = -kernelSize/2; j<= kernelSize/2; j++){
             color = QColor(image->pixel(x+i, y+j));
             r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];
             g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];
             b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];
           }
         }

         r = qBound(0, r/sumKernel, 255);
         g = qBound(0, g/sumKernel, 255);
         b = qBound(0, b/sumKernel, 255);

         newImage->setPixel(x,y, qRgb(r,g,b));

       }
     }
     return newImage;
}

