#ifndef FRMABOUT_H
#define FRMABOUT_H

#include <QMainWindow>

namespace Ui {
class frmAbout;
}

class frmAbout : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmAbout(QWidget *parent = 0);
    ~frmAbout();

public slots:
    void retranslate();

private:
    Ui::frmAbout *ui;
};

#endif // FRMABOUT_H
