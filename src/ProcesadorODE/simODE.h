#ifndef SIMODE_H
#define SIMODE_H
#include <QVector>
#include <QMap>
#include "../Serpiente/simSerpiente.h"
#include "../Escenario/simEscenario.h"
#include "constantesODE.h"
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <ode/ode.h>
#include <iostream>


using namespace std;

class simODE
{
private:
    //Parametros de simulación

    double CFM;
    double ERP;

    double LINEAR_DAMPING;
    double ANGULAR_DAMPING;

    double MAX_ANGULAR_SPEED;
    double CONTACT_MAX_CORRECTING_VEL;


    double STEP;

    //Surface
    double mu_value;
    double bounce_value;
    double bounce_vel_value;
    int CurrentStep;
    int ContadorDynamixar;

  dWorldID world;
  dSpaceID space;
  dJointGroupID cgroup;
  QVector <simSerpiente *> serpientes;
  //Puntos de contacto

  QVector <double *> puntosContacto;
  
  //Escenario
    simEscenario * escenario;

public:
  //Constructor
  simODE();
  
  //Setters
  void setSurfaceParameters(double mu, double bounce, double bounce_vel);
  void setParameters(double CFM, double ERP, double LINEAR_DAMPING, double ANGULAR_DAMPING, double MAX_ANGULAR_SPEED, double CONTACT_MAX_CORRECTING_VEL, double STEP);
  //void setSpace(dSpaceID * nSpace);
  void setParametrosMotor(QMap <QString,double> values, int numSerpiente);
  
  //Getters
  dWorldID & getWorld();
  dSpaceID & getSpace();
  int getCurrentStep();
  QVector <double > getCurrentAngulos(int numSerpiente);
  QVector <double > getCurrentVelocidades(int numSerpiente);
  QVector<double *> getCurrentTorques(int numSerpiente);
  QMap <QString,double> getParametrosMotor(int numSerpiente);
  
  //funcion de ode
  void cicloODE();
  void nearCallback(void *data, dGeomID o1, dGeomID o2);
  
  //funciones de la serpientes
  void crearSerpiente(int tam, QMap<QString, double> values);
  simSerpiente * getSerpienteNumero(int i);
  void configurarSerpiente(int numeroSerpiente,QVector<double> angulos);
  void setParametrosSerpiente(int numeroSerpiente,QMap <QString, double> values );
	
  //funcione del escenario
  void crearHeightfield(double * data,
			double tamanno_x, double tamanno_z,
			int c, int r,
			double escala, double offset, double thick,
			double min_height, double max_height);
  void crearPlano();
  
  //Destructor
  ~simODE();
  
  //para el callback
  static void Wrapper_To_Call_NearCallBack(void *data, dGeomID o1, dGeomID o2);
  
  //Tomar puntos de contacto
  QVector <double *> getPuntosContacto();

};


#endif
