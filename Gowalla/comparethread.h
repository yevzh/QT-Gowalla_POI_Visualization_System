#ifndef COMPARETHREAD_H
#define COMPARETHREAD_H

#include <QObject>
#include"include.h"
#include"dataset.h"
class CompareThread : public QObject
{
    Q_OBJECT
public:
    explicit CompareThread(QObject *parent = nullptr);

    void filter_3(DataSet*dt,userComparison u);
signals:

    void finish_3(QVector<int>user_nt1,QVector<int>user_nt2,QVector<int>c1,QVector<int>c2);
public slots:
};

#endif // COMPARETHREAD_H
