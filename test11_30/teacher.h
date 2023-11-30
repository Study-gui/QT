#ifndef TEACHER_H
#define TEACHER_H

#include <QWidget>
#include<QString>
class Teacher : public QWidget
{
    Q_OBJECT
public:
    explicit Teacher(QWidget *parent = nullptr);

signals:
    void hungry();
    void hungry(QString what);
};

#endif // TEACHER_H
