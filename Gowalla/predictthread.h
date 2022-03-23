#ifndef PREDICTTHREAD_H
#define PREDICTTHREAD_H

#include <QObject>
#include "dataset.h"
#include "include.h"
class PredictThread : public QObject
{
    Q_OBJECT
public:
    explicit PredictThread(QObject *parent = nullptr);

    void userPrediction(DataSet*dt,pred pre);
    void poiPrediction(DataSet*dt,pred pre);
signals:
    void finish_up(int j);
    void finish_lp(int a,int b,int c);

public slots:
};

#endif // PREDICTTHREAD_H
