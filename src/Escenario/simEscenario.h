#ifndef SIMESCENARIO_H
#define SIMESCENARIO_H
#include <ode/ode.h>
using namespace std;

class simEscenario {

private:

dGeomID pisoGeom;


public:
//Constructores
	simEscenario();
    simEscenario(dSpaceID space,
			double * data,
			double tamanno_x, double tamanno_z,
			int c, int r,
			double escala, double offset, double thick,
			double min_height, double max_height);
  
//Destructor
  ~simEscenario();
    dGeomID getpisoGeom();
   void crearPlano(dSpaceID space);
};


#endif //SIMESCENARIO
