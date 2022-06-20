#include "dauthread.h"

DAUThread::DAUThread(QObject *parent) : QObject(parent)
{

}
void DAUThread::filter_4(DataSet *dt, locationComparison lo)
{
    QVector<int>d1;
    QVector<int>d2;
    QVector<int>c;
    int*dom1=new int[22325];
    int*dom2=new int[22325];
    int*cover=new int[50835];
    for(int i=0;i<22325;i++)
    {
        dom1[i]=0;
        dom2[i]=0;
    }
    for(int i=0;i<50835;i++)
    {
        cover[i]=0;
    }

    for(int i=0;i<dt->user.size();i++)
    {
        if(dt->timeIn[i]>=lo.time&&dt->timeIn[i]<=(lo.time+86400))
        {
            cover[dt->location[i]]++;
            if(dt->location[i]==lo.location1)
            {
                dom1[dt->user[i]]++;
            }
            else if(dt->location[i]==lo.location2)dom2[dt->user[i]]++;
        }
    }
    for(int i=0;i<22325;i++)
    {
        d1.push_back(dom1[i]);
        d2.push_back(dom2[i]);
    }
    for(int i=0;i<50835;i++)
    {
        c.push_back(cover[i]);
    }
    delete []dom1;
    delete []dom2;
    delete []cover;
    emit finish_4(d1,d2,c);

}
