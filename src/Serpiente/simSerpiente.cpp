#include "Constantes.h"
#include <iostream>
#include "simSerpiente.h"

////////////////////////////////////////////METODOS///////////////////////////////////////////
//Constructor por defecto
simSerpiente::simSerpiente()
{
	this->tamano=0;

}

//Constructor que inicializa los módulos y las junturas
simSerpiente::simSerpiente(dWorldID *world, dSpaceID *space, int tam, QMap<QString, double> values)
{
    //Tamaño de la serpiente
	this->tamano=tam;
    this->modelo=values["Modelo"];
    //Parametros del modelo 1
    this->H=values["H"];
    this->W=values["W"];
    this->L=values["L"];
    this->QUAT_X=values["Quati_X"];
    this->QUAT_Y=values["Quati_Y"];
    this->QUAT_Z=values["Quati_Z"];
    this->QUAT_ANGULO=values["Quati_W"];
    this->Posi_x=values["Posi_X"];
    this->Posi_y=values["Posi_Y"];
    this->Posi_z=values["Posi_Z"];
    this->OFFSET_MODULO=values["OFFSET_MODULO"];
    this->OFFSET_EJE = values["OFFSET_EJE"];
    if (values["Modelo"]==0)
    {
        this->MASA=values["Masa"];
    }
    else
    {
        this->Masa_Cilindro=values["Masa_Cilindro"];
        this->Offset_Cilindro=values["Offset_Cilindro"];
        this->Masa_Cubo=values["Masa_Cubo"];
        this->Offset_Cubo=values["Offset_Cubo"];
        this->H_Cilindro=values["H_Cilindro"];
        this->dInt_Cilindro=values["dInt_Cilindro"];
        this->dExt_Cilindro=values["dExt_Cilindro"];
    }

    //Llenado de los vectores
    dBodyID* anterior;
    dBodyID* nuevoBody;
    dJointID* nuevoMotor;
    dJointID* nuevaJuntura;
    dJointFeedback* nuevoFeedback;
    for (int i=0;i<tamano;i++)
    {
        //Creación del body
        nuevoBody=new dBodyID();
        *nuevoBody = dBodyCreate(*world);
        this->modulos.push_back(nuevoBody);

        //Variables necesarias dentro del switch
        //Case0
        dGeomID moduloGeom;
        dMass moduloMass;
        dQuaternion moduloQuat;
        //Case1
        dGeomID geomCubo;
        dGeomID geomCilindro;
        dQuaternion cilindroQuat;
        dMass masa;
        switch(this->modelo)
        {
            case 0:
                //Creación del Geom
                moduloGeom = dCreateBox(*space,H,W,L);
                dGeomSetBody(moduloGeom, *nuevoBody);
                //Creación del Mass
                dMassSetZero(&moduloMass);
                dMassSetBoxTotal(&moduloMass, MASA, H, W, L);
                dBodySetMass(*nuevoBody, &moduloMass);
            break;
            case 1:
                //Creación del Geom
                geomCubo 	= dCreateBox(*space, this->H,this->W,this->L);
                geomCilindro 	= dCreateCylinder(*space, this->dExt_Cilindro/2, this->H_Cilindro);

                dGeomSetBody(geomCubo, *nuevoBody);
                dGeomSetBody(geomCilindro, *nuevoBody);

                dQFromAxisAndAngle(cilindroQuat, 0, 1, 0, QUAT_ANGULO);
                dGeomSetOffsetQuaternion(geomCilindro, cilindroQuat);

                dGeomSetOffsetPosition(geomCubo, -this->Offset_Cubo, 0.0, 0.0);
                dGeomSetOffsetPosition(geomCilindro, this->Offset_Cilindro, 0.0, 0.0);

                //Creación del Mass
                double T11Cubo=(((this->H*this->H+this->L*this->L)*this->Masa_Cubo)/12+this->Masa_Cubo*this->Offset_Cubo*this->Offset_Cubo);
                double T11Cilindro=(((this->dInt_Cilindro*this->dInt_Cilindro+this->dExt_Cilindro*this->dExt_Cilindro)*this->Masa_Cilindro)/12+this->Masa_Cilindro*this->Offset_Cilindro*this->Offset_Cilindro);
                double T22Cubo=(((this->W*this->W+this->L*this->L)*this->Masa_Cubo)/12);
                double T22Cilindro=((((this->dInt_Cilindro*this->dInt_Cilindro+this->dExt_Cilindro*this->dExt_Cilindro)*3 + this->H_Cilindro*this->H_Cilindro)*this->Masa_Cilindro)/12);
                double T33Cubo=(((this->W*this->W+this->H*this->H)*this->Masa_Cubo)/12);
                double T33Cilindro=((((this->dInt_Cilindro*this->dInt_Cilindro+this->dExt_Cilindro*this->dExt_Cilindro)*3 + this->H_Cilindro*this->H_Cilindro)*this->Masa_Cilindro)/12);

                dMassSetParameters (&masa, this->Masa_Cilindro+this->Masa_Cubo,
                                 0.0, 0.0, 0.0,
                                 T11Cubo+T11Cilindro, T22Cubo+T22Cilindro, T33Cubo + T33Cilindro,
                                 0.0, 0.0, 0.0);
                dBodySetMass(*nuevoBody, &masa);
            break;
        }

        //Creación del quat
        dQFromAxisAndAngle(moduloQuat, QUAT_X, QUAT_Y, QUAT_Z, QUAT_ANGULO*((i)%2));
        dBodySetPosition(*nuevoBody,Posi_x+i*(OFFSET_MODULO+H),Posi_y,Posi_z);
        dBodySetQuaternion(*nuevoBody, moduloQuat);

        if(i>0)
        {
            //Juntura
            nuevaJuntura = new dJointID();
            *nuevaJuntura = dJointCreateHinge(*world, 0);
            this->junturas.push_back(nuevaJuntura);

            dJointAttach(*nuevaJuntura, *anterior, *nuevoBody);
            dJointSetHingeAnchor(*nuevaJuntura, Posi_x+i*(OFFSET_MODULO+H)-OFFSET_EJE,Posi_y,Posi_z);
            dJointSetHingeAxis(*nuevaJuntura, 0.0, ((i-1)%2)*1.0, (i%2)*1.0);
            dJointSetHingeParam (*nuevaJuntura, dParamLoStop, -3.1415/2);
            dJointSetHingeParam (*nuevaJuntura, dParamHiStop, 3.1415/2);
            //Motor
            nuevoMotor = new dJointID();
            *nuevoMotor = dJointCreateAMotor(*world, 0);
            this->motores.push_back(nuevoMotor);

            dJointAttach(*nuevoMotor, *anterior,*nuevoBody);
            dJointSetAMotorMode (*nuevoMotor, dAMotorUser);
            dJointSetAMotorNumAxes (*nuevoMotor, 1);
            dJointSetAMotorAxis (*nuevoMotor, 0.0, 1, 0.0, ((i-1)%2)*1.0, (i%2)*1.0);	//eje del motor
            dJointSetAMotorAngle (*nuevoMotor, 0.0, 0.0);					//inicialización del angulo
            dJointSetAMotorParam(*nuevoMotor, dParamFMax, 100);				//Torque maximo
            dJointSetAMotorParam (*nuevoMotor, dParamLoStop, -3.1415/2);			//Stops
            dJointSetAMotorParam (*nuevoMotor, dParamHiStop, 3.1415/2);

            //Estructura Feedback
            nuevoFeedback = new dJointFeedback();					//esta estructura se llena
            this->realimentacionJuntura.push_back(nuevoFeedback);			//automaticamente, hay que buscar
            dJointSetFeedback (*nuevaJuntura, nuevoFeedback);			//el apuntador, no hay getter
        }
        anterior=nuevoBody;
        //Configuración inicial
        this->configuracion.push_back(0);
    }

    //Parametros del motor
    k=1.95;
    Fmax= 1.5;
    resolucion=(0.29*3.1415/180);
    Fviscosa=0.2;
    Velocity=-6.178465545;
}

void simSerpiente::dynamixar()
{
    for (int i=0;i<tamano-1;i++)
    {
        float diferencia = 0.0;
        diferencia = dJointGetHingeAngle(*(this->junturas[i]))-(this->configuracion[i]);
        if(fabs(diferencia) > resolucion )				//0.29 grados por defecto
        {
            if (Fmax*k*fabs(diferencia)>Fmax)
            {
                dJointSetAMotorParam(*(this->motores[i]), dParamFMax, Fmax);
            }
            else
            {
                dJointSetAMotorParam(*(this->motores[i]), dParamFMax, Fmax*k*fabs(diferencia));
            }
            dJointSetAMotorParam(*(this->motores[i]), dParamVel, Velocity*diferencia/fabs(diferencia));	//improvisa, constante (59 rpm); dParamFMax si proporcional, improvisa tambien (1.5 Nm).
            //dJointSetAMotorParam(*(this->motores[i]), dParamFMax, 1.5*diferencia);
        } else {
            dJointSetAMotorParam(*(this->motores[i]), dParamVel, 0.0);
            dJointSetAMotorParam(*(this->motores[i]), dParamFMax, Fviscosa);
        }
    }
}

////////////////////////////////////////////GETTERS///////////////////////////////////////////
//Retorna el módulo de la posición i de la lista
dBodyID * simSerpiente::getModuloPos(int i)
{
	return this->modulos[i];
}
//Retorna el elemento de realimentación de una juntura que está en la posición i
dJointFeedback * simSerpiente::getRealimentacionJunturaPos(int i)
{
	return this->realimentacionJuntura[i];
}
//Retorna el motor de la posición i
dJointID * simSerpiente::getMotorPos(int i)
{
	return this->motores[i];
}
dJointID * simSerpiente::getJunturaPos(int i)
{
	return this->junturas[i];
}
int simSerpiente::getTamano()
{
    return this->tamano;
}

QMap <QString,double> simSerpiente::getParametrosMotor()
{
    QMap <QString, double> Valor;
    Valor["k"]=k;
    Valor["FMax"]=Fmax;
    Valor["Resolucion"]=resolucion;
    Valor["Fviscosa"]=Fviscosa;
    Valor["Velocity"]=Velocity;
    return Valor;
}
////////////////////////////////////////////SETTERS///////////////////////////////////////////
void simSerpiente::setConfiguracion(QVector <double> nuevaConfig)
{
    //Si no hay lineas suficientes para los actuadores se completan con ceros
    if (nuevaConfig.size()<this->tamano)
        for(int i=0; i < this->tamano; i++)
            nuevaConfig.push_back(0.0);
    this->configuracion=nuevaConfig;
}

void simSerpiente::setParameters(QMap <QString,double> values)
{
    this->modelo=values["Modelos"];
    this->Posi_x=values["Posi_X"];
    this->Posi_y=values["Posi_Y"];
    this->Posi_z=values["Posi_Z"];
    this->QUAT_X=values["Quati_X"];
    this->QUAT_Y=values["Quati_Y"];
    this->QUAT_Z=values["Quati_Z"];
    this->QUAT_ANGULO=values["Quati_W"];
    this->W=values["W"];
    this->H=values["H"];
    this->L=values["L"];

    if (values["Modelos"]==0)
    {
        this->MASA=values["Masa"];
        this->OFFSET_EJE=values["OFFSET_EJE"];
        this->OFFSET_MODULO=values["OFFSET_MODULO"];
    }
    else if (values["Modelos"]==1)
    {
        this->Masa_Cilindro=values["Masa_Cilindro"];
        this->Offset_Cilindro=values["Offset_Cilindro"];
        this->Masa_Cubo=values["Masa_Cubo"];
        this->Offset_Cubo=values["Offset_Cubo"];
        this->H_Cilindro=values["H_Cilindro"];
        this->dInt_Cilindro=values["dInt_Cilindro"];
        this->dExt_Cilindro=values["dExt_Cilindro"];
    }
}

void simSerpiente::setParametrosMotor(QMap <QString, double> nuevosValores)
{
    k=nuevosValores["k"];
    Fmax=nuevosValores["FMax"];
    resolucion=nuevosValores["Resolucion"];
    Fviscosa=nuevosValores["FViscosa"];
    Velocity=nuevosValores["Velocity"];
}
////////////////////////////////////////////DESTRUCTOR///////////////////////////////////////////
simSerpiente::~simSerpiente()
{
    this->configuracion.clear();
    this->junturas.clear();
    this->modulos.clear();
    this->motores.clear();
    this->realimentacionJuntura.clear();
}
