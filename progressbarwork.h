#ifndef STATUSBARWORK_H
#define STATUSBARWORK_H

#include <QThread>
#include<QProgressBar>


class ProgressBarWork : public QThread
{
    Q_OBJECT
public:
    ProgressBarWork(QProgressBar *);
    void run();
    void stop();
signals:

public slots:

private slots:
    void work();
private:
    QProgressBar *progressBar;
    int inc;
    QTimer *mtimer;


};

#endif // STATUSBARWORK_H
