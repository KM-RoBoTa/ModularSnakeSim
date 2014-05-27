
#include "simODE.h"

////////////////////////////////////////////Auxiliares///////////////////////////////////////////
void* pt2simODE;  	

////////////////////////////////////////////Constructores///////////////////////////////////////////
simODE::simODE()
{
    this->CFM=0.0004;
    this->ERP=0.9;
    this->LINEAR_DAMPING=0.0001;
    this->ANGULAR_DAMPING=0.005;
    this->MAX_ANGULAR_SPEED=200;
    this->CONTACT_MAX_CORRECTING_VEL=0.1;
    this->STEP=0.0001;
    this->CurrentStep=0;
    this->ContadorDynamixar=0;
    this->mu_value=0.8;
    this->bounce_value=0.000010;
    this->bounce_vel_value=1.0;
  	dInitODE();

    //crear el mundo
    this->world = dWorldCreate();
    this->space = dSimpleSpaceCreate(0);

    dWorldSetGravity(this->world, GRAVEDAD_X, GRAVEDAD_Y, GRAVEDAD_Z);

    dWorldSetERP(this->world, ERP);
    dWorldSetCFM(this->world, CFM);

    dWorldSetLinearDamping(this->world, LINEAR_DAMPING);
    dWorldSetAngularDamping(this->world, ANGULAR_DAMPING);

    dWorldSetMaxAngularSpeed(this->world, MAX_ANGULAR_SPEED);
    dWorldSetContactMaxCorrectingVel(this->world,CONTACT_MAX_CORRECTING_VEL);



    this->cgroup = dJointGroupCreate(0);						//joint group para los contactos Cgroup
}

void simODE::setSurfaceParameters(double mu, double bounce, double bounce_vel)
{
    this->mu_value=mu;
    this->bounce_value=bounce;
    this->bounce_vel_value=bounce_vel;
}

////////////////////////////////////////////funciones///////////////////////////////////////////
  
//ciclo ODE
void simODE::cicloODE()
{
    this->CurrentStep++;
  	pt2simODE = (void*) this;
    this->ContadorDynamixar++;
//    if(ContadorDynamixar*STEP>0.01)
//    {
        this->serpientes[0]->dynamixar();
        this->ContadorDynamixar=0;
//    }

  	dSpaceCollide(space, 0, &simODE::Wrapper_To_Call_NearCallBack);	//busca colisiones en space y si las encuentra despierta al nearCallBack
    dWorldQuickStep(world, STEP);					//con los joints de contacto ya puestos toma una paso
  	dJointGroupEmpty(cgroup);					//destruye los joints de contacto
	
}

void simODE::Wrapper_To_Call_NearCallBack(void *data, dGeomID o1, dGeomID o2)
{
    using namespace std;
       // explicitly cast global variable <pt2Object> to a pointer to TClassB
       // warning: <pt2Object> MUST point to an appropriate object!
       simODE* mySelf = (simODE*) pt2simODE;

       // call member
       mySelf->nearCallback(data, o1, o2);
}

QVector<double *> simODE::getPuntosContacto()
{
    return this->puntosContacto;
}

//nearCallBack
void simODE::nearCallback(void *data, dGeomID o1, dGeomID o2) 
{
    dGeomID piso= this->escenario->getpisoGeom();
  	dBodyID b1 = dGeomGetBody(o1);
  	dBodyID b2 = dGeomGetBody(o2);
    if (o1==piso||o2==piso)
    {
        double *contacto;
        dContact contact;
        contact.surface.mode = dContactBounce | dContactApprox1;
        contact.surface.mu = this->mu_value;					// friction parameter
        contact.surface.bounce = this->bounce_value;					// bounce is the amount of "bouncyness".
        contact.surface.bounce_vel = this->bounce_vel_value;				// bounce_vel is the minimum incoming velocity to cause a bounce
        contact.surface.soft_cfm = 0.00001;  				// constraint force mixing parameter
        if (dCollide (o1,o2,1,&contact.geom,sizeof(dContact)))
        {
                dJointID c = dJointCreateContact (this->world,this->cgroup,&contact);
                dJointAttach (c,b1,b2);
                contacto=new double[3];
                contacto[0]=*contact.geom.pos;
                contacto[1]=*(contact.geom.pos+1);
                contacto[2]=*(contact.geom.pos+2);
                this->puntosContacto.push_back(contacto);
        }
    }
}

////////////////////////////////////////////setters///////////////////////////////////////////
//void simODE::setWorld(dWorldID * nWorld)
//{
//  this->world=*nWorld;
//}
//void simODE::setSpace(dSpaceID * nSpace)
//{
//  this->space=*nSpace;
//}
  
////////////////////////////////////////////getters///////////////////////////////////////////
dWorldID & simODE::getWorld()
{
  return (this->world);
}
dSpaceID & simODE::getSpace()
{
  return (this->space);
}
int simODE::getCurrentStep()
{
    return this->CurrentStep;
}

QVector<double> simODE::getCurrentAngulos(int numSerpiente)
{
    dJointID * aux;
    QVector <double>rta;
    for (int i = 0 ; i < this->serpientes[numSerpiente]->getTamano()-1 ; i++)
    {
        aux= this->serpientes[numSerpiente]->getJunturaPos(i);
        rta.push_back((double)dJointGetHingeAngle(*aux));
    }
    return rta;
}

QVector<double> simODE::getCurrentVelocidades(int numSerpiente)
{
    dJointID * aux;
    QVector <double>rta;
    for (int i = 0 ; i < this->serpientes[numSerpiente]->getTamano()-1 ; i++)
    {
        aux= this->serpientes[numSerpiente]->getJunturaPos(i);
        rta.push_back((double)dJointGetHingeAngleRate(*aux));
    }
    return rta;
}

QVector<double*> simODE::getCurrentTorques(int numSerpiente)
{
    dJointFeedback * aux;
    QVector <double*>rta;
    double * values;
    for (int i = 0 ; i < this->serpientes[numSerpiente]->getTamano()-1 ; i++)
    {
        values= new double[12];
        aux= this->serpientes[numSerpiente]->getRealimentacionJunturaPos(i);
        values[0]=(double)aux->f1[0];
        values[1]=(double)aux->f1[1];
        values[2]=(double)aux->f1[2];
        values[3]=(double)aux->f2[0];
        values[4]=(double)aux->f2[1];
        values[5]=(double)aux->f2[2];
        values[6]=(double)aux->t1[0];
        values[7]=(double)aux->t1[1];
        values[8]=(double)aux->t1[2];
        values[9]=(double)aux->t2[0];
        values[10]=(double)aux->t2[1];
        values[11]=(double)aux->t2[2];
        rta.push_back(values);
    }
    return rta;
}

QMap<QString, double> simODE::getParametrosMotor(int numSerpiente)
{
    return this->serpientes[numSerpiente]->getParametrosMotor();
}

/////////////////////////////////////////////Funciones de la serpiente //////////////////////////

//Crea una nueva serpiente
void simODE::crearSerpiente(int tam,QMap <QString,double> values)
{
  simSerpiente * nueva;
  nueva=new simSerpiente(&(this->world),&(this->space),tam,values);
  this->serpientes.push_back(nueva);
}

//Devuelve la serpiente en la posición i, está creado para simular varias serpientes al mismo tiempo
simSerpiente * simODE::getSerpienteNumero(int i)
{
  return this->serpientes[i];
}

//Permite la configuración de la serpiente numeroSerpiente con los ángulos en el arreglo de ángulos que debe ser del tamaño de la serpiente
void simODE::configurarSerpiente(int numeroSerpiente,QVector <double> angulos)
{
    this->serpientes[numeroSerpiente]->setConfiguracion(angulos);
}

void simODE::setParametrosSerpiente(int numeroSerpiente, QMap<QString, double> values)
{
    this->serpientes[numeroSerpiente]->setParameters(values);
}

/////////////////////////////////////////////Funciones del escenario //////////////////////////
void simODE::crearHeightfield(double * data,
			double tamanno_x, double tamanno_z,
			int c, int r,
			double escala, double offset, double thick,
			double min_height, double max_height)
{

  this->escenario = new simEscenario((this->space),
			data,
			tamanno_x, tamanno_z,
			c, r,
            escala, offset, thick,
                           min_height, max_height);
}

void simODE::crearPlano()
{
    this->escenario=new simEscenario;
    this->escenario->crearPlano(this->space);
}



////////////////////////////////////////////destructor///////////////////////////////////////////
simODE::~simODE()
{
    this->serpientes.clear();
    dSpaceClean(space);
    dSpaceDestroy(space);
  dWorldDestroy(world);
  dCloseODE();
}


void simODE::setParameters(double CFM, double ERP, double LINEAR_DAMPING, double ANGULAR_DAMPING, double MAX_ANGULAR_SPEED, double CONTACT_MAX_CORRECTING_VEL, double STEP)
{
    this->CFM=CFM;
    this->ERP=ERP;
    this->LINEAR_DAMPING=LINEAR_DAMPING;
    this->ANGULAR_DAMPING=ANGULAR_DAMPING;
    this->MAX_ANGULAR_SPEED=MAX_ANGULAR_SPEED;
    this->CONTACT_MAX_CORRECTING_VEL=CONTACT_MAX_CORRECTING_VEL;
    this->STEP=STEP;

    dWorldSetERP(this->world, ERP);
    dWorldSetCFM(this->world, CFM);

    dWorldSetLinearDamping(this->world, LINEAR_DAMPING);
    dWorldSetAngularDamping(this->world, ANGULAR_DAMPING);

    dWorldSetMaxAngularSpeed(this->world, MAX_ANGULAR_SPEED);
    dWorldSetContactMaxCorrectingVel(this->world,CONTACT_MAX_CORRECTING_VEL);
}

void simODE::setParametrosMotor(QMap<QString, double> values, int numSerpiente)
{
    this->serpientes[numSerpiente]->setParametrosMotor(values);
}


