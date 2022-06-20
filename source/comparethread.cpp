#include "comparethread.h"
#include"include.h"
#include"dataset.h"
CompareThread::CompareThread(QObject *parent) : QObject(parent)
{

}
void CompareThread::filter_3(DataSet *dt, userComparison u)
{
    uint ran[3]={604800,86400,3600};
    int x=(u.timeMax-u.timeMin)/ran[u.comboBox]+1;

    QVector<int>c1;
    QVector<int>c2;
    int*p1=new int[x];
    int*cover1=new int[50835];
    int*cover2=new int[50835];
    int*p2=new int[x];
    for(int i=0;i<x;i++)
    {
        p1[i]=0;
        p2[i]=0;
    }
    for(int i=0;i<50835;i++)
    {
        cover1[i]=0;
        cover2[i]=0;
    }
//    int de=0;
    for(int i=0;i<dt->user.size();i++)
    {
        int index=0;
        if((dt->timeIn[i])>=u.timeMin&&(dt->timeIn[i])<=u.timeMax)
        {
            index=(dt->timeIn[i]-u.timeMin)/ran[u.comboBox];
            if(dt->user[i]==u.user1)
            {
                p1[index]++;
                cover1[dt->location[i]]++;

//                qDebug()<<dt->location[i];
//                qDebug()<<cover1[dt->location[i]];
            }
            else if(dt->user[i]==u.user2)
            {
                p2[index]++;
                cover2[dt->location[i]]++;
            }
        }
    }

//    qDebug()<<cover1[113];
    for(int i=0;i<50835;i++)
    {
        c1.push_back(cover1[i]);
//        if(de<cover1[i])de=cover1[i];
        c2.push_back(cover2[i]);
    }
    QVector<int>user1_nt;
    QVector<int>user2_nt;
    for(int i=0;i<x;i++)
    {
        user1_nt.push_back(p1[i]);
        user2_nt.push_back(p2[i]);
    }
//    qDebug()<<de;
    delete[]cover1;
    delete[]cover2;
    emit finish_3(user1_nt,user2_nt,c1,c2);

}
