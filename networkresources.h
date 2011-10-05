#ifndef NETWORKRESOURCES_H
#define NETWORKRESOURCES_H

#include <QThread>
#include <QTreeWidgetItem>
#include "getmac.h"
#include <QTreeWidget>


class NetworkResources : public QThread
{
    Q_OBJECT
public:
    NetworkResources(QTreeWidget *);
    void run();
    void sendTreeWidget(QTreeWidgetItem *);
    void stop();

private:
    QTreeWidgetItem* createTreeWidgetItem(LPNETRESOURCE lpnr,int type);
    BOOL WINAPI EnumerateFunc(LPNETRESOURCE lpnr);
    void errorHandler(DWORD errorCode,QString str);
    void displayResource(int, LPNETRESOURCE);
    QTreeWidget *treeWidget;
    QTreeWidgetItem *lastDomain;
    QTreeWidgetItem *lastServer;
    QTreeWidgetItem *lastShare;
    QTreeWidgetItem *lastNetwork;
    bool isRun;


signals:
    void searchResultSignal(QString);
    void searchLanResourcesCompleteSignal();
    void sendTreeWidgetItemSignal(QTreeWidgetItem *);
    void sendMessage(QString msg);
    void NRSearchLanResourcesStoppedSignal();
public slots:

};

#endif // NETWORKRESOURCES_H
