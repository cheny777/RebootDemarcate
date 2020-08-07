#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "pointset.h"
#include <vector>
#include <QPushButton>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QQueue>
#include <algorithm>


#define LINESNUMBER 100
using namespace std;
using namespace QtCharts;


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    /*采集按钮容器*/
    vector<QPushButton *> btn_vector_collection;
    vector<QPushButton *> btn_vector_save;
    vector<QPushButton *> btn_vector_load;
    bool bl_collection[3];

    /*采集计数*/
    long long_num;

    /*time ID*/
    int timeID;
    /*空载点列*/
    CPointSet *point_no_load;
    /*横向空载*/
    CPointSet *point_crosswise_no_load;
    /*挂载数据*/
    CPointSet *point_mount;

    vector<CPointSet*> m_vector_point;


    /*绘图框*/
    QChartView *m_chartview;
    QChart *m_chart;
    QLineSeries *m_firstline;
    QLineSeries *m_secondline;

    void initchart();
    void updataChartview(int minc,int maxc);
    QQueue<double > m_fristqueue;
    QQueue<double > m_secondqueue;

    /*ip地址*/
    char ipchar[4];
    /*零位偏移量*/
    double m_angle;
protected:
    void timerEvent(QTimerEvent *);

private slots:
    void slotConnectNet();
    void slotCIConfig();

    void slotCollection();
    void slotSavefile();

    /*标定并同步*/
    void slotDemarcate();
    /*加载文件*/
    void slotLoadFile();

    /*零位偏移量*/
    void slotZero();




};

#endif // WIDGET_H
