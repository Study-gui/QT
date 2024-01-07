#ifndef STUDENT_H
#define STUDENT_H
#include<QString>
class Student
{
public:
    Student()
    {

    }
    Student(QString name, int id)
    {
        this->m_name = name;
        this->m_id = id;
    }


    QString m_name;
    int m_id;
};
#endif // STUDENT_H
