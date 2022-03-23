#ifndef MAPTHREAD_H
#define MAPTHREAD_H

#include <QObject>
#include"include.h"
#include"dataset.h"
class MapThread : public QObject
{
    Q_OBJECT
public:
    explicit MapThread(QObject *parent = nullptr);

    void filter_2(DataSet*dt,range r);
signals:
    void finish_2(QVector<int>nm);

public slots:
};

#endif // MAPTHREAD_H
