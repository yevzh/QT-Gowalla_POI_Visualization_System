#include "loadthread.h"
#include"include.h"
#include"dataset.h"
LoadThread::LoadThread(QObject *parent) : QObject(parent)
{

}
void LoadThread::working(QString *pt, DataSet *dt)
{
    QFile file(*pt);
    if(!file.open(QIODevice::ReadOnly))
        qDebug()<<"OPEN FILE FAILED!";
    QTextStream*out=new QTextStream(&file);
    QStringList tempOption=out->readAll().split('\n');
    for(int i=1;i<tempOption.count();i++)
    {
        QStringList tt=tempOption.at(i-1).split(",");

        dt->user.push_back(tt.at(0).toInt());

        QDateTime time=QDateTime::fromString(tt.at(2),"yyyy-MM-ddThh:mm:ssZ");

        int timeInterval=time.toTime_t();

        dt->timeIn.push_back(timeInterval);

        dt->location.push_back(tt.at(1).toInt());
        dt->longtitude.push_back(tt.at(4).toFloat());
        dt->latitude.push_back(tt.at(3).toFloat());

        emit progress(i);
    }
    emit finished();
}
