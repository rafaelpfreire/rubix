#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::showFullScreen();

    QWidget *centralW = new QWidget(this);
    QGridLayout *glayout = new QGridLayout(centralW);

    OpenGLWidget *glwidget = new OpenGLWidget();

    glayout->addWidget(glwidget, 0, 0, 2, 1);

    this->setCentralWidget(centralW);
}

MainWindow::~MainWindow()
{
    delete ui;
}
