#include "editode.h"
#include "ui_editode.h"

EditODE::EditODE(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditODE)
{
    ui->setupUi(this);
}

EditODE::~EditODE()
{
    delete ui;
}

QMap <QString, double> EditODE::getParameters()
{
    QMap <QString, double> rta;
    rta["CFM"]=this->ui->CFM->value();
    rta["ERP"]=this->ui->ERP->value();
    rta["LIN_DAMPING"]=this->ui->LINEAR_DAMP->value();
    rta["ANG_DAMPING"]=this->ui->ANGULAR_DAMP->value();
    rta["MAX_ANG_SPEED"]=this->ui->MAX_ANG_SPEED->value();
    rta["CONTACT_MAX_CORR_VEL"]=this->ui->CONTACT_MAX_COR_VEL->value();
    rta["STEP"]=this->ui->STEP->value();
    rta["MU"]=this->ui->MU->value();
    rta["BOUNCE"]=this->ui->BOUNCE->value();
    rta["BOUNCE_VEL"]=this->ui->BOUNCE_VEL->value();
    return rta;
}

void EditODE::setParameters(QMap <QString, double> values)
{
    this->ui->CFM->setValue(values["CFM"]);
    this->ui->ERP->setValue(values["ERP"]);
    this->ui->LINEAR_DAMP->setValue(values["LIN_DAMPING"]);
    this->ui->ANGULAR_DAMP->setValue(values["ANG_DAMPING"]);
    this->ui->MAX_ANG_SPEED->setValue(values["MAX_ANG_SPEED"]);
    this->ui->CONTACT_MAX_COR_VEL->setValue(values["CONTACT_MAX_CORR_VEL"]);
    this->ui->STEP->setValue(values["STEP"]);
    this->ui->MU->setValue(values["MU"]);
    this->ui->BOUNCE->setValue(values["BOUNCE"]);
    this->ui->BOUNCE_VEL->setValue(values["BOUNCE_VEL"]);
}
