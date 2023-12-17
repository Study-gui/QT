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
    void on_actionabout_triggered();

    void on_toolButton_5_triggered(QAction *arg1);

    void on_toolButton_triggered(QAction *arg1);

    void on_toolButton_2_triggered(QAction *arg1);

    void on_toolButton_3_triggered(QAction *arg1);

    void on_toolButton_4_triggered(QAction *arg1);

    void on_toolButton_6_triggered(QAction *arg1);

    void on_toolButton_6_clicked();

    void on_toolButton_5_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
