#include "stopwatch.h"
#include <QDebug>

StopWatch::StopWatch(QStatusBar *parent)
{
    timer = NULL;
    startTime = 0;
    currentTime = 0;
    isRunnig = false;

    statusBar = parent;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerInterrupt()));

    statusBar->showMessage(tr("Time    00:00:00"));
}

int StopWatch::time_ms()
{
    if( isRunnig )
        return (totalTime() - this->startTime);
    else
        return (currentTime - startTime);
}

void StopWatch::start()
{
    isRunnig = true;
    currentTime = 0;
    startTime = totalTime();

    timer->start(STOPWATCH_TIMER_TIMEOUT_MS);
}

void StopWatch::stop()
{
    if( isRunnig ) {
        isRunnig = false;
        currentTime = totalTime();
    }

    timer->stop();

    QString str = QString(tr("Time:    ")) + this->toString();
    statusBar->showMessage(str);
}

void StopWatch::reset()
{
    isRunnig = false;
    startTime = 0;
    currentTime = 0;

    statusBar->showMessage(tr("Time:   00:00:00"));
}

void StopWatch::timerInterrupt()
{
    QString str = QString(tr("Time:    ")) + this->toString();
    statusBar->showMessage(str);
}

int StopWatch::totalTime()
{
    return (QTime::currentTime().msec() +
            QTime::currentTime().second()*1000 +
            QTime::currentTime().minute()*60000 +
            QTime::currentTime().hour()*3600000);
}

QString StopWatch::toString()
{
    int time = time_ms();
    int minute = time / 60000;
    int second = (time % 60000) / 1000;
    int msec   = ((time % 60000) % 1000)/10;

    QString minStr = (minute > 9) ? QString::number(minute) : QString("0")+QString::number(minute);
    QString secStr = (second > 9) ? QString::number(second) : QString("0")+QString::number(second);
    QString msStr  = (msec   > 9) ? QString::number(msec  ) : QString("0")+QString::number(msec  );

    return minStr + QString(":") + secStr + QString(":") + msStr;
}
