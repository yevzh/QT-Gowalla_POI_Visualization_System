#include "predictthread.h"
PredictThread::PredictThread(QObject *parent) : QObject(parent)
{

}
void PredictThread::userPrediction(DataSet *dt, pred pre)
{
    QVector<int>seqLocation;
    QVector<int>seqList;
    int tmp[50835]={0};
    int hash[50835]={0};
    for(int i=0;i<dt->user.size();i++)
    {
        if(pre.interestinguser==dt->user[i])
        {
            seqLocation.push_back(dt->location[i]);
            tmp[dt->location[i]]++;
        }
    }
    int d=0;
    for(int i=0;i<50835;i++)
    {
        if(tmp[i]!=0)
        {
            hash[i]=d;
            d++;
        }
    }
//    qDebug()<<d;
    //一共d个数
    int num=seqLocation.size();
    for(int i=0;i<num;i++)
    {
        seqList.push_back(hash[seqLocation[i]]);
//        qDebug()<<hash[seqLocation[i]];
    }

//    for(int i=0;i<num;i++)qDebug()<<seqList[i];


    double matrix[d][d];
    for(int i=0;i<d;i++)
    {
        for(int j=0;j<d;j++)
        {
            matrix[i][j]=0;
        }
    }
    for(int i=0;i<num-1;i++)
    {
        matrix[seqList[i]][seqList[i+1]]++;
//        qDebug()<<matrix[seqList[i]][seqList[i+1]];
    }

//    for(int j=0;j<d-1;j++)qDebug()<<matrix[0][j];

    for(int i=0;i<d;i++)
    {
        double sum=0;
        for(int j=0;j<d;j++)
        {
            if(matrix[i][j]!=0)sum++;
        }
//        qDebug()<<sum;
        if(sum!=0)
        {
            for(int j=0;j<d;j++)
            {
                if(matrix[i][j]!=0)
                {
                    matrix[i][j]=1;
                    matrix[i][j] /= sum;
                }
            }
        }
        else
        {
            for(int j=0;j<d;j++)
            {
                matrix[i][j]=1;
                matrix[i][j]/=d;
            }
        }
    }
    double init[d],temp[d];
    for(int i=0;i<d;i++)
    {
        init[i]=0;
        temp[i]=0;
    }
//    qDebug()<<seqList[num-1];
    for(int i=0;i<d;i++)
    {
        if(i==seqList[num-1])
        {
            init[i]=1;
            temp[i]=1;
        }
    }
    int dd=0;
    while(1)
    {
        for(int i=0;i<d;i++)
        {
            double k=0;
            for(int j=0;j<d;j++)
            {
                k+=init[j] * matrix[j][i];
            }
            temp[i]=k;
//            qDebug()<<k;
        }
        for(int i=0;i<d;i++)
        {
            init[i]=temp[i];
        }
        ++dd;
        if(dd==100)break;
    }
    double max=0;
    int maxn;
    for(int i=0;i<d;i++)
    {
//        qDebug()<<init[i];
        if(max<init[i])
        {
            max=init[i];
            maxn=i;
        }
    }
//    qDebug()<<maxn;
    for(int i=0;i<50835;i++)
    {
        if(hash[i]==maxn)
        {
//            qDebug()<<i;
            emit finish_up(i);
            break;
        }
    }







}

void PredictThread::poiPrediction(DataSet *dt, pred pre)
{
    //SEP-01-2010 to OCT-18-2010
    QDateTime front=QDateTime::fromString("2010-07-01 00:00:00","yyyy-MM-dd hh:mm:ss");
    uint frontTime=front.toTime_t();
    QDateTime rear=QDateTime::fromString("2010-10-17 23:59:59","yyyy-MM-dd hh:mm:ss");
    uint rearTime=rear.toTime_t();
    int x=(rearTime-frontTime)/86400+1;
    int*p=new int[x];
    for(int i=0;i<x;i++)
    {
        p[i]=0;
    }
    int index;
    for(int i=0;i<dt->user.size();i++)
    {
        if(dt->location[i]==pre.pointofinterest&&dt->timeIn[i]>=frontTime&&dt->timeIn[i]<=rearTime)
        {
            index=(dt->timeIn[i]-frontTime)/86400;
            p[index]++;
        }
    }

    int dd1=0,dd2=0,dd3=0;
    int count1=x-6,count2=x-5,count3=x-4;
    QVector<int>la1;
    QVector<int>la2;
    QVector<int>la3;
    while(count1>0)
    {
        la1.push_back(p[count1]);
        count1-=7;
        dd1++;
    }
    while(count2>0)
    {
        la2.push_back(p[count2]);
        count2-=7;
        dd2++;
    }
    while(count3>0)
    {
        la3.push_back(p[count3]);
        count3-=7;
        dd3++;
    }

    int num1=la1.size();
    double ini1=(la1[0]+la1[1]+la1[2])/3;
    for(int i=3;i<num1;i++)
    {
        ini1=(ini1+la1[i])/2;
    }

    int num2=la2.size();
    double ini2=(la2[0]+la2[1]+la2[2])/3;
    for(int i=3;i<num2;i++)
    {
        ini2=(ini2+la2[i])/2;
    }

    int num3=la3.size();
    double ini3=(la3[0]+la3[1]+la3[2])/3;
    for(int i=3;i<num3;i++)
    {
        ini3=(ini3+la3[i])/2;
    }


    int a=ini1;

    int b=ini2;
    int c=ini3;
    emit finish_lp(a,b,c);




}
