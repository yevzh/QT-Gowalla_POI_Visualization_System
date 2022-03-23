#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include <QObject>
#include"include.h"
#include"dataset.h"

class TimeThread : public QObject
{
    Q_OBJECT
public:
    explicit TimeThread(QObject *parent = nullptr);
    void filter(DataSet*dt,range r);
signals:
    void finish(QVector<int>nt);
public slots:
};

#endif // TIMETHREAD_H
