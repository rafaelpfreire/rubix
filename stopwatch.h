#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QStatusBar>
#include <QTimer>
#include <QTime>


#define STOPWATCH_TIMER_TIMEOUT_MS  30

class StopWatch : public QObject
{
    Q_OBJECT
public:
    StopWatch(QStatusBar *parent);
    QString toString();
    int time_ms();

protected:
    int totalTime();

    QTimer *timer;
    QStatusBar *statusBar;
    int startTime;
    int currentTime;
    bool isRunnig;

public slots:
    void start();
    void stop();
    void reset();

protected slots:
    void timerInterrupt();
};

#endif // STOPWATCH_H
