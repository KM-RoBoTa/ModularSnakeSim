#include "modelo1.h"
#include "ui_modelo1.h"

modelo1::modelo1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modelo1)
{
    ui->setupUi(this);
    this->rutaImagen=":/images/Resources/modelo1.png";

}

modelo1::modelo1( QMap<QString, double> values,QWidget *parent):
    QWidget(parent),
    ui(new Ui::modelo1)
{
    ui->setupUi(this);
    this->rutaImagen=":/images/Resources/modelo1.png";
    this->setValues(values);
}

modelo1::~modelo1()
{
    delete ui;
}

//Recibe los valores en MKS
void modelo1::setValues(QMap <QString,double> values)
{
    this->ui->Masa_Value->setValue(values["Masa"]*1000);
    this->ui->W_Value->setValue(values["W"]*1000);
    this->ui->H_Value->setValue(values["H"]*1000);
    this->ui->L_Value->setValue(values["L"]*1000);
    this->ui->E_Value->setValue(values["H"]*500 - values["OFFSET_EJE"]*1000);
    this->ui->D_Value->setValue(values["H"]*1000 + values["OFFSET_MODULO"]*1000);

}

//Devuelve los valores en MKS
QMap <QString,double> modelo1::getValues()
{
    QMap <QString,double> rta;
    rta["Masa"]=this->ui->Masa_Value->value()/1000;
    rta["W"]=this->ui->W_Value->value()/1000;
    rta["H"]=this->ui->H_Value->value()/1000;
    rta["L"]=this->ui->L_Value->value()/1000;
    rta["OFFSET_EJE"]=this->ui->H_Value->value()/2000 - (this->ui->E_Value->value()/1000);
    rta["OFFSET_MODULO"]=this->ui->D_Value->value()/1000-this->ui->H_Value->value()/1000;
    return rta;
}

QString modelo1::getRutaImagen()
{
    return this->rutaImagen;
}
