#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    formLoad = new frmLoad(this);
    formLoad->show();

    timer = new QTimer();
    timer->setInterval(2000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerLimit()));
    timer->start();

    MainWindow::hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerLimit()
{
    if (cont == 0) cont++;
    else {
        timer->stop();

        QWidget *centralW = new QWidget(this);
        QGridLayout *glayout = new QGridLayout(centralW);

        OpenGLWidget *glwidget = new OpenGLWidget(this->statusBar());
        glwidget->setMinimumSize(QSize(400, 300));
        glayout->addWidget(glwidget, 0, 0, 2, 1);

        QLabel* help = new QLabel();
        help->setMaximumWidth(300);
        help->setMinimumWidth(300);
        QString helpContent = tr("Controls: \n"
                                 "\nSpace: Shuffle the cube and starts the count timer.\n"
                                 "\nUp arrow: Rotates the cube view upward."
                                 "\nDown arrow: Rotates the cube view downward."
                                 "\nLeft arrow: Rotates the cube view leftward."
                                 "\nRight arrow: Rotates the cube view rightward.\n"
                                 "\nW Key: Rotates the top row."
                                 "\nS Key: Rotates the bottom row."
                                 "\nA Key: Rotates the left column."
                                 "\nD Key: Rotates the right column."
                                 "\nQ Key: Rotates the rear face."
                                 "\nF Key: Rotates the frontal face.\n"
                                 "\nShift: When combined with movement buttons, it performs the countermove instead.");
        help->setText(helpContent);
        help->setFont(QFont( "Arial", 12, QFont::Normal));
        help->setWordWrap(true);
        glayout->addWidget(help, 0, 1, 2, 1);

        this->setCentralWidget(centralW);

        formAbout = new frmAbout(this);
        connect(ui->actionAbout, SIGNAL(triggered()), formAbout, SLOT(show()));
        connect(ui->actionStart_challenge, SIGNAL(triggered()), glwidget, SLOT(shuffleCube()));

        formLoad->close();
        this->showMaximized();
    }
}
