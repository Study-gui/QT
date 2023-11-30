#ifndef STUDENT_H
#define STUDENT_H

#include <QWidget>
#include<QString>
class Student : public QWidget
{
    Q_OBJECT
public:
    explicit Student(QWidget *parent = nullptr);
    void treat();
    void treat(QString what);
signals:
};

#endif // STUDENT_H
