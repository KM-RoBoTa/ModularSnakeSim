#include "simOSG.h"


simOSG::simOSG()
{
    ReceivesShadowTraversalMask = 0x1;
    CastsShadowTraversalMask = 0x2;
    shadowedScene = new osgShadow::ShadowedScene;
    //Prepare the shadows
    this->shadowedScene->setReceivesShadowTraversalMask(this->ReceivesShadowTraversalMask);
    this->shadowedScene->setCastsShadowTraversalMask(this->CastsShadowTraversalMask);
    this->sm = new osgShadow::ShadowMap;
    this->shadowedScene->setShadowTechnique(sm.get());
    int mapres = 1024;
    sm->setTextureSize(osg::Vec2s(mapres,mapres));
    //Create the memory for the Group which is the root node of the SceneGraph
    root=new osg::Group;
    this->shadowedScene->addChild(root.get());
    this->viewer=new osgViewer::Viewer;
    //Clear color to black and viewport to the entire widget
    viewer->getCamera()->setClearColor( osg::Vec4(0.0, 0.0, 0.0, 1.0) );
    //Some video stadistics
    viewer->addEventHandler( new osgViewer::StatsHandler() );
    viewer->addEventHandler( new osgViewer::ScreenCaptureHandler() );
    // Use single thread here to avoid known issues under Linux
    viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded );
    //Attach the root to the viewer
    viewer->setSceneData(this->shadowedScene.get());


    //Don't close the window with the keyboard.
    viewer->setKeyEventSetsDone(0);

    //Add some realism to the scene with multisampling,
    //osg::DisplaySettings::instance()->setNumMultiSamples( 2 );

    //Import the CAD.
    AgregarModulo();
    //Draw the axes
    this->axis=new osg::Switch;
    this->axis->addChild(DibujarEjes(10.0).get(),false);
    this->root->addChild(this->axis.get());
    this->axis->setNodeMask(this->ReceivesShadowTraversalMask);
    this->Ambiente=new osg::Group;
    this->root->addChild(this->Ambiente.get());
    //Draw the ambience
    //DibujarAmbiente();
    //Add light
    AgregarLuces();
    //Add fog
    AgregarFog();
    this->fogState=true;
}

simOSG::~simOSG()
{
    this->shadowedScene=NULL;
    this->root=NULL;
    this->modelo=NULL;
    this->modulo=NULL;
    this->axis=NULL;
    this->sm=NULL;
    for(int i = 0 ; i < this->serpientes.size() ; i ++)
    {
        this->serpientes[i].contenedor=NULL;
        this->serpientes[i].switchContactPoints=NULL;
        for (int j = 0 ; j < this->serpientes[i].numModulos ; j++)
        {
            this->serpientes[i].switchEjes[j]=NULL;
            this->serpientes[i].ModulosAnimator[j]=NULL;
            this->serpientes[i].ModulosAnimatorCallback[j]=NULL;
            this->serpientes[i].switchModelos[j]=NULL;
            this->serpientes[i].switchTrayectoria[j]=NULL;
        }
    }
    this->viewer=NULL;
}

int simOSG::Dibujar()
{
  //Set the windows size.
  //viewer.setUpViewInWindow(100, 100, 800, 600);
  //Start drawing the Scene Graph
  return viewer->run();
}

void simOSG::DibujarAmbiente()
{
    this->Ambiente.release();
    this->Ambiente=new osg::Group;
    osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
    color->push_back(osg::Vec4(1.0,1.0,1.0,1.0));
  //Create a Geometry for the Plane XZ
    osg::ref_ptr<osg::Geometry> PlaneXZ( new osg::Geometry);
    osg::ref_ptr<osg::Geometry> PlaneYZ( new osg::Geometry); 
    osg::ref_ptr<osg::Geometry> PlaneXY( new osg::Geometry); 
    osg::ref_ptr<osg::Geometry> PlaneZY( new osg::Geometry); 
    osg::ref_ptr<osg::Geometry> PlaneYX( new osg::Geometry); 
    //The vertex array for the vertices of the plane
    osg::ref_ptr<osg::Vec3Array> VerticesPlaneXZ = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> VerticesPlaneYZ = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> VerticesPlaneXY = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> VerticesPlaneZY = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> VerticesPlaneYX = new osg::Vec3Array;
    //Define the different vertices
    //Piso
    osg::Vec3 p1(-10,0,10); 
    osg::Vec3 p2(-10,0,-10);
    osg::Vec3 p3(10,0,-10);
    osg::Vec3 p4(10,0,10);
    //derecha
    osg::Vec3 p5(10,10,10); 
    osg::Vec3 p6(10,0,10);
    osg::Vec3 p7(10,0,-10);
    osg::Vec3 p8(10,10,-10);
    //frontal
    osg::Vec3 p9(-10,0,10); 
    osg::Vec3 p10(10,0,10);
    osg::Vec3 p11(10,10,10);
    osg::Vec3 p12(-10,10,10);
    //izquierda
    osg::Vec3 p13(-10,0,10); 
    osg::Vec3 p14(-10,0,-10);
    osg::Vec3 p15(-10,10,-10);
    osg::Vec3 p16(-10,10,10);
    //Trasera
    osg::Vec3 p17(-10,0,-10); 
    osg::Vec3 p18(-10,10,-10);
    osg::Vec3 p19(10,10,-10);
    osg::Vec3 p20(10,0,-10);
    
    
    
    //Add the vertices to the vertex array
    VerticesPlaneXZ->push_back(p1);
    VerticesPlaneXZ->push_back(p2);
    VerticesPlaneXZ->push_back(p3);
    VerticesPlaneXZ->push_back(p4);
   
    VerticesPlaneYZ->push_back(p5);
    VerticesPlaneYZ->push_back(p6);
    VerticesPlaneYZ->push_back(p7);
    VerticesPlaneYZ->push_back(p8);
    
    VerticesPlaneXY->push_back(p9);
    VerticesPlaneXY->push_back(p10);
    VerticesPlaneXY->push_back(p11);
    VerticesPlaneXY->push_back(p12);
    
    VerticesPlaneZY->push_back(p13);
    VerticesPlaneZY->push_back(p14);
    VerticesPlaneZY->push_back(p15);
    VerticesPlaneZY->push_back(p16);
    
    VerticesPlaneYX->push_back(p17);
    VerticesPlaneYX->push_back(p18);
    VerticesPlaneYX->push_back(p19);
    VerticesPlaneYX->push_back(p20);
    
    //Define the Normal Array and add the same normal to every vertex
    osg::ref_ptr<osg::Vec3Array> normalsXZ = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> normalsYZ = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> normalsXY = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> normalsZY = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> normalsYX = new osg::Vec3Array;
    //Piso
    normalsXZ->push_back( osg::Vec3(0.0f,1.0f, 0.0f) );
    //Derecha
    normalsYZ->push_back( osg::Vec3(-1.0f,0.0f, 0.0f) );
    //Frontal
    normalsXY->push_back( osg::Vec3(0.0f,0.0f, -1.0f) );
    //izquierda
    normalsZY->push_back( osg::Vec3(1.0f,0.0f, 0.0f) );
    //Trasera
    normalsYX->push_back( osg::Vec3(0.0f,0.0f, 1.0f) );
    
    //Define the vertex array and the Normal Array to the geometry
    PlaneXZ->setVertexArray(VerticesPlaneXZ.get());
    PlaneXZ->setNormalArray(normalsXZ.get());
    //Every vertex has the same geometry, BIND_OVERALL
    PlaneXZ->setNormalBinding( osg::Geometry::BIND_OVERALL );
    //Define how the vertex array is read to draw the ambience. GL_QUADS means that every 4 vertex is a square, 0 is te initial offset and 4 is the step
    PlaneXZ->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));
    //Define the color
    PlaneXZ->setColorArray(color.get());
    PlaneXZ->setColorBinding(osg::Geometry::BIND_OVERALL);

    //Define the vertex array and the Normal Array to the geometry
    PlaneYZ->setVertexArray(VerticesPlaneYZ.get());
    PlaneYZ->setNormalArray(normalsYZ.get());
    //Every vertex has the same geometry, BIND_OVERALL
    PlaneYZ->setNormalBinding( osg::Geometry::BIND_OVERALL );
    //Define how the vertex array is read to draw the ambience. GL_QUADS means that every 4 vertex is a square, 0 is te initial offset and 4 is the step
    PlaneYZ->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));
    //Define the color
    PlaneYZ->setColorArray(color.get());
    PlaneYZ->setColorBinding(osg::Geometry::BIND_OVERALL);

    //Define the vertex array and the Normal Array to the geometry
    PlaneXY->setVertexArray(VerticesPlaneXY.get());
    PlaneXY->setNormalArray(normalsXY.get());
    //Every vertex has the same geometry, BIND_OVERALL
    PlaneXY->setNormalBinding( osg::Geometry::BIND_OVERALL );
    //Define how the vertex array is read to draw the ambience. GL_QUADS means that every 4 vertex is a square, 0 is te initial offset and 4 is the step
    PlaneXY->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));
    //Define the color
    PlaneXY->setColorArray(color.get());
    PlaneXY->setColorBinding(osg::Geometry::BIND_OVERALL);

    //Define the vertex array and the Normal Array to the geometry
    PlaneZY->setVertexArray(VerticesPlaneZY.get());
    PlaneZY->setNormalArray(normalsZY.get());
    //Every vertex has the same geometry, BIND_OVERALL
    PlaneZY->setNormalBinding( osg::Geometry::BIND_OVERALL );
    //Define how the vertex array is read to draw the ambience. GL_QUADS means that every 4 vertex is a square, 0 is te initial offset and 4 is the step
    PlaneZY->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));
    //Define the color
    PlaneZY->setColorArray(color.get());
    PlaneZY->setColorBinding(osg::Geometry::BIND_OVERALL);
    
    //Define the vertex array and the Normal Array to the geometry
    PlaneYX->setVertexArray(VerticesPlaneYX.get());
    PlaneYX->setNormalArray(normalsYX.get());
    //Every vertex has the same geometry, BIND_OVERALL
    PlaneYX->setNormalBinding( osg::Geometry::BIND_OVERALL );
    //Define how the vertex array is read to draw the ambience. GL_QUADS means that every 4 vertex is a square, 0 is te initial offset and 4 is the step
    PlaneYX->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));
    //Define the color
    PlaneYX->setColorArray(color.get());
    PlaneYX->setColorBinding(osg::Geometry::BIND_OVERALL);

    osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
    texcoords->push_back( osg::Vec2(0,1) );
    texcoords->push_back( osg::Vec2(0,0) );
    texcoords->push_back( osg::Vec2(1, 0) );
    texcoords->push_back( osg::Vec2(1,1) );
    
    
    PlaneXZ->setTexCoordArray( 0, texcoords.get() );
    PlaneYZ->setTexCoordArray( 0, texcoords.get() );
    PlaneXY->setTexCoordArray( 0, texcoords.get() );
    PlaneZY->setTexCoordArray( 0, texcoords.get() );
    PlaneYX->setTexCoordArray( 0, texcoords.get() );
    
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile( ":/images/Resources/baldosa.bmp" );
    texture->setImage( image.get() );
    texture->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
    texture->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
    texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    //Create a Geode to add the Geometry
    osg::ref_ptr <osg::Geode> PlaneGeode = new osg::Geode();
    //Add the geometry to the geode
    PlaneGeode->addDrawable(PlaneXZ);
    PlaneGeode->addDrawable(PlaneYZ);
    PlaneGeode->addDrawable(PlaneXY);
    PlaneGeode->addDrawable(PlaneZY);
    PlaneGeode->addDrawable(PlaneYX);
    PlaneGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get() );
    //Add the plane to the root, the scene graph
    PlaneGeode->setNodeMask(this->ReceivesShadowTraversalMask);
    PlaneGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    this->Ambiente->addChild(PlaneGeode);
    this->root->addChild(this->Ambiente);
}

void simOSG::AgregarSerpiente(int tamanno, double H, double W, double L, int modelo)
{
  //Create a new snake (container)
  serpiente nueva;
  //Set the size parameter
  nueva.numModulos=tamanno;
  nueva.H=H;
  nueva.L=L;
  nueva.W=W;
  nueva.modelo=modelo;
  //set the ID as the serpientes vector size
  nueva.ID=serpientes.size();
  //Reserve memory for the container
  nueva.contenedor = new osg::Group;
  //Switch de puntos de contacto
  nueva.switchContactPoints= new osg::Switch;
  root->addChild(nueva.switchContactPoints.get());
  //For each module of the snake we create a Matrix transform, the animationPath and the update call back for the animation path, also add each animation path to the container.
  for(int i=0; i< tamanno;i++)
  {
    //Create the Matrix Transform
    osg::ref_ptr<osg::MatrixTransform> transform= new osg::MatrixTransform;
    //Switch de modelos
    osg::ref_ptr <osg::Switch> modelos3d = new osg::Switch;
    if (nueva.modelo==0)
        modelos3d->addChild(modulo.get(),true);
    else if (nueva.modelo==1)
        modelos3d->addChild(modulo2.get(),true);
    if (i%2==0) //Crea  los cubos de color azul (pares) y rojo (impares)
        modelos3d->addChild(crearCubo(osg::Vec4(1.0,0.0,0.0,1.0),osg::Vec3(H,W,L)).get(),false);
    else
        modelos3d->addChild(crearCubo(osg::Vec4(0.0,0.0,1.0,1.0),osg::Vec3(H,W,L)).get(),false);
    //Transformaciòn de posicion y orientaciòn
    transform->addChild(modelos3d.get());
    nueva.switchModelos.push_back(modelos3d);
    //Switch para trayectoria
    osg::ref_ptr <osg::Switch> trayectoria=new osg::Switch;
    nueva.switchTrayectoria.push_back(trayectoria);
    osg::ref_ptr<osg::Vec3Array> vertices_puntos (new osg::Vec3Array());
    nueva.Trayectorias.push_back(vertices_puntos.get());
    root->addChild(trayectoria.get());
    //switch de ejes
    osg::ref_ptr <osg::Switch> ejes=new osg::Switch;
    ejes->addChild(DibujarEjes(0.1).get(),false);
    nueva.switchEjes.push_back(ejes);
    transform->addChild(ejes.get());
    //Create the animationPath
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
    //Set the mode in LOOP, repeate the animation forever
    path->setLoopMode( osg::AnimationPath::LOOP );
    //Add the animationPath to the Serpiente Container, so we can modify the module animation path later
    nueva.ModulosAnimator.push_back(path);
    //Create the AnimationPathCallback
    osg::ref_ptr<osg::AnimationPathCallback> pathCallback=new osg::AnimationPathCallback();
    //Add the animationPath to the AnimationPathCallback
    pathCallback->setAnimationPath(path.get());
    nueva.ModulosAnimatorCallback.push_back(pathCallback);
    //Set the Update Callback with the AnimationPathCallback in the transform node.
    transform->setUpdateCallback(pathCallback.get());
    //Add the transform to the container of the snake-
    nueva.contenedor->addChild(transform.get());
  }
  //Add the snake to the scene graph
  nueva.contenedor->setNodeMask(this->CastsShadowTraversalMask);
  root->addChild(nueva.contenedor.get());
  //Add the snake to the snakes vector
  serpientes.push_back(nueva);
}

void simOSG::AgregarControlPoint(int ID_Serpiente, int Num_Modulo, float tiempo, osg::Vec3 posicion, osg::Quat rotacion)
{
  serpientes[ID_Serpiente].ModulosAnimator[Num_Modulo]->insert(tiempo,osg::AnimationPath::ControlPoint(posicion,rotacion));

  //Se crean los puntos para
  this->serpientes[ID_Serpiente].Trayectorias[Num_Modulo]->push_back(posicion);

}

osg::ref_ptr<osg::Geode> simOSG::crearCubo(osg::Vec4 color)
{
    osg::ref_ptr<osg::ShapeDrawable> shape1 = new osg::ShapeDrawable;
    shape1->setShape( new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),0.04f));
    osg::ref_ptr<osg::Geode> Cubo = new osg::Geode;
    shape1->setColor(color);
    Cubo->addDrawable( shape1.get() );
    return Cubo.release();
}

osg::ref_ptr<osg::Geode> simOSG::crearCubo(osg::Vec4 color, osg::Vec3 tamanno)
{
    osg::ref_ptr<osg::ShapeDrawable> shape1 = new osg::ShapeDrawable;
    shape1->setShape( new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),tamanno[0],tamanno[1],tamanno[2]));
    osg::ref_ptr<osg::Geode> Cubo = new osg::Geode;
    shape1->setColor(color);
    Cubo->addDrawable( shape1.get() );
    return Cubo.release();
}

void simOSG::setTrayectory()
{

    osg::ref_ptr<osg::LineWidth> line=new osg::LineWidth;
    line->setWidth(4);
    for (int i = 0 ; i < this->serpientes.size() ; i ++)
    {
        for (int j = 0 ; j < this->serpientes[i].numModulos ; j ++)
        {
            (this->serpientes[i].switchTrayectoria[j])=new osg::Switch;
            root->addChild((this->serpientes[i].switchTrayectoria[j]).get());
            osg::ref_ptr<osg::Geode> puntos =new osg::Geode();
            osg::ref_ptr<osg::Geometry> Geometria_Puntos =new osg::Geometry();
            Geometria_Puntos->setVertexArray (this->serpientes[i].Trayectorias[j]);
            Geometria_Puntos->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,this->serpientes[i].Trayectorias[j]->size()));
            puntos->getOrCreateStateSet()->setAttribute(line.get());
            puntos->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
            osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
            color->push_back(osg::Vec4(0.6,0.0,0.0,1.0));
            Geometria_Puntos->setColorArray(color);
            Geometria_Puntos->setColorBinding(osg::Geometry::BIND_OVERALL);
            puntos->addDrawable(Geometria_Puntos.get());
            this->serpientes[i].switchTrayectoria[j]->addChild(puntos.get(),false);
            puntos->setNodeMask(this->ReceivesShadowTraversalMask);
        }
    }
}

void simOSG::deleteSnakes()
{
    for (int i = 0 ; i < this->serpientes.size() ; i ++)
    {
         this->root->removeChild(this->serpientes[i].contenedor);
        this->root->removeChild(this->serpientes[i].switchContactPoints);
        for (int j = 0 ; j < this->serpientes[i].switchTrayectoria.size() ; j ++)
            this->root->removeChild(this->serpientes[i].switchTrayectoria[j]);

    }
    this->serpientes.remove(0,this->serpientes.size());
}

osg::ref_ptr <osg::Group> simOSG::DibujarEjes(double size)
{
    /////////////EJES
    osg::Vec3 Centro(0,0,0); 
    osg::Vec3 x(size,0,0);
    osg::Vec3 y(0,size,0);
    osg::Vec3 z(0,0,size);
    osg::ref_ptr<osg::Geometry> XGeometry( new osg::Geometry);
    osg::ref_ptr<osg::Geometry> YGeometry( new osg::Geometry);
    osg::ref_ptr<osg::Geometry> ZGeometry( new osg::Geometry); 
    osg::ref_ptr<osg::Vec3Array> PuntosX = new osg::Vec3Array; 
    osg::ref_ptr<osg::Vec3Array> PuntosY = new osg::Vec3Array; 
    osg::ref_ptr<osg::Vec3Array> PuntosZ = new osg::Vec3Array; 
    PuntosX->push_back(Centro); 
    PuntosX->push_back(x); 
    PuntosY->push_back(Centro); 
    PuntosY->push_back(y); 
    PuntosZ->push_back(Centro); 
    PuntosZ->push_back(z); 
    osg::ref_ptr<osg::Vec4Array> colorX = new osg::Vec4Array; 
    osg::ref_ptr<osg::Vec4Array> colorY = new osg::Vec4Array; 
    osg::ref_ptr<osg::Vec4Array> colorZ = new osg::Vec4Array; 
    colorX->push_back(osg::Vec4(1.0,0.0,0.0,1.0)); 
    colorY->push_back(osg::Vec4(0.0,1.0,0.0,1.0)); 
    colorZ->push_back(osg::Vec4(0.0,0.0,1.0,1.0));
    XGeometry->setVertexArray(PuntosX.get()); 
    XGeometry->setColorArray(colorX.get()); 
    XGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    XGeometry->addPrimitiveSet(new osg::DrawArrays(GL_LINES,0,2));
    YGeometry->setVertexArray(PuntosY.get()); 
    YGeometry->setColorArray(colorY.get()); 
    YGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    YGeometry->addPrimitiveSet(new osg::DrawArrays(GL_LINES,0,2));
    ZGeometry->setVertexArray(PuntosZ.get()); 
    ZGeometry->setColorArray(colorZ.get()); 
    ZGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    ZGeometry->addPrimitiveSet(new osg::DrawArrays(GL_LINES,0,2));
    
    
    //State set for the line width
    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth;
    lineWidth->setWidth( 5.0f );
    
    // Declare a instance of the geode class: 
    osg::ref_ptr <osg::Geode> lineaXGeode = new osg::Geode;
    lineaXGeode->addDrawable(XGeometry);
    lineaXGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    lineaXGeode->getOrCreateStateSet()->setAttribute(lineWidth.get());
    
    osg::ref_ptr <osg::Geode> lineaYGeode = new osg::Geode;
    lineaYGeode->addDrawable(YGeometry);   
    lineaYGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    lineaYGeode->getOrCreateStateSet()->setAttribute(lineWidth.get());
    
   osg::ref_ptr <osg::Geode> lineaZGeode = new osg::Geode;
    lineaZGeode->addDrawable(ZGeometry);   
    lineaZGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    lineaZGeode->getOrCreateStateSet()->setAttribute(lineWidth.get());
    
    osg::ref_ptr<osg::Group> rta= new osg::Group;
    rta->addChild(lineaXGeode.get());
    rta->addChild(lineaYGeode.get());
    rta->addChild(lineaZGeode.get());
    return rta.release();
    
}

void simOSG::AgregarModulo()
{
    //Para el modelo CAD sin foamy
    modelo = osgDB::readNodeFile( ":/other/Resources/modelo2.3ds" );
   // modelo->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    modulo = new osg::PositionAttitudeTransform;
    modulo->addChild(modelo.get());
    modulo->setScale(osg::Vec3(1.0,1.0,1.0));
    //Para el modelo CAD con foamy
    modelo2 = osgDB::readNodeFile( ":/other/Resources/modelo3.3ds" );
   // modelo2->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    modulo2 = new osg::PositionAttitudeTransform;
    modulo2->addChild(modelo2.get());
    modulo2->setScale(osg::Vec3(1.0,1.0,1.0));
}

void simOSG::AgregarLuces()
{
  //Create the light
  osg::ref_ptr<osg::Light> light = new osg::Light;
  //Set the light number to identify it.
    this->sm->cleanSceneGraph();
  light->setLightNum( 1 );
  light->setDiffuse( osg::Vec4(0.9f, 0.9f, 0.9f, 1.0f) );
  light->setAmbient(osg::Vec4(0.9f, 0.9f, 0.9f, 1.0f));
  light->setSpecular(osg::Vec4(0.7f, 0.7f, 0.7f, 1.0f));
  light->setPosition( osg::Vec4(10.0f, 10.0f, 10.0f, 1.0f) );
  light->setConstantAttenuation(0.5);
  osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
  lightSource->setLight( light );
  //root->addChild(lightSource.get());
  //root->getOrCreateStateSet()->setMode(GL_LIGHT0,osg::StateAttribute::OFF);
  //root->getOrCreateStateSet()->setMode(GL_LIGHT1,osg::StateAttribute::ON);
  this->shadowedScene->getOrCreateStateSet()->setMode(GL_LIGHT0,osg::StateAttribute::OFF);
  this->shadowedScene->getOrCreateStateSet()->setMode(GL_LIGHT1,osg::StateAttribute::ON);
  this->shadowedScene->addChild(lightSource.get());
  this->sm->setLight(lightSource.get());

}

void simOSG::AgregarFog()
{
  osg::ref_ptr<osg::Fog> fog = new osg::Fog;
  fog->setMode( osg::Fog::LINEAR );
  fog->setStart( 1.0f );
  fog->setEnd( 10.0f );
  fog->setColor( osg::Vec4(0.3f, 0.3f, 0.3f, 0.5f));
  this->shadowedScene->getOrCreateStateSet()->setAttributeAndModes( fog.get() );
  this->shadowedScene->getOrCreateStateSet()->setMode(GL_FOG,osg::StateAttribute::ON);
}


osg::ref_ptr <osgViewer::Viewer> simOSG::getViewer()
{
    return this->viewer.release();
}

void simOSG::pause()
{
    for(int i = 0 ; i < this->serpientes.size() ; i ++)
    {
        for (int  j = 0 ; j < this->serpientes[i].ModulosAnimatorCallback.size() ; j ++)
        {
            this->serpientes[i].ModulosAnimatorCallback[j]->setPause(true);
        }
    }
}

void simOSG::unpause()
{
    for(int i = 0 ; i < this->serpientes.size() ; i ++)
    {
        for (int j = 0 ; j < this->serpientes[i].ModulosAnimatorCallback.size() ; j ++)
        {
            this->serpientes[i].ModulosAnimatorCallback[j]->setPause(false);
        }
    }
}

void simOSG::setMultiplier(double multiplier)
{
    for(int i = 0 ; i < this->serpientes.size() ; i ++)
    {
        for (int j = 0 ; j < this->serpientes[i].ModulosAnimatorCallback.size() ; j ++)
        {
            this->serpientes[i].ModulosAnimatorCallback[j]->setTimeMultiplier(multiplier);
        }
    }
}
void simOSG::AgregarHeighfield(osg::ref_ptr<osg::Node> hf,osg::Vec3 origin)
{
    this->Ambiente.release();
    this->Ambiente=new osg::Group;
    osg::ref_ptr<osg::PositionAttitudeTransform> transform =new osg::PositionAttitudeTransform;
    transform->addChild(hf.get());
    transform->setAttitude(osg::Quat(osg::DegreesToRadians(90.0),osg::Vec3(1.0,0.0,0.0)));
    transform->setPosition(origin);
    osg::ref_ptr<osg::Group> aux_grp=new osg::Group;
    aux_grp->addChild(transform.get());
    aux_grp->setNodeMask(this->ReceivesShadowTraversalMask);
    aux_grp->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    osg::ref_ptr<osg::PositionAttitudeTransform> transform2 =new osg::PositionAttitudeTransform;
    transform2->addChild(aux_grp.get());
    transform2->setAttitude(osg::Quat(osg::DegreesToRadians(-90.0),osg::Vec3(0.0,1.0,0.0)));
    this->Ambiente->addChild(transform2.get());
    this->root->addChild(this->Ambiente);
}

void simOSG::switchModelo(int Id_Serpiente, int Num_Modulo,bool estado)
{
    //Num_Modulo==-1 quiere decir todos los modulos.
    if (Num_Modulo==-1)
    {
        for (int i = 0; i < this->serpientes[Id_Serpiente].numModulos ; i++)
        {
            this->serpientes[Id_Serpiente].switchModelos[i]->setValue(0,estado);//Cero es modelo cad
            this->serpientes[Id_Serpiente].switchModelos[i]->setValue(1,!estado); //Uno es modelo cubo
        }
    }
    else if(Num_Modulo<this->serpientes[Id_Serpiente].numModulos)
    {
        this->serpientes[Id_Serpiente].switchModelos[Num_Modulo]->setValue(0,estado); //Cero es modelo cad
        this->serpientes[Id_Serpiente].switchModelos[Num_Modulo]->setValue(1,!estado); //Uno es modelo cubo
    }

//    if (estado)
//    {
//        this->root->getOrCreateStateSet()->setMode(GL_COLOR_MATERIAL,osg::StateAttribute::OFF);
//    }
//    else
//    {
//        this->root->getOrCreateStateSet()->setMode(GL_COLOR_MATERIAL,osg::StateAttribute::ON);
//    }

}

void simOSG::switchEjesSerpiente(int Id_Serpiente, int Num_Modulo, bool estado)
{
    //Num_Modulo==-1 quiere decir todos los modulos.
    if (Num_Modulo==-1)
    {
        for (int i = 0; i < this->serpientes[Id_Serpiente].numModulos ; i++)
        {
            this->serpientes[Id_Serpiente].switchEjes[i]->setValue(0,estado);//Cero son los ejes
        }
    }
    else if(Num_Modulo<this->serpientes[Id_Serpiente].numModulos)
    {
            this->serpientes[Id_Serpiente].switchEjes[Num_Modulo]->setValue(0,estado);//Cero son los ejes

    }
}

void simOSG::switchPuntosContacto(int Id_Serpiente, bool estado)
{
    this->serpientes[Id_Serpiente].switchContactPoints->setValue(0,estado);//Cero son los puntos de contacto
}

void simOSG::switchEjesGenerales(bool estado)
{
    this->axis->setValue(0,estado); //Axis generales
}

void simOSG::switchFOG(bool estado)
{
    if (estado)
    {
        this->root->getOrCreateStateSet()->setMode(GL_FOG,osg::StateAttribute::ON);
        this->fogState=estado;
    }
    else
    {
        this->root->getOrCreateStateSet()->setMode(GL_FOG,osg::StateAttribute::OFF);
        this->fogState=estado;
    }
}

void simOSG::switchTrayectoria(int Id_Serpiente, int Num_Modulo, bool estado)
{
    //Num_Modulo==-1 quiere decir todos los modulos.
    if (Num_Modulo==-1)
    {
        for (int i = 0; i < this->serpientes[Id_Serpiente].numModulos ; i++)
        {
            this->serpientes[Id_Serpiente].switchTrayectoria[i]->setValue(0,estado);
        }
    }
    else if(Num_Modulo<this->serpientes[Id_Serpiente].numModulos)
    {
        this->serpientes[Id_Serpiente].switchTrayectoria[Num_Modulo]->setValue(0,estado);
    }
}

QMap<QString, bool> simOSG::getStates(int Id_Serpiente,int Modulo)
{
    QMap <QString,bool> states;
    states["Gen_Ejes"]=this->axis->getValue(0);
    states["Gen_FOG"]=this->fogState;
    states["Mod_Ejes"]=this->serpientes[Id_Serpiente].switchEjes[Modulo]->getValue(0);
    states["Mod_CadCub"]=this->serpientes[Id_Serpiente].switchModelos[Modulo]->getValue(0);
    states["Mod_ContactPoints"]=this->serpientes[Id_Serpiente].switchContactPoints->getValue(0);
    states["Mod_trayectoria"]=this->serpientes[Id_Serpiente].switchTrayectoria[Modulo]->getValue(0);
    return states;
}

void simOSG::setContactPoints(int Id_Serpiente, QVector<double *> contactPoints)
{
    int numChild=this->serpientes[Id_Serpiente].switchContactPoints->getNumChildren();
    this->serpientes[Id_Serpiente].switchContactPoints->removeChild(0,numChild);
//    osg::ref_ptr<osg::Group> contactos=new osg::Group;
        osg::ref_ptr<osg::Geode> geode (new osg::Geode());
    osg::ref_ptr<osg::Geometry> geometry (new osg::Geometry());
    osg::ref_ptr<osg::Vec3Array> vertices (new osg::Vec3Array());
    for (int i = 0 ; i < contactPoints.size() ; i ++)
    {
//        osg::ref_ptr <osg::PositionAttitudeTransform> tf=new osg::PositionAttitudeTransform;
//        tf->setPosition(osg::Vec3(*contactPoints.at(i),*(contactPoints.at(i)+1) , *(contactPoints.at(i)+2)));
//        tf->addChild(crearCubo(osg::Vec4(1.0,0.0,0.0,1.0),osg::Vec3(0.01,0.001,0.01)));
//        contactos->addChild(tf.get());
        vertices->push_back (osg::Vec3 ( *contactPoints.at(i),*(contactPoints.at(i)+1) , *(contactPoints.at(i)+2)));
    }
    geometry->setVertexArray (vertices.get());
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,vertices->size()));
    osg::ref_ptr<osg::Point> point=new osg::Point;
    point->setSize(5);
    geode->getOrCreateStateSet()->setAttribute(point.get());
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
    color->push_back(osg::Vec4(0.0,0.0,0.6392156863,1.0));
    geometry->setColorArray(color);
    geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
    geode->addDrawable(geometry.get());
    this->serpientes[Id_Serpiente].switchContactPoints->addChild(geode.get(),false);
    geode->setNodeMask(this->ReceivesShadowTraversalMask);
//    this->serpientes[Id_Serpiente].switchContactPoints->addChild(contactos.get(),false);
//    contactos->setNodeMask(this->ReceivesShadowTraversalMask);
}

