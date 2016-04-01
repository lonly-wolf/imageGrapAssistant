#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include"mylabel.h"
#include "myimage.h"
#include<aboutdialog.h>

class QSerialPort;
class QTimer;
class QSettings;
class QVBoxLayout;
class QGridLayout;
class QHBoxLayout;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_openSerial_clicked();//打开/关闭 串口
    void updateDate();//接收数据
    void on_sendData_clicked();//发送数据
    void on_debugAction_triggered();//命令调试助手
    void on_homeButton_clicked();
    void on_yAddButton_clicked();
    void on_ySubButton_clicked();
    void on_xAddButton_clicked();
    void on_xSubButton_clicked();
    void on_setPulseButton_clicked();
    void on_slowStop_clicked();
    void on_nowStop_clicked();
    void on_updateSerialButton_clicked();
    void on_updateSerialButton_pressed();
    void on_updateSerialButton_released();
    void on_followPoint_clicked();
    void on_oneMulone_triggered();
    void on_twoMultwo_triggered();
    void on_threeMulthree_triggered();
    void on_threeMulfour_triggered();
    void on_fourMulfour_triggered();
    void on_saveAllImage_triggered();
    void on_aboutAction_triggered();
    void on_motorSpeed_triggered();
    void on_saveCommand_clicked();
    void on_actionShowFunction_triggered(); //调用工作台

    void enabledExecute();

private:
    Ui::MainWindow *ui;
    QSerialPort *mySerialPort;
    QByteArray requestData;//串口返回数据
    QTimer *receiveTimer; //串口数据接收计时器
    QTimer *sendTimer;//串口数据发送计时器
    QTimer *pointTimer;//坐标实时追踪刷新
    QTimer *executeCommandT; //检测命令运行间隔
    aboutDialog *myDialog;
    bool isOpenSerial;
    QPoint nowMousePoint;
    QSettings *mySettingSave;//记忆模式
    QSettings *mySettingRead;
    myLabel *picLabel;
    myImage *picImage;
    QVBoxLayout *vboxLayOut;
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayOut2;
    QHBoxLayout *hboxLayOut2;
    myImage *myImageList[16][16];
    QDockWidget *dockInsert;
    int picImageRow;
    int picImageColumn;
    char pulseNumber[7];
    int currentPulse;
    bool issetPulse;
    bool clearPosition; //坐标清零
    bool findPosition; //已到达零点
    bool isGetPoint;  //判断是否获取当前坐标
    bool enableToExecute;
    bool isPositionData;
    QByteArray tempRequestData;
    int motorSpeed;

    void scanAvaliableSerial(); //扫描可用串口
    void initImageWidget(int x,int y); //初始化x*y的widget
    void showCurrentPosition(); //虚拟化显示当前位置坐标
    void savePicLocation();  //保存图片坐标信息
    void readPicLocation();//读取图片坐标信息
    void saveCommandText();//保存命令终端命令
    void initCommandText();//初始化命令中断
    void followCurrentPoint();//获取当前位置坐标

protected:
    void closeEvent(QCloseEvent *event);





};

#endif // MAINWINDOW_H
