#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QMainWindow>

namespace Ui {
class aboutDialog;
}

class aboutDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit aboutDialog(QWidget *parent = 0);
    ~aboutDialog();
private slots:
    void on_label_9_linkActivated(const QString &link);

private:
    Ui::aboutDialog *ui;

};

#endif // ABOUTDIALOG_H
