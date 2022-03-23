#ifndef HEATTHREAD_H
#define HEATTHREAD_H

#include <QObject>
#include"include.h"
#include"dataset.h"
class HeatThread : public QObject
{
    Q_OBJECT
public:
    explicit HeatThread(QObject *parent = nullptr);

    void filter_h(DataSet*dt,locationComparison lo);
signals:
    void finish_h(QVector<int>xx,QVector<int>yy);

public slots:
};

#endif // HEATTHREAD_H
