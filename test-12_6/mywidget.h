#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

namespace Ui {
class Mywidget;
}

class Mywidget : public QWidget
{
    Q_OBJECT

public:
    explicit Mywidget(QWidget *parent = nullptr);
    ~Mywidget();
    int getvalue();
    void setvalue(int value);

private:
    Ui::Mywidget *ui;
};

#endif // MYWIDGET_H
