#ifndef GETMAC_H
#define GETMAC_H

#include <QThread>
#include <windows.h>
#include <wincon.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <Winnetwk.h>
#include "NB30.h"
#include <mprapi.h>

class GetMac : public QThread
{
    Q_OBJECT
public:
    GetMac();
    void run();

private:
    void getMacAdress();
    void searchError();

signals:
    void searchResult(QString);
    void searchMacAdressCompleteSignal();
public slots:

};

#endif // GETMAC_H
