#ifndef DAUTHREAD_H
#define DAUTHREAD_H

#include <QObject>
#include"dataset.h"
class DAUThread : public QObject
{
    Q_OBJECT
public:
    explicit DAUThread(QObject *parent = nullptr);

    void filter_4(DataSet*dt,locationComparison lo);
signals:
    void finish_4(QVector<int>lc_1,QVector<int>lc_2,QVector<int>cc);
public slots:
};

#endif // DAUTHREAD_H
