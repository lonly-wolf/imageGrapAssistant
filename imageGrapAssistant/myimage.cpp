#include "myimage.h"
#include<QLabel>
#include<QImage>
#include<QSpinBox>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QPushButton>
#include<QVBoxLayout>
#include<QEvent>
#include<QMenu>
#include<QDebug>
#include<QFileDialog>
#include<QMessageBox>

myImage::myImage(QWidget *parent) : QWidget(parent)
{

  myimage=new QImage;
  picedit=new picEdit(this);
  image=new QImage;
  label=new QLabel;
  imageLabel=new QLabel;
  pointXSpinBox=new QSpinBox;
  pointYSpinBox=new QSpinBox;
  gridLayout=new QGridLayout;
  hboxLayout=new QHBoxLayout;
  vboxLayout=new QVBoxLayout;
  vboxLayout2=new QVBoxLayout;
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
    delete vboxLayout2;
    delete changePoint;
    delete ImageCenterP;
    delete myimage;

}
//初始化自定义控件
void myImage::myImageInit()
{
    myimage->load(":347143.jpg");
    label->setText(QStringLiteral("图像中心:"));
    //label->resize(QSize(50,30));
    imageLabel->setPixmap(QPixmap::fromImage(myimage->scaled(imageLabel->width(),imageLabel->height(),Qt::KeepAspectRatio)));
    //imageLabel->setAutoFillBackground(true);
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
    gridLayout->addLayout(vboxLayout,0,0,Qt::AlignCenter);
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
QImage * myImage::getMyImage()
{
    return myimage;

}

void myImage::setMyImage(QImage *image)
{
    myimage=image;
    imageLabel->setPixmap(QPixmap::fromImage(myimage->scaled(imageLabel->width(),imageLabel->height(),Qt::KeepAspectRatio)));

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

void  myImage::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu=new QMenu(this);
    QAction *create=new QAction(QStringLiteral("编辑"),this);
    QAction *save=new QAction(QStringLiteral("另存为"),this);
    menu->addAction(create);
    menu->addAction(save);
    menu->move(cursor().pos());
    connect(create,SIGNAL(triggered(bool)),this,SLOT(editPicture()));
    connect(save,SIGNAL(triggered(bool)),this,SLOT(savePicture()));
    menu->show();

}
 //编辑图片
void myImage::editPicture()
{
    picedit->show();
    picedit->drawLabel->setImageByImage(this->getMyImage());
    picedit->myImage=*this->getMyImage();
    picedit->drawLabel->resize(this->getMyImage()->width(),this->getMyImage()->height());

}

//保存图片
void myImage::savePicture()
{
    QString saveName=QFileDialog::getSaveFileName(this,QStringLiteral("Please open a picture"),"","Images (*.png *.bmp *.jpg *.tif *.GIF)");
    qDebug()<<saveName;
    if(!saveName.isNull()){
      this->getMyImage()->save(saveName);
        QMessageBox::warning(this,"save","save successful");
    }
}
