#include "getmac.h"
#include <QDebug>

typedef struct _ASTAT_
{

   ADAPTER_STATUS adapt;
   NAME_BUFFER    NameBuff [30];

}ASTAT, * PASTAT;

ASTAT Adapter;

GetMac::GetMac()
{
}

void GetMac::getMacAdress(){
    NCB Ncb;
    UCHAR uRetCode;
    LANA_ENUM   lenum;
    int      i;

    memset( &Ncb, 0, sizeof(Ncb) );
    Ncb.ncb_command = NCBRESET;
    Ncb.ncb_buffer = (UCHAR *)&lenum;
    Ncb.ncb_length = sizeof(lenum);
    uRetCode = Netbios( &Ncb );

    memset( &Ncb, 0, sizeof(Ncb) );
    Ncb.ncb_command = NCBENUM;
    Ncb.ncb_buffer = (UCHAR *)&lenum;
    Ncb.ncb_length = sizeof(lenum);
    uRetCode = Netbios( &Ncb );

    if(uRetCode == 0){

        for(i=0; i < lenum.length ;i++)
        {
            memset( &Ncb, 0, sizeof(Ncb) );
            Ncb.ncb_command = NCBRESET;
            Ncb.ncb_lana_num = lenum.lana[i];

            uRetCode = Netbios( &Ncb );
            if(uRetCode != 0){
                searchError();
                return;
            }
            memset( &Ncb, 0, sizeof(Ncb) );
            Ncb.ncb_command = NCBASTAT;
            Ncb.ncb_lana_num = lenum.lana[i];

            strcpy( (char*)Ncb.ncb_callname,  "*               " );
            Ncb.ncb_buffer = (unsigned char*)  &Adapter;
            Ncb.ncb_length = sizeof(Adapter);

            uRetCode = Netbios( &Ncb );
            if ( uRetCode == 0 )
            {
                QString strAdress = "%1-%2-%3-%4-%5-%6";
                strAdress = strAdress.arg(Adapter.adapt.adapter_address[0],2,16)
                        .arg(Adapter.adapt.adapter_address[1],2,16)
                        .arg(Adapter.adapt.adapter_address[2],2,16)
                        .arg(Adapter.adapt.adapter_address[3],2,16)
                        .arg(Adapter.adapt.adapter_address[4],2,16)
                        .arg(Adapter.adapt.adapter_address[5],2,16).toUpper().replace(" ","0");
                emit searchResult(strAdress);
                qDebug() << strAdress;
            }
            else{
                searchError();
                return;
            }

        }
        emit searchMacAdressCompleteSignal();
    }
    else{
        searchError();
        return;
    }
}

void GetMac::searchError(){
    emit searchResult("Mac adress search error");
}


void GetMac::run(){
    getMacAdress();
}

