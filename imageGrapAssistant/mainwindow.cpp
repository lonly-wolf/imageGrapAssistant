#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QtSerialPort/QSerialPort>
#include<QSerialPortInfo>
#include<QTimer>
#include<QDebug>
#include<QMessageBox>
#include<QSettings>
#include<QVBoxLayout>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QChar>
#include<QByteArray>
#include<QString>
#include<QSettings>
#include<QFileDialog>
#include<QDateTime>
#include<QImageWriter>
#include<QBuffer>
#include<QInputDialog>
#include<QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //实例化
    ui->setupUi(this);
    mySerialPort=new QSerialPort(this);
    sendTimer=new QTimer(this); //判断是否连续发送
    mySettingRead=new QSettings(this);
    mySettingSave=new QSettings(this);
    myMotorDrive=new motorDrive(this);
    myDialog=new aboutDialog(this);
    dockInsert = new QDockWidget(QStringLiteral("开发者工具"), this); //命令调试助手
    picLabel=new myLabel;
    picImage=new myImage(ui->imageWidget);

    vboxLayOut=new QVBoxLayout(ui->widget);
    gridLayout=new QGridLayout(ui->imageWidget);
    vboxLayOut2=new QVBoxLayout;
    hboxLayOut2=new QHBoxLayout;


    //初始化
    this->setMinimumSize(QSize(1345,748));
    dockInsert->close(); //隐藏开发者模式dock
    picImage->hide();//隐藏图像列表
    dataLength=0;
    isGetPoint=false;
    isPositionData=false;
    motorSpeed=300; //后期通过QSettings初始化该速度
    QSettings mySettingsRead; //不加参数表明采用系统默认路径 windows下为注册表 linux下为......
    ui->pulseSpinBox->setEnabled(false);
    isOpenSerial=false;
    issetPulse=false;
    ui->functionDockWidget2->setMinimumSize(QSize(340,768));
    ui->updateSerialButton->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->setPulseButton->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    vboxLayOut->addWidget(picLabel);
    ui->widget->setLayout(vboxLayOut); //虚拟化坐标显示
    //绑定信号槽
    connect(mySerialPort,SIGNAL(aboutToClose()),this,SLOT(isMySerialOpen()));//串口状态监测
    connect(mySerialPort,SIGNAL(readyRead()),this,SLOT(updateDate())); //接收数据槽函数
    //程序记忆设置
    bool ok;
    QPoint lastPoint=mySettingsRead.value("pos").toPoint();//设置上次窗口位置
    //设置记忆模式
    ui->serialCB->setCurrentText(mySettingsRead.value("serial").toString());
    ui->baudRateCB->setCurrentText(mySettingsRead.value("baudRate").toString());
    ui->dataBitCB->setCurrentText(mySettingsRead.value("dataBit").toString());
    ui->checkCB->setCurrentText(mySettingsRead.value("checkBit").toString());
    ui->stopBitCB->setCurrentText(mySettingsRead.value("stopBit").toString());
    ui->pulseSpinBox->setValue(mySettingsRead.value("pulseValue").toInt(&ok));
    ui->constantCheckBox->setChecked(mySettingsRead.value("isConstant").toBool());
    motorSpeed=mySettingsRead.value("motorSpeed").toInt(&ok); //设置上次电机速度
    //this->move(lastPoint);//设置上次窗口位置
    picImageRow=mySettingsRead.value("picImageRow").toInt(&ok);//设置上次m*n图像显示模式
    picImageColumn=mySettingsRead.value("picImageColumn").toInt(&ok);
    if(picImageRow==0 || picImageColumn==0){
        picImageRow=4;
        picImageColumn=4;
    }

    initImageWidget(picImageRow,picImageColumn); //初始化imageWidget
    ui->groupBox_3->hide();
    initCommandText(); //初始化命令行模式
    picLabel->setCurrentPoint(&QPoint(0,0));//设置虚拟化坐标起始点位置
    scanAvaliableSerial();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mySerialPort;
    delete sendTimer;
    delete mySettingRead;
    delete mySettingSave;
    delete picLabel;
    delete vboxLayOut2;
    delete hboxLayOut2;
    delete myDialog;
    delete dockInsert;
    delete myMotorDrive;

}

//打开串口
void MainWindow::on_openSerial_clicked()
{
    QString currentString=ui->openSerial->text();
    if(!isOpenSerial){
         myMotorDrive->initSerialPort(mySerialPort); //初始化我的驱动
         ui->serialCB->setEnabled(false);
         ui->baudRateCB->setEnabled(false);
         ui->checkCB->setEnabled(false);
         ui->updateSerialButton->setEnabled(false);
         ui->dataBitCB->setEnabled(false);
         ui->stopBitCB->setEnabled(false);
         isOpenSerial=true;
         ui->openSerial->setText(QStringLiteral("关闭串口"));
         mySerialPort->setPortName(ui->serialCB->currentText());
         mySerialPort->open(QIODevice::ReadWrite);
         mySerialPort->setFlowControl(QSerialPort::NoFlowControl);//取默认值
         mySerialPort->setBaudRate(QSerialPort::BaudRate(ui->baudRateCB->currentText().toInt()),QSerialPort::AllDirections);
         //设置停止位
          switch (ui->stopBitCB->currentIndex()) {
          case 0:
               mySerialPort->setStopBits(QSerialPort::OneStop);
              break;
          case 1:
              mySerialPort->setStopBits(QSerialPort::OneAndHalfStop);
             break;
          case 2:
               mySerialPort->setStopBits(QSerialPort::TwoStop);
              break;
          default:
              break;
          }
          //设置校验位
          switch (ui->checkCB->currentIndex()) {
          case 0:
                mySerialPort->setParity(QSerialPort::NoParity);
              break;
          case 1:
              mySerialPort->setParity(QSerialPort::OddParity);
            break;
          case 2:
                mySerialPort->setParity(QSerialPort::EvenParity);
              break;

          default:
              break;
          }
          //设置数据位
          switch (ui->dataBitCB->currentIndex()) {
          case 0:
                mySerialPort->setDataBits(QSerialPort::Data5);
              break;
          case 1:
                mySerialPort->setDataBits(QSerialPort::Data6);
              break;
          case 2:
                mySerialPort->setDataBits(QSerialPort::Data7);
              break;
          case 3:
                mySerialPort->setDataBits(QSerialPort::Data8);
              break;
          default:
              break;
          }
          //初始化电机速度
          myMotorDrive->setMotorSpeed(motorSpeed);
          ui->statusBar->showMessage(QStringLiteral("正在监听端口...."));
    }
    else{
        isOpenSerial=false;
        ui->serialCB->setEnabled(true);
        ui->baudRateCB->setEnabled(true);
        ui->checkCB->setEnabled(true);
        ui->updateSerialButton->setEnabled(true);
        ui->dataBitCB->setEnabled(true);
        ui->stopBitCB->setEnabled(true);
        ui->openSerial->setText(QStringLiteral("打开串口"));
        ui->statusBar->showMessage(QStringLiteral("端口关闭"));
        mySerialPort->close();
    }
}

//发送数据(只针对开发者模式有效)
void MainWindow::on_sendData_clicked()
{
    QByteArray sendText=ui->sendContextBox->currentText().toLocal8Bit();
    if(mySerialPort->isOpen()){
        if(ui->isHexSend){
            //十六进制发送 
          mySerialPort->write(QByteArray::fromHex(sendText));
        }
        else{
            //非十六进制发送
            mySerialPort->write(sendText);
        }
    }
    else{
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("请打开串口"),QMessageBox::Ok);
    }
    //判断连续发送
    if(ui->constantSend->isChecked()){
        bool ok=false;
        sendTimer->start(ui->constantTime->text().toInt(&ok,10));
    }
    else{
        sendTimer->stop();
    }
}
//接收数据
void MainWindow::updateDate()
{
    if(mySerialPort->isOpen()){
        requestData=mySerialPort->readAll();
        if(ui->showHex->isChecked()){
            ui->statusBar->showMessage(requestData.toHex());
            ui->showLabel->setText("Respo:" + requestData.toHex());
            qDebug()<<requestData.toHex();
        }
        else{
            ui->statusBar->showMessage(requestData,0);
            ui->showLabel->setText("Respo:" + requestData);
        }
        //执行虚拟化坐标转换------------------------>(添加起始结束校验位)
        if(requestData[0]=='\x09'){
            isPositionData=true;
            dataLength=0;
              }
        if(isPositionData==true){
            tempRequestData.append(requestData);
            dataLength++;
        }

            int x=0,y=0;
            if(requestData[0]=='\x57' && dataLength==9){
                 QMessageBox::warning(this,"hi",tempRequestData.toHex());
                x=(tempRequestData[3] & 0x0f) *16*16+(tempRequestData[3] & 0xf0)*16*16*16 + (tempRequestData[4] & 0x0f)+(tempRequestData[4] & 0xf0)*16 ;
                y=(tempRequestData[7] & 0x0f) *16*16+(tempRequestData[7] & 0xf0)*16*16*16 + (tempRequestData[8] & 0x0f)+(tempRequestData[8] & 0xf0)*16 ;
                picLabel->setCurrentPoint(&QPoint(y/25,y/25));
                qDebug()<<x;
                qDebug()<<y;
                qDebug()<<tempRequestData;
                tempRequestData.clear();
                isPositionData=false;
                dataLength=0;
                QMessageBox::warning(this,"stop","point location is arrived!",QMessageBox::Yes);
            }

        //坐标清零后重新给定速度------------------>(添加起始结束校验位)
        if(requestData[1]=='\x4c' ){
            QMessageBox::warning(this,"stop","homeSet",QMessageBox::Yes);

               myMotorDrive->setMotorSpeed(motorSpeed);
               requestData.clear();
        }
    }
}
//扫描可用串口
void MainWindow::scanAvaliableSerial()
{
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
       QSerialPort serial;
       serial.setPort(info);
       if(serial.open(QIODevice::ReadWrite)){
           ui->serialCB->addItem(info.portName());
           ui->serialCB->setCurrentText(info.portName());
           serial.close();
       }
    }
}

void MainWindow::on_debugAction_triggered()
{
    ui->groupBox_3->show();
    dockInsert->setMinimumSize(521,141);
    dockInsert->setMaximumSize(521,141);
    dockInsert->setAcceptDrops(true);
    dockInsert->setFloating(true);
    dockInsert->setMouseTracking(true);
    dockInsert->setAllowedAreas(Qt::BottomDockWidgetArea);
    dockInsert->setWidget(ui->groupBox_3);
    dockInsert->show();

}

void MainWindow::initImageWidget(int x, int y)
{
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            myImageList[i][j]=new myImage(this);
            gridLayout->addWidget(myImageList[i][j],i,j);
        }    
    }
   readPicLocation();
}
//寻找home
void MainWindow::on_homeButton_clicked()
{
myMotorDrive->homeSet();
}
//y轴正向连续驱动
void MainWindow::on_yAddButton_clicked()
{
    myMotorDrive->moveYPlus();
}

//y轴负向连续驱动
void MainWindow::on_ySubButton_clicked()
{
    myMotorDrive->moveYMinus();
}
//x轴正向
void MainWindow::on_xAddButton_clicked()
{
    myMotorDrive->moveXPlus();
}
//x轴负向连续驱动
void MainWindow::on_xSubButton_clicked()
{
    myMotorDrive->moveXMinus();
}
//设置脉冲
void MainWindow::on_setPulseButton_clicked()
{
     QPalette buttonPal;

    if(!issetPulse){
        ui->pulseSpinBox->setEnabled(true);
        ui->setPulseButton->setText(QStringLiteral("保存设置"));
        buttonPal.setColor(QPalette::ButtonText,Qt::red);
        ui->setPulseButton->setPalette(buttonPal);
        myMotorDrive->setPulse(ui->pulseSpinBox->value());
    }
    else{
        ui->pulseSpinBox->setEnabled(false);
        ui->setPulseButton->setText(QStringLiteral("设置脉冲"));
        buttonPal.setColor(QPalette::ButtonText,Qt::black);
        ui->setPulseButton->setPalette(buttonPal);
    }
    issetPulse=!issetPulse;
}
//减速停止
void MainWindow::on_slowStop_clicked()
{
    myMotorDrive->slowStop();
}
//立即停止
void MainWindow::on_nowStop_clicked()
{
    myMotorDrive->nowStop();
}
//串口数据更新
void MainWindow::on_updateSerialButton_clicked()
{
    scanAvaliableSerial();
}
//串口刷新按下
void MainWindow::on_updateSerialButton_pressed()
{   ui->serialCB->clear();
    ui->updateSerialButton->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;background-color:rgb(170,0,255)");
}
//串口刷新抬起
void MainWindow::on_updateSerialButton_released()
{
    ui->updateSerialButton->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;background-color:rgb(255,255,255)");
}
//坐标追踪
void MainWindow::on_followPoint_clicked()
{
       isGetPoint=!isGetPoint;
        myMotorDrive->followPoint(isGetPoint);
        if(isGetPoint){
             ui->followPoint->setText(QStringLiteral("取消追踪"));
        }
        else{
             ui->followPoint->setText(QStringLiteral("坐标追踪"));
        } 
}
//关闭窗口
void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings mySettingsWrite;
    mySettingsWrite.setValue("pos",MainWindow::pos());
    mySettingsWrite.setValue("size",this->size());
    mySettingsWrite.setValue("serial",ui->serialCB->currentText());
    mySettingsWrite.setValue("baudRate",ui->baudRateCB->currentText());
    mySettingsWrite.setValue("dataBit",ui->dataBitCB->currentText());
    mySettingsWrite.setValue("checkBit",ui->checkCB->currentText());
    mySettingsWrite.setValue("stopBit",ui->stopBitCB->currentText());
    mySettingsWrite.setValue("pulseValue",ui->pulseSpinBox->value());
    mySettingsWrite.setValue("isConstant",ui->constantCheckBox->isChecked());
    mySettingsWrite.setValue("picImageRow",picImageRow);
    mySettingsWrite.setValue("picImageColumn",picImageColumn);
    mySettingsWrite.setValue("motorSpeed",motorSpeed);
    savePicLocation();
    saveCommandText(); //保存命令终端所使用的指令到本地
    QMessageBox::StandardButton result;
    result=QMessageBox::warning(this,QStringLiteral("离开"),QStringLiteral("确定要离开吗?"),QMessageBox::Yes|QMessageBox::No);
    if(result==QMessageBox::Yes){
       event->accept();
    }
    else if(result==QMessageBox::No) {
     event->ignore();
    }
}

//1*1模式
void MainWindow::on_oneMulone_triggered()
{
    savePicLocation();
    for(int i=0;i<picImageRow;i++){
        for(int j=0;j<picImageColumn;j++){
          delete  myImageList[i][j] ;
        }
    }
     picImageRow=1;
     picImageColumn=1;
     initImageWidget(picImageRow,picImageColumn);

}
//2*2模式
void MainWindow::on_twoMultwo_triggered()
{
    savePicLocation();
    for(int i=0;i<picImageRow;i++){
        for(int j=0;j<picImageColumn;j++){
            delete  myImageList[i][j] ;
        }
    }
    picImageRow=2;
    picImageColumn=2;
     initImageWidget(picImageRow,picImageColumn);

}
//3*3模式
void MainWindow::on_threeMulthree_triggered()
{
    savePicLocation();
    for(int i=0;i<picImageRow;i++){
        for(int j=0;j<picImageColumn;j++){
             delete  myImageList[i][j] ;
        }
    }
    picImageRow=3;
    picImageColumn=3;
    initImageWidget(picImageRow,picImageColumn);
}
//4*4模式
void MainWindow::on_threeMulfour_triggered()
{
    savePicLocation();
    for(int i=0;i<picImageRow;i++){
        for(int j=0;j<picImageColumn;j++){
             delete  myImageList[i][j] ;
        }
    }
    picImageRow=3;
    picImageColumn=4;
    initImageWidget(picImageRow,picImageColumn);
}
//5*5模式
void MainWindow::on_fourMulfour_triggered()
{
    savePicLocation();
    for(int i=0;i<picImageRow;i++){
        for(int j=0;j<picImageColumn;j++){
             delete  myImageList[i][j] ;
        }
    }
    picImageRow=4;
    picImageColumn=4;
    initImageWidget(picImageRow,picImageColumn);
}
void MainWindow::on_saveAllImage_triggered()
{
    QString saveImage=QFileDialog::getExistingDirectory(this,QStringLiteral("请选择要保存的文件夹"),"",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks) + "/";              //QFileDialog::getSaveFileName(this,tr("保存收发信息"),"../dataStore.txt","Text files(*.txt)");
    qDebug()<<saveImage;
    int k=1;
    for(int i=0;i<picImageRow;i++){
        for(int j=0;j<picImageColumn;j++){
            int randomNum=k;                          //(i+1)*picImageRow*picImageColumn+j;
            QImage outputImage=myImageList[i][j]->getMyImage();
            outputImage.save(saveImage + QDateTime::currentDateTime().toString("hh-mm-ss-ddd") + QString::number(randomNum,10) + ".png");
            qDebug()<<saveImage + QDateTime::currentDateTime().toString("hh-mm-ss-ddd") + QString::number(randomNum,10) + ".png";
            ui->statusBar->showMessage(QStringLiteral("系统正在保存图片，请稍后......共 ") + QString::number(picImageColumn*picImageRow,10)+QStringLiteral(" 张，正在保存图片：")+ QString::number(k++,10));
        }
    }
    ui->statusBar->showMessage(QStringLiteral("图片保存成功!"));
}
//关于信息
void MainWindow::on_aboutAction_triggered()
{
    myDialog->show();
}
//电机速率设置
void MainWindow::on_motorSpeed_triggered()
{
    bool ok;
   motorSpeed=QInputDialog::getInt(this,QStringLiteral("请输入电机速度"),QStringLiteral("请输入电机速度"),motorSpeed,1,500,1,&ok);
   if(mySerialPort->isOpen()){
            myMotorDrive->setMotorSpeed(motorSpeed);
   }
   else{
       QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("请打开串口"),QMessageBox::Ok);
   }
}
//保存图片坐标
void MainWindow::savePicLocation()
{
    QSettings mySettingsWrite;
    if(picImageRow==1 && picImageColumn==1){
        int k=1;
        for(int i=0;i<picImageRow;i++){
            for(int j=0;j<picImageColumn;j++){
                mySettingsWrite.setValue("1picPointX" +QString::number(k,10),myImageList[i][j]->getXPoint());
                mySettingsWrite.setValue("1picPointY" +QString::number(k,10),myImageList[i][j]->getYPoint());
                k++;
            }
        }
    }
    else if(picImageRow==2 && picImageColumn==2){
        int k=1;
        for(int i=0;i<picImageRow;i++){
            for(int j=0;j<picImageColumn;j++){
                mySettingsWrite.setValue("2picPointX" +QString::number(k,10),myImageList[i][j]->getXPoint());
                mySettingsWrite.setValue("2picPointY" +QString::number(k,10),myImageList[i][j]->getYPoint());
                k++;
            }
        }
    }
    else if(picImageRow==3 && picImageColumn==3){
        int k=1;
        for(int i=0;i<picImageRow;i++){
            for(int j=0;j<picImageColumn;j++){
                mySettingsWrite.setValue("3picPointX" +QString::number(k,10),myImageList[i][j]->getXPoint());
                mySettingsWrite.setValue("3picPointY" +QString::number(k,10),myImageList[i][j]->getYPoint());
                k++;
            }
        }

    }
    else if(picImageRow==3 && picImageColumn==4){
        int k=1;
        for(int i=0;i<picImageRow;i++){
            for(int j=0;j<picImageColumn;j++){
                mySettingsWrite.setValue("4picPointX" +QString::number(k,10),myImageList[i][j]->getXPoint());
                mySettingsWrite.setValue("4picPointY" +QString::number(k,10),myImageList[i][j]->getYPoint());
                k++;
            }
        }

    }
    else if(picImageRow==4 && picImageColumn==4){
        int k=1;
        for(int i=0;i<picImageRow;i++){
            for(int j=0;j<picImageColumn;j++){
                mySettingsWrite.setValue("5picPointX" +QString::number(k,10),myImageList[i][j]->getXPoint());
                mySettingsWrite.setValue("5picPointY" +QString::number(k,10),myImageList[i][j]->getYPoint());
                k++;
            }
        }
    }
}
//读取图片坐标信息
void MainWindow::readPicLocation()
{
    QSettings mySettingsRead; //不加参数表明采用系统默认路径 windows下为注册表 linux下为......
    bool ok;
    if(picImageRow==1 && picImageColumn==1){
        int k=1;
        for(int i=0;i<picImageRow;i++){
            for(int j=0;j<picImageColumn;j++){
                 myImageList[i][j]->setXPoint(mySettingRead->value("1picPointX" +QString::number(k,10)).toInt(&ok));
                 myImageList[i][j]->setYPoint(mySettingRead->value("1picPointY" +QString::number(k,10)).toInt(&ok));
                 k++;
            }
        }

    }
    else if(picImageRow==2 && picImageColumn==2){
        int k=1;
        for(int i=0;i<picImageRow;i++){
            for(int j=0;j<picImageColumn;j++){
                myImageList[i][j]->setXPoint(mySettingRead->value("2picPointX" +QString::number(k,10)).toInt(&ok));
                myImageList[i][j]->setYPoint(mySettingRead->value("2picPointY" +QString::number(k,10)).toInt(&ok));
                k++;
            }
        }

    }
    else if(picImageRow==3 && picImageColumn==3){
        int k=1;
        for(int i=0;i<picImageRow;i++){
            for(int j=0;j<picImageColumn;j++){
                myImageList[i][j]->setXPoint(mySettingRead->value("3picPointX" +QString::number(k,10)).toInt(&ok));
                myImageList[i][j]->setYPoint(mySettingRead->value("3picPointY" +QString::number(k,10)).toInt(&ok));
                k++;
            }
        }

    }
    else if(picImageRow==3 && picImageColumn==4){
        int k=1;
        for(int i=0;i<picImageRow;i++){
            for(int j=0;j<picImageColumn;j++){
                myImageList[i][j]->setXPoint(mySettingRead->value("4picPointX" +QString::number(k,10)).toInt(&ok));
                myImageList[i][j]->setYPoint(mySettingRead->value("4picPointY" +QString::number(k,10)).toInt(&ok));
                k++;
            }
        }

    }
    else if(picImageRow==4 && picImageColumn==4){
        int k=1;
        for(int i=0;i<picImageRow;i++){
            for(int j=0;j<picImageColumn;j++){
                myImageList[i][j]->setXPoint(mySettingRead->value("5picPointX" +QString::number(k,10)).toInt(&ok));
                myImageList[i][j]->setYPoint(mySettingRead->value("5picPointY" +QString::number(k,10)).toInt(&ok));
                k++;
            }
        }
    }
}

//保存指令
void MainWindow::on_saveCommand_clicked()
{
   if(!ui->sendContextBox->currentText().trimmed().isEmpty()) {

    ui->sendContextBox->addItem(ui->sendContextBox->currentText());
   }

}
//保存命令终端命令
void MainWindow::saveCommandText()
{
    QString savePath=QCoreApplication::applicationDirPath();
    savePath.append("/command.txt");
    //创建写入文件
    QFile file(savePath);
    if(!file.exists()){
    file.open(QIODevice::WriteOnly);
    file.close();
    }
    //创建写入文件流
    if(!file.open(QIODevice::Text|QFile::WriteOnly|QIODevice::Truncate)){
        QMessageBox::information(this,QStringLiteral("文件写入失败"),file.errorString());
        return;
    }
    QTextStream fileOut(&file);
    for(int i=0;i<ui->sendContextBox->count();i++){
        if(!(ui->sendContextBox->itemText(i).simplified()==""))
        fileOut<<ui->sendContextBox->itemText(i)<<"\n";
    }

    file.flush();
    file.close();
}
//初始化开发者模式命令记忆
void MainWindow::initCommandText()
{
    QString savePath=QCoreApplication::applicationDirPath();
    QString readCommand;
    savePath.append("/command.txt");
    //创建写入文件
    QFile file(savePath);
    if(file.exists()){
         file.open(QIODevice::ReadOnly|QIODevice::Text);
         }
    QTextStream fileIn(&file);
    while(!fileIn.atEnd())
    ui->sendContextBox->addItem(fileIn.readLine());
    file.flush();
    file.close();
    }
//调用工作台
void MainWindow::on_actionShowFunction_triggered()
{
    ui->functionDockWidget2->show();
}
//串口关闭自动触发函数
 void MainWindow::isMySerialOpen()
 {
     myMotorDrive->isMySerialOpen(false);
     qDebug()<<"MainWindow closed!";
     isOpenSerial=false;
     ui->serialCB->setEnabled(true);
     ui->baudRateCB->setEnabled(true);
     ui->checkCB->setEnabled(true);
     ui->updateSerialButton->setEnabled(true);
     ui->dataBitCB->setEnabled(true);
     ui->stopBitCB->setEnabled(true);
     ui->openSerial->setText(QStringLiteral("打开串口"));
     ui->statusBar->showMessage(QStringLiteral("端口关闭"));
 }
 //连续发送指令
void MainWindow::on_constantCheckBox_clicked()
{
    if(ui->constantCheckBox->isChecked()){
        myMotorDrive->isConstantSend(true);
    }
    else{
         myMotorDrive->isConstantSend(false);
    }
}
