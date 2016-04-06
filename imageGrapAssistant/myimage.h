#ifndef MYIMAGE_H
#define MYIMAGE_H
#include <QWidget>
#include<picedit.h>

class QImage;
class QLabel;
class QSpinBox;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class myImage : public QWidget
{
    Q_OBJECT
public:
    explicit myImage(QWidget *parent = 0);
    ~myImage();
    void myImageInit();
    QImage * getMyImage();
    void setMyImage(QImage *image);
    QPoint getImageLocation();
    void setXPoint(int x);
    void setYPoint(int y);
    int getXPoint();
    int getYPoint();



signals:


public slots:
    void changeMyPoint();
    void editPicture(); //编辑图片
    void savePicture();//保存图片

private:
    QImage *image;
    QLabel *label;
    QLabel *imageLabel;
    QSpinBox *pointXSpinBox;
    QSpinBox *pointYSpinBox;
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QVBoxLayout *vboxLayout2;
    QPushButton *changePoint;
    QImage *myimage; //控件背景图
    QPoint *ImageCenterP;
    picEdit *picedit;
    bool isClickChange;

    void setImageLocation(); //设置图像采集中心
    void contextMenuEvent(QContextMenuEvent *event); //右键菜单




};

#endif // MYIMAGE_H
