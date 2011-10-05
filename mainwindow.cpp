#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeWidgetItem>
#include <QProgressBar>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QApplication::setStyle("plastique");
    QApplication::setPalette(QApplication::style()->standardPalette());

    searchMacAdressThread = new GetMac();
    searchLanResourcesThread = new NetworkResources(ui->resourceTree);
    timer = new QTimer(this);
    lanProgressBarWork = new ProgressBarWork(ui->searchLanProgressBar);

    QObject::connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(clearAll()));

    QObject::connect(ui->searchMac,SIGNAL(clicked()),this,SLOT(getMacAdress()));
    QObject::connect(ui->searchLan,SIGNAL(clicked()),this,SLOT(startSearchLanResources()));

    QObject::connect(searchMacAdressThread,SIGNAL(searchResult(QString)),this,SLOT(incomingTextMessage(QString)));
    QObject::connect(searchMacAdressThread,SIGNAL(searchMacAdressCompleteSignal()),this,SLOT(searchMacAdressCompleteSlot()));

    QObject::connect(searchLanResourcesThread,SIGNAL(sendTreeWidgetItemSignal(QTreeWidgetItem*)),this,SLOT(incomingTreeWidgetItemSlot(QTreeWidgetItem*)));
    QObject::connect(searchLanResourcesThread,SIGNAL(searchLanResourcesCompleteSignal()),this,SLOT(searchLanResourcesComplete()));
    QObject::connect(searchLanResourcesThread,SIGNAL(sendMessage(QString)),this,SLOT(incomingTextMessage(QString)));
    QObject::connect(searchLanResourcesThread,SIGNAL(NRSearchLanResourcesStoppedSignal()),this,SLOT(MWSearchLanResourcesSpoppedSlot()));

    QObject::connect(timer,SIGNAL(timeout()),lanProgressBarWork,SLOT(work()));

    ui->statusBar->showMessage(QString::fromLocal8Bit("Ожидание..."));
    ui->searchLanProgressBar->hide();
    ui->searchMacProgressBar->hide();
    ui->textEdit->setReadOnly(true);

    ui->resourceTree->setColumnWidth(0,200);
    ui->resourceTree->setColumnWidth(1,70);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::incomingTextMessage(QString msg){
    ui->textEdit->append(msg);
}

void MainWindow::getMacAdress(){
    ui->textEdit->clear();
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск\nMAC адресов"));
    searchMacAdressThread->start();
}


void MainWindow::stopSearchMacAdress(){
    if(searchMacAdressThread){
        searchMacAdressThread->exit(0);
        QObject::disconnect(searchMacAdressThread,SIGNAL(searchResult(QString)),this,SLOT(incomingTextMessage(QString)));
        QObject::disconnect(searchMacAdressThread,SIGNAL(searchMacAdressCompleteSignal()),this,SLOT(searchMacAdressCompleteSlot()));
        delete searchMacAdressThread;
        searchMacAdressThread = NULL;
        ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск MAC\nадресов прерван"));
    }
}


void MainWindow::onSearchMacButtonCliked(){
    if(!searchMacAdressThread){
        ui->searchMac->setText(QString::fromLocal8Bit("Остановить поиск\nMAC адресов"));
        getMacAdress();
    }
    else{
        ui->searchMac->setText(QString::fromLocal8Bit("Поиск\nMAC адресов"));
        stopSearchMacAdress();
    }
}

void MainWindow::searchMacAdressCompleteSlot(){
    //ui->searchMac->setText(QString::fromLocal8Bit("Поиск\nMAC адреса"));
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск MAC\nадресов закончен"));
    if(searchLanResourcesThread->isRunning()) ui->statusBar->showMessage(QString::fromLocal8Bit("Остановить поиск\nсетевых ресурсов"));
}

void MainWindow::onSearchLanButtonCliked(){

}

void MainWindow::startSearchLanResources(){
    ui->textEdit->clear();
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск сетевых ресурсов..."));\
    ui->resourceTree->clear();
    ui->searchMac->setDisabled(true);
    ui->searchLanProgressBar->show();

    lanProgressBarWork->start();
    searchLanResourcesThread->start();

    QObject::disconnect(ui->searchLan,SIGNAL(clicked()),this,SLOT(startSearchLanResources()));
    QObject::connect(ui->searchLan,SIGNAL(clicked()),this,SLOT(stopSearchLanResources()));
    ui->searchLan->setText(QString::fromLocal8Bit("Остановить поиск\nсетевых ресурсов"));

    timer->start(40);

}

void MainWindow::stopSearchLanResources(){
    this->setCursor(Qt::WaitCursor);

    QObject::disconnect(ui->searchLan,SIGNAL(clicked()),this,SLOT(stopSearchLanResources()));
    QObject::connect(ui->searchLan,SIGNAL(clicked()),this,SLOT(startSearchLanResources()));

    ui->searchLan->setDisabled(true);
    ui->statusBar->showMessage(QString::fromLocal8Bit("Остановка поиска..."));
    ui->searchLanProgressBar->hide();

    searchLanResourcesThread->stop();
    timer->stop();
    lanProgressBarWork->stop();


}


void MainWindow::MWSearchLanResourcesSpoppedSlot(){
    this->setCursor(Qt::ArrowCursor);

    ui->searchLan->setEnabled(true);
    ui->searchMac->setEnabled(true);
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск сетевых ресурсов остановлен"));

    ui->searchLan->setText(QString::fromLocal8Bit("Поиск\nсетевых ресурсов"));




}

void MainWindow::searchLanResourcesComplete(){

    QObject::disconnect(ui->searchLan,SIGNAL(clicked()),this,SLOT(stopSearchLanResources()));
    QObject::connect(ui->searchLan,SIGNAL(clicked()),this,SLOT(startSearchLanResources()));

    ui->searchLanProgressBar->hide();
    ui->searchMac->setEnabled(true);
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск сетевых\nресурсов закончен"));
    ui->searchLan->setText(QString::fromLocal8Bit("Поиск\nсетевых ресурсов"));

    searchLanResourcesThread->stop();
    lanProgressBarWork->stop();
    timer->stop();
}

void MainWindow::incomingTreeWidgetItemSlot(QTreeWidgetItem *tmpTreeWidget){
    ui->resourceTree->addTopLevelItem(tmpTreeWidget);
}

void MainWindow::getTreeForStr(QString str){

}


void MainWindow::clearAll(){
    ui->textEdit->clear();
    ui->resourceTree->clear();
}
