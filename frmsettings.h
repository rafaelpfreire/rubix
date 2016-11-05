#ifndef FRMSETTINGS_H
#define FRMSETTINGS_H

#include <QMainWindow>

namespace Ui {
class frmSettings;
}

class frmSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmSettings(QWidget *parent = 0);
    ~frmSettings();

public slots:
    void loadSettings(QString lang);
    void retranslate();

signals:
    void saveSettings(const QString &lang);

private slots:
    void on_btSave_clicked();

private:
    Ui::frmSettings *ui;
};

#endif // FRMSETTINGS_H
