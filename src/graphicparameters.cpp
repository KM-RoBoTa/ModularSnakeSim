#include "graphicparameters.h"
#include "ui_graphicparameters.h"

GraphicParameters::GraphicParameters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphicParameters)
{
    ui->setupUi(this);
    connect(this->ui->Gen_Ejes_Button,SIGNAL(clicked()),this,SLOT(onGenEjesButton_press()));
    connect(this->ui->Gen_FOG_Button,SIGNAL(clicked()),this,SLOT(onGenFOGButton_press()));
    connect(this->ui->Mod_Ejes_Button,SIGNAL(clicked()),this,SLOT(onModEjesButton_press()));
    connect(this->ui->Mod_CADCub_Button,SIGNAL(clicked()),this,SLOT(onmodCADButton_press()));
    connect(this->ui->Mod_ContPoints_Button,SIGNAL(clicked()),this,SLOT(onModContactPoints_press()));
    connect(this->ui->Mod_Trayectoria_Button,SIGNAL(clicked()),this,SLOT(onModTrayectoria_press()));

}

GraphicParameters::~GraphicParameters()
{
    delete ui;
}

void GraphicParameters::setComboBoxList(int numModulos)
{
    if (this->ui->Mod_Selection->currentIndex()==-1&&numModulos!=0)
    {
        this->ui->Mod_Selection->addItem("--- All ---");
        for (int i = 0 ; i < numModulos ; i++)
        {
            this->ui->Mod_Selection->addItem("Module "+QString::number(i+1));
        }
        connect (this->ui->Mod_Selection,SIGNAL(currentIndexChanged(int)),this,SLOT(onModSelector_Change(int)));
    }
}

void GraphicParameters::setButtonsStates(int Modulo, QMap<QString, bool> states)
{
    if (Modulo==-1)
    {
        this->ui->Mod_Ejes_Button->setText("--");
        this->ui->Mod_Ejes_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(200, 200, 200);"));
        this->ui->Mod_CADCub_Button->setText("--");
        this->ui->Mod_CADCub_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(200, 200, 200);"));
        this->ui->Mod_Trayectoria_Button->setText("--");
        this->ui->Mod_Trayectoria_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(200, 200, 200);"));
    }
    else
    {
        if (states["Gen_Ejes"])
        {
            this->ui->Gen_Ejes_Button->setText("ON");
            this->ui->Gen_Ejes_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        }
        else
        {
            this->ui->Gen_Ejes_Button->setText("OFF");
            this->ui->Gen_Ejes_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        }
        if (states["Gen_FOG"])
        {
            this->ui->Gen_FOG_Button->setText("ON");
            this->ui->Gen_FOG_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        }
        else
        {
            this->ui->Gen_FOG_Button->setText("OFF");
            this->ui->Gen_FOG_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        }
        if (states["Mod_Ejes"])
        {
            this->ui->Mod_Ejes_Button->setText("ON");
            this->ui->Mod_Ejes_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        }
        else
        {
            this->ui->Mod_Ejes_Button->setText("OFF");
            this->ui->Mod_Ejes_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        }
        if (states["Mod_CadCub"])
        {
            this->ui->Mod_CADCub_Button->setText("CAD");
            this->ui->Mod_CADCub_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        }
        else
        {
            this->ui->Mod_CADCub_Button->setText("Box");
            this->ui->Mod_CADCub_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        }
        if (states["Mod_ContactPoints"])
        {
            this->ui->Mod_ContPoints_Button->setText("ON");
            this->ui->Mod_ContPoints_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        }
        else
        {
            this->ui->Mod_ContPoints_Button->setText("OFF");
            this->ui->Mod_ContPoints_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        }
        if (states["Mod_trayectoria"])
        {
            this->ui->Mod_Trayectoria_Button->setText("ON");
            this->ui->Mod_Trayectoria_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        }
        else
        {
            this->ui->Mod_Trayectoria_Button->setText("OFF");
            this->ui->Mod_Trayectoria_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        }

    }
    if (this->ui->Mod_Selection->currentIndex()!=Modulo+1)
    {
        this->ui->Mod_Selection->setCurrentIndex(Modulo+1);
    }
}
void GraphicParameters::onGenEjesButton_press()
{
    if (this->ui->Gen_Ejes_Button->text()=="OFF")
    {
        this->ui->Gen_Ejes_Button->setText("ON");
        this->ui->Gen_Ejes_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        emit Gen_EjesButton_press(true);
    }
    else
    {
        this->ui->Gen_Ejes_Button->setText("OFF");
        this->ui->Gen_Ejes_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        emit Gen_EjesButton_press(false);
    }

}

void GraphicParameters::onGenFOGButton_press()
{
    if (this->ui->Gen_FOG_Button->text()=="OFF")
    {
        this->ui->Gen_FOG_Button->setText("ON");
        this->ui->Gen_FOG_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        emit GenFOGButton_press(true);
    }
    else
    {
        this->ui->Gen_FOG_Button->setText("OFF");
        this->ui->Gen_FOG_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        emit GenFOGButton_press(false);
    }

}

void GraphicParameters::onModEjesButton_press()
{
    if (this->ui->Mod_Ejes_Button->text()=="OFF")
    {
        this->ui->Mod_Ejes_Button->setText("ON");
        this->ui->Mod_Ejes_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        emit ModEjesButton_press(this->ui->Mod_Selection->currentIndex()-1,true);
    }
    else
    {
        this->ui->Mod_Ejes_Button->setText("OFF");
        this->ui->Mod_Ejes_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        emit ModEjesButton_press(this->ui->Mod_Selection->currentIndex()-1,false);
    }

}

void GraphicParameters::onmodCADButton_press()
{
    if (this->ui->Mod_CADCub_Button->text()=="CAD")
    {
        this->ui->Mod_CADCub_Button->setText("Box");
        this->ui->Mod_CADCub_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        emit modCADButton_press(this->ui->Mod_Selection->currentIndex()-1,false);
    }
    else
    {
        this->ui->Mod_CADCub_Button->setText("CAD");
        this->ui->Mod_CADCub_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        emit modCADButton_press(this->ui->Mod_Selection->currentIndex()-1,true);
    }

}

void GraphicParameters::onModContactPoints_press()
{
    if (this->ui->Mod_ContPoints_Button->text()=="OFF")
    {
        this->ui->Mod_ContPoints_Button->setText("ON");
        this->ui->Mod_ContPoints_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        emit ModContactPoints_press(this->ui->Mod_Selection->currentIndex()-1,true);
    }
    else
    {
        this->ui->Mod_ContPoints_Button->setText("OFF");
        this->ui->Mod_ContPoints_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        emit ModContactPoints_press(this->ui->Mod_Selection->currentIndex()-1,false);
    }

}

void GraphicParameters::onModSelector_Change(int Modulo)
{
//    QMap <QString,bool> states;
//    if(this->ui->Gen_Ejes_Button->text()=="OFF")
//        states["Gen_Ejes"]=false;
//    else
//        states["Gen_Ejes"]=true;
//    if(this->ui->Gen_FOG_Button->text()=="OFF")
//        states["Gen_FOG"]=false;
//    else
//        states["Gen_FOG"]=true;
//    if(this->ui->Mod_Ejes_Button->text()=="OFF")
//        states["Mod_Ejes"]=false;
//    else
//        states["Mod_Ejes"]=true;
//    if(this->ui->Mod_CADCub_Button->text()=="Cubo")
//        states["Mod_CadCub"]=false;
//    else
//        states["Mod_CadCub"]=true;
//    if(this->ui->Mod_ContPoints_Button->text()=="OFF")
//        states["Mod_ContactPoints"]=false;
//    else
//        states["Mod_ContactPoints"]=true;
    emit ModSelector_Change(Modulo-1);
}

void GraphicParameters::onModTrayectoria_press()
{
    if (this->ui->Mod_Trayectoria_Button->text()=="OFF")
    {
        this->ui->Mod_Trayectoria_Button->setText("ON");
        this->ui->Mod_Trayectoria_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(42, 210, 0);"));
        emit ModTrayectoriaButton_press(this->ui->Mod_Selection->currentIndex()-1,true);
    }
    else
    {
        this->ui->Mod_Trayectoria_Button->setText("OFF");
        this->ui->Mod_Trayectoria_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        emit ModTrayectoriaButton_press(this->ui->Mod_Selection->currentIndex()-1,false);
    }
}


