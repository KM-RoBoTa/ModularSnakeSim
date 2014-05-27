#ifndef SIMSERPIENTE_H
#define SIMSERPIENTE_H
#include <QVector>
#include <ode/ode.h>
#include <QMap>
#include <QString>

using namespace std;

class simSerpiente {

private:
    //Parametros serpiente
    //Estan los de ambos modelos, deberían organizarse diferente.
    double H;
    double W;
    double L;
    double MASA;
    double QUAT_X;
    double QUAT_Y;
    double QUAT_Z;
    double QUAT_ANGULO;
    double Posi_x;
    double Posi_y;
    double Posi_z;
    double OFFSET_MODULO;
    double OFFSET_EJE;
    int modelo;
    double Masa_Cilindro;
    double Offset_Cilindro;
    double Masa_Cubo;
    double Offset_Cubo;
    double H_Cilindro;
    double dInt_Cilindro;
    double dExt_Cilindro;

    //Parametros del motor
    double k;
    double Fmax;
    double resolucion;
    double Fviscosa;
    double Velocity;

    QVector <dBodyID*> modulos;
    QVector <dJointID*> motores;
    QVector <dJointID*> junturas;
    QVector <dJointFeedback*> realimentacionJuntura;
    QVector <double>	configuracion;
	int tamano;

public:
	void dynamixar();
//Constructores
	simSerpiente();
    simSerpiente(dWorldID *world, dSpaceID *space,int tam,QMap <QString,double> values);
  
//Getters
	dBodyID * getModuloPos(int i);
	dJointFeedback * getRealimentacionJunturaPos(int i);
	dJointID * getMotorPos(int i);
	dJointID * getJunturaPos(int i);
	int getTamano();
    QMap <QString, double> getParametrosMotor();

//Setters
    void setConfiguracion(QVector <double> nuevaConfig);
    void setParameters(QMap <QString,double>);
    void setParametrosMotor (QMap<QString, double> nuevosValores);

//Destructor
  ~simSerpiente();


};

#endif
