#include "frmload.h"
#include "ui_frmload.h"

frmLoad::frmLoad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmLoad)
{
    ui->setupUi(this);
}

frmLoad::~frmLoad()
{
    delete ui;
}

void frmLoad::finishLoad()
{
    frmLoad::close();
}

void frmLoad::retranslate()
{
    ui->retranslateUi(this);
}
