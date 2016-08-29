#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::showMaximized();

    QWidget *centralW = new QWidget(this);
    QGridLayout *glayout = new QGridLayout(centralW);

    OpenGLWidget *glwidget = new OpenGLWidget(this->statusBar());
    glayout->addWidget(glwidget, 0, 0, 2, 1);

    this->setCentralWidget(centralW);

    formAbout = new frmAbout(this);
    connect(ui->actionAbout, SIGNAL(triggered()), formAbout, SLOT(show()));
    connect(ui->actionStart_challenge, SIGNAL(triggered()), glwidget, SLOT(shuffleCube()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
