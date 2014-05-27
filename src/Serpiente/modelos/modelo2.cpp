#include "modelo2.h"
#include "ui_modelo2.h"

modelo2::modelo2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modelo2)
{
    ui->setupUi(this);
    this->rutaImagen=":/images/Resources/modelo2.png";
}

modelo2::modelo2(QMap<QString, double> values,QWidget *parent) :QWidget(parent),ui(new Ui::modelo2)
{
    ui->setupUi(this);
    this->rutaImagen=":/images/Resources/modelo2.png";
    this->setValues(values);
}

modelo2::~modelo2()
{
    delete ui;
}

//Recibe los valores en MKS y se muestran en mm y gramos
void modelo2::setValues(QMap <QString,double> values)
{
    this->ui->MasaCil_Value->setValue(values["Masa_Cilindro"]*1000);
    this->ui->OCil_Value->setValue(values["Offset_Cilindro"]*1000);
    this->ui->MasaCub_Value->setValue(values["Masa_Cubo"]*1000);
    this->ui->OCub_Value->setValue(values["Offset_Cubo"]*1000);
    this->ui->W_Value->setValue(values["W"]*1000);
    this->ui->H_Value->setValue(values["H"]*1000);
    this->ui->L_Value->setValue(values["L"]*1000);
    this->ui->Hc_Value->setValue(values["H_Cilindro"]*1000);
    this->ui->d1_Value->setValue(values["dInt_Cilindro"]*1000);
    this->ui->d2_Value->setValue(values["dExt_Cilindro"]*1000);
    this->ui->E_Value->setValue(values["H"]*500 - values["OFFSET_EJE"]*1000);
    this->ui->D_Value->setValue(values["H"]*1000 + values["OFFSET_MODULO"]*1000);

}

//Devuelve los valores en MKS
QMap <QString,double> modelo2::getValues()
{
    QMap <QString,double> rta;
    rta["Masa_Cilindro"]=this->ui->MasaCil_Value->value()/1000;
    rta["Offset_Cilindro"]=this->ui->OCil_Value->value()/1000;
    rta["Masa_Cubo"]=this->ui->MasaCub_Value->value()/1000;
    rta["Offset_Cubo"]=this->ui->OCub_Value->value()/1000;
    rta["W"]=this->ui->W_Value->value()/1000;
    rta["H"]=this->ui->H_Value->value()/1000;
    rta["L"]=this->ui->L_Value->value()/1000;
    rta["H_Cilindro"]=this->ui->Hc_Value->value()/1000;
    rta["dInt_Cilindro"]=this->ui->d1_Value->value()/1000;
    rta["dExt_Cilindro"]=this->ui->d2_Value->value()/1000;
    rta["OFFSET_EJE"]=this->ui->H_Value->value()/2000 - (this->ui->E_Value->value()/1000);
    rta["OFFSET_MODULO"]=this->ui->D_Value->value()/1000-this->ui->H_Value->value()/1000;
    return rta;
}

QString modelo2::getRutaImagen()
{
    return this->rutaImagen;
}
