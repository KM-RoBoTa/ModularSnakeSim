#include "simulador.h"

simulador::simulador()
{
    this->CFM=1e-4;
    this->ERP=0.9;
    this->LINEAR_DAMPING = 0.0001;
    this->ANGULAR_DAMPING=0.005;
    this->MAX_ANGULAR_SPEED=200;
    this->CONTACT_MAX_CORRECTING_VEL=0.1;
    this->mu_value=0.8;
    this->bounce_value=0.000010;
    this->bounce_vel_value=1.0;
    this->STEP=0.0001;
    this->state=0;
    this->sim_time=0;
    this->osg_timer=0;
    this->Snakes.clear();
    this->motorODE=NULL;
    this->RutaHeighField="";
    this->RutaTextura="";
    this->freqMuestreo=0.1;
    this->crearLog=false;
}

simulador::~simulador()
{
    delete this->motorODE;
}

bool simulador::addSnake(QString Ruta, int modelo)
{
    snake * newSnake= new snake;
    newSnake->AnglesFile=Ruta;
    newSnake->H=0.05;
    newSnake->W=0.032;
    newSnake->L=0.04;
    newSnake->QUAT_X=1.0;
    newSnake->QUAT_Y=0.0;
    newSnake->QUAT_Z=0.0;
    newSnake->QUAT_ANGULO=1.57079633;
    newSnake->Posi_x=0;
    newSnake->Posi_y=0.05;
    newSnake->Posi_z=0;
    if (modelo==0)
    {
        newSnake->MASA=0.055;
        newSnake->OFFSET_MODULO=0.0175;
        newSnake->OFFSET_EJE=0.0135;
    }
    else if (modelo==1)
    {
        newSnake->Masa_Cilindro=0.01;
        newSnake->Offset_Cilindro=0.022;
        newSnake->Masa_Cubo=0.055;
        newSnake->Offset_Cubo=0.011;
        newSnake->H_Cilindro=0.035;
        newSnake->dInt_Cilindro=0.035;
        newSnake->dExt_Cilindro=0.075;
    }
    //Parametros del motor
    newSnake->k=1.95;
    newSnake->Fmax= 1.5;
    newSnake->resolucion=(0.29*3.1415/180);
    newSnake->Fviscosa=0.2;
    newSnake->Velocity=-6.178465545;
    newSnake->model=modelo;
    newSnake->dt=flujo.tomarDeltaT(Ruta);
    newSnake->fileSize=flujo.contarLineas(Ruta);
    newSnake->numModules=flujo.tomarTamannoSerpiente(Ruta);
    //Fallo la lectura del archivo en algún punto
    if (newSnake->dt==-1||newSnake->fileSize==-1||newSnake->numModules==-1)
        return false;
    newSnake->currentCounterPosition=0;
    this->Snakes[Ruta]=newSnake;
    if(((double)this->Snakes[Ruta]->fileSize-1)*this->Snakes[Ruta]->dt > this->sim_time)
        this->sim_time=(this->Snakes[Ruta]->fileSize-1)*this->Snakes[Ruta]->dt;
    return true;
}

////Si el modelo es 1 el orden es:
////H,W,L,MASA,QUAT_X,QUAT_Y,QUAT_Z,QUAT_ANGULO,Posi_x,Posi_y,Posi_z,OFFSET_MODULO,OFFSET_EJE
////Si el modelo es 2 el orden es:
////
//void simulador::addSnake(QString Ruta, int modelo, ...)
//{
//}

void simulador::changeODE(double CFM, double ERP, double LINEAR_DAMPING, double ANGULAR_DAMPING, double MAX_ANGULAR_SPEED, double CONTACT_MAX_CORRECTING_VEL, double STEP)
{
    this->CFM=CFM;
    this->ERP=ERP;
    this->LINEAR_DAMPING=LINEAR_DAMPING;
    this->ANGULAR_DAMPING=ANGULAR_DAMPING;
    this->MAX_ANGULAR_SPEED=MAX_ANGULAR_SPEED;
    this->CONTACT_MAX_CORRECTING_VEL=CONTACT_MAX_CORRECTING_VEL;
    this->STEP=STEP;
}

void simulador::changeSurfaceODE(double mu, double bounce, double bounce_vel)
{
    this->bounce_value=bounce;
    this->mu_value=mu;
    this->bounce_vel_value=bounce_vel;
}

////////////////////////////// IMPORTANTE LAS FUNCIONES DE ODE NO ESTA ADAPTADAS PARA VARIAS SERPIENTES! QUEDA CON LA PRIMERA SERPIENTE SOLAMENTE!!!

bool simulador::simulate()
{

     QString Linea2="";
     QString Linea="";
    //Si no hay serpientes no simule nada
    if ((this->Snakes).isEmpty())
    {
        return false;
    }
    //Si está en stop debe volver a crear todo
    if (this->state == 0)
    {

        //Se pide el nombre del archivo donde va guardado el LOG de posion y de torque y fuerzas
        if (this->crearLog)
        {
            QMessageBox msgBox;
            msgBox.setText("Please specify the path for the Log files");
            msgBox.exec();
            LogPR = QFileDialog::getSaveFileName(0, QObject::tr("Position and orientation Log File"),QObject::tr("PRLogfile.log"),QObject::tr("*.log"));
            LogAVT = QFileDialog::getSaveFileName(0, QObject::tr("Angles, moment and speed Log File"),QObject::tr("AVTLogfile.log"),QObject::tr("*.log"));
        }
        QProgressDialog *progress=new QProgressDialog;
        progress->setLabelText("Simulating...");
        progress->setMaximum(100);
        progress->setMinimum(0);
        progress->setWindowModality(Qt::WindowModal);
        progress->setWindowState(Qt::WindowActive);
        progress->setGeometry(QApplication::desktop()->screenGeometry().width()/2-progress->geometry().width()/2,QApplication::desktop()->screenGeometry().height()/2-progress->geometry().height()/2,progress->geometry().width(),progress->geometry().height());
        progress->setWindowFlags(Qt::WindowStaysOnTopHint);
        //Creamos el motor de ode
        this->motorODE = new simODE();
        this->motorODE->setParameters(this->CFM,this->ERP,this->LINEAR_DAMPING,this->ANGULAR_DAMPING,this->MAX_ANGULAR_SPEED,this->CONTACT_MAX_CORRECTING_VEL,this->STEP);
        this->motorODE->setSurfaceParameters(this->mu_value,this->bounce_value,this->bounce_vel_value);
        //Recorremos todas las serpientes agregándolas al motor ode
        QMapIterator <QString, snake*> i(this->Snakes);
        while (i.hasNext())
        {
            i.next();
            this->motorODE->crearSerpiente(i.value()->numModules+1,this->getSnakeParameters(i.key()));
            this->visualizador.AgregarSerpiente(i.value()->numModules+1,i.value()->H,i.value()->W,i.value()->L,i.value()->model);
            /// El cero debe cambiarse para varias serpientes
            this->motorODE->setParametrosMotor(this->getParametrosMotor(i.key()),0);
        }

        //Si se definió un HF
        if (this->RutaHeighField!="")
        {
            //Creamos el geode que va a contener el heighfield construido por simDatos
            osg::ref_ptr<osg::Geode> hf=new osg::Geode;
            //Valores de origen y tamaño
            osg::Vec3 vecOrigen(this->OrigenHF[0],this->OrigenHF[1],this->OrigenHF[2]);
            osg::Vec3 size(this->TamannoHF[0],this->TamannoHF[1],0);
            //Columnas y filas del archivo, tamaño en pixeles del archivo
            int c,r;
            //Altura mínima y máxima para ODE
            float max_height,min_height;
            //Solicitamos los datos
            //si no se definió una textura
            double * hfODE;
            if (this->RutaTextura=="")
            {
                this->RutaTextura=":/images/Resources/baldosa.bmp";
                hfODE = flujo.leerHeighfield(this->RutaHeighField,this->RutaTextura, c,  r, max_height, min_height,hf,size,this->escalaHF);
            }
            else
            {
                hfODE = flujo.leerHeighfield(this->RutaHeighField,this->RutaTextura, c,  r, max_height, min_height,hf,size,this->escalaHF);
            }
            //Agregamos el heighfield al visualizador.
            visualizador.AgregarHeighfield(hf,vecOrigen);
            //Agregamos el heighfield a ODE
            this->motorODE->crearHeightfield(hfODE,size[0],size[1],c,r,this->escalaHF/255,0,0.5,min_height,max_height);
        }
        else
        {
            this->motorODE->crearPlano();
            this->visualizador.DibujarAmbiente();
        }

        this->pauseVid();
        //Salir del stado stop al estado simulando
        this->state=1;
        //Variable para hacer la simulación hasta que no haya más datos que recolectar
        bool step_done=true;
        //Variable para calcular que hayan pasado los 100ms aproximados para guardar un dato en OSG
        while(step_done&&!progress->wasCanceled())
        {
            this->osg_timer+=this->STEP;
            step_done=false;
            //Recorremos todas las serpientes buscando que: 1) Hayan más datos en el archivo que falten pro procesar y 2) El step de ode concuerda con
            //el delta t de una serpiente para tomar datos nuevamente.
            QMapIterator <QString, snake*> i(this->Snakes);
            while (i.hasNext())
            {
                i.next();
                //Caso 1, verificamos que al menis una serpiente tenga valores para ofrecer
                if(i.value()->currentCounterPosition<i.value()->fileSize)
                {
                    step_done=true;
                    //Verificamos que ya pasó el tiempo entre lectura de lineas y que no haya acabado el archivo
                    if(this->motorODE->getCurrentStep()*this->STEP>=(i.value()->currentCounterPosition)*i.value()->dt)
                    {
                        i.value()->currentCounterPosition ++;
                        this->motorODE->configurarSerpiente(0,this->flujo.tomarLinea(i.key(),i.value()->currentCounterPosition));
                    }
                }
            }
            if (this->osg_timer>this->freqMuestreo&&this->crearLog)
            {
                if(LogPR!="")
                {
                    Linea="";
                    //Para posicion y rotacio
                    for (int j = 0  ; j < (motorODE->getSerpienteNumero(0))->getTamano() ; j++)
                    {
                        const dReal *realP1 = dBodyGetPosition(*((motorODE->getSerpienteNumero(0))->getModuloPos(j)));
                        //Tomamos la rotación del módulo i
                        const dReal *realR1 = dBodyGetQuaternion(*((motorODE->getSerpienteNumero(0))->getModuloPos(j)));
                        Linea+=QString::number(realP1[0])+'\t';
                        Linea+=QString::number(realP1[1])+'\t';
                        Linea+=QString::number(realP1[2])+'\t';
                        if (j!=(motorODE->getSerpienteNumero(0))->getTamano()-1)
                            Linea+='\t';

                    }
                flujo.escribirLineaLog(LogPR,Linea);
                }
                if (LogAVT!="")
                {
                    Linea2="";
                    //Para Angulos, velocidades y torques
                    QVector <double > angulos;
                    QVector <double > velocidades;
                    QVector <double *> torques;

                    /// Cero para el caso de 1 serpiente.... cambiar para varias serpientes
                    angulos=motorODE->getCurrentAngulos(0);
                    velocidades=motorODE->getCurrentVelocidades(0);
                    torques= motorODE->getCurrentTorques(0);
                    for (int j=0 ; j < angulos.size() ; j ++)
                    {
                        Linea2+=QString::number(angulos[j])+'\t';
                    }
                    for (int j=0 ; j < velocidades.size() ; j ++)
                    {
                        Linea2+=QString::number(velocidades[j])+'\t';
                    }
                    for (int j=0 ; j < torques.size() ; j ++)
                    {
                        for(int k = 0 ; k < 12 ; k ++)
                        {
                            if (k==11&&j==torques.size()-1)
                                Linea2+=QString::number(torques[j][k]);
                            else
                                Linea2+=QString::number(torques[j][k])+'\t';
                        }
                    }
                flujo.escribirLineaLog(LogAVT,Linea2);
                }


            }

            //si el tiempo del contador de osg es mayor a 100 ms tome una muestra.
            if (this->osg_timer>0.1)
            {

                for(int j=0;j<i.value()->numModules+1;j++)
                {
                    /*
                    cuando queramos traer los datos de realimentacion de juntura, no usamos una funcion de
                    ODE sino que accedemos directamente al apuntador que entrega el getter de junturaFeedback
                    */

                    const dReal *realP1 = dBodyGetPosition(*((motorODE->getSerpienteNumero(0))->getModuloPos(j)));
                    //Tomamos la rotación del módulo i
                    const dReal *realR1 = dBodyGetQuaternion(*((motorODE->getSerpienteNumero(0))->getModuloPos(j)));
                    osg::Vec3 pos( realP1[0], realP1[1], realP1[2]);
                    osg::Quat rot( realR1[1],realR1[2],realR1[3],realR1[0] );
                    visualizador.AgregarControlPoint(0, j, motorODE->getCurrentStep()*STEP, pos , rot);
                }
                this->osg_timer=0;
            }
            progress->setValue((int)((this->motorODE->getCurrentStep()*this->STEP)*100/this->sim_time));
            this->motorODE->cicloODE();
        }
        /// El cero se debe cambiar para implementaciòn con varias serpientes
        visualizador.setContactPoints(0,this->getContactPoints());
        this->visualizador.setTrayectory();
        delete (progress);
    }
    this->unpauseVid();
    return true;
}

void simulador::stopSimulation()
{
    this->visualizador.deleteSnakes();
    delete (this->motorODE);
    this->motorODE=NULL;

    QMapIterator <QString, snake*> i(this->Snakes);
    while (i.hasNext())
    {
        i.next();
        i.value()->currentCounterPosition=0;
    }
    this->state=0;
}

osg::ref_ptr<osgViewer::Viewer> simulador::getViewer()
{
    return this->visualizador.getViewer().release();
}

void simulador::setParametrosMotor(QString ruta, QMap<QString, double > value)
{
    this->Snakes[ruta]->k=value["k"];
    this->Snakes[ruta]->Fmax=value["FMax"];
    this->Snakes[ruta]->Fviscosa=value["Fviscosa"];
    this->Snakes[ruta]->Velocity=value["Velocity"];
    this->Snakes[ruta]->resolucion=value["Resolucion"];
}

QMap<QString, double> simulador::getParametrosMotor(QString ruta)
{
    QMap <QString, double> Valor;
    Valor["k"]=this->Snakes[ruta]->k;
    Valor["FMax"]=this->Snakes[ruta]->Fmax;
    Valor["Resolucion"]=this->Snakes[ruta]->resolucion;
    Valor["Fviscosa"]=this->Snakes[ruta]->Fviscosa;
    Valor["Velocity"]=this->Snakes[ruta]->Velocity;
    return Valor;
}

int simulador::getNumSnakes()
{
    return this->Snakes.size();
}

void simulador::pauseVid()
{
    this->visualizador.pause();
}
void simulador::unpauseVid()
{
    this->visualizador.unpause();
}

void simulador::setVidMultiplier(double multiplier)
{
    this->visualizador.setMultiplier(multiplier);
}


//ERROR PENDIENTE: Cuando hay varias serpientes y se refrescan 2 archivos en tiempos diferentes, se agregan cada 100 ms puntos de control, generando una pausa en la serpiente que no se actualizò de primero.
bool simulador::refresh()
{
    //Siempre que la simulacion no estè en stop
    if (this->state == 1)
    {

        //Actualizamos todos los archiivos
        QMapIterator <QString, snake*> i(this->Snakes);
        bool cambio = false;
        this->sim_time=0;
        while (i.hasNext())
        {
            i.next();
            i.value()->fileSize=flujo.contarLineas(i.key());
            if (i.value()->currentCounterPosition < i.value()->fileSize)
            {
                cambio = true;
                /// PARA CUADRAR LA SIMULACION ES NECESARIO TENER EN CUENTA CUANTO TIEMPO DURA LO QUE RECIEN SE SIMULA.
                if (this->sim_time<(i.value()->fileSize-1)*i.value()->dt)
                    this->sim_time=(i.value()->fileSize-1)*i.value()->dt;
            }
        }

        //Sòlo si hubo un cambio en los archivos es necesario continuar haciendo steps de simulaciòn. Controla steps en falso.
        if (cambio)
        {
            QString Linea2="";
            QString Linea="";
            QProgressDialog *progress=new QProgressDialog;
            progress->setLabelText("Simulating...");
            progress->setMaximum(100);
            progress->setMinimum(0);
            progress->setWindowModality(Qt::WindowModal);
            progress->setWindowState(Qt::WindowActive);
            progress->setGeometry(QApplication::desktop()->screenGeometry().width()/2-progress->geometry().width()/2,QApplication::desktop()->screenGeometry().height()/2-progress->geometry().height()/2,progress->geometry().width(),progress->geometry().height());
            progress->setWindowFlags(Qt::WindowStaysOnTopHint);
            //Variable para hacer la simulación hasta que no haya más datos que recolectar
            bool step_done=true;
            //Variable para calcular que hayan pasado los 100ms aproximados para guardar un dato en OSG
            while(step_done&&!progress->wasCanceled())
            {
                this->osg_timer+=this->STEP;
                step_done=false;
                //Recorremos todas las serpientes buscando que: 1) Hayan más datos en el archivo que falten pro procesar y 2) El step de ode concuerda con
                //el delta t de una serpiente para tomar datos nuevamente.
                QMapIterator <QString, snake*> i(this->Snakes);
                while (i.hasNext())
                {
                    i.next();
                    //Caso 1, verificamos que al menis una serpiente tenga valores para ofrecer
                    if(i.value()->currentCounterPosition<i.value()->fileSize)
                    {
                        step_done=true;
                        //Verificamos que ya pasó el tiempo entre lectura de lineas y que no haya acabado el archivo
                        if(this->motorODE->getCurrentStep()*this->STEP>=(i.value()->currentCounterPosition)*i.value()->dt)
                        {
                            i.value()->currentCounterPosition ++;
                            this->motorODE->configurarSerpiente(0,this->flujo.tomarLinea(i.key(),i.value()->currentCounterPosition));
                        }
                    }
                }
                //Si el contador es mayor a la frecuencia de muestreo con la que se crea el log
                if (this->osg_timer>this->freqMuestreo&&this->crearLog)
                {
                    if(LogPR!="")
                    {
                        Linea="";
                        //Para posicion y rotacio
                        for (int j = 0  ; j < (motorODE->getSerpienteNumero(0))->getTamano() ; j++)
                        {
                            if(j==0||j==7||j==16)
                            {
                            const dReal *realP1 = dBodyGetPosition(*((motorODE->getSerpienteNumero(0))->getModuloPos(j)));
                            //Tomamos la rotación del módulo i
                            const dReal *realR1 = dBodyGetQuaternion(*((motorODE->getSerpienteNumero(0))->getModuloPos(j)));
                            Linea+=QString::number(realP1[0])+'\t';
                            //Linea+=QString::number(realP1[1])+'\t';
                            Linea+=QString::number(realP1[2])+'\t';
                            if (j!=(motorODE->getSerpienteNumero(0))->getTamano()-1)
                                Linea+='\t';
                            }
                        }
                        flujo.escribirLineaLog(LogPR,Linea);
                    }
                    if (LogAVT!="")
                    {
                        Linea2="";
                        //Para Angulos, velocidades y torques
                        QVector <double > angulos;
                        QVector <double > velocidades;
                        QVector <double *> torques;

                        /// Cero para el caso de 1 serpiente.... cambiar para varias serpientes
                        angulos=motorODE->getCurrentAngulos(0);
                        velocidades=motorODE->getCurrentVelocidades(0);
                        torques= motorODE->getCurrentTorques(0);
                        for (int j=0 ; j < angulos.size() ; j ++)
                        {
                            Linea2+=QString::number(angulos[j])+'\t';
                        }
                        for (int j=0 ; j < velocidades.size() ; j ++)
                        {
                            Linea2+=QString::number(velocidades[j])+'\t';
                        }
                        for (int j=0 ; j < torques.size() ; j ++)
                        {
                            for(int k = 0 ; k < 12 ; k ++)
                            {
                                if (k==11&&j==torques.size()-1)
                                    Linea2+=QString::number(torques[j][k]);
                                else
                                    Linea2+=QString::number(torques[j][k])+'\t';
                            }
                        }
                        flujo.escribirLineaLog(LogAVT,Linea2);
                    }


                }

                //si el tiempo del contador de osg es mayor a 100 ms tome una muestra.
                if (this->osg_timer>0.1)
                {

                    for(int j=0;j<i.value()->numModules+1;j++)
                    {
                        /*
                        cuando queramos traer los datos de realimentacion de juntura, no usamos una funcion de
                        ODE sino que accedemos directamente al apuntador que entrega el getter de junturaFeedback
                        */

                        const dReal *realP1 = dBodyGetPosition(*((motorODE->getSerpienteNumero(0))->getModuloPos(j)));
                        //Tomamos la rotación del módulo i
                        const dReal *realR1 = dBodyGetQuaternion(*((motorODE->getSerpienteNumero(0))->getModuloPos(j)));
                        osg::Vec3 pos( realP1[0], realP1[1], realP1[2]);
                        osg::Quat rot( realR1[1],realR1[2],realR1[3],realR1[0] );
                        visualizador.AgregarControlPoint(0, j, motorODE->getCurrentStep()*STEP, pos , rot);
                    }
                    this->osg_timer=0;
                }
                progress->setValue((int)((this->motorODE->getCurrentStep()*this->STEP)*100/this->sim_time));
                this->motorODE->cicloODE();
            }
            /// El cero se debe cambiar para implementaciòn con varias serpientes
            visualizador.setContactPoints(0,this->getContactPoints());
            this->visualizador.setTrayectory();
            delete (progress);
    }
        this->unpauseVid();
        return true;
    }
    else
    {
        return false;
    }
    return true;
}

int simulador::getState()
{
    return this->state;
}

bool simulador::addHeighfield(QString ruta, QString texture, QVector <double>origen, QVector <double> tamanno, double heightLimit)
{
    if (this->state==0)
    {
        if (ruta != "")
        {
            this->RutaHeighField=ruta;
            this->RutaTextura=texture;
            this->OrigenHF= origen;
            this->TamannoHF=tamanno;
            this->escalaHF=heightLimit;
        }
        else
        {
            //Valores por defecto, se borra el heightfield
            this->RutaHeighField="";
            this->RutaTextura="";
            this->OrigenHF.clear();
            this->TamannoHF.clear();
            this->escalaHF=8.0;
        }
    }
    else
    {
        return false;
    }
    return true;
}

QString simulador::getRutaHeightfield()
{
    return this->RutaHeighField;
}

QString simulador::getRutaTextura()
{
    return this->RutaTextura;
}

QVector<double> simulador::getOrigenHeightfield()
{
    QVector <double> zeroReturn;
    if (this->OrigenHF.size()==0)
    {
        zeroReturn.push_back(-10.0);
        zeroReturn.push_back(0.0);
        zeroReturn.push_back(-10.0);
        return zeroReturn;
    }
    else
    {
        return this->OrigenHF;
    }
}

QVector<double> simulador::getTamannoHeightfield()
{
    QVector <double> zeroReturn;
    if (this->TamannoHF.size()==0)
    {
        zeroReturn.push_back(20.0);
        zeroReturn.push_back(20.0);
        return zeroReturn;
    }
    else
    {
        return this->TamannoHF;
    }
}

double simulador::getEscalaHeightfield()
{
    if (this->RutaHeighField=="")
        return 8.0;
    else
        return this->escalaHF;
}

void simulador::setSnakeParameters(QString snakeRuta, QMap<QString, double> values)
{
    this->Snakes[snakeRuta]->model=values["Modelo"];
    this->Snakes[snakeRuta]->Posi_x=values["Posi_X"];
    this->Snakes[snakeRuta]->Posi_y=values["Posi_Y"];
    this->Snakes[snakeRuta]->Posi_z=values["Posi_Z"];
    this->Snakes[snakeRuta]->QUAT_X=values["Quati_X"];
    this->Snakes[snakeRuta]->QUAT_Y=values["Quati_Y"];
    this->Snakes[snakeRuta]->QUAT_Z=values["Quati_Z"];
    this->Snakes[snakeRuta]->QUAT_ANGULO=values["Quati_W"];
    this->Snakes[snakeRuta]->W=values["W"];
    this->Snakes[snakeRuta]->H=values["H"];
    this->Snakes[snakeRuta]->L=values["L"];
    this->Snakes[snakeRuta]->OFFSET_EJE=values["OFFSET_EJE"];
    this->Snakes[snakeRuta]->OFFSET_MODULO=values["OFFSET_MODULO"];
    if (values["Modelo"]==0)
    {
        this->Snakes[snakeRuta]->MASA=values["Masa"];
    }
    else if(values["Modelo"]==1)
    {
        this->Snakes[snakeRuta]->Masa_Cilindro=values["Masa_Cilindro"];
        this->Snakes[snakeRuta]->Offset_Cilindro=values["Offset_Cilindro"];
        this->Snakes[snakeRuta]->Masa_Cubo=values["Masa_Cubo"];
        this->Snakes[snakeRuta]->Offset_Cubo=values["Offset_Cubo"];
        this->Snakes[snakeRuta]->H_Cilindro=values["H_Cilindro"];
        this->Snakes[snakeRuta]->dInt_Cilindro=values["dInt_Cilindro"];
        this->Snakes[snakeRuta]->dExt_Cilindro=values["dExt_Cilindro"];
    }
}

QMap <QString, double> simulador::getSnakeParameters(QString rutaSnake)
{
    QMap <QString, double> values;
    values["Modelo"]=this->Snakes[rutaSnake]->model;
    values["Masa"]=this->Snakes[rutaSnake]->MASA;
    values["W"]=this->Snakes[rutaSnake]->W;
    values["H"]=this->Snakes[rutaSnake]->H;
    values["L"]=this->Snakes[rutaSnake]->L;
    values["OFFSET_EJE"]=this->Snakes[rutaSnake]->OFFSET_EJE;
    values["OFFSET_MODULO"]=this->Snakes[rutaSnake]->OFFSET_MODULO;
    values["Posi_X"]=this->Snakes[rutaSnake]->Posi_x;
    values["Posi_Y"]=this->Snakes[rutaSnake]->Posi_y;
    values["Posi_Z"]=this->Snakes[rutaSnake]->Posi_z;
    values["Quati_X"]=this->Snakes[rutaSnake]->QUAT_X;
    values["Quati_Y"]=this->Snakes[rutaSnake]->QUAT_Y;
    values["Quati_Z"]=this->Snakes[rutaSnake]->QUAT_Z;
    values["Quati_W"]=this->Snakes[rutaSnake]->QUAT_ANGULO;
    values["Masa_Cilindro"]=this->Snakes[rutaSnake]->Masa_Cilindro;
    values["Offset_Cilindro"]=this->Snakes[rutaSnake]->Offset_Cilindro;
    values["Masa_Cubo"]=this->Snakes[rutaSnake]->Masa_Cubo;
    values["Offset_Cubo"]=this->Snakes[rutaSnake]->Offset_Cubo;
    values["H_Cilindro"]=this->Snakes[rutaSnake]->H_Cilindro;
    values["dInt_Cilindro"]=this->Snakes[rutaSnake]->dInt_Cilindro;
    values["dExt_Cilindro"]=this->Snakes[rutaSnake]->dExt_Cilindro;
    return values;
}

int simulador::getNumModulos(QString snakeRuta)
{
    return this->Snakes[snakeRuta]->numModules+1;
}

void simulador::DeleteSnakes()
{
    this->Snakes.clear();
    this->sim_time=0;
}

void simulador::switchModelo(QString rutaSnake, int Num_Modulo, bool estado)
{
    if (this->state==1)
    {
        /// el cero debe cambiarse para cuando se implementan varias serpientes
        this->visualizador.switchModelo(0,Num_Modulo,estado);
    }
}

void simulador::switchEjesSerpiente(QString rutaSnake, int Num_Modulo, bool estado)
{
    if (this->state==1)
    {
        /// el cero debe cambiarse para cuando se implementan varias serpientes
        this->visualizador.switchEjesSerpiente(0,Num_Modulo,estado);
    }
}

void simulador::switchPuntosContacto(QString rutaSnake, bool estado)
{
    if (this->state==1)
    {
        /// el cero debe cambiarse para cuando se implementan varias serpientes
        this->visualizador.switchPuntosContacto(0,estado);
    }
}

void simulador::switchEjesGenerales(bool estado)
{
    if (this->state==1)
    {
        this->visualizador.switchEjesGenerales(estado);
    }
}

void simulador::switchFOGGeneral(bool estado)
{
    if (this->state==1)
    {
        this->visualizador.switchFOG(estado);
    }
}

void simulador::switchTrayectoria(QString rutaSnake, bool estado,int Num_Modulo)
{
    if (this->state==1)
    {
        /// el cero debe cambiarse para cuando se implementan varias serpientes
        this->visualizador.switchTrayectoria(0,Num_Modulo,estado);
    }
}

QMap<QString, bool> simulador::getGraphicsStates(QString rutaSnake,int Modulo)
{
    if (this->state==1)
    {
        /// el cero debe cambiarse para cuando se implementan varias serpientes
        return this->visualizador.getStates(0,Modulo);
    }
}

QVector<double *> simulador::getContactPoints()
{
    QVector <double *> rta = motorODE->getPuntosContacto();
    return rta;
}

void simulador::setCrearLogFile(bool estado)
{
    this->crearLog=estado;
}

QMap <QString, double> simulador::getParameters()
{
    QMap <QString, double> rta;
    rta["CFM"]=this->CFM;
    rta["ERP"]=this->ERP;
    rta["LIN_DAMPING"]=this->LINEAR_DAMPING;
    rta["ANG_DAMPING"]=this->ANGULAR_DAMPING;
    rta["MAX_ANG_SPEED"]=this->MAX_ANGULAR_SPEED;
    rta["CONTACT_MAX_CORR_VEL"]=this->CONTACT_MAX_CORRECTING_VEL;
    rta["STEP"]=this->STEP;
    rta["MU"]=this->mu_value;
    rta["BOUNCE"]=this->bounce_value;
    rta["BOUNCE_VEL"]=this->bounce_vel_value;
    return rta;
}

