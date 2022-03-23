#include "heatthread.h"

HeatThread::HeatThread(QObject *parent) : QObject(parent)
{

}

void HeatThread::filter_h(DataSet *dt, locationComparison lo)
{

    QVector<int>xx;
    QVector<int>yy;
    for(int i=0;i<dt->user.size();i++)
    {
        if(dt->timeIn[i]>=lo.time&&dt->timeIn[i]<=(lo.time+86400))
        {
            int x=(dt->longtitude[i]+180)*4;
            int y=(90-dt->latitude[i])*4;
            xx.push_back(x);
            yy.push_back(y);
        }
    }
    emit finish_h(xx,yy);

}
