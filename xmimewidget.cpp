#include "xmimewidget.h"
#include "ui_xmimewidget.h"

XMIMEWidget::XMIMEWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XMIMEWidget)
{
    ui->setupUi(this);
}

XMIMEWidget::~XMIMEWidget()
{
    delete ui;
}
