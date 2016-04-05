#ifndef MOTORDRIVE_H
#define MOTORDRIVE_H
#include <QMainWindow>
#include<QVector>

class QSerialPort;
class QTimer;

class motorDrive : public QObject
{
    Q_OBJECT
    friend class MainWindow;
public:
    explicit motorDrive(QObject *parent = 0);
    ~motorDrive();

signals:

private:
    QSerialPort *mySerialPort;
    QTimer *sendTimer;
    bool constantSend;
    bool isOpenSerial;//端口开放判断
    int myPulse;
    int mySpeed;
    QVector<QByteArray> sendData;//可变数据数组
    QVector<QByteArray> commonData;//心跳数据数组



    void moveXPlus(); //电机驱动
    void moveXMinus();
    void moveYPlus();
    void moveYMinus();
    void slowStop();//停止模式
    void nowStop();
    void followPoint(bool isGetPoint);//坐标实时追踪
    void homeSet();//原点搜索
    void setPulse(int p); //设置脉冲
    void initSerialPort(QSerialPort *serialPort); //初始化串口函数
    void isConstantSend(bool b); //连续驱动选择函数
    void setMotorSpeed(int speed);//电机速度设置
    void isMySerialOpen(bool b);//串口状态监测

private slots:
    void sendQueue();//发送数据队列

};

#endif // MOTORDRIVE_H
