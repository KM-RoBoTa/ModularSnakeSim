#include "Constantes.h"
#include <iostream>
#include "simEscenario.h"
////////////////////////////////////////////METODOS///////////////////////////////////////////
//Constructor por defecto
simEscenario::simEscenario()
{

}

//Constructor que inicializa los módulos y las junturas
simEscenario::simEscenario(dSpaceID space,
            double * data,
            double tamanno_x, double tamanno_z,
            int c, int r,
            double escala, double offset, double thick,
            double min_height, double max_height)
{
	//creo un "campo" para el heightfield
	dHeightfieldDataID pisoHF;
	pisoHF = dGeomHeightfieldDataCreate();

	//formateo los datos del heightfield
	dGeomHeightfieldDataBuildDouble(pisoHF,
                                      data,
                                      0,
                                      tamanno_x, tamanno_z,
                                      c, r,
                                      escala, offset, thick, 0);
	dGeomHeightfieldDataSetBounds(pisoHF, min_height, max_height);

	//creo el geom

    pisoGeom = dCreateHeightfield(space, pisoHF, 0);
}

////////////////////////////////////////////DESTRUCTOR///////////////////////////////////////////
simEscenario::~simEscenario()
{

}

dGeomID simEscenario::getpisoGeom()
{
    return this->pisoGeom;
}

void simEscenario::crearPlano(dSpaceID space)
{
    this->pisoGeom = dCreatePlane(space, 0.0, 1.0, 0.0, 0.0);

}
