#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actiondia_triggered();

    void on_actioninfomation_triggered();

    void on_actionwarning_triggered();

    void on_actioncritical_triggered();

    void on_actionquestion_triggered();

    void on_actionFiledialog_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
