#include "snakeparameters.h"
#include "ui_snakeparameters.h"



SnakeParameters::SnakeParameters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SnakeParameters)
{
    ui->setupUi(this);
    this->actual=0;
    this->Mod1=new modelo1();
    this->Mod2=new modelo2();
    cont=new QVBoxLayout;
    cont->addWidget(this->Mod1);
    this->ui->ValoresWidget->setLayout(cont);
    QPixmap *pixmap= new QPixmap(this->Mod1->getRutaImagen());
    this->ui->imagen->setPixmap(*pixmap);
}

SnakeParameters::~SnakeParameters()
{
    delete ui;
}

QMap<QString, double> SnakeParameters::getParameters()
{
    QMap <QString, double> rta;
    if(this->actual==0) //MODELO 1
    {
        rta=Mod1->getValues();
        rta["Modelo"]=0;

    }
    else if (this->actual==1) //Modelo 2
    {
        rta=Mod2->getValues();
        rta["Modelo"]=1;
    }
    rta["Posi_X"]=this->ui->InPos_X_Value->value();
    rta["Posi_Y"]=this->ui->InPos_Y_Value->value();
    rta["Posi_Z"]=this->ui->InPos_Z_Value->value();
    rta["Quati_X"]=this->ui->InDir_X_Value->value();
    rta["Quati_Y"]=this->ui->InDir_Y_Value->value();
    rta["Quati_Z"]=this->ui->InDir_Z_Value->value();
    rta["Quati_W"]=this->ui->InDir_W_Value->value();

    return rta;
}

void SnakeParameters::setParametrosMotor(QMap <QString,double> values)
{
    this->ui->K_Value->setValue(values["k"]);
    this->ui->Fmax_Value->setValue(values["FMax"]);
    this->ui->Resolucion_Value->setValue(values["Resolucion"]);
    this->ui->FViscosa_Value->setValue(values["Fviscosa"]);
    this->ui->Velocidad_Value->setValue(values["Velocity"]);
}

QMap<QString, double> SnakeParameters::getParametersMotor()
{
    QMap<QString, double> Valor;
    Valor["k"]=this->ui->K_Value->value();
    Valor["FMax"]=this->ui->Fmax_Value->value();
    Valor["Resolucion"]=this->ui->Resolucion_Value->value();
    Valor["Fviscosa"]=this->ui->FViscosa_Value->value();
    Valor["Velocity"]=this->ui->Velocidad_Value->value();
    return Valor;
}

void SnakeParameters::setParameters(QMap<QString, double> values)
{
    this->ui->InPos_X_Value->setValue(values["Posi_X"]);
    this->ui->InPos_Y_Value->setValue(values["Posi_Y"]);
    this->ui->InPos_Z_Value->setValue(values["Posi_Z"]);
    this->ui->InDir_X_Value->setValue(values["Quati_X"]);
    this->ui->InDir_Y_Value->setValue(values["Quati_Y"]);
    this->ui->InDir_Z_Value->setValue(values["Quati_Z"]);
    this->ui->InDir_W_Value->setValue(values["Quati_W"]);
    if (values["Modelo"]==0)
    {
        while(this->actual!=0)
            this->on_NextButton_clicked();
        this->Mod1->setValues(values);
    }
    else
    {
        while(this->actual!=1)
            this->on_NextButton_clicked();
        this->Mod2->setValues(values);
    }
}

void SnakeParameters::on_NextButton_clicked()
{
    QMap <QString,double> values;
    QPixmap * pixmap;
    actual++;
    if (actual==2)
    {
        actual=0;
    }
    switch (actual)
    {
        case 0:
            values=this->Mod1->getValues();
            this->Mod1=new modelo1(values);
            cont->removeWidget(this->Mod2);
            delete(this->cont);
            cont=new QVBoxLayout;
            cont->addWidget(this->Mod1);
            this->ui->ValoresWidget->setLayout(cont);
            pixmap= new QPixmap(this->Mod1->getRutaImagen());
            this->ui->imagen->setPixmap(*pixmap);
            break;
        case 1:
            values=this->Mod2->getValues();
            this->Mod2=new modelo2(values);
            cont->removeWidget(this->Mod1);
            delete(this->cont);
            cont=new QVBoxLayout;
            cont->addWidget(this->Mod2);
            this->ui->ValoresWidget->setLayout(cont);
            pixmap=new QPixmap(this->Mod2->getRutaImagen());
            this->ui->imagen->setPixmap(*pixmap);
            break;
    }
}

void SnakeParameters::on_PrevButton_clicked()
{
    QPixmap * pixmap;
    actual--;
    if (actual==-1)
    {
        actual=1;
    }
    switch (actual)
    {
        case 0:
            this->Mod1=new modelo1();
            cont->removeWidget(this->Mod2);
            delete(this->cont);
            cont=new QVBoxLayout;
            cont->addWidget(this->Mod1);
            this->ui->ValoresWidget->setLayout(cont);
            pixmap= new QPixmap(this->Mod1->getRutaImagen());
            this->ui->imagen->setPixmap(*pixmap);
            break;
        case 1:
            this->Mod2=new modelo2();
            cont->removeWidget(this->Mod1);
            delete(this->cont);
            cont=new QVBoxLayout;
            cont->addWidget(this->Mod2);
            this->ui->ValoresWidget->setLayout(cont);
            pixmap=new QPixmap(this->Mod2->getRutaImagen());
            this->ui->imagen->setPixmap(*pixmap);
            break;
    }
}
