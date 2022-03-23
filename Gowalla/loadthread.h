#ifndef LOADTHREAD_H
#define LOADTHREAD_H

#include <QObject>
#include"include.h"
#include"dataset.h"

class LoadThread : public QObject
{
    Q_OBJECT
public:
    explicit LoadThread(QObject *parent = nullptr);
    void working(QString *pt,DataSet*dt);

signals:
    void finished();
    void progress(int i);

public slots:
};

#endif // LOADTHREAD_H
