#include "myimage.h"
#include<QLabel>
#include<QImage>
#include<QSpinBox>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QPushButton>
#include<QVBoxLayout>
#include<QEvent>

myImage::myImage(QWidget *parent) : QWidget(parent)
{

  image=new QImage;
  label=new QLabel;
  imageLabel=new QLabel;
  pointXSpinBox=new QSpinBox;
  pointYSpinBox=new QSpinBox;
  gridLayout=new QGridLayout;
  hboxLayout=new QHBoxLayout;
  vboxLayout=new QVBoxLayout;
  changePoint=new QPushButton;
  ImageCenterP=new QPoint;
  QObject::connect(changePoint,SIGNAL(pressed()),this,SLOT(changeMyPoint()));
  isClickChange=false;
  changePoint->setText(QStringLiteral("更改"));
  pointXSpinBox->setReadOnly(true);
  pointYSpinBox->setReadOnly(true);
  myImageInit();

}

myImage::~myImage()
{
    delete image;
    delete label;
    delete pointXSpinBox;
    delete pointYSpinBox;
    delete hboxLayout;
    delete imageLabel;
    delete vboxLayout;
    delete changePoint;
    delete ImageCenterP;

}
//初始化自定义控件
void myImage::myImageInit()
{
    image2.load(":525897.png");
    label->setText(QStringLiteral("图像中心:"));
    //label->resize(QSize(50,30));
    imageLabel->setPixmap(QPixmap::fromImage(image2.scaled(imageLabel->width(),imageLabel->height(),Qt::KeepAspectRatio)));
    imageLabel->setAutoFillBackground(true);
    pointXSpinBox->setValue(20);
    pointYSpinBox->setValue(30);
    ImageCenterP->setX(20);
    ImageCenterP->setY(30);

    hboxLayout->addWidget(label);
    hboxLayout->addWidget(pointXSpinBox);
    hboxLayout->addWidget(pointYSpinBox);
    hboxLayout->addWidget(changePoint);
    vboxLayout->addWidget(imageLabel);
    vboxLayout->addLayout(hboxLayout);
    gridLayout->addLayout(vboxLayout,0,0);
    this->setLayout(gridLayout);

}
//改变坐标
void myImage::changeMyPoint()
{
    QPalette buttonPal;
    isClickChange=!isClickChange;
    if(isClickChange){
        pointXSpinBox->setReadOnly(false);
        pointYSpinBox->setReadOnly(false);
          changePoint->setText(QStringLiteral("保存"));
          buttonPal.setColor(QPalette::ButtonText,Qt::red);
          changePoint->setPalette(buttonPal);

    }
    else{
        pointXSpinBox->setReadOnly(true);
        pointYSpinBox->setReadOnly(true);
          changePoint->setText(QStringLiteral("更改"));
          buttonPal.setColor(QPalette::ButtonText,Qt::black);
          changePoint->setPalette(buttonPal);
          setImageLocation();

    }


}
QImage myImage::getMyImage()
{
    return image2;

}

void myImage::setMyImage(QImage *image)
{
    image2=*image;

}
//设置图像采集中心
void myImage::setImageLocation()
{
   ImageCenterP->setX(pointXSpinBox->value());
   ImageCenterP->setY(pointYSpinBox->value());
}
void myImage::setXPoint(int x)
{
    pointXSpinBox->setValue(x);

}

void myImage::setYPoint(int y)
{
    pointYSpinBox->setValue(y);
}
int myImage::getXPoint()
{
 return pointXSpinBox->value();
}

int myImage::getYPoint()
{
    return pointYSpinBox->value();
}

//获取图像中心
QPoint myImage::getImageLocation()
{
    return *ImageCenterP;
}



