#ifndef MYPEN_H
#define MYPEN_H
#include<QLabel>
#include <QWidget>


class QPoint;
class myPen : public QLabel
{
    Q_OBJECT

public:
     ~myPen();
    explicit myPen(QLabel *parent = 0);
    bool isClicking;
    bool isPen;
    QImage myImage;
    QMouseEvent *mouseEvent2;
    void setImage(QString name);
    void setImageByImage(QImage *image);
    void setImageWidth(int width);
    void setImageHeight(int height);
    void setImageRotate(QMatrix matrix);
    QImage getMyImage();
    QPoint beginPoint;//原始坐标

signals:

public slots:

private:

     QPoint currentPoint;
     QPoint beginPointer;
     QPoint originalPoint;
     QPainter *myPainter;
     QPainter *showPicPainter;
     QList<QPointF> pointArray;
     QPointF straightLine[2];
     QString imageName;
     QMatrix myMatrix;


     QPainter *linePainter;
     bool isLoadPic;

protected:
     void paintEvent(QPaintEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void mousePressEvent(QMouseEvent *event);
};

#endif // myPen_H
