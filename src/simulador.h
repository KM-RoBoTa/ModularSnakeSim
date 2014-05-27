#ifndef SIMULADOR_H
#define SIMULADOR_H
#include <QVector>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <cstdarg>
#include <QMap>
#include <QProgressDialog>

#include <ode/ode.h>

#include "Serpiente/simSerpiente.h"
#include "Escenario/simEscenario.h"
#include "ProcesadorODE/simODE.h"
#include "ProcesadorGL/simOSG.h"
#include "Datos/simDatos.h"


class simulador
{
private:

    struct snake
    {
        QString AnglesFile;
        int numModules;
        double dt;
        int model;
        int fileSize;
        int currentCounterPosition;
        //Valores Modelo
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
        double Masa_Cilindro;
        double Offset_Cilindro;
        double Masa_Cubo;
        double Offset_Cubo;
        double H_Cilindro;
        double dInt_Cilindro;
        double dExt_Cilindro;
        //Parametros de motor
        //Parametros del motor
        double k;
        double Fmax;
        double resolucion;
        double Fviscosa;
        double Velocity;

    };

    QString name;


    //Running=1 or stoped=0
    bool state;

    //Valores de ODE
    double CFM;
    double ERP;
    double LINEAR_DAMPING;
    double ANGULAR_DAMPING;
    double MAX_ANGULAR_SPEED;
    double CONTACT_MAX_CORRECTING_VEL;
    double STEP;
    //de superficie
    double mu_value;
    double bounce_value;
    double bounce_vel_value;


//    Snakes
    QMap <QString, snake*> Snakes;
    //Variable para el porcentaje
    double sim_time;
    double osg_timer;

    //Heighfield
    QString RutaHeighField;
    QString RutaTextura;
    QVector <double> OrigenHF;
    QVector <double> TamannoHF;
    double escalaHF;

   //Stream
    simDatos flujo;

   //Motor ODE
    simODE *motorODE;

    //Motor de visualización
    simOSG visualizador;

    //LogFile
    double freqMuestreo;
    bool crearLog;
    QString LogPR;
    QString LogAVT;

public:
    simulador();
    ~simulador();
    bool addSnake(QString Ruta, int modelo);
    void changeODE(double CFM, double ERP, double LINEAR_DAMPING, double ANGULAR_DAMPING, double MAX_ANGULAR_SPEED, double CONTACT_MAX_CORRECTING_VEL, double STEP);
    void changeSurfaceODE(double mu, double bounce, double bounce_vel);
    QMap <QString, double> getParameters();
    int getNumSnakes();

    osg::ref_ptr<osgViewer::Viewer> getViewer();

    //Para modificar los valores de motor
    void setParametrosMotor(QString ruta, QMap<QString, double> value);
    QMap <QString,double> getParametrosMotor(QString ruta);

    //Videocontrol and simulation control
    void pauseVid();
    void unpauseVid();
    void setVidMultiplier(double multiplier);
    bool refresh();
    bool simulate();
    void stopSimulation();

    //State
    int getState();

    //Heighfield
    bool addHeighfield(QString ruta,QString texture, QVector <double> origen,QVector <double> tamanno, double heightLimit);
    QString getRutaHeightfield();
    QString getRutaTextura();
    QVector <double> getOrigenHeightfield();
    QVector <double> getTamannoHeightfield();
    double getEscalaHeightfield();

    //Snake control
    void setSnakeParameters(QString snakeRuta,QMap <QString,double> values);
    QMap<QString, double> getSnakeParameters(QString rutaSnake);
    int getNumModulos(QString snakeRuta);
    void DeleteSnakes();

    //Graphics control
    void switchModelo(QString rutaSnake, int Num_Modulo, bool estado);
    void switchEjesSerpiente(QString rutaSnake,int Num_Modulo, bool estado);
    void switchPuntosContacto(QString rutaSnake, bool estado);
    void switchEjesGenerales(bool estado);
    void switchFOGGeneral(bool estado);
    void switchTrayectoria(QString rutaSnake, bool estado, int Num_Modulo);
    QMap <QString,bool> getGraphicsStates(QString rutaSnake, int Modulo);

    //Contact Points
    QVector <double *> getContactPoints();

    //Log File
    void setCrearLogFile(bool estado);

};

#endif // SIMULADOR_H
