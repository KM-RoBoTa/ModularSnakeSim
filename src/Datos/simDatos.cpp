#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "simDatos.h"

simDatos::simDatos()
{

}

int simDatos::contarLineas(QString Ruta)
{
    int count=0;
    QFile file(Ruta);
    if (file.open(QFile::ReadOnly))
    {
        //Tomamos la primera linea para verificar el encabezado
        QTextStream in(&file);
        while(!in.atEnd())
        {
            in.readLine();
            count++;
        }
        //Le quitamos la línea del encabezado.
        return count-1;
    }
    //Error de lectura del archivo
    return -1;

}

double * simDatos::leerHeighfield(QString ruta,QString texture, int &c, int &r, float &max_height, float &min_height, osg::ref_ptr<osg::Geode> heightfieldGeode,osg::Vec3 size, double heightLimit)
{
    osg::Image* heightMap = osgDB::readImageFile(ruta.toStdString());
    osg::ref_ptr <osg::HeightField> heightField = new osg::HeightField;
    c=heightMap->s();
    r=heightMap->t();
    max_height=*heightMap->data(0, 0);
    min_height=*heightMap->data(0, 0);
    heightField->allocate(c, r);
    heightField->setOrigin(osg::Vec3(0,0,0));
    heightField->setXInterval(size[0]/c);
    heightField->setYInterval(size[1]/r);
    heightField->setSkirtHeight(0.0f);

    double * rta=new double[c*r];

    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            rta[i*r+j]=*heightMap->data(i, j);
            if(*heightMap->data(i, j)>max_height)
                max_height=*heightMap->data(i, j);
            if(*heightMap->data(i, j)<min_height)
                min_height=*heightMap->data(i, j);
            heightField->setHeight(i ,j,-((*heightMap->data(i, j)) / 255.0f)*heightLimit);
        }
    }
    osg::ref_ptr <osg::ShapeDrawable > test = new osg::ShapeDrawable(heightField);

    heightfieldGeode->addDrawable(test);
    osg::Texture2D* tex = new osg::Texture2D(osgDB::readImageFile(texture.toStdString()));
    tex->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR_MIPMAP_LINEAR);
    tex->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
    tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    heightfieldGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex);
    return rta;
}

int simDatos::tomarTamannoSerpiente(QString Ruta)
{
    QFile file(Ruta);
    if (file.open(QFile::ReadOnly))
    {
        //Tomamos la primera linea para verificar el encabezado
        QTextStream in(&file);
        QString text = in.readLine();
        QStringList result = text.split('\t');
        file.close();
        return result[0].toInt();
    }
    //Error de lectura del archivo
    return -1;
}


double simDatos::tomarDeltaT(QString Ruta)
{
    QFile file(Ruta);
    if (file.open(QFile::ReadOnly))
    {
        //Tomamos la primera linea para verificar el encabezado
        QTextStream in(&file);
        QString text = in.readLine();
        QStringList result = text.split('\t');
        file.close();
        return result[1].toDouble();
    }
    return -1;
}


QVector<double> simDatos::tomarLinea(QString Ruta, int linea)
{
    QVector <double> result;
    QFile file(Ruta);

    if (file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        QString header = in.readLine();
        QStringList headerValues= header.split('\t');
        for (int i = 1; i < linea ; i++ )
        {
            in.readLine();
        }
        QStringList list= in.readLine().split('\t');
        for (int i = 0 ; i < list.size() ; i ++)
        {
            if (headerValues[2]=="g")
                result.push_back(list[i].toDouble()*PI/180);
            else if (headerValues[2]=="r")
                result.push_back(list[i].toDouble());
            else
                result.push_back(0.0);
        }
    }

    return result;
}

//write LogFile
void simDatos::escribirLineaLog(QString ruta, QString linea)
{
    QFile file(ruta);
    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
    {
        QTextStream escritor(&file);
        escritor<<linea<<endl;
        file.flush();
        file.close();
    }
}


simDatos::~simDatos()
{

}


