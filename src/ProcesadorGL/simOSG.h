#ifndef SIMOSG_H
#define SIMOSG_H
#include "constantesOSG.h"
#include <iostream>
#include <math.h>
#include <string>
#include <QVector>
#include <QResource>

#include <osg/ShapeDrawable>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/LineWidth>
#include <osg/LightSource>
#include <osg/Fog>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/ViewerEventHandlers>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osg/ShadeModel>
#include <osg/LineWidth>
#include <osg/Point>




class simOSG
{
  //This struct is a container for a snake.
    struct serpiente
    {
      osg::ref_ptr<osg::Group> contenedor;
      int numModulos;
      QVector < osg::ref_ptr < osg::AnimationPath > > ModulosAnimator;
      QVector < osg::ref_ptr < osg::AnimationPathCallback > > ModulosAnimatorCallback;
      QVector < osg::ref_ptr < osg::Switch> > switchModelos;
      QVector < osg::ref_ptr < osg::Switch> > switchEjes;
      QVector < osg::ref_ptr < osg::Switch> > switchTrayectoria;
      QVector < osg::ref_ptr < osg::Vec3Array> > Trayectorias;
      osg::ref_ptr <osg::Switch>  switchContactPoints;
      double H;
      double W;
      double L;
      int modelo;
      int ID;
    };
    //This PAT is for the imported CAD to fit the size
    osg::ref_ptr<osg::PositionAttitudeTransform> modulo;
    osg::ref_ptr <osg::PositionAttitudeTransform> modulo2;
    //Node for the Imported CAD
    osg::ref_ptr<osg::Node> modelo;
    osg::ref_ptr<osg::Node> modelo2;
    //Viewer for OSG
    osg::ref_ptr<osgViewer::Viewer> viewer;
    //root for the Scene Graph
    osg::ref_ptr<osg::Group> root;
    //Vector for all the snakes in the scene
    QVector <serpiente> serpientes;
    //Axis switch
    osg::ref_ptr<osg::Switch> axis;
    bool fogState;
    //Function that create the cubes to be drawn insted of the CADS
    osg::ref_ptr<osg::Geode> crearCubo(osg::Vec4 color);
    osg::ref_ptr<osg::Group> Ambiente;

  public:
    simOSG();
    ~simOSG();
    //Function to start the drawing process of the viewer
    int Dibujar();
    //Function to add the ambience to the Scene Graph
    void DibujarAmbiente();
    //Function to add a snake to the Scene Graph and the serpientes Vector
    void AgregarSerpiente(int tamanno, double H, double W, double L, int modelo);
    //Function to add a control point to a snake. The snake ID is in order of adding from 0 to n where n is the number of snakes or size of serpientes
    void AgregarControlPoint(int ID_Serpiente, int Num_Modulo, float tiempo, osg::Vec3 posicion, osg::Quat rotacion);
    //Draw the axes of the scene. X, Y , Z
    osg::ref_ptr <osg::Group> DibujarEjes(double size);
    //Function to import and scale the CAD from a file. 
    void AgregarModulo();
    //Function to add the scene lights to the scene Graph
    void AgregarLuces();
    //Function to add fog to the scene.
    void AgregarFog();
    //Getter  of the viewer to use with Qt
    osg::ref_ptr<osgViewer::Viewer> getViewer();
    //Function to pause the animation.
    void pause();
    //Function to un pause the animation
    void unpause();

    //Function to change the speed in the video
    void setMultiplier(double multiplier);

    //Function to add heighfield
    void AgregarHeighfield(osg::ref_ptr <osg::Node> hf,osg::Vec3);

    //Switching functions
    void switchModelo(int Id_Serpiente, int Num_Modulo, bool estado);
    void switchEjesSerpiente(int Id_Serpiente,int Num_Modulo, bool estado);
    void switchPuntosContacto(int Id_Serpiente, bool estado);
    void switchEjesGenerales(bool estado);
    void switchFOG(bool estado);
    void switchTrayectoria(int Id_Serpiente,int Num_Modulo, bool estado);

    //Get state funcition
    QMap <QString,bool> getStates(int Id_Serpiente, int Modulo);

    //for the shadows
    int ReceivesShadowTraversalMask;
    int CastsShadowTraversalMask;
    osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene;
    osg::ref_ptr<osgShadow::ShadowMap> sm;

    //For the contact points
    void setContactPoints(int Id_Serpiente,QVector <double*> contactPoints);
    osg::ref_ptr<osg::Geode> crearCubo(osg::Vec4 color, osg::Vec3 tamanno);

    //For the trayectory points
    void setTrayectory();

    //For the stop button, delete the snake
    void deleteSnakes();
};

#endif
