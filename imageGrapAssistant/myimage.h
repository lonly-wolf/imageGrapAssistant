#ifndef MYIMAGE_H
#define MYIMAGE_H
#include <QWidget>

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
    QImage getMyImage();
    void setMyImage(QImage *image);
    QPoint getImageLocation();
    void setXPoint(int x);
    void setYPoint(int y);
    int getXPoint();
    int getYPoint();



signals:


public slots:
    void changeMyPoint();

private:
    QImage *image;
    QLabel *label;
    QLabel *imageLabel;
    QSpinBox *pointXSpinBox;
    QSpinBox *pointYSpinBox;
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QPushButton *changePoint;
    QImage image2; //控件背景图
    QPoint *ImageCenterP;
    bool isClickChange;

    void setImageLocation(); //设置图像采集中心



};

#endif // MYIMAGE_H
