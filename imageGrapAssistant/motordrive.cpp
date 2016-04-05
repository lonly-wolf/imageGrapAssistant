#include "motordrive.h"
#include <QSerialPort>
#include<QSerialPortInfo>
#include<QMessageBox>
#include<QDebug>
#include<QByteArray>
#include<QString>
#include<QTimer>
#include<QObject>

motorDrive::motorDrive(QObject *parent) : QObject(parent)
{
    mySerialPort=new QSerialPort;
    sendTimer=new QTimer;
    constantSend=true; //连续驱动标示初始化
    myPulse=3000;
    mySpeed=300;
    connect(sendTimer,SIGNAL(timeout()),this,SLOT(sendQueue()));


}

motorDrive::~motorDrive()
{
    //delete mySerialPort;
   //delete sendTimer;
}

void motorDrive::moveYPlus()
{

    QByteArray sendText("\x4e\x02\x22",3);

    if(mySerialPort->isOpen()){
        //连续驱动
        if(constantSend){
           // mySerialPort->write(sendText);
            sendData.append(sendText);
        }
        else{
            int v = myPulse;
            QByteArray data("\x00\x00\x00\x00\x00\x00\x00\x00",8);
            data[0]=0x4d;
            data[1]=0x02;
            data[2]=0x08;
            data[3]=0x20;
            data[4]=v>>24 & 0xff;
            data[5]=v>>16 & 0xff;
            data[6]=v>>8  & 0xff;
            data[7]=v & 0xff;
            qDebug()<<data;
            //mySerialPort->write(data);
            sendData.append(data);
        }

    }
    else{
        qDebug()<<"null SerialPort opened";
    }
}

void motorDrive::moveYMinus()
{
    QByteArray sendText("\x4e\x02\x23",3);
    if(mySerialPort->isOpen()){
        if(constantSend){
           // mySerialPort->write(sendText);
            sendData.append(sendText);
        }
        else{
            int v = myPulse;
            QByteArray data("\x00\x00\x00\x00\x00\x00\x00\x00",8);
            data[0]=0x4d;
            data[1]=0x02;
            data[2]=0x08;
            data[3]=0x21;
            data[4]=v>>24 & 0xff;
            data[5]=v>>16 & 0xff;
            data[6]=v>>8  & 0xff;
            data[7]=v & 0xff;
            qDebug()<<data;
            //mySerialPort->write(data);
            sendData.append(data);
        }

    }


}

void motorDrive::moveXPlus()
{
    QByteArray sendText("\x4e\x01\x22",3);
    if(mySerialPort->isOpen()){
        if(constantSend){
           // mySerialPort->write(sendText);
            sendData.append(sendText);
        }
        else{
            int v = myPulse;
            QByteArray data("\x00\x00\x00\x00\x00\x00\x00\x00",8);
            data[0]=0x4d;
            data[1]=0x01;
            data[2]=0x08;
            data[3]=0x20;
            data[4]=v>>24 & 0xff;
            data[5]=v>>16 & 0xff;
            data[6]=v>>8  & 0xff;
            data[7]=v & 0xff;
            qDebug()<<data;
           // mySerialPort->write(data);
            sendData.append(data);
        }
    }
}

void motorDrive::moveXMinus()
{
    QByteArray sendText("\x4e\x01\x23",3);
    if(mySerialPort->isOpen()){
         if(constantSend){
            //mySerialPort->write(sendText);
             sendData.append(sendText);
         }
         else{
             int v = myPulse;
             QByteArray data("\x00\x00\x00\x00\x00\x00\x00\x00",8);
             data[0]=0x4d;
             data[1]=0x01;
             data[2]=0x08;
             data[3]=0x21;
             data[4]=v>>24 & 0xff;
             data[5]=v>>16 & 0xff;
             data[6]=v>>8  & 0xff;
             data[7]=v & 0xff;
             qDebug()<<data;
             //mySerialPort->write(data);
             sendData.append(sendText);
         }
    }


}

//连续驱动选择函数
void motorDrive::isConstantSend(bool b)
{
    constantSend=b;
}
//初始化串口函数
void motorDrive::initSerialPort(QSerialPort *serialPort)
{

        qDebug()<<"initSerialPort------";
        mySerialPort=serialPort;
        isOpenSerial=true;
        sendTimer->start(2000);


}
//设置脉冲
void motorDrive::setPulse(int p)
{
    myPulse=p;
}
//停止模式
void motorDrive::slowStop()
{
    QByteArray sendText("\x56\x03\x26",3);
    if(mySerialPort->isOpen()){
            // mySerialPort->write(sendText);
             sendData.append(sendText);
    }



}
void motorDrive::nowStop()
{
    QByteArray sendText("\x56\x03\x27",3);
   if(mySerialPort->isOpen()){
            //mySerialPort->write(sendText);
              sendData.append(sendText);
   }


}
//坐标实时追踪
void motorDrive::followPoint(bool isGetPoint)
{
    QByteArray sendText=QByteArray("\x57\x03",2);
        if(isGetPoint){
            commonData.append(sendText);
        }
        else{
             commonData.clear();
        }
}
//原点搜索
void motorDrive::homeSet()
{
    QByteArray data("\x54\x02",2);
    if(mySerialPort->isOpen()){
        //mySerialPort->write(data);
        sendData.append(data);
    }

}
//电机速度设置
void motorDrive::setMotorSpeed(int speed)
{
    mySpeed=speed;
    if(mySerialPort->isOpen()){
             int v =speed;
             QByteArray data("\x00\x00\x00\x00",4);
             data[0]=0x4a;
             data[1]=0x0f;
             data[2]=v>>8  & 0xff;
             data[3]=v & 0xff;
            // mySerialPort->write(data);
             sendData.append(data);

    }

}
//发送数据队列
void motorDrive::sendQueue()
{

    if(!sendData.isEmpty() && isOpenSerial){
        mySerialPort->write(sendData.first());
        qDebug()<<"motorDrive-------sendQueue()" + sendData.first();
        sendData.removeFirst();
    }
    else if(!commonData.isEmpty() && isOpenSerial){
        mySerialPort->write(commonData.first());
    }
}
//串口状态监测
void  motorDrive::isMySerialOpen(bool b)
{
    if(!b){
        isOpenSerial=false;
        sendTimer->stop();
    }
}


