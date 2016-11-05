#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTimer>
#include <QTranslator>
#include <QSettings>
#include <openglwidget.h>
#include <stopwatch.h>

#include "frmabout.h"
#include "frmload.h"
#include "frmsettings.h"

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
    void language_change();
    void loadSettings(QString lang);

private:
    Ui::MainWindow *ui;
    QTimer* timer;

    frmAbout* formAbout;
    frmLoad* formLoad;
    frmSettings* formSettings;

    QLabel* help;
    OpenGLWidget *glwidget;
    QTranslator translator;
    QTranslator translatorQt;
    QSettings* settings;

    QString helpContent;
    QString langDir, currLanguage = "en";

    bool firstRun;

    int cont = 0;

protected:
    void changeTranslator(QTranslator &translator, const QString &localFile);
    void readLanguage(const QString &language);
    void retranslateNonUI();
    void changeEvent(QEvent* event);

private:
    void readSettings();

private slots:
    void timerLimit();
    void saveSettings(const QString &lang);
    void quit();
};

#endif // MAINWINDOW_H
