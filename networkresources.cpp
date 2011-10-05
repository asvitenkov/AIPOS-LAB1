#include "networkresources.h"
#include <QTreeWidgetItem>
#include <QDebug>
#include <QTextCodec>
#include <QChar>

NetworkResources::NetworkResources(QTreeWidget *tw)
{
    treeWidget = tw;
    isRun = false;
    lastDomain = NULL;
    lastServer = NULL;
    lastShare = NULL;
    lastNetwork = NULL;


}



void NetworkResources::run(){
    isRun = true;
    LPNETRESOURCE lpnr = NULL;
    EnumerateFunc(lpnr);
    // äîïèñàòü òóò
    // ##############
    // ##############
    if(!isRun) emit NRSearchLanResourcesStoppedSignal();
    else emit searchLanResourcesCompleteSignal();
}


void NetworkResources::displayResource(int i, LPNETRESOURCE lpnrLocal){
    QTreeWidgetItem *item = new QTreeWidgetItem(0);
    QString str = QString::fromWCharArray(lpnrLocal->lpRemoteName);
    str=str+" "+QString::number(lpnrLocal->dwDisplayType);
    item->setText(0,str);
    //treeWidget->addTopLevelItem(item);
    //emit sendTreeWidgetItemSignal(item);
    //                QString().fromWCharArray(lpnrLocal[i].lpRemoteName)

    switch (lpnrLocal->dwDisplayType) {
    //        case (RESOURCEDISPLAYTYPE_GENERIC):
    //            printf("generic\n");
    //            break;
    case (RESOURCEDISPLAYTYPE_DOMAIN):
        //printf("domain\n");
        // ÂÎÒ ÒÓÒ ÈÇÌÅÍÅÍÎ
        lastDomain = createTreeWidgetItem(lpnrLocal,1);
        lastServer = NULL;
        lastShare = NULL;
        lastNetwork->addChild(lastDomain);
        //emit sendTreeWidgetItemSignal(lastDomain);
        break;
    case (RESOURCEDISPLAYTYPE_SERVER):
        //printf("server\n");
        if(lastDomain){
            lastServer = createTreeWidgetItem(lpnrLocal,2);
            lastDomain->addChild(lastServer);
            lastShare = NULL;
        }
        else{
            // îøèáêà íåïîíÿòíàÿ 1
            emit sendMessage("error1 in NetworkResources::displayResource");
        }
        break;
    case (RESOURCEDISPLAYTYPE_SHARE):
        //printf("share\n");
        if(lastDomain && lastServer){
            lastShare = createTreeWidgetItem(lpnrLocal,3);
            lastServer->addChild(lastShare);
        }
        else{
            // îøèáêà íåïîíÿòíàÿ 2
            emit sendMessage("error2 in NetworkResources::displayResource");
        };
        break;

    case (RESOURCEDISPLAYTYPE_FILE):
        //printf("file\n");
        if(lastDomain && lastServer && lastShare){
            lastShare->addChild(createTreeWidgetItem(lpnrLocal,4));
        }
        break;
    case (RESOURCEDISPLAYTYPE_GROUP):
        //printf("group\n");
        emit sendTreeWidgetItemSignal(createTreeWidgetItem(lpnrLocal,5));
        break;
    case (RESOURCEDISPLAYTYPE_NETWORK):
        //printf("network\n");
        // ÂÎÒ ÒÓÒ ÈÇÌÅÍÅÍÎ
        lastNetwork = createTreeWidgetItem(lpnrLocal,6);
        emit sendTreeWidgetItemSignal(lastNetwork);
        break;
    default:
        //printf("unknown display type %d\n", lpnrLocal->dwDisplayType);
        emit sendTreeWidgetItemSignal(createTreeWidgetItem(lpnrLocal,-1));
        break;
    }

}


BOOL WINAPI NetworkResources::EnumerateFunc(LPNETRESOURCE lpnr)
{

    if(!isRun){
        qDebug()<<"in EnumerateFunc isRun = false"<<endl;
        return false;
    }
    qDebug()<<"in EnumerateFunc isRun = true"<<endl;

    DWORD dwResult, dwResultEnum;
    HANDLE hEnum;
    DWORD cbBuffer = 16384;     // 16K is a good size
    DWORD cEntries = -1;        // enumerate all possible entries
    LPNETRESOURCE lpnrLocal;    // pointer to enumerated structures
    DWORD i;
    HWND hwnd = NULL;
    //
    // Call the WNetOpenEnum function to begin the enumeration.
    //
    dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, // all network resources
                            RESOURCETYPE_ANY,   // all resources
                            0,  // enumerate all resources
                            lpnr,       // NULL first time the function is called
                            &hEnum);    // handle to the resource

    if(!isRun){
        qDebug()<<"in EnumerateFunc isRun = false"<<endl;
        return false;
    }

    if (dwResult != NO_ERROR) {
        // îøèáêà WNetOpenEnum íóæíî ïîïûòàòüñÿ îáðàáîòàòü
        errorHandler(dwResult,"WNetOpenEnum");
        return FALSE;
    }
    //
    // Call the GlobalAlloc function to allocate resources.
    //

    lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);
    if (lpnrLocal == NULL) {
        //printf("WnetOpenEnum failed with error %d\n", dwResult);
        errorHandler(dwResult,"WNetOpenEnum");
        return FALSE;
    }

    do {
        if(!isRun){
            qDebug()<<"in EnumerateFunc isRun = false"<<endl;
            return false;
        }
        //
        // Initialize the buffer.
        //
        ZeroMemory(lpnrLocal, cbBuffer);
        //
        // Call the WNetEnumResource function to continue
        //  the enumeration.
        //
        dwResultEnum = WNetEnumResource(hEnum,  // resource handle
                                        &cEntries,      // defined locally as -1
                                        lpnrLocal,      // LPNETRESOURCE
                                        &cbBuffer);     // buffer size
        //
        // If the call succeeds, loop through the structures.
        //

        if (dwResultEnum == NO_ERROR) {
            for (i = 0; i < cEntries; i++) {
                if(!isRun){
                    qDebug()<<"in EnumerateFunc isRun = false"<<endl;
                    return false;
                }
                // Call an application-defined function to
                //  display the contents of the NETRESOURCE structures.
                //
                //DisplayStruct(i, &lpnrLocal[i]);
                //cout<<lpnrLocal[i].lpLocalName<<endl;
                displayResource(i,&lpnrLocal[i]);


                // If the NETRESOURCE structure represents a container resource,
                //  call the EnumerateFunc function recursively.

                if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage
                                                & RESOURCEUSAGE_CONTAINER))
                    //          if(!EnumerateFunc(hwnd, hdc, &lpnrLocal[i]))
                    if (!EnumerateFunc(&lpnrLocal[i])) ;
                // !!!!!!!
            }
        }
        // Process errors.
        //

        else if (dwResultEnum != ERROR_NO_MORE_ITEMS) {
            errorHandler(dwResultEnum,"WNetEnumResource");
            //break;
        }
    }
    //
    // End do.
    //
    while (dwResultEnum != ERROR_NO_MORE_ITEMS);
    //
    // Call the GlobalFree function to free the memory.
    //

    if(!isRun){
        qDebug()<<"in EnumerateFunc isRun = false"<<endl;
        return false;
    }

    GlobalFree((HGLOBAL) lpnrLocal);
    //
    // Call WNetCloseEnum to end the enumeration.
    //
    dwResult = WNetCloseEnum(hEnum);

    if (dwResult != NO_ERROR) {
        //
        // Process errors.
        //
        errorHandler(dwResultEnum," WNetCloseEnum");

        return FALSE;
    }

    return TRUE;
}

void NetworkResources::errorHandler(DWORD errorCode, QString dsc){
    // ÂÎÒ ÒÓÒ ÈÇÌÅÍÅÍÎ
    return;
    qDebug()<<"in errorHandler with error "<<QString::number(errorCode)<<endl;
    DWORD dwWNetResult, dwLastError;
    CHAR szDescription[256];
    CHAR szProvider[256];

    QChar tlpdwtr1[256];
    QChar tlpdwtr2[256];
    dwWNetResult = WNetGetLastError(&dwLastError, // error code
                                    (LPTSTR)tlpdwtr1,
                                    sizeof(szDescription),  // size of error buffer
                                    (LPTSTR)tlpdwtr2,
                                    sizeof(szProvider));    // size of name buffer

    if(dwWNetResult==NO_ERROR){
        QString describing(tlpdwtr1);
        QString msg=" - "+dsc+"\n     error "+ QString::number(errorCode)+"; "+"\n     network provider error code "+QString::number(dwLastError);
        if(!describing.isEmpty()) msg+="\n"+QString::fromLocal8Bit("     îïèñàíèå: ")+describing;
        emit sendMessage(msg);
    }
    else emit sendMessage(QString::fromLocal8Bit("errorHandler îøèáêà ")+QString::number(dwWNetResult));


}


void NetworkResources::sendTreeWidget(QTreeWidgetItem *tmp){
    emit sendTreeWidgetItemSignal(tmp);
}


void NetworkResources::stop(){
    isRun = false;
}

QTreeWidgetItem* NetworkResources::createTreeWidgetItem(LPNETRESOURCE lpnr,int type){
    QTreeWidgetItem *item = new QTreeWidgetItem(0);
    QString str = QString::fromWCharArray(lpnr->lpRemoteName);
    item->setText(0,str.replace("\\\\",""));
    switch(type){
    case 1:
        item->setText(1,"DOMAIN");
        break;
    case 2:
        item->setText(1,"SERVER");
        break;
    case 3:
        item->setText(1,"SHARE");
        item->setText(0,str.replace(lastServer->text(0),""));
        break;
    case 4:
        item->setText(1,"FILE");
        break;
    case 5:
        item->setText(1,"GROUP");
        break;
    case 6:
        item->setText(1,"NETWORK");
        break;
    default:
        // error
        item->setText(1,"UNKNOWN");
        break;
    }

    return item;
}


