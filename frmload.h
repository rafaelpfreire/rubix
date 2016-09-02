#ifndef FRMLOAD_H
#define FRMLOAD_H

#include <QMainWindow>

namespace Ui {
class frmLoad;
}

class frmLoad : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmLoad(QWidget *parent = 0);
    ~frmLoad();

private:
    Ui::frmLoad *ui;

public slots:
    void finishLoad();
    void retranslate();
};

#endif // FRMLOAD_H
