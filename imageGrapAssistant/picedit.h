#ifndef PICEDIT_H
#define PICEDIT_H
#include <QMainWindow>
#include"myPen.h"

class QPainter;
class QLabel;

namespace Ui {
class picEdit;
}

class picEdit : public QMainWindow
{
    Q_OBJECT
    friend class myImage;

public:
    QImage   *myImage2; //经过处理的图片
    explicit picEdit(QWidget *parent = 0);
    ~picEdit();
    Ui::picEdit *ui;
    QPainter *painter;
    QImage   myImage;//图片
    QImage *myImage3; //保存原始图片
    QString imageName;//图片路径
    QTransform transform; //图片旋转
    QMatrix matrix; //图片旋转角度
    QPoint beginPoint;//原始坐标
    QPoint lastPoint;//前一次放大时的坐标
    myPen *drawLabel; //重写label
    int imageWidth;          //scrollArea中心点
    int imageHeight;
    int picSize; //图片放大/缩小的倍数
    int picPosition;//图片旋转角度
    int lastPosition; //图片的上一次的角度
    bool hasTrans;  //判断上一个动作是旋转
    bool isPen;
    int picSizeCount; //放大、缩小倍数
    int picSliderCount; //滑块上一次的值
    int initWidth;//图像初始高度
    int initHeight;//图像初始宽度

    void drawPaint(); //绘制图形（测试版）
    void transformImage();// 调整图像位置 大小
    QImage * greyScale(QImage *image); //灰度调节
    QImage * lightScale(QImage *image,int lightSize); //亮度调节
    QImage * warmScale(QImage *image,int warmSize);//暖色调节
    QImage * mohuScale(QImage *image);//模糊调节
    QImage * sharpScale(QImage *image);//锐化调节
private slots:
    void on_actionOpen_triggered(); //菜单栏打开图片
    void on_imageRotate_sliderMoved(int position);//旋转图片
    void on_pushButton_clicked(); //放大图片
    void on_pushButton_2_clicked();//缩小图片
    void on_horizontalSlider_sliderMoved(int position); //显示比例
    void on_actionSave_triggered(); //保存图片
    void on_penButton_clicked();//画笔点击事件
    void on_grayAction_triggered();//灰度调节
    void on_lightAdd15_triggered();
    void on_lightAdd30_triggered();
    void on_lightAdd45_triggered();
    void on_lightAdd65_triggered();
    void on_lightAdd85_triggered();
    void on_lightAdd100_triggered();
    void on_warmAction2_triggered();
    void on_mohuAction_triggered();
    void on_sharpAction_triggered();


protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
  //  bool eventFilter(QObject *obj,QEvent *event);   //事件过滤器
};

#endif // PICEDIT_H
