#ifndef INCLUDE_H
#define INCLUDE_H

#include<QMainWindow>
#include<QObject>
#include<QWidget>
#include<QVector>
#include<QString>
#include<QThread>
#include<QStringList>
#include<QDebug>
#include<QFileDialog>
#include<QFile>
#include<QDateTime>
#include<QtCharts>
#include<QSplineSeries>
#include<QChart>
#include<QLineSeries>
#include<QChartView>
#include<QDateTimeAxis>
#include<QComboBox>
#include<QSplineSeries>
#include<Q3DSurface>
#include<Q3DBars>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QFont>
#include<QFontComboBox>
#include <Qt3DExtras/Qt3DWindow>
#include<QBarSet>
#include<QBarSeries>

struct range{
    int userMin;
    int userMax;
    int locationMin;
    int locationMax;
    uint timeMin;
    uint timeMax;
    float latitudeMin;
    float latitudeMax;
    float longtitudeMin;
    float longtitudeMax;
    int comboBox;
};

struct userComparison{
    int user1;
    int user2;
    uint timeMin;
    uint timeMax;
    int comboBox;
};

struct locationComparison{
    int location1;
    int location2;
    uint time;
};

struct userPie{
    int loc;
    int num;
};

struct pred{
    int pointofinterest;
    int interestinguser;
};







#endif // INCLUDE_H
