#ifndef DATASET_H
#define DATASET_H

#include <QObject>
#include"include.h"

class DataSet : public QObject
{
    Q_OBJECT


private:

public:
    explicit DataSet(QObject *parent = nullptr);
    QVector<int>user;
    QVector<int>location;
    QVector<float>latitude;
    QVector<float>longtitude;
    QVector<uint>timeIn;
    void getUser(int num);
    void getLocation(int num);
    void getLat(float num);
    void getLon(float num);
    void getTime(uint time);

//    QVector<int> user_get()
//    {
//        return user;
//    }

//    QVector<int> location_get()
//    {
//        return location;
//    }

//    QVector<float>latitude_get()
//    {
//        return latitude;
//    }

//    QVector<float>longtitude_get()
//    {
//        return longtitude;
//    }

//    QVector<int>timeIn_get()
//    {
//        return timeIn;
//    }

signals:

public slots:
};

#endif // DATASET_H
