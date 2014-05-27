#ifndef SIMDATOS_H
#define SIMDATOS_H
#include <fstream>
#include <QVector>
#include <sstream>
#include <QString>
#include <QVector>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#define PI 3.14159265


class simDatos
{
public:
    simDatos();
    QVector <double> tomarLinea(QString Ruta, int linea);
    int tomarTamannoSerpiente(QString Ruta);
    double tomarDeltaT(QString Ruta);
    int contarLineas(QString Ruta);
    double * leerHeighfield(QString ruta,QString texture, int &c, int &r, float &max_height,float &min_height,osg::ref_ptr<osg::Geode> heightfieldGeode,osg::Vec3 size, double heightLimit);
    void escribirLineaLog(QString ruta, QString linea);

    ~simDatos();
};
#endif //SIMDATOS_H
