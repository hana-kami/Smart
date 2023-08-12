#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPainter>
#include <QtCharts>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "data.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace QtCharts;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //串口
    void Read_Data();
    void getTemp(QString xxTemp);
    void onReceiveData(QString data);
    bool createSmart();

    //void filterResults();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void parseJson(QByteArray &byteArry);
    void updateUI();
    void getWeatherInfo(QString cityName);
    //绘制高低温曲线
    void paintHighCurve();
    void paintLowCurve();
private:
    void onReplied(QNetworkReply* reply);
private:
    Ui::MainWindow *ui;
    QSerialPort *serialAG;
    QSqlQueryModel *modelKind;
    QSqlQueryModel *modelYear;
    QSqlQueryModel *modelStatistics;
    QStandardItemModel *modelFiltered;
    QChart *chart;
    QLineSeries *series;
    today mToday;
    QChartView *chartView;
    QSqlTableModel *model;
    Day mDay[6];
    QList<QString> iconList;

    QList<QLabel*> mWeekList;
    QList<QLabel*> mDateList;

    QList<QWidget*> mzuoList;
    QList<QLabel*> mTypeList;
    QList<QLabel*> mTypeIconList;

    QList<QLabel*> mAqiList;
    QList<QWidget*> AqiList;
    QList<QLabel*> mFxList;
    QList<QLabel*> mFlList;
    QMap<QString,QString> mTypeMap;

    QNetworkAccessManager* mNetAccessManager;

signals:
    void sendTemp(QString);

private slots:
    void on_radioButton_clicked(bool checked);
    void on_pushButton_4_clicked();
    void on_actSave_triggered();
    void on_comboBoxKind_currentTextChanged(const QString &arg1);
    void on_comboBoxYear_currentTextChanged(const QString &arg1);
    void on_comboBoxInfo_currentTextChanged(const QString &arg1);
    void on_comboBoxSmart1_currentTextChanged(const QString &arg1);
    void on_comboBox_num_currentTextChanged(const QString &arg1);
    void on_pushButton_No_clicked();
    void on_pushButton_Yes_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
