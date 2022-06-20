#include "timethread.h"
#include"include.h"


QT_CHARTS_USE_NAMESPACE
TimeThread::TimeThread(QObject *parent) : QObject(parent)
{

}
void TimeThread::filter(DataSet*dt,range r)
{
    uint ran[3]={604800,86400,3600};
    int x=(r.timeMax-r.timeMin)/ran[r.comboBox]+1;
//    QVector<int>rangeNum;
    int*p=new int[x];
    for(int i=0;i<x;i++)p[i]=0;
    for(int i=0;i<dt->user.size();i++)
    {
        int index=0;
        if((dt->user[i])>=r.userMin&&(dt->user[i])<=r.userMax&&(dt->location[i])>=r.locationMin&&(dt->location[i])<=r.locationMax&&(dt->timeIn[i])>=r.timeMin&&(dt->timeIn[i])<=r.timeMax&&(dt->latitude[i])>=r.latitudeMin&&(dt->latitude[i])<=r.latitudeMax&&(dt->longtitude[i])>=r.longtitudeMin&&(dt->longtitude[i])<=r.longtitudeMax)
        {
            index=(dt->timeIn[i]-r.timeMin)/ran[r.comboBox];
            p[index]++;
        }
//        qDebug()<<i;

//        emit progress1(i);
    }
    QVector<int>num_time;
    for(int i=0;i<x;i++)
    {
        num_time.push_back(p[i]);
    }
    emit finish(num_time);

}
