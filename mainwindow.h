#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTimer>
#include <openglwidget.h>
#include <stopwatch.h>

#include "frmabout.h"
#include "frmload.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void loaded();

private:
    Ui::MainWindow *ui;
    QTimer* timer;

    frmAbout* formAbout;
    frmLoad* formLoad;

    int cont = 0;

private slots:
    void timerLimit();
};

#endif // MAINWINDOW_H
