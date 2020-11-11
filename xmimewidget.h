#ifndef XMIMEWIDGET_H
#define XMIMEWIDGET_H

#include <QWidget>

namespace Ui {
class XMIMEWidget;
}

class XMIMEWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XMIMEWidget(QWidget *parent = nullptr);
    ~XMIMEWidget();

private:
    Ui::XMIMEWidget *ui;
};

#endif // XMIMEWIDGET_H
