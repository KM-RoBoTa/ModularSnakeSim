#include "melosimula.h"
#include "ui_melosimula.h"
#include <QErrorMessage>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QPixmap>
#include <osg/Image>
#include <osgDB/WriteFile>
#include <editode.h>
#include "heighfield.h"
#include "snakeparameters.h"

Melosimula::Melosimula(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Melosimula)
{
    ui->setupUi(this);
    this->saved=true;
    this->simulator=NULL;
    this->setGeometry(200,10,800,80);
    this->visualizacion=NULL;
    //Copying neccesary files.
    QFile::copy(":/others/Resources/modelo2.3ds","./modelo2.3ds");
    QFile::copy(":/others/Resources/modelo3.3ds","./modelo3.3ds");
    QFile::copy(":/images/Resources/baldosa.bmp","./baldosa.bmp");
    this->gp=new GraphicParameters(this);
    //this->gp->setWindowFlags(Qt::FramelessWindowHint);
    //conecciones para las opciones gráficas
    connect(this->gp,SIGNAL(Gen_EjesButton_press(bool)),this,SLOT(onGraph_GenEjesButton_press(bool)));
    connect(this->gp,SIGNAL(GenFOGButton_press(bool)),this,SLOT(onGraph_GenFOGButton_press(bool)));
    connect(this->gp,SIGNAL(ModEjesButton_press(int,bool)),this,SLOT(onGraph_ModEjesButton_press(int,bool)));
    connect(this->gp,SIGNAL(modCADButton_press(int,bool)),this,SLOT(onGraph_modCADButton_press(int,bool)));
    connect(this->gp,SIGNAL(ModContactPoints_press(int,bool)),this,SLOT(onGraph_ModContactPoints_press(int,bool)));
    connect(this->gp,SIGNAL(ModSelector_Change(int)),this,SLOT(onGraph_ModSelector_Change(int)));
    connect(this->gp,SIGNAL(ModTrayectoriaButton_press(int,bool)),this,SLOT(onGraph_ModTrayectoria_Change(int,bool)));

    this->ag_plugin= new angleGenerator;
    //Conecciones de plugins
    connect(this->ag_plugin,SIGNAL(generationSucced(QString)),this,SLOT(onAngleGenerationSucced(QString)));
    connect(this->ag_plugin,SIGNAL(generationSucced(QString)),this->ui->stopButton,SLOT(click()));

    this->ui->actionGr_ficos->setEnabled(false);
    this->ui->actionHeighfield->setEnabled(false);
    this->ui->actionODE->setEnabled(false);
    this->ui->actionDetener->setEnabled(false);
    this->ui->actionLogFile->setEnabled(false);
    this->ui->actionSimular->setEnabled(false);
    this->ui->actionSnake->setEnabled(false);
    connect(this->ui->actionSimular,SIGNAL(triggered()),this->ui->SimularButton,SLOT(click()));
    connect(this->ui->actionDetener,SIGNAL(triggered()),this->ui->stopButton,SLOT(click()));
    connect(this->ui->actionODE,SIGNAL(triggered()),this->ui->ODEValues,SLOT(click()));
    connect(this->ui->actionGr_ficos,SIGNAL(triggered()),this->ui->GraphicValues,SLOT(click()));
    connect(this->ui->actionHeighfield,SIGNAL(triggered()),this->ui->Heighfield,SLOT(click()));
    connect(this->ui->actionSnake,SIGNAL(triggered()),this->ui->SnakeValues,SLOT(click()));
    connect(this->ui->actionNueva_Simulaci_n,SIGNAL(triggered()),this->ui->stopButton,SLOT(click()));
    connect(this->ui->actionNueva_Simulaci_n,SIGNAL(triggered()),this->ui->LoadAnglesButton,SLOT(click()));

    int screenX=QApplication::desktop()->screenGeometry().width();
    int width=this->geometry().width();
    int placeX=(int)((double)screenX/2 -  (double)width/2);
    this->setGeometry(placeX,200,this->geometry().width(),this->geometry().height());
}


Melosimula::~Melosimula()
{
    delete ui;
    delete this->visualizacion;
    delete this->simulator;
    delete this->gp;

}

void Melosimula::on_actionSalir_triggered()
{
    if (!this->saved)
    {

    }
    else
    {
        this->close();
    }
}


void Melosimula::on_actionAbout_triggered()
{
//    Acerca about;
//    about.setModal(true);
//    about.exec();
}

//simulador *Melosimula::getSimulator()
//{
//    return this->simulator;
//}

void Melosimula::on_SimularButton_clicked()
{
    //Si aùn no hay objeto simulador quiere decir que no se ha cargado ningun archivo
    if (this->simulator==NULL)
    {
        QErrorMessage errorMess;
        errorMess.showMessage("You have to create a simulation firs.");
        errorMess.setModal(true);
        errorMess.exec();
    }
    else
    {
        //Reiniciamos el multiplicador a 1.
        this->simulator->setVidMultiplier(1.0);
        this->ui->multiplier->setText(QString::number(1.0)+"x");
        //Creamos la visualización
        if(this->visualizacion==NULL)
        {
            visualizacion = new ViewerWidget(this->simulator->getViewer(),this);
            visualizacion->setGeometry( 0, 0, 800, 600 );
            visualizacion->show();
            this->ui->verticalLayout->addWidget(visualizacion);
            this->setGeometry(this->geometry().x(),this->geometry().y(),800,580);
        }
        bool result;
//        if (this->simulator->getState()==0)
//            this->setWindowState(Qt::WindowMinimized);
        result=this->simulator->simulate();
        if (!result) //Se realiza la simulaciòn y si hay errores se manejan.
        {
            QErrorMessage errorMess;
            errorMess.showMessage("Incomplete simulation, set all the parameters firs.");
            errorMess.setModal(true);
            errorMess.exec();
            delete(this->visualizacion);
            this->visualizacion=NULL;
            this->ui->verticalLayout->removeWidget(visualizacion);
            this->setGeometry(this->geometry().x(),this->geometry().y(),800,80);
            //Se habilitan los botones y las acciones
            this->ui->stopButton->setEnabled(false);
            this->ui->StepBWButton->setEnabled(false);
            this->ui->stepFWButton->setEnabled(false);
            this->ui->pausaButton->setEnabled(false);
            this->ui->SpeedButton->setEnabled(false);
            this->ui->GraphicValues->setEnabled(false);
            this->ui->reverseSpeedButton->setEnabled(false);
            this->ui->refreshButton->setEnabled(false);
            this->ui->actionGr_ficos->setEnabled(false);
            this->ui->actionDetener->setEnabled(false);
            //Se deshabilitan los botones de ODE y carga
            this->ui->SnakeValues->setEnabled(true);
            this->ui->ODEValues->setEnabled(true);
            this->ui->Heighfield->setEnabled(true);
            this->ui->LoadAnglesButton->setEnabled(true);
            this->ui->actionSnake->setEnabled(true);
            this->ui->actionODE->setEnabled(true);
            this->ui->actionHeighfield->setEnabled(true);
            this->ui->actionLogFile->setEnabled(true);
        }
        this->setWindowState(Qt::WindowActive);
        //Se habilitan los botones y las acciones
        this->ui->stopButton->setEnabled(true);
        this->ui->StepBWButton->setEnabled(true);
        this->ui->stepFWButton->setEnabled(true);
        this->ui->pausaButton->setEnabled(true);
        this->ui->SpeedButton->setEnabled(true);
        this->ui->GraphicValues->setEnabled(true);
        this->ui->reverseSpeedButton->setEnabled(true);
        this->ui->refreshButton->setEnabled(true);
        this->ui->actionGr_ficos->setEnabled(true);
        this->ui->actionDetener->setEnabled(true);
        //Se deshabilitan los botones de ODE y carga
        this->ui->SnakeValues->setEnabled(false);
        this->ui->ODEValues->setEnabled(false);
        this->ui->Heighfield->setEnabled(false);
        this->ui->LoadAnglesButton->setEnabled(false);
        this->ui->actionSnake->setEnabled(false);
        this->ui->actionODE->setEnabled(false);
        this->ui->actionHeighfield->setEnabled(false);
        this->ui->actionLogFile->setEnabled(false);
    }
    //Si la ventana está por debajo de los limites de la pantalla, subala hasta que quede dentro de la pantalla
    if(this->geometry().height()+this->geometry().y()>QApplication::desktop()->screenGeometry().height())
    {
        int screenX=QApplication::desktop()->screenGeometry().width();
        int width=this->geometry().width();
        int placeX=(int)((double)screenX/2 -  (double)width/2);
        this->setGeometry(placeX,100,this->geometry().width(),this->geometry().height());
    }
}

void Melosimula::on_LoadAnglesButton_clicked()
{
    //Creamos un cuadro para cargar un archivo, la ruta quedarà almacenada en NombreArchivo
    QString NombreArchivo;
    NombreArchivo = QFileDialog::getOpenFileName(this, tr("Select Angle File"),"../");
    QFile file(NombreArchivo);
    //Revisamos si podemos abrir el archivo
    if (file.open(QFile::ReadOnly))
    {
        //Tomamos la primera linea para verificar el encabezado
        QTextStream in(&file);
        QString text = in.readLine();
        if (text.split('\t').size()==3)
        {
            //Agregamos la ruta del archivo
            if (this->simulator==NULL)
                this->simulator=new simulador;
            else
                this->simulator->DeleteSnakes();
            this->ui->ruta->setText(NombreArchivo);
            this->simulator->addSnake(NombreArchivo,0);
            //Habilitamos los botones
            this->ui->SimularButton->setEnabled(true);
            this->ui->SnakeValues->setEnabled(true);
            this->ui->ODEValues->setEnabled(true);
            this->ui->Heighfield->setEnabled(true);
            this->ui->actionSnake->setEnabled(true);
            this->ui->actionODE->setEnabled(true);
            this->ui->actionHeighfield->setEnabled(true);
            this->ui->actionLogFile->setEnabled(true);
            this->ui->actionSimular->setEnabled(true);
        }
        else //Si el archivo no tiene el formato de encabezado correcto, se saca un mensaje de error
        {
            QErrorMessage errorMess;
            errorMess.showMessage("Unsupported File Format");
            errorMess.setModal(true);
            errorMess.exec();
        }
        file.close();
    }
    else if(NombreArchivo!="") //SI no se puede abrir el archivo se genera un mensaje de error
    {
        QErrorMessage errorMess;
        errorMess.showMessage("Unable to read the file");
        errorMess.setModal(true);
        errorMess.exec();
    }
}

void Melosimula::on_pausaButton_clicked()
{
    if (this->simulator==NULL)
    {
        QErrorMessage errorMess;
        errorMess.showMessage("You have to create a simulation first");
        errorMess.setModal(true);
        errorMess.exec();
    }
    else
    {
        this->simulator->pauseVid();
    }
}

void Melosimula::on_SnakeValues_clicked()
{
    if (this->simulator!=NULL)
    {
        if(this->simulator->getState()==0)
        {
            SnakeParameters sp;
            sp.setModal(true);
            sp.setParameters(this->simulator->getSnakeParameters(this->ui->ruta->text()));
            sp.setParametrosMotor(this->simulator->getParametrosMotor(this->ui->ruta->text()));
            if (sp.exec())
            {
                QMap <QString,double> values = sp.getParameters();
                this->simulator->setSnakeParameters(this->ui->ruta->text(),values);
                this->simulator->setParametrosMotor(this->ui->ruta->text(), sp.getParametersMotor());
            }
        }
    }
    else
    {
        QErrorMessage errorMess;
        errorMess.showMessage("You have to create a simulation first");
        errorMess.setModal(true);
        errorMess.exec();
    }
}

void Melosimula::on_reverseSpeedButton_clicked()
{
    if (this->simulator!=NULL)
    {
        //Quitamos la pausa del video si existe
        this->simulator->unpauseVid();
        //Tomamos el multiplicador actual
        double mult;
        mult=this->ui->multiplier->text().split('x')[0].toDouble();
        //Hacemos rotar el valor del multiplicador entre -1, -2, -4 y -10.
        if(mult == -1.0)
        {
            mult = -2.0 ;
        }
        else if (mult == -2.0)
        {
            mult = -4.0;
        }
        else if (mult == -4.0)
        {
            mult = -10.0;
        }
        else if (mult == -10.0)
        {
            mult = -1.0;
        }
        else
        {
            mult = -1.0;
        }
        //Asignamos el multiplicador y lo escribimos en el label
        this->simulator->setVidMultiplier(mult);
        this->ui->multiplier->setText(QString::number(mult)+"x");
    }
}

//Para hacer el paso a paso damos play cuando estamos presionados y pausa cuando soltamos el boton y establecemos el multiplciado a 0.5
void Melosimula::on_stepFWButton_pressed()
{
    if (this->simulator!=NULL)
    {
        this->simulator->setVidMultiplier(0.5);
        this->ui->multiplier->setText(QString::number(0.5)+"x");
        this->simulator->unpauseVid();
    }
}

void Melosimula::on_stepFWButton_released()
{
    if(this->simulator!=NULL)
    {
        this->simulator->pauseVid();
    }
}

//Para hacer el paso a paso damos play cuando estamos presionados y pausa cuando soltamos el boton y establecemos el multiplicador a -0.5
void Melosimula::on_StepBWButton_pressed()
{
    if(this->simulator!=NULL)
    {
        this->simulator->setVidMultiplier(-0.5);
        this->ui->multiplier->setText(QString::number(-0.5)+"x");
        this->simulator->unpauseVid();
    }
}

//Para hacer el paso a paso damos play cuando estamos presionados y pausa cuando soltamos el boton
void Melosimula::on_StepBWButton_released()
{
    if (this->simulator!=NULL)
    {
        this->simulator->pauseVid();
    }
}

void Melosimula::on_stopButton_clicked()
{
    if (this->simulator != NULL)
    {
        this->simulator->stopSimulation();
        //Se deshabilitan los botones
        this->ui->stopButton->setEnabled(false);
        this->ui->StepBWButton->setEnabled(false);
        this->ui->stepFWButton->setEnabled(false);
        this->ui->pausaButton->setEnabled(false);
        this->ui->SpeedButton->setEnabled(false);
        this->ui->GraphicValues->setEnabled(false);
        this->ui->reverseSpeedButton->setEnabled(false);
        this->ui->refreshButton->setEnabled(false);
        this->ui->actionGr_ficos->setEnabled(false);
        //Se habilitan los botones
        this->ui->SnakeValues->setEnabled(true);
        this->ui->ODEValues->setEnabled(true);
        this->ui->Heighfield->setEnabled(true);
        this->ui->LoadAnglesButton->setEnabled(true);
        this->ui->actionSnake->setEnabled(true);
        this->ui->actionODE->setEnabled(true);
        this->ui->actionHeighfield->setEnabled(true);
        this->ui->actionLogFile->setEnabled(true);
        this->ui->actionSimular->setEnabled(true);
        this->gp->hide();
    }
}

void Melosimula::on_refreshButton_clicked()
{
    if (this->simulator!=NULL)
    {
        this->simulator->refresh();
    }
}

void Melosimula::on_ODEValues_clicked()
{
    //Revisamos que la simulación no esté corriendo.
    if (this->simulator!=NULL)
    {
        if (this->simulator->getState()==0)
        {
            EditODE editode;
            editode.setParameters(this->simulator->getParameters());
            editode.setModal(true);
            if(editode.exec()==1)//Si fue aceptado
            {
                QMap <QString,double> parameters=editode.getParameters();
                this->simulator->changeODE(parameters["CFM"],parameters["ERP"],parameters["LIN_DAMPING"],parameters["ANG_DAMPING"],parameters["MAX_ANG_SPEED"],parameters["CONTACT_MAX_CORR_VEL"],parameters["STEP"]);
                this->simulator->changeSurfaceODE(parameters["MU"],parameters["BOUNCE"],parameters["BOUNCE_VEL"]);
            }
        }
        else
        {
            QErrorMessage errorMess;
            errorMess.showMessage("It is necessary to stop the simulation in order to modify ODE Parameters");
            errorMess.setModal(true);
            errorMess.exec();
        }
    }
    else
    {
        QErrorMessage errorMess;
        errorMess.showMessage("Load an angles file first.");
        errorMess.setModal(true);
        errorMess.exec();
    }
}

void Melosimula::on_SpeedButton_clicked()
{
    if (this->simulator!=NULL)
    {
        //si està en pausa, quitamos la pausa
        this->simulator->unpauseVid();
        //Tomamos el multiplicador actual
        double mult;
        mult=this->ui->multiplier->text().split('x')[0].toDouble();
        //Rotamos el valor del multiplicador entre 1, 2, 4 y 10
        if(mult == 1.0)
        {
            mult = 2.0;
        }
        else if (mult == 2.0)
        {
            mult = 4.0;
        }
        else if (mult == 4.0)
        {
            mult = 10.0;
        }
        else if (mult == 10.0)
        {
            mult = 1.0;
        }
        else
        {
            mult = 2.0;
        }
        //Asignamos el multiplicador y lo escribimos en el label
        this->simulator->setVidMultiplier(mult);
        this->ui->multiplier->setText(QString::number(mult)+"x");
    }
}

void Melosimula::on_Heighfield_clicked()
{
    if (this->simulator!=NULL)
    {
        if (this->simulator->getState()==0)
        {
            Heighfield hfDialog;
            hfDialog.setModal(true);
            hfDialog.setValues(this->simulator->getRutaHeightfield(),this->simulator->getRutaTextura(),this->simulator->getOrigenHeightfield(),this->simulator->getTamannoHeightfield(),this->simulator->getEscalaHeightfield());
            if (hfDialog.exec()==1)
            {
                QString rutaHF=hfDialog.getRutaHeighfield();
                QString rutaTX=hfDialog.getRutaTextura();
                QVector <double> origen=hfDialog.getOrigen();
                QVector <double> dimesiones=hfDialog.getTamanno();
                double limiteAltura=hfDialog.getHeighLimit();
                this->simulator->addHeighfield(rutaHF,rutaTX,origen,dimesiones,limiteAltura);
            }
        }
        else
        {
            QErrorMessage errorMess;
            errorMess.showMessage("You have to stop the simulation in order to change the Heightfield file");
            errorMess.setModal(true);
            errorMess.exec();
        }
    }
    else
    {
        QErrorMessage errorMess;
        errorMess.showMessage("Load an Angles file first");
        errorMess.setModal(true);
        errorMess.exec();
    }
}

void Melosimula::on_GraphicValues_clicked()
{
    if (this->simulator!=NULL)
    {
        if(this->simulator->getState()==1)//Si hay video reproduciendose
        {
            if (this->gp->isHidden())
            {
                this->gp->show();
                this->gp->setComboBoxList(this->simulator->getNumModulos(this->ui->ruta->text()));
                this->gp->setGeometry(this->geometry().x()+this->geometry().width(),this->geometry().y(),this->gp->width(),this->gp->height());
            }
            else
                this->gp->hide();
        }
        else
        {
            QErrorMessage errorMess;
            errorMess.showMessage("Load an Angle File first");
            errorMess.setModal(true);
            errorMess.exec();
        }
    }
    else
    {
        QErrorMessage errorMess;
        errorMess.showMessage("Load an Angle File first");
        errorMess.setModal(true);
        errorMess.exec();
    }
}

void Melosimula::onGraph_GenEjesButton_press(bool State)
{
    this->simulator->switchEjesGenerales(State);
}

void Melosimula::onGraph_GenFOGButton_press(bool State)
{
    this->simulator->switchFOGGeneral(State);
}

void Melosimula::onGraph_ModEjesButton_press(int Modulos, bool State)
{
    this->simulator->switchEjesSerpiente(this->ui->ruta->text(),Modulos,State);
}

void Melosimula::onGraph_modCADButton_press(int Modulos, bool State)
{
    this->simulator->switchModelo(this->ui->ruta->text(),Modulos,State);
}

void Melosimula::onGraph_ModContactPoints_press(int Modulos, bool State)
{
    this->simulator->switchPuntosContacto(this->ui->ruta->text(),State);
}

void Melosimula::onGraph_ModSelector_Change(int Modulo)
{
    QMap <QString,bool> states;
    if (Modulo!=-1)
        states=this->simulator->getGraphicsStates(this->ui->ruta->text(),Modulo);
    this->gp->setButtonsStates(Modulo,states);
}

void Melosimula::onGraph_ModTrayectoria_Change(int Modulo, bool State)
{
    this->simulator->switchTrayectoria(this->ui->ruta->text(),State,Modulo);
}

void Melosimula::on_actionLogFile_triggered(bool checked)
{
    if (this->simulator!=NULL)
    {
        this->simulator->setCrearLogFile(checked);
    }
}

void Melosimula::on_actionGenerador_de_ngulos_triggered()
{

    this->ag_plugin->show();
}

void Melosimula::onAngleGenerationSucced(QString ruta)
{

    QString NombreArchivo=ruta;
    QFile file(NombreArchivo);
    //Revisamos si podemos abrir el archivo
    if (file.open(QFile::ReadOnly))
    {
        //Tomamos la primera linea para verificar el encabezado
        QTextStream in(&file);
        QString text = in.readLine();
        if (text.split('\t').size()==3)
        {
            //Agregamos la ruta del archivo
            if (this->simulator==NULL)
                this->simulator=new simulador;
            else
                this->simulator->DeleteSnakes();
            this->ui->ruta->setText(NombreArchivo);
            this->simulator->addSnake(NombreArchivo,0);
            //Habilitamos los botones
            this->ui->SimularButton->setEnabled(true);
            this->ui->SnakeValues->setEnabled(true);
            this->ui->ODEValues->setEnabled(true);
            this->ui->Heighfield->setEnabled(true);
            this->ui->actionSnake->setEnabled(true);
            this->ui->actionODE->setEnabled(true);
            this->ui->actionHeighfield->setEnabled(true);
            this->ui->actionLogFile->setEnabled(true);
            this->ui->actionSimular->setEnabled(true);
        }
        else //Si el archivo no tiene el formato de encabezado correcto, se saca un mensaje de error
        {
            QErrorMessage errorMess;
            errorMess.showMessage("Unsupported Format File");
            errorMess.setModal(true);
            errorMess.exec();
        }
        file.close();
    }
}
