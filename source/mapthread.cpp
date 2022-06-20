#include "mapthread.h"
#include"include.h"
#include"dataset.h"

MapThread::MapThread(QObject *parent) : QObject(parent)
{

}

void MapThread::filter_2(DataSet*dt,range r)
{
    int row=18,column=36;
    int pp[row][column]={0};
    for(int i=0;i<dt->user.size();i++)
    {
        if((dt->user[i])>=r.userMin&&(dt->user[i])<=r.userMax&&(dt->location[i])>=r.locationMin&&(dt->location[i])<=r.locationMax&&(dt->timeIn[i])>=r.timeMin&&(dt->timeIn[i])<=r.timeMax&&(dt->latitude[i])>=r.latitudeMin&&(dt->latitude[i])<=r.latitudeMax&&(dt->longtitude[i])>=r.longtitudeMin&&(dt->longtitude[i])<=r.longtitudeMax)
        {
            int indexx=(dt->latitude[i]+90)/10;
            int indexy=(dt->longtitude[i]+180)/10;
            pp[indexx][indexy]++;
        }

    }
    QVector<int>num_map;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            num_map.push_back(pp[i][j]);
        }
    }
    emit finish_2(num_map);



}
