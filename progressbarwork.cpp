#include "progressbarwork.h"
#include <QTimer>
#include <QApplication>



ProgressBarWork::ProgressBarWork(QProgressBar *bar)
{
    progressBar = bar;
    inc = 1;
    bar->setValue(0);
    //mtimer = new QTimer(this);
    //connect(mtimer,SIGNAL(timeout()),this,SLOT(work()));
    //mtimer->start(40);
}

void ProgressBarWork::run(){

}

void ProgressBarWork::work(){
    //progressBar->setValue(75);
    int value = progressBar->value();
//    if(value==100) value=0;



//    if(value==0) inc = 1;
//    else if(value==100) inc = -1;
//    progressBar->setValue(value+inc);

    if(value==0) inc = 1;
    else if(value==100) progressBar->setValue(0);
    progressBar->setValue(value+inc);
}



void ProgressBarWork::stop(){
    progressBar->setValue(0);
}
