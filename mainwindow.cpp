#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "weathertool.h"

#include <QSqlDatabase>
#include <QSqlQuery>


#define INCREMENT 3 //温度变化一度，y轴增量
#define POINT_RADIUS 3//曲线描点的大小
#define TEXT_OFFSET_X 2
#define TEXT_OFFSET_Y 12


char flag;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗口标题
    setWindowTitle("智慧果园系统");
    mNetAccessManager = new QNetworkAccessManager(this);


    //串口部分
    QStringList list;
    list<<"115200"<<"4800"<<"9600";
    ui->comboBox2->addItems(list);
    ui->textEdit->setStyleSheet("QTextEdit { line-height: 1.2em; padding: 0px; margin: 0px; }");
    QStringList listNumber;
    listNumber<<"全部"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12"<<"13"<<"14"<<"15"<<"16";
    ui->comSmart->addItems(listNumber);

    //表格部分
    modelKind = new QSqlQueryModel(this);
    modelYear = new QSqlQueryModel(this);
    modelStatistics = new  QSqlQueryModel(this);
    modelFiltered = new QStandardItemModel(this);
    chartView = new QChartView();
    chart = new QChart();
    chart->setMinimumSize(ui->label_3->size());

    QStringList listClass;
    listClass<<"全部"<<"葡萄"<<"橙子"<<"梨子"<<"桃子"<<"柑橘"<<"樱桃"<<"草莓"<<"樱桃";
    ui->comboBoxKind->addItems(listClass);

    QStringList listNutrients;
    listNutrients<<"全部"<<"维生素C"<<"柠檬酸"<<"膳食纤维"<<"多酚类化合物";
    ui->comboBoxInfo->addItems(listNutrients);

    QStringList listYear;
    listYear<<"全部"<<"2012"<<"2013"<<"2014"<<"2015"<<"2016"<<"2017"<<"2018"<<"2019"<<"2020"<<"2021"<<"2022"<<"2023";
    ui->comboBoxYear->addItems(listYear);

    //统计部分
    ui->comboBoxSmart1->addItems(listClass);
    ui->comboBox_num->addItems(listNumber);
    QStringList listYear1;
    listYear1<<"2012"<<"2013"<<"2014"<<"2015"<<"2016"<<"2017"<<"2018"<<"2019"<<"2020"<<"2021"<<"2022"<<"2023";
    ui->comboBoxyear_max->addItems(listYear1);
    ui->comboBoxyear_min->addItems(listYear1);
    //天气部分

    //给标签添加事件过滤器
    ui->HighLabel->installEventFilter(this);
    ui->LowLabel->installEventFilter(this);

    //将控件添加到数组
    /*添加日期和天气*/
    mDateList << ui->date1  << ui->date2  << ui->date3  << ui->date4  << ui->date5  << ui->date6;
    mWeekList << ui->week1  << ui->week2  << ui->week3  << ui->week4  << ui->week5  << ui->week6;

    /*添加天气和天气图标*/
    mTypeIconList << ui->label_weather1 <<  ui->label_weather2 <<  ui->label_weather3 << ui->label_weather4<< ui->label_weather5<<  ui->label_weather6;
    iconList << "label_weather1" <<  "label_weather2" <<  "label_weather3" << "label_weather4"<< "label_weather5" << "label_weather6";
    mTypeList << ui->weather1 << ui->weather2 << ui->weather3 << ui->weather4 << ui->weather5 << ui->weather6;

    mzuoList << ui->widget_flfx1 << ui->widget_flfx2 << ui->widget_flfx3 << ui->widget_flfx4 << ui->widget_flfx5<< ui->widget_flfx6;

    /*天气指数*/
    mAqiList << ui->label1 << ui->label2 << ui->label3 << ui->label4 << ui->label5 << ui->label6;
    AqiList << ui->widget1 << ui->widget2 << ui->widget3 << ui->widget4 << ui->widget5 << ui->widget6;
    //风向风力
    mFxList << ui->fx_1 << ui->fx_2 << ui->fx_3 << ui->fx_4 << ui->fx_5 << ui->fx_6;
    mFlList << ui->fl_1 << ui->fl_2 << ui->fl_3 << ui->fl_4 << ui->fl_5 << ui->fl_6;

    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu.png");
    mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大到暴雨",":/res/type/DaDaoBaoYu.png");
    mTypeMap.insert("大雪",":/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/res/type/DaYu.png");
    mTypeMap.insert("冻雨",":/res/type/DongYu.png");
    mTypeMap.insert("多云",":/res/type/DuoYun.png");
    mTypeMap.insert("浮沉",":/res/type/FuChen.png");
    mTypeMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("霾",":/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
    mTypeMap.insert("晴",":/res/type/Qing.png");
    mTypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    mTypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    mTypeMap.insert("undegfined",":/res/type/undegfined.png");
    mTypeMap.insert("雾",":/res/type/Wu.png");
    mTypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
    mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
    mTypeMap.insert("小雨",":/res/type/XiaoYu.png");
    mTypeMap.insert("雪",":/res/type/Xue.png");
    mTypeMap.insert("扬沙",":/res/type/YangSha.png");
    mTypeMap.insert("阴",":/res/type/Yin.png");
    mTypeMap.insert("雨",":/res/type/Yu.png");
    mTypeMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
    mTypeMap.insert("阵雪",":/res/type/ZhongXue.png");
    mTypeMap.insert("阵雨",":/res/type/ZhongYu.png");
    mTypeMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
    mTypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪",":/res/type/ZhongXue.png");
    mTypeMap.insert("中雨",":/res/type/ZhongYu.png");

    connect(mNetAccessManager,&QNetworkAccessManager::finished,this,&MainWindow::onReplied);
    getWeatherInfo("南通");

    createSmart();
    model = new QSqlTableModel(this);
    // 设置数据源
    model->setTable("CanEnter");

    // 设置排序方式
    model->setSort(0, Qt::AscendingOrder);

    // 设置编辑策略
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // 提交所有挂起的更改并刷新模型
    model->select();
    ui->tableView->setModel(model);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Read_Data()
{
    QByteArray array = serialAG->readAll();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString temp1 = codec->toUnicode(array);
    ui->textEdit->append(temp1);
    //emit sendTemp(temp1);
}

void MainWindow::getTemp(QString xxTemp)
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss ddd");
    QString temp2=xxTemp;
    QString path="./temp.txt";
    if(path.isEmpty()==0)
    {
        QFile file(path);
        bool isOk=file.open(QIODevice::WriteOnly|QIODevice::Append);
        if(isOk==1)
        {
            if(flag==1)
            {
                flag=0;
                file.resize(0);  // 清空 temp.txt
            }
            file.write(current_date.toUtf8()+temp2.toUtf8()+"\r\n");
         }
         file.close();
    }
}

void MainWindow::onReceiveData(QString data)
{
    ui->textEdit->append(data);
}
//创建果园相关表格
bool MainWindow::createSmart()
{
    QSqlQuery query(QSqlDatabase::database("smartOrchard"));

    query.exec("create table CanEnter(id Integer Primary key, uid int,class varchar(20), Nutrients varchar(20), useTime YEAR, PlantingArea int, Harvest int)");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(1,1, '橙子', '维生素C', '2012', '30', '200')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(2,2, '桃子', '维生素C', '2012', '40', '200')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(3, 3,'菠萝', '维生素C', '2012', '50', '200')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(4,4, '柑橘', '柠檬酸', '2012', '30', '200')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(5, 5,'葡萄', '多酚类化合物', '2012', '60', '200')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(6,6, '樱桃', '柠檬酸', '2012', '20', '200')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(7,7, '柚子', '膳食纤维', '2012', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(8,8, '草莓', '维生素C', '2012', '100', '800')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(9,9, '橙子', '维生素C', '2012', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(10,10, '橙子', '维生素C', '2012', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(11,11, '菠萝', '维生素C', '2012', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(12,12, '柑橘', '柠檬酸', '2012', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(13, 13,'葡萄', '多酚类化合物', '2012', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(14,14, '樱桃', '柠檬酸', '2012', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(15,15, '草莓', '维生素C', '2012', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(16, 16,'草莓', '维生素C', '2012', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(17,1, '橙子', '维生素C', '2013', '30', '200')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(18,2, '桃子', '维生素C', '2013', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(19, 3,'菠萝', '维生素C', '2013', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(20,4, '柑橘', '柠檬酸', '2013', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(21, 5,'葡萄', '多酚类化合物', '2013', '60', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(22,6, '樱桃', '柠檬酸', '2013', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(23,7, '柚子', '膳食纤维', '2013', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(24,8, '草莓', '维生素C', '2013', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(25,9, '橙子', '维生素C', '2013', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(26,10, '橙子', '维生素C', '2013', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(27,11, '菠萝', '维生素C', '2013', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(28,12, '柑橘', '柠檬酸', '2013', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(29, 13,'葡萄', '多酚类化合物', '2013', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(30,14, '樱桃', '柠檬酸', '2013', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(31,15, '草莓', '维生素C', '2013', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(32, 16,'草莓', '维生素C', '2013', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(33,1, '橙子', '维生素C', '2014', '30', '600')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(34,2, '桃子', '维生素C', '2014', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(35, 3,'菠萝', '维生素C', '2014', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(36,4, '柑橘', '柠檬酸', '2014', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(37, 5,'葡萄', '多酚类化合物', '2014', '60', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(38,6, '樱桃', '柠檬酸', '2014', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(39,7, '柚子', '膳食纤维', '2014', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(40,8, '草莓', '维生素C', '2014', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(41,9, '橙子', '维生素C', '2014', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(42,10, '橙子', '维生素C', '2014', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(43,11, '菠萝', '维生素C', '2014', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(44,12, '柑橘', '柠檬酸', '2014', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(45, 13,'葡萄', '多酚类化合物', '2014', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(46,14, '樱桃', '柠檬酸', '2014', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(47,15, '草莓', '维生素C', '2014', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(48, 16,'草莓', '维生素C', '2014', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(49,1, '橙子', '维生素C', '2015', '30', '500')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(50,2, '桃子', '维生素C', '2015', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(51, 3,'菠萝', '维生素C', '2015', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(52,4, '柑橘', '柠檬酸', '2015', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(53, 5,'葡萄', '多酚类化合物', '2015', '60', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(54,6, '樱桃', '柠檬酸', '2015', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(55,7, '柚子', '膳食纤维', '2015', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(56,8, '草莓', '维生素C', '2015', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(57,9, '橙子', '维生素C', '2015', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(58,10, '橙子', '维生素C', '2015', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(59,11, '菠萝', '维生素C', '2015', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(60,12, '柑橘', '柠檬酸', '2015', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(61, 13,'葡萄', '多酚类化合物', '2015', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(62,14, '樱桃', '柠檬酸', '2015', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(63,15, '草莓', '维生素C', '2015', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(64, 16,'草莓', '维生素C', '2015', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(65,1, '橙子', '维生素C', '2016', '30', '500')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(66,2, '桃子', '维生素C', '2016', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(67, 3,'菠萝', '维生素C', '2016', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(68,4, '柑橘', '柠檬酸', '2016', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(69, 5,'葡萄', '多酚类化合物', '2016', '60', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(70,6, '樱桃', '柠檬酸', '2016', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(71,7, '柚子', '膳食纤维', '2016', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(72,8, '草莓', '维生素C', '2016', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(73,9, '橙子', '维生素C', '2016', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(74,10, '橙子', '维生素C', '2016', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(75,11, '菠萝', '维生素C', '2016', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(76,12, '柑橘', '柠檬酸', '2016', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(77, 13,'葡萄', '多酚类化合物', '2016', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(78,14, '樱桃', '柠檬酸', '2016', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(79,15, '草莓', '维生素C', '2016', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(80, 16,'草莓', '维生素C', '2016', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(81,1, '橙子', '维生素C', '2017', '30', '500')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(82,2, '桃子', '维生素C', '2017', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(83, 3,'菠萝', '维生素C', '2017', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(84,4, '柑橘', '柠檬酸', '2017', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(85, 5,'葡萄', '多酚类化合物', '2017', '60', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(86,6, '樱桃', '柠檬酸', '2017', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(87,7, '柚子', '膳食纤维', '2017', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(88,8, '草莓', '维生素C', '2017', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(89,9, '橙子', '维生素C', '2017', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(90,10, '橙子', '维生素C', '2017', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(91,11, '菠萝', '维生素C', '2017', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(92,12, '柑橘', '柠檬酸', '2017', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(93, 13,'葡萄', '多酚类化合物', '2017', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(94,14, '樱桃', '柠檬酸', '2017', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(95,15, '草莓', '维生素C', '2017', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(96, 16,'草莓', '维生素C', '2017', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(97,1, '橙子', '维生素C', '2018', '30', '200')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(98,2, '桃子', '维生素C', '2018', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(99, 3,'菠萝', '维生素C', '2018', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(100,4, '柑橘', '柠檬酸', '2018', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(101, 5,'葡萄', '多酚类化合物', '2018', '60', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(102,6, '樱桃', '柠檬酸', '2018', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(103,7, '柚子', '膳食纤维', '2018', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(104,8, '草莓', '维生素C', '2018', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(105,9, '橙子', '维生素C', '2018', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(106,10, '橙子', '维生素C', '2018', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(107,11, '菠萝', '维生素C', '2018', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(108,12, '柑橘', '柠檬酸', '2018', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(109, 13,'葡萄', '多酚类化合物', '2018', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(110,14, '樱桃', '柠檬酸', '2018', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(111,15, '草莓', '维生素C', '2018', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(112, 16,'草莓', '维生素C', '2018', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(113,1, '橙子', '维生素C', '2019', '30', '350')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(114,2, '桃子', '维生素C', '2019', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(115, 3,'菠萝', '维生素C', '2019', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(116,4, '柑橘', '柠檬酸', '2019', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(117, 5,'葡萄', '多酚类化合物', '2019', '60', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(118,6, '樱桃', '柠檬酸', '2019', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(119,7, '柚子', '膳食纤维', '2019', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(120,8, '草莓', '维生素C', '2019', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(121,9, '橙子', '维生素C', '2019', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(122,10, '橙子', '维生素C', '2019', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(123,11, '菠萝', '维生素C', '2019', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(124,12, '柑橘', '柠檬酸', '2019', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(125, 13,'葡萄', '多酚类化合物', '2019', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(126,14, '樱桃', '柠檬酸', '2019', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(127,15, '草莓', '维生素C', '2019', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(128, 16,'草莓', '维生素C', '2019', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(129,1, '橙子', '维生素C', '2020', '30', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(130,2, '桃子', '维生素C', '2020', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(131, 3,'菠萝', '维生素C', '2020', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(132,4, '柑橘', '柠檬酸', '2020', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(133, 5,'葡萄', '多酚类化合物', '2020', '60', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(134,6, '樱桃', '柠檬酸', '2020', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(135,7, '柚子', '膳食纤维', '2020', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(136,8, '草莓', '维生素C', '2020', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(137,9, '橙子', '维生素C', '2020', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(138,10, '橙子', '维生素C', '2020', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(139,11, '菠萝', '维生素C', '2020', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(140,12, '柑橘', '柠檬酸', '2020', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(141, 13,'葡萄', '多酚类化合物', '2020', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(142,14, '樱桃', '柠檬酸', '2020', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(143,15, '草莓', '维生素C', '2020', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(144, 16,'草莓', '维生素C', '2020', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(145,1, '橙子', '维生素C', '2021', '30', '100')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(146,2, '桃子', '维生素C', '2021', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(147, 3,'菠萝', '维生素C', '2021', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(148,4, '柑橘', '柠檬酸', '2021', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(149, 5,'葡萄', '多酚类化合物', '2021', '60', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(150,6, '樱桃', '柠檬酸', '2021', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(151,7, '柚子', '膳食纤维', '2021', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(152,8, '草莓', '维生素C', '2021', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(153,9, '橙子', '维生素C', '2021', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(154,10, '橙子', '维生素C', '2021', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(155,11, '菠萝', '维生素C', '2021', '50', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(156,12, '柑橘', '柠檬酸', '2021', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(157, 13,'葡萄', '多酚类化合物', '2021', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(158,14, '樱桃', '柠檬酸', '2021', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(159,15, '草莓', '维生素C', '2021', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(160, 16,'草莓', '维生素C', '2021', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(161,1, '橙子', '维生素C', '2022', '30', '200')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(162,2, '桃子', '维生素C', '2022', '40', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(163, 3,'菠萝', '维生素C', '2022', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(164,4, '柑橘', '柠檬酸', '2022', '30', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(165, 5,'葡萄', '多酚类化合物', '2022', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(166,6, '樱桃', '柠檬酸', '2022', '20', '100')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(167,7, '柚子', '膳食纤维', '2022', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(168,8, '草莓', '维生素C', '2022', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(169,9, '橙子', '维生素C', '2022', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(170,10, '橙子', '维生素C', '2022', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(171,11, '菠萝', '维生素C', '2022', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(172,12, '柑橘', '柠檬酸', '2022', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(173, 13,'葡萄', '多酚类化合物', '2022', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(174,14, '樱桃', '柠檬酸', '2022', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(175,15, '草莓', '维生素C', '2022', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(176, 16,'草莓', '维生素C', '2022', '100', '300')");

    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(177,1, '橙子', '维生素C', '2023', '30', '500')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(178,2, '桃子', '维生素C', '2023', '40', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(179, 3,'菠萝', '维生素C', '2023', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(180,4, '柑橘', '柠檬酸', '2023', '30', '400')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(181, 5,'葡萄', '多酚类化合物', '2023', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(182,6, '樱桃', '柠檬酸', '2023', '20', '100')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(183,7, '柚子', '膳食纤维', '2023', '50', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(184,8, '草莓', '维生素C', '2022', '100', '300')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(185,9, '橙子', '维生素C', '2023', '40', '600')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(186,10, '橙子', '维生素C', '2023', '40', '700')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(187,11, '菠萝', '维生素C', '2023', '50', '700')");
    query.exec("insert into CanEnter(id,uid, class, Nutrients, useTime, PlantingArea, Harvest) values(188,12, '柑橘', '柠檬酸', '2023', '30', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(189, 13,'葡萄', '多酚类化合物', '2023', '60', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(190,14, '樱桃', '柠檬酸', '2023', '20', '100')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(191,15, '草莓', '维生素C', '2023', '100', '400')");
    query.exec("insert into CanEnter(id, uid,class, Nutrients, useTime, PlantingArea, Harvest) values(192, 16,'草莓', '维生素C', '2023', '100', '300')");
    return 1;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->HighLabel && event->type() == QEvent::Paint){
        paintHighCurve();
    }
    if(watched == ui->LowLabel && event->type() == QEvent::Paint){
        paintLowCurve();
    }

    return QWidget::eventFilter(watched,event);
}

void MainWindow::parseJson(QByteArray &byteArry)
{
    QJsonParseError err;
    QJsonDocument doc =  QJsonDocument::fromJson(byteArry,&err);
    if(err.error != QJsonParseError::NoError){
        return;
    }
    QJsonObject rootObj = doc.object();
    qDebug() << rootObj.value("message").toString();
    //解析日期和城市
    mToday.date =  rootObj.value("date").toString();
    mToday.city =  rootObj.value("cityInfo").toObject().value("city").toString();
    //解析yesterday
    QJsonObject objData =  rootObj.value("data").toObject();
    QJsonObject objYesterday = objData.value("yesterday").toObject();
    mDay[0].week = objYesterday.value("week").toString();
    mDay[0].date = objYesterday.value("ymd").toString();
    mDay[0].type = objYesterday.value("type").toString();

    QString S;
    S = objYesterday.value("low").toString().split(" ").at(1);
    S = S.left(S.length()-1);
    mDay[0].low = S.toInt();
    S = objYesterday.value("high").toString().split(" ").at(1);
    S = S.left(S.length()-1);
    mDay[0].high = S.toInt();
    //风向风力
    mDay[0].fx = objYesterday.value("fx").toString();
    mDay[0].fl = objYesterday.value("fl").toString();

    //污染指数
    mDay[0].aqi = objYesterday.value("aqi").toDouble();


    //解析forcast中五天的数据
    QJsonArray forecastArr =  objData.value("forecast").toArray();

    for(int i = 0;i < 5;i++)
    {
        QJsonObject objForecast = forecastArr[i].toObject();
        mDay[i+1].type = objForecast.value("type").toString();
        mDay[i+1].date = objForecast.value("ymd").toString();
        mDay[i+1].week = objForecast.value("week").toString();

        //高温低温
        QString Highdu;
        Highdu =  objForecast.value("high").toString().split(" ").at(1);
        Highdu = Highdu.left(Highdu.length()-1);
        mDay[i+1].high =  Highdu.toInt();

        Highdu = objForecast.value("low").toString().split(" ").at(1);
        Highdu = Highdu.left(Highdu.length()-1);
        mDay[i+1].low = Highdu.toInt();

        //风向风力
        mDay[i+1].fx = objForecast.value("fx").toString();
        mDay[i+1].fl = objForecast.value("fl").toString();

        //污染指数
        mDay[i+1].aqi = objForecast.value("aqi").toDouble();
    }

    //解析今天天气
    mToday.ganmao = objData.value("ganmao").toString();
    mToday.wendu = objData.value("wendu").toString().toInt();
    mToday.shidu = objData.value("shidu").toString();
    mToday.pm25 = objData.value("pm25").toDouble();
    mToday.quality = objData.value("quality").toString();

    //上面mDay[1]也是今天的天气
    mToday.type = mDay[1].type;
    mToday.date = mDay[1].date;
    mToday.high = mDay[1].high;
    mToday.low = mDay[1].low;
    mToday.fx = mDay[1].fx;
    mToday.fl = mDay[1].fl;

    //更新ui
    updateUI();
    //绘制温度曲线
    ui->HighLabel->update();
    ui->LowLabel->update();
}

void MainWindow::updateUI()
{
    //更新日期和城市
    qDebug() << mToday.date;
    ui->labelCity->setText(mToday.city);
    //更新今天
    ui->label_ganmao->setText(mToday.ganmao);
    QString style = "QLabel#label_Today{border-image:url("+mTypeMap[mToday.type]+");}";
    ui->label_Today->setStyleSheet(style);
    //ui->label_Today->setPixmap(mTypeMap[mToday.type]);
    ui->wendu_Today->setText(QString::number(mToday.wendu) + "°C");
    ui->todayW->setText(mToday.type);
    ui->toDaywen->setText(QString::number(mToday.low)+ "~" + QString::number(mToday.high) + "°C");
    ui->fl->setText(mToday.fl);
    ui->fx->setText(mToday.fx);
    ui->lblPM25->setText(QString::number(mToday.pm25));
    ui->Quality->setText(mToday.quality);
    //更新六天
    for(int i=0; i<6; i++)
    {
        mWeekList[i]->setText("周" + mDay[i].week.right(1));
        QStringList ymdList = mDay[i].date.split("-");
        mDateList[i]->setText(ymdList[1] + "/" +ymdList[2]);
        //更新天气类型
        mTypeList[i]->setText(mDay[i].type);
        //QString style1 = QString("QLabel#%1{border-image:url(%2%3%4);}").arg(mTypeIconList[i]).arg("").arg(mTypeMap[mToday.type]).arg("");
        style = QString("QLabel#"+iconList[i]+"{border-image:url("+"%1);}").arg(mTypeMap[mDay[i].type]);
        qDebug() << style;
        mTypeIconList[i]->setStyleSheet(style);
        //mTypeIconList[i]->setPixmap(mTypeMap[mDay[i].type]);
        //更新空气质量
        if(mDay[i].aqi >= 0 && mDay[i].aqi <= 50)
        {
            mAqiList[i]->setText("优");
            AqiList[i]->setStyleSheet("background-color: rgb(121,184,0)");
        }else if(mDay[i].aqi >= 50 && mDay[i].aqi <= 100)
        {
            mAqiList[i]->setText("良");
            AqiList[i]->setStyleSheet("background-color: rgb(255,187,23)");
        }else if(mDay[i].aqi >= 100 && mDay[i].aqi <= 150)
        {
            mAqiList[i]->setText("轻度");
            AqiList[i]->setStyleSheet("background-color: rgb(255,87,97)");
        }else if(mDay[i].aqi >= 150 && mDay[i].aqi <= 200)
        {
            mAqiList[i]->setText("中度");
            AqiList[i]->setStyleSheet("background-color: rgb(235,17,27)");
        }else if(mDay[i].aqi >= 100 && mDay[i].aqi <= 250)
        {
            mAqiList[i]->setText("重度");
            AqiList[i]->setStyleSheet("background-color: rgb(170,0,0)");
        }else
        {
            mAqiList[i]->setText("严重污染");
            AqiList[i]->setStyleSheet("background-color: rgb(110,0，0)");
        }
        //更新风力
        mFxList[i]->setText(mDay[i].fx);
        mFlList[i]->setText(mDay[i].fl);

    }
}

void MainWindow::getWeatherInfo(QString cityName)
{
    QString CityCode =  weathertool::getCityCode(cityName);
//    if(CityCode.isEmpty()){
//        QMessageBox::warning(this,"天气","请检查是否正确",QMessageBox::Ok);
//    }
    QUrl url("http://t.weather.itboy.net/api/weather/city/" + CityCode);
    mNetAccessManager->get(QNetworkRequest(url));
}

void MainWindow::paintHighCurve()
{
    QPainter painter(ui->HighLabel);

    //设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing,true);

    //获取x坐标
    int pointX[6] = {0};
    for(int i = 0;i < 6; i++){
        pointX[i] = mzuoList[i]->pos().x() + mzuoList[i]->width()/2;
    }

    //获取Y坐标
    int tempSun = 0;
    int tempAverage =0;
    for(int i = 0;i < 6;i++){
        tempSun += mDay[i].high;
    }
    tempAverage = tempSun/6; //平均温度

    int pointY[6] = {0};
    int yCenter = ui->HighLabel->height()/2;
    for(int i=0;i<6;i++){
        pointY[i] = yCenter - ((mDay[i].high - tempAverage) * INCREMENT);
    }
    //开始绘制
    QPen pen = painter.pen();
    pen.setWidth(1); //设置画笔颜色
    pen.setColor(QColor(255,170,0));//设置画笔的颜色

    painter.setPen(pen);
    painter.setBrush(QColor(255,170,0));//设置内部填充的颜色
    for(int i=0;i < 6;i++)
    {
        painter.drawEllipse(QPoint(pointX[i],pointY[i]),POINT_RADIUS,POINT_RADIUS);
        //显示文本
        painter.drawText(pointX[i]-TEXT_OFFSET_X,pointY[i]-TEXT_OFFSET_Y,QString::number(mDay[i].high) + "°");
    }

    //绘制曲线
    for(int i=0;i < 5;i++){
        if(i == 0){
            pen.setStyle(Qt::DotLine);//虚线
            painter.setPen(pen);
        }else {
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
        }

        painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
    }
}

void MainWindow::paintLowCurve()
{
    QPainter painter(ui->LowLabel);
    //设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing,true);

    //获取x坐标
    int pointX[6] = {0};
    for(int i = 0;i < 6; i++){
        pointX[i] = mzuoList[i]->pos().x() + mzuoList[i]->width()/2;
    }

    //获取Y坐标
    int tempSun = 0;
    int tempAverage =0;
    for(int i = 0;i < 6;i++){
        tempSun += mDay[i].low;
    }
    tempAverage = tempSun/6; //平均温度

    int pointY[6] = {0};
    int yCenter = ui->LowLabel->height()/2;
    for(int i=0;i<6;i++){
        pointY[i] = yCenter - ((mDay[i].low - tempAverage) * INCREMENT);
    }
    //开始绘制
    QPen pen = painter.pen();
    pen.setWidth(1); //设置画笔颜色
    pen.setColor(QColor(255,170,0));//设置画笔的颜色

    painter.setPen(pen);
    painter.setBrush(QColor(255,170,0));//设置内部填充的颜色
    for(int i=0;i < 6;i++)
    {
        painter.drawEllipse(QPoint(pointX[i],pointY[i]),POINT_RADIUS,POINT_RADIUS);
        //显示文本
        painter.drawText(pointX[i]-TEXT_OFFSET_X,pointY[i]-TEXT_OFFSET_Y,QString::number(mDay[i].low) + "°");
    }

    //绘制曲线
    for(int i=0;i < 5;i++){
        if(i == 0){
            pen.setStyle(Qt::DotLine);//虚线
            painter.setPen(pen);
        }else {
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
        }

        painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
    }
}

void MainWindow::onReplied(QNetworkReply *reply)
{
    qDebug() << "nihao";
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() != QNetworkReply::NoError || status_code !=200)
    {
        qDebug() << reply->errorString().toLatin1().data();
        QMessageBox::warning(this,"天气","请求数据失败",QMessageBox::Ok);
    }else{
    qDebug() << status_code;
    QByteArray bytearry = reply->readAll();
    qDebug() << bytearry.data();
    parseJson(bytearry);

    reply->deleteLater();
    }
}


void MainWindow::on_radioButton_clicked(bool checked)
{
    if(checked)
    {
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())  // 查找可用的串口
        {
            QSerialPort serial;
            serial.setPort(info);
            if(serial.open(QIODevice::ReadWrite))
            {
                ui->comboBox->addItem(serial.portName());
                serial.close();
            }
         }
        serialAG=new QSerialPort(this);
        serialAG->setPortName(ui->comboBox->currentText());
        serialAG->open(QIODevice::ReadWrite);
        serialAG->setBaudRate(ui->comboBox2->currentText().toInt());
        serialAG->setDataBits(QSerialPort::Data8);
        serialAG->setStopBits(QSerialPort::OneStop);
        serialAG->setParity(QSerialPort::NoParity);
        serialAG->setFlowControl(QSerialPort::NoFlowControl);
        ui->comboBox2->setEnabled(0);
        ui->radioButton->setText("关闭串口");

        connect(serialAG, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
        connect(this,&MainWindow::sendTemp,this,&MainWindow::onReceiveData);
        //connect(this,&Widget::sendTemp,thread,&myThread::getTemp2);
    }
    else
    {
        serialAG->clear();
        serialAG->close();
        serialAG->deleteLater();

        ui->radioButton->setText("打开串口");
        ui->comboBox2->setEnabled(1);
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    // 发送数据
    QByteArray data("1");
    serialAG->write(data);
    ui->textEdit->append(data);
}


void MainWindow::on_actSave_triggered()
{
    // 提交更改
    model->submitAll();
}




void MainWindow::on_comboBoxKind_currentTextChanged(const QString &arg1)
{
    QString queryKind = "SELECT * FROM CanEnter WHERE class = :class";
    QString year = ui->comboBoxYear->currentText();
    QString nutrients = ui->comboBoxInfo->currentText();
    if (!year.isEmpty()&& year!= "全部") {
        queryKind += " AND useTime = :year";
    }
    if (!nutrients.isEmpty()&&nutrients!= "全部") {
        queryKind += " AND Nutrients = :nutrients";
    }
    QSqlQuery q;
    q.prepare(queryKind);
    q.bindValue(":class", arg1);
    if (!year.isEmpty()&&year!= "全部") {
        q.bindValue(":year", year);
    }
    if (!nutrients.isEmpty()&&nutrients!= "全部") {
        q.bindValue(":nutrients", nutrients);
    }
    q.exec();
    modelKind->setQuery(q);
    qDebug()<<modelKind<<endl;
    ui->tableView->setModel(modelKind);
}



void MainWindow::on_comboBoxYear_currentTextChanged(const QString &arg1)
{

    QString year = "SELECT * FROM CanEnter WHERE useTime = :year";
    QString queryKind = ui->comboBoxKind->currentText();
    QString nutrients = ui->comboBoxInfo->currentText();
    if (!queryKind.isEmpty()&&queryKind!= "全部") {
        year += " AND class = :class";
    }
    if (!nutrients.isEmpty()&& nutrients!= "全部") {
        year += " AND Nutrients = :nutrients";
    }
    QSqlQuery q;
    q.prepare(year);
    q.bindValue(":year", arg1);
    if (!queryKind.isEmpty()&& queryKind!= "全部") {
        q.bindValue(":class", queryKind);
    }
    if (!nutrients.isEmpty()&& nutrients!= "全部") {
        q.bindValue(":nutrients", nutrients);
    }
    q.exec();
    modelYear->setQuery(q);
    ui->tableView->setModel(modelYear);
}

//void MainWindow::filterResults()
//{
//    if (modelKind->query().isActive() || modelYear->query().isActive()) {
//            // 如果有一个或两个查询还没有执行完毕，则等待
//        qDebug() << "1" <<endl;
//            return;
//        }
//    if (modelKind->query().executedQuery() == modelYear->query().executedQuery()) {
//        QSet<int> rowIndexes;

//        QMap<QString, int> mapKind;
//        for (int i = 0; i < modelKind->rowCount(); i++) {
//            QModelIndex index = modelKind->index(i, 0);
//            QString key = modelKind->data(index).toString();
//            mapKind.insert(key, i);
//        }

//        for (int i = 0; i < modelYear->rowCount(); i++) {
//            QModelIndex index = modelYear->index(i, 0);
//            QString key = modelYear->data(index).toString();
//            if (mapKind.contains(key)) {
//                rowIndexes.insert(mapKind.value(key));
//            }
//        }
//        modelFiltered->insertColumns(0, modelKind->columnCount());
//        modelFiltered->insertRows(0, rowIndexes.size());
//        int row = 0;
//        foreach (int index, rowIndexes) {
//            for (int col = 0; col < modelKind->columnCount(); col++) {
//                QModelIndex srcIndex = modelKind->index(index, col);
//                QModelIndex destIndex = modelFiltered->index(row, col);
//                modelFiltered->setData(destIndex, modelKind->data(srcIndex));
//            }
//            row++;
//        }
//        qDebug()<<"2"<<endl;
//        // 将筛选后的模型设置为表视图的模型
//        ui->tableView->setModel(modelFiltered);
//    } else {
//        // 一个或两个查询尚未执行，因此使用执行的查询设置模型
//        if (modelKind->query().isActive()) {
//            ui->tableView->setModel(modelKind);
//            qDebug()<<"4"<<endl;
//        } else if (modelYear->query().isActive()) {
//            ui->tableView->setModel(modelYear);
//            qDebug()<<"5"<<endl;
//        }
//    }
//}


void MainWindow::on_comboBoxInfo_currentTextChanged(const QString &arg1)
{
    QString nutrients = "SELECT * FROM CanEnter WHERE Nutrients = :nutrients";
    QString queryKind = ui->comboBoxKind->currentText();
    QString year = ui->comboBoxYear->currentText();
    if (!queryKind.isEmpty()&&queryKind!= "全部") {
        nutrients += " AND class = :class";
    }
    if (!queryKind.isEmpty()) {
        nutrients += " AND Nutrients = :nutrients";
    }
    QSqlQuery q;
    q.prepare(nutrients);
    q.bindValue(":nutrients", arg1);
    if (!queryKind.isEmpty()&&queryKind!= "全部") {
        q.bindValue(":class", queryKind);
    }
    if (!year.isEmpty()&&year!= "全部") {
        q.bindValue(":year", year);
    }
    q.exec();
    modelYear->setQuery(q);
    ui->tableView->setModel(modelYear);
}




void MainWindow::on_comboBoxSmart1_currentTextChanged(const QString &arg1)
{
    series = new QLineSeries();
    QString queryKind = "SELECT * FROM CanEnter WHERE class = :class";
    QString uid = ui->comboBox_num->currentText();
    QString yearMin = ui->comboBoxyear_min->currentText();
    QString yearMax = ui->comboBoxyear_max->currentText();
    qDebug() << yearMin <<yearMax<< endl;
    if (!yearMin.isEmpty()&&!yearMax.isEmpty()) {
        queryKind += " AND useTime BETWEEN :yearmin AND :yearmax";
    }
    if (!uid.isEmpty()&& uid!= "全部") {
        queryKind += " AND uid = :uid";
        qDebug() << "cer1" << endl;
    }
    QSqlQuery q;
    q.prepare(queryKind);
    q.bindValue(":class", arg1);
    if (!uid.isEmpty()) {
        q.bindValue(":uid", uid);
        qDebug() << "cer2" << endl;
    }
    if (!yearMin.isEmpty()) {
        q.bindValue(":yearmin", yearMin);
    }
    if (!yearMax.isEmpty()) {
        q.bindValue(":yearmax", yearMax);
    }
    q.exec();
    while (q.next()) {
    int time = q.value(4).toInt();
    int data = q.value(6).toInt();
    series->append(time, data);
    }
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(yearMin.toInt(), yearMax.toInt());  // 设置横坐标范围
    axisX->setLabelFormat("%d");  // 设置横坐标标签格式
    axisX->setTickCount(yearMax.toInt()-yearMin.toInt());
    axisX->setTitleText("Samrt");     //设置坐标标题
    axisX->setLabelsAngle(45);
    chart->addAxis(axisX, Qt::AlignBottom);  // 添加横坐标到底部
    series->attachAxis(axisX);  // 将数据系列与横坐标关联
    modelStatistics->setQuery(q);
    ui->tableView->setModel(modelStatistics);

}

void MainWindow::on_comboBox_num_currentTextChanged(const QString &arg1)
{
//    QString nutrients = "SELECT * FROM CanEnter WHERE useTime = :nutrients";
//    QString queryKind = ui->comboBoxKind->currentText();
//    QString year = ui->comboBoxYear->currentText();
//    if (!queryKind.isEmpty()) {
//        nutrients += " AND class = :class";
//    }
//    if (!queryKind.isEmpty()) {
//        nutrients += " AND Nutrients = :nutrients";
//    }
//    QSqlQuery q;
//    q.prepare(nutrients);
//    q.bindValue(":nutrients", arg1);
//    if (!queryKind.isEmpty()) {
//        q.bindValue(":class", queryKind);
//    }
//    if (!year.isEmpty()) {
//        q.bindValue(":year", year);
//    }
//    q.exec();
//    modelYear->setQuery(q);
//    ui->tableView->setModel(modelYear);
}


void MainWindow::on_pushButton_No_clicked()
{
    QFile file("myTable.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // 写入表头
        for (int i = 0; i < modelStatistics->columnCount(); i++) {
            out << modelStatistics->headerData(i, Qt::Horizontal).toString() << ",";
        }
        out << "\n";
        // 写入数据
        for (int i = 0; i < modelStatistics->rowCount(); i++) {
            for (int j = 0; j < modelStatistics->columnCount(); j++) {
                out << modelStatistics->data(modelStatistics->index(i, j)).toString() << ",";
            }
            out << "\n";
        }
        file.close();
    }
}


void MainWindow::on_pushButton_Yes_clicked()
{
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Smart");
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // 获取QChartView的截图
    QPixmap pixmap = chartView->grab();

    // 将QPixmap的大小设置为QLabel的大小
    pixmap = pixmap.scaled(ui->label_3->size(), Qt::KeepAspectRatio);

    // 将QPixmap设置为QLabel的图像
    ui->label_3->setPixmap(pixmap);

}


void MainWindow::on_pushButton_5_clicked()
{
    // 发送数据
    QByteArray data("2");
    serialAG->write(data);
    ui->textEdit->append(data);

}


void MainWindow::on_pushButton_6_clicked()
{
    // 发送数据
    QByteArray data("3");
    serialAG->write(data);
    ui->textEdit->append(data);
}


void MainWindow::on_pushButton_7_clicked()
{
    // 发送数据
    QByteArray data("4");
    serialAG->write(data);
    ui->textEdit->append(data);
}


void MainWindow::on_pushButton_clicked()
{
    // 清空文本框
    ui->textEdit->clear();
}

