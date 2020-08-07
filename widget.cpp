#include "widget.h"
#include "ui_widget.h"
#include "ciportal.h"
#include "fileportal.h"
#include <iostream>
#include <QHBoxLayout>
#include <QFileDialog>
#include <cfile.h>

using namespace std;


#define MARKPTAH "./FSENSOR_DEFAULT_C.RCF"
#define MARKLOAD "./TOOL_MASS_C.RCF"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("机器人标定");
    connect(ui->btn_connectNet,SIGNAL(clicked(bool)),this,SLOT(slotConnectNet()));
    connect(ui->btn_CIPathConfig,SIGNAL(clicked(bool)),this ,SLOT(slotCIConfig()));

    connect(ui->btn_collection_1,SIGNAL(clicked(bool)),this,SLOT(slotCollection()));
    connect(ui->btn_collection_2,SIGNAL(clicked(bool)),this,SLOT(slotCollection()));
    connect(ui->btn_collection_3,SIGNAL(clicked(bool)),this,SLOT(slotCollection()));

    connect(ui->btn_save_1,SIGNAL(clicked(bool)),this,SLOT(slotSavefile()));
    connect(ui->btn_save_2,SIGNAL(clicked(bool)),this,SLOT(slotSavefile()));
    connect(ui->btn_save_3,SIGNAL(clicked(bool)),this,SLOT(slotSavefile()));

    connect(ui->btn_load1,SIGNAL(clicked(bool)),this,SLOT(slotLoadFile()));
    connect(ui->btn_load2,SIGNAL(clicked(bool)),this,SLOT(slotLoadFile()));
    connect(ui->btn_load3,SIGNAL(clicked(bool)),this,SLOT(slotLoadFile()));


    connect(ui->btn_zero,SIGNAL(clicked(bool)),this,SLOT(slotZero()));
    btn_vector_load.push_back(ui->btn_load1);
    btn_vector_load.push_back(ui->btn_load2);
    btn_vector_load.push_back(ui->btn_load3);

    btn_vector_collection.push_back(ui->btn_collection_1);
    btn_vector_collection.push_back(ui->btn_collection_2);
    btn_vector_collection.push_back(ui->btn_collection_3);
    btn_vector_save.push_back(ui->btn_save_1);
    btn_vector_save.push_back(ui->btn_save_2);
    btn_vector_save.push_back(ui->btn_save_3);
    bl_collection[0] = true;
    bl_collection[1] = true;
    bl_collection[2] = true;

    initchart();

    point_no_load = new CPointSet;
    point_crosswise_no_load = new CPointSet;
    point_mount = new CPointSet;

    point_no_load->OnInit(PoinType::PotT_2d);
    point_crosswise_no_load->OnInit(PoinType::PotT_2d);
    point_mount->OnInit(PoinType::PotT_2d);

    m_vector_point.push_back(point_no_load);
    m_vector_point.push_back(point_crosswise_no_load);
    m_vector_point.push_back(point_mount);

    connect(ui->btn_Demarcate,SIGNAL(clicked(bool)),this,SLOT(slotDemarcate()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initchart()
{
    m_chartview = new QChartView(ui->widget);
    QHBoxLayout *layout = new QHBoxLayout(ui->widget);
    layout->addWidget(m_chartview);
    layout->setSpacing(0);
    layout->setMargin(0);

    m_chart = new QChart;
    m_chartview->setChart(m_chart);
    /*防止图形走样*/
    m_chartview->setRenderHint(QPainter::Antialiasing);

    /*初始化数据源*/
    m_firstline = new QLineSeries;
    m_secondline = new QLineSeries;

    m_firstline->setPen(QPen(Qt::red,1,Qt::SolidLine));
    m_secondline->setPen(QPen(Qt::blue,1,Qt::SolidLine));

    //m_chart->setTitle("力量曲线");
    m_chart->addSeries(m_firstline);
    m_chart->addSeries(m_secondline);

    /*创建默认坐标系*/
    m_chart->createDefaultAxes();
    /*设置边距*/
    QMargins mar(0,0,0,0);
    m_chart->setMargins(mar);
    /*返回图例对象*/
    QLegend *le = m_chart->legend();
    le->setVisible(false);
    /*设置范围*/
    m_chart->axisX()->setMin(0);
    m_chart->axisX()->setMax(110);

    m_chart->axisY()->setMin(-500);
    m_chart->axisY()->setMax(500);
}

void Widget::updataChartview(int minc, int maxc)
{
    m_firstline->clear();
    m_secondline->clear();
    for(int i =0;i<m_secondqueue.size();i++)
    {
        m_firstline->append(i,m_fristqueue[i]);
        m_secondline->append(i,m_secondqueue[i]);
    }
}

void Widget::slotConnectNet()
{
    QString str_ip = ui->lineEdit->text();
    QStringList str_list_ip = str_ip.split(".");

    ipchar[0] = str_list_ip[0].toInt();
    ipchar[1] = str_list_ip[1].toInt();
    ipchar[2] = str_list_ip[2].toInt();
    ipchar[3] = str_list_ip[3].toInt();

    if(OnLinkMachine(0, ipchar[0], ipchar[1], ipchar[2], ipchar[3]))
    {
        ui->btn_connectNet->setEnabled(false);
        ui->label_message->setText("连接成功");
    }
    else
    {
        ui->label_message->setText("连接失败");
    }

}

void Widget::slotCIConfig()
{
    QStringList str_list_CI = ui->lineEdit_2->text().split(".");

    long long_CI[str_list_CI.size()];
    for(int i =0;i<str_list_CI.size();i++)
    {
        long_CI[i] = str_list_CI[i].toInt();
    }

    if(OnRegCiValue(0,long_CI,str_list_CI.size(),0.002,0.002))
    {
        ui->label_message->setText("设置成功");
    }
    else
    {
        ui->label_message->setText("设置失败");
    }
}

void Widget::slotCollection()
{
    for(int i =0;i<btn_vector_collection.size();i++)
    {
        if(btn_vector_collection[i] == sender())
        {
            bl_collection[i] = !bl_collection[i];
            if(!bl_collection[i])
            {
                btn_vector_collection[i]->setText("停止");
                timeID = startTimer(10);
                long_num = 0;
                m_vector_point[i]->OnEmpty();
            }
            else
            {
                btn_vector_collection[i]->setText(tr("采集%1").arg(i+1));
                killTimer(timeID);
            }
        }
    }
}

void Widget::slotSavefile()
{
    for(int i =0;i<btn_vector_save.size();i++)
    {
        if(btn_vector_save[i] == sender())
        {
            QString name = QFileDialog::getSaveFileName(NULL,"save","",tr("Config Files (*.po)"));
            m_vector_point[i]->OnSave(name.toUtf8().data());
            cout<<m_vector_point[i]->OnGetPointNum()<<endl;
        }
    }
}

void Widget::slotDemarcate()
{

    /*计算空载值*/
    int m_x_base ,m_y_base,m_z_base;
    long j_x=0,j_y=0,j_z=0;
    for(int i =0;i<m_vector_point[0]->OnGetPointNum();i++)
    {
        double *p = m_vector_point[0]->OnGetPoint(i);
        j_x+=p[0];
        j_y+=p[1];
    }

    m_x_base = j_x/m_vector_point[0]->OnGetPointNum();
    m_y_base = j_y/m_vector_point[0]->OnGetPointNum();
    m_z_base = 0;


    /****************************************/
    double *p = m_vector_point[1]->OnGetPoint(0);
    int maxv[2],minv[2];
    maxv[0] = p[0];
    maxv[1] = p[0];
    minv[0] = p[1];
    minv[1] = p[1];



    //计算最大最小
    for(int i =0;i<m_vector_point[1]->OnGetPointNum();i++)
    {
        double *p = m_vector_point[1]->OnGetPoint(i);
        if(p[0]>maxv[0])
        {
            maxv[0] = p[0];
        }
        if(p[0]<minv[0])
        {
            minv[0] =p[0];
        }

        if(p[1]>maxv[1])
        {
            maxv[1] = p[1];
        }
        if(p[1]<minv[1])
        {
            minv[1] =p[1];
        }
    }


    /*计算负载最大最小*/
    double *p2 = m_vector_point[2]->OnGetPoint(0);
    int maxv2[2],minv2[2];
    maxv2[0] = p2[0];
    maxv2[1] = p2[0];
    minv2[0] = p2[1];
    minv2[1] = p2[1];
    for(int i =0;i<m_vector_point[2]->OnGetPointNum();i++)
    {
        double *p = m_vector_point[2]->OnGetPoint(i);
        if(p[0]>maxv2[0])
        {
            maxv2[0] = p[0];
        }
        if(p[0]<minv2[0])
        {
            minv2[0] =p[0];
        }

        if(p[1]>maxv2[1])
        {
            maxv2[1] = p[1];
        }
        if(p[1]<minv2[1])
        {
            minv2[1] =p[1];
        }
    }

    double dx1 = (maxv2[0] - minv2[0])/2.0-(maxv[0] - minv[0])/2.0;
    double dx2 = (maxv2[1] - minv2[1])/2.0-(maxv[1] - minv[1])/2.0;

    cout<<"dx1 :"<<dx1<<"  "<<dx2<<endl;
    double k[2];
    //负载
    int n_n = ui->lineEdit_load->text().toInt();

    k[0] = (n_n*9.8)/dx1;
    k[1] = (n_n*9.8)/dx2;

    double loadn = ((maxv[0] - minv[0])/2.0 -m_x_base)*k[0];
    cout<<"lo:"<<loadn<<endl;
    cout<<"K:"<<k[0]<<"  "<<k[1]<<endl;
    FILE * fp = fopen(MARKPTAH,"wb");
    if(fp == NULL)
    {
        ui->label_message->setText("文件打开失败！");
        return;
    }

    fflush(fp);

    if(1)
    {
        long type = 7;
        CCFGFILE::OnSaveLong(fp,type);
    }

    if(1)
    {
        long type = 1;
        long addr = 111;
        double v = m_x_base;
        CCFGFILE::OnSaveLong(fp,type);
        CCFGFILE::OnSaveLong(fp,addr);
        CCFGFILE::OnSaveDouble(fp,v);
    }
    if(1)
    {
        long type = 1;
        long addr = 112;
        double v = m_y_base;
        CCFGFILE::OnSaveLong(fp,type);
        CCFGFILE::OnSaveLong(fp,addr);
        CCFGFILE::OnSaveDouble(fp,v);
    }
    if(1)
    {
        long type = 1;
        long addr = 113;
        double v = m_z_base;
        CCFGFILE::OnSaveLong(fp,type);
        CCFGFILE::OnSaveLong(fp,addr);
        CCFGFILE::OnSaveDouble(fp,v);
    }

    if(1)
    {
        long type = 1;
        long addr = 121;
        double v = k[0];
        CCFGFILE::OnSaveLong(fp,type);
        CCFGFILE::OnSaveLong(fp,addr);
        CCFGFILE::OnSaveDouble(fp,v);
    }
    if(1)
    {
        long type = 1;
        long addr = 122;
        double v = k[1];
        CCFGFILE::OnSaveLong(fp,type);
        CCFGFILE::OnSaveLong(fp,addr);
        CCFGFILE::OnSaveDouble(fp,v);
    }
    if(1)
    {
        long type = 1;
        long addr = 123;
        double v = 0;
        CCFGFILE::OnSaveLong(fp,type);
        CCFGFILE::OnSaveLong(fp,addr);
        CCFGFILE::OnSaveDouble(fp,v);
    }


    if(1)
    {
        long type = 1;
        long addr = 131;
        double v = m_angle;
        CCFGFILE::OnSaveLong(fp,type);
        CCFGFILE::OnSaveLong(fp,addr);
        CCFGFILE::OnSaveDouble(fp,v);
    }
    fflush(fp);
    fclose(fp);

    if( SendFile(ipchar[0],ipchar[1],ipchar[2],ipchar[3],MARKPTAH,"/mnt/NCFiles/RSHMCFG/FSENSOR_DEFAULT.RCF") == true)
    {
        ui->label_message->setText("标定成功！");
    }
    else
    {
        ui->label_message->setText("标定失败！");
        return;
    }

    /**********************************************/
    //标定负载重量
    FILE * fp2 = fopen(MARKLOAD,"wb");
    if(fp2 == NULL)
    {
        return;
    }

    fflush(fp2);

    if(1)
    {
        long type = 1;
        CCFGFILE::OnSaveLong(fp2,type);
    }
    if(1)
    {
        long type = 1;
        long addr = 132;
        double v = loadn/9.8;
        CCFGFILE::OnSaveLong(fp2,type);
        CCFGFILE::OnSaveLong(fp2,addr);
        CCFGFILE::OnSaveDouble(fp2,v);
        cout<<"load:"<<loadn<<endl;
    }

    fflush(fp2);
    fclose(fp2);

    if( SendFile(ipchar[0],ipchar[1],ipchar[2],ipchar[3],MARKLOAD,"/mnt/NCFiles/RSHMCFG/TOOL_MASS.RCF") == true)
    {
        ui->label_message->setText("标定成功！");
    }
    else
    {
        ui->label_message->setText("标定失败！");
        return;
    }


    /*同步数据信号*/
    if(1)
    {
        long addr[20];
        addr[0] = 800;
        double v[20];
        v[0] = 1;

        if( OnSetCiValue(0,addr,v,1) == false)
        {
            ui->label_message->setText("同步失败！");
            return;
        }

        v[0] = 2;

        if( OnSetCiValue(0,addr,v,1) == false)
        {
            ui->label_message->setText("同步失败！");
            return;
        }
    }

}

void Widget::slotLoadFile()
{
    for(int i =0;i<btn_vector_load.size();i++)
    {
        if(btn_vector_load[i] == sender())
        {
            QString strp = QFileDialog::getOpenFileName(NULL,"open","",tr("Config Files (*.po)"));
            m_vector_point[i]->OnLoad(strp.toUtf8().data());
        }
    }
}

void Widget::slotZero()
{
    double v[3];
    unsigned long c;

    while (1)
    {
        long d = OnGetCiValue(0,v,&c,3);
        if(d>0)
        {
            /*******************************************/
            //计算方向角
            double *po = m_vector_point[0]->OnGetPoint(0);

            m_angle = 0;
            double fx = v[0]-po[0];
            double fy = v[1]-po[1];

            m_angle = atan2(fy,fx);
            m_angle *= 57.295779513082320876798154814105;
            cout<<fx<<" "<<fy<<"zero:"<<m_angle<<endl;
            break;
        }
    }

}

void Widget::timerEvent(QTimerEvent *)
{

    double v[3];
    unsigned long c;
    long d = OnGetCiValue(0,v,&c,3);
    if(d>0)
    {
        if(m_fristqueue.size()<100)
        {
            m_fristqueue.append(v[0]);
            m_secondqueue.append(v[1]);
        }
        else
        {
            m_fristqueue.dequeue();
            m_secondqueue.dequeue();
            m_fristqueue.append(v[0]);
            m_secondqueue.append(v[1]);
        }

        auto res1 = minmax_element(m_fristqueue.begin(),m_fristqueue.end());
        auto res2 = minmax_element(m_secondqueue.begin(),m_secondqueue.end());
        \

        int mina,maxa;
        *(res1.first)>*(res2.first)?mina = *(res2.first):mina =*(res2.first);
        *(res1.second)>*(res2.second)?maxa = *(res1.second):maxa =*(res2.second);

        updataChartview(mina,maxa);


        for(int i =0;i<3;i++)
        {
            if(!bl_collection[i])
            {
                m_vector_point[i]->OnSetPoint(v);
            }
        }
        ui->label_message->setText(QString::number(long_num));
        long_num ++;
    }
}











