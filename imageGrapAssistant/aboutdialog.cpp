#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
    ui->label_9->setOpenExternalLinks(true);
    ui->label_9->setTextFormat(Qt::RichText);
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::on_label_9_linkActivated(const QString &link)
{

}
