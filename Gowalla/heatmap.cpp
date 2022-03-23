#include "heatmap.h"
#include "ui_heatmap.h"

HeatMap::HeatMap(QWidget *parent,QVector<int>x,QVector<int>y) :
    QMainWindow(parent),
    ui(new Ui::HeatMap)
{
    ui->setupUi(this);

    for(int i=0;i<x.size();i++)
    {
        dx.push_back(x[i]);
        dy.push_back(y[i]);
    }

    setFixedSize(ImgWidth,ImgHeight);


    _dataImg=QImage(ImgWidth,ImgHeight,QImage::Format_Alpha8);
    _dataImg.fill(Qt::transparent);

    _heatImg=QImage(ImgWidth,ImgHeight,QImage::Format_ARGB32);
    _heatImg.fill(Qt::transparent);


    QLinearGradient linear=QLinearGradient(QPoint(0,0),QPoint(255,0));
    linear.setColorAt(0, Qt::blue);
    linear.setColorAt(0.4, Qt::blue);
    linear.setColorAt(0.5, Qt::cyan);
    linear.setColorAt(0.6, Qt::green);
    linear.setColorAt(0.8, Qt::yellow);
    linear.setColorAt(0.95, Qt::red);


    QImage img(256,1,QImage::Format_ARGB32);
    QPainter painter(&img);
    painter.fillRect(img.rect(),linear);


    quint32 alpha=0;
    for(quint32 i=0;i<256;i++){

        alpha=HeatAlpha/255.0*i;
        //typedef unsigned int QRgb: format #AARRGGBB

        _colorList[i]=img.pixel(i,0)&0x00FFFFFF|(alpha<<24);
    }
//    for(int i=0;i<dx.size();i++)
//    {
//        paintMap(i);
//    }



}

HeatMap::~HeatMap()
{
    delete []_countTable;
    delete ui;
}


void HeatMap::paintEvent(QPaintEvent *event)
{
    QPainter p(this);


    p.setPen(QPen(Qt::black,2));
    for(int i=0;i<width();i+=40){
        p.drawLine(i,0,i,height());
    }
    for(int i=0;i<height();i+=40){
        p.drawLine(0,i,width(),i);
    }

    p.drawImage(0,0,_heatImg);
    QMainWindow::paintEvent(event);
}

void HeatMap::paintMap(int n)
{


        const Point pt{dx[n],dy[n],80,1};
        const int pos_count=appendPoint(pt);

        if(pos_count>1)
        {
            if(pos_count>_maxCount)
                _maxCount=pos_count;
            drawDataImg();
        }
        else
        {
            drawDataPoint(pt);
        }
        drawHeatImg();
        update();

}

int HeatMap::appendPoint(const Point &pt)
{

    if(pt.posX<0||pt.posY<0||pt.radius<0||
            pt.posX>=ImgWidth||pt.posY>=ImgHeight)
        return 0;

    if(_countTable[pt.posX+pt.posY*ImgWidth]>0){
        for(Point &the_pos:_posList)
        {
            if(the_pos.posX==pt.posX&&the_pos.posY==pt.posY){

                the_pos.count+=pt.count;
                break;
            }
        }
    }else{
        _posList.push_back(pt);
    }
    _countTable[pt.posX+pt.posY*ImgWidth]+=pt.count;
    return _countTable[pt.posX+pt.posY*ImgWidth];
}

void HeatMap::drawDataImg()
{

    _dataImg.fill(Qt::transparent);
    QPainter painter(&_dataImg);
    painter.setPen(Qt::transparent);

    const double max_count=_maxCount;
    for(int i=0;i<_posList.count();i++)
    {
        const Point &pt=_posList.at(i);

        const uchar alpha=uchar(_countTable[pt.posX+pt.posY*ImgWidth]/max_count*255);
        QRadialGradient gradient(pt.posX,pt.posY,pt.radius);
        gradient.setColorAt(0,QColor(0,0,0,alpha));
        gradient.setColorAt(1,QColor(0,0,0,0));
        painter.setBrush(gradient);
        painter.drawEllipse(QPointF(pt.posX,pt.posY),pt.radius,pt.radius);
    }
}


void HeatMap::drawDataPoint(const Point &pt)
{
    QPainter painter(&_dataImg);
    painter.setPen(Qt::transparent);


    const uchar alpha=uchar(_countTable[pt.posX+pt.posY*ImgWidth]/(double)_maxCount*255);

    QRadialGradient gradient(pt.posX,pt.posY,pt.radius);
    gradient.setColorAt(0,QColor(0,0,0,alpha));
    gradient.setColorAt(1,QColor(0,0,0,0));
    painter.setBrush(gradient);
    painter.drawEllipse(QPointF(pt.posX,pt.posY),pt.radius,pt.radius);
}

void HeatMap::drawHeatImg()
{

    for(int row=0;row<_dataImg.height();row++)
    {
        //dataimg QImage::Format_Alpha8，一个点1个字节
        const uchar *line_data=_dataImg.scanLine(row);
        //heatimg QImage::Format_ARGB32，一个点4个字节
        QRgb *line_heat=reinterpret_cast<QRgb*>(_heatImg.scanLine(row));
        for(int col=0;col<_dataImg.width();col++)
        {

            line_heat[col]=_colorList[line_data[col]];
        }
    }
}


