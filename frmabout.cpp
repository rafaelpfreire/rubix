#include "frmabout.h"
#include "ui_frmabout.h"

frmAbout::frmAbout(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmAbout)
{
    ui->setupUi(this);
}

frmAbout::~frmAbout()
{
    delete ui;
}

void frmAbout::retranslate()
{
    ui->retranslateUi(this);
}
