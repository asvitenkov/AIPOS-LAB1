#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "getmac.h"
#include <networkresources.h>
#include <QTreeWidgetItem>
#include <QTimer>
#include "progressbarwork.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void getMacAdress();
    void stopSearchMacAdress();
    void onSearchMacButtonCliked();
    void onSearchLanButtonCliked();
    void startSearchLanResources();
    void stopSearchLanResources();
    void clearAll();

public slots:
    void incomingTextMessage(QString);
    void incomingTreeWidgetItemSlot(QTreeWidgetItem *);
    void searchMacAdressCompleteSlot();
    void searchLanResourcesComplete();
    void getTreeForStr(QString str);
    void MWSearchLanResourcesSpoppedSlot();
    //void deleteSearchLanResourseThread();

private:
    Ui::MainWindow *ui;
    GetMac *searchMacAdressThread;
    NetworkResources *searchLanResourcesThread;
    ProgressBarWork *lanProgressBarWork;
    QTimer *timer;

signals:
    void sendTreeForStr();

};

#endif // MAINWINDOW_H
