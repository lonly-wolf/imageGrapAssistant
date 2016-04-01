#ifndef MYLABEL_H
#define MYLABEL_H
#include<QLabel>
#include <QWidget>

class myLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myLabel(QLabel *parent = 0);
    ~myLabel();
    QPoint getCurrentPoint();
    QPoint getrelativePoint();
    void setCurrentPoint(QPoint *point); //设置当前位置坐标

signals:

private:
    QPoint nowMousePoint;
    QPoint relativeMousePoint;
    QPoint RTPPoint;


public slots:

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);




};

#endif // MYLABEL_H
