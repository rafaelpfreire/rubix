#include "frmsettings.h"
#include "ui_frmsettings.h"

frmSettings::frmSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmSettings)
{
    ui->setupUi(this);
}

frmSettings::~frmSettings()
{
    delete ui;
}

void frmSettings::loadSettings(QString lang)
{
    if (lang == "en") ui->comboLanguage->setCurrentIndex(0);
    else ui->comboLanguage->setCurrentIndex(1);
}

void frmSettings::retranslate()
{
    ui->retranslateUi(this);
}

void frmSettings::on_btSave_clicked()
{
    emit saveSettings(ui->comboLanguage->currentIndex() == 0 ? "en" : "pt");
    frmSettings::hide();
}
