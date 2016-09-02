#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    firstRun = true;

    formLoad = new frmLoad(this);
    connect(this, SIGNAL(language_change()), formLoad, SLOT(retranslate()));

    formAbout = new frmAbout(this);
    connect(ui->actionAbout, SIGNAL(triggered()), formAbout, SLOT(show()));
    connect(this, SIGNAL(language_change()), formAbout, SLOT(retranslate()));

    formSettings = new frmSettings(this);
    connect(formSettings, SIGNAL(saveSettings(QString)), this, SLOT(saveSettings(QString)));
    connect(ui->actionSettings, SIGNAL(triggered(bool)), formSettings, SLOT(show()));
    connect(this, SIGNAL(language_change()), formSettings, SLOT(retranslate()));
    connect(this, SIGNAL(loadSettings(QString)), formSettings, SLOT(loadSettings(QString)));

    langDir = QApplication::applicationDirPath();
    langDir.append("/Languages/");
    settings = new QSettings("Nintersoft","Rubix");
    if (settings->childGroups().contains("general", Qt::CaseInsensitive)) readSettings();

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

        glwidget = new OpenGLWidget(this->statusBar());
        glwidget->setMinimumSize(QSize(400, 300));
        glayout->addWidget(glwidget, 0, 0, 2, 1);

        help = new QLabel(this);
        help->setMaximumWidth(300);
        help->setMinimumWidth(300);
        helpContent = tr("Controls: \n"
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

        connect(ui->actionStart_challenge, SIGNAL(triggered()), glwidget, SLOT(shuffleCube()));
        connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quit()));

        firstRun = false;

        formLoad->close();
        this->showMaximized();
    }
}

void MainWindow::readSettings()
{
    settings->beginGroup("general");
    if (settings->value("language").toString() != currLanguage){
        currLanguage = settings->value("language").toString();
        readLanguage(currLanguage);
    }
    settings->endGroup();
    emit loadSettings(currLanguage);
}

void MainWindow::changeTranslator(QTranslator &translator, const QString &localFile)
{
    QApplication::removeTranslator(&translator);
    if(translator.load(localFile)) QApplication::installTranslator(&translator);
}

void MainWindow::readLanguage(const QString &language)
{
    currLanguage = language;
    changeTranslator(translator, QString(langDir + "Rubix_%1.qm").arg(language));
    changeTranslator(translatorQt, QString(langDir + "qt_%1.qm").arg(language));
}

void MainWindow::saveSettings(const QString &lang)
{
    if (currLanguage != lang){
        currLanguage = lang;
        readLanguage(lang);
    }
    settings->clear();
    settings->beginGroup("general");
    settings->setValue("language", lang);
    settings->endGroup();
}

void MainWindow::changeEvent(QEvent* event)
{
    if(0 != event) {
        switch(event->type()) {

            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                if (!firstRun) retranslateNonUI();
                emit language_change();
                emit loadSettings(currLanguage);
            break;

            case QEvent::LocaleChange:
            {
                QString local = QLocale::system().name();
                local.truncate(local.lastIndexOf('_'));
                readLanguage(local);
            }
            break;
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::retranslateNonUI()
{
    helpContent = tr("Controls: \n"
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
}

void MainWindow::quit()
{
    QApplication::quit();
}
