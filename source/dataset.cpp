#include "dataset.h"
#include "include.h"
DataSet::DataSet(QObject *parent) : QObject(parent)
{

}
void DataSet::getUser(int num)
{
    user.push_back(num);
}
void DataSet::getLocation(int num)
{
    location.push_back(num);
}
void DataSet::getLat(float num)
{
    latitude.push_back(num);
}
void DataSet::getLon(float num)
{
    longtitude.push_back(num);
}
void DataSet::getTime(uint time)
{
    timeIn.push_back(time);
}
