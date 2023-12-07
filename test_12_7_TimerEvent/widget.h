#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    int timerId1;
    int timerId2;
    ~Widget();
protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void on_pushButtonstart_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButtonstart_2_clicked();

    void on_pushButton_stop_2_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
