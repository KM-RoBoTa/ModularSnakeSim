#include "viewerwidget.h"
#include "ui_viewerwidget.h"
#include <osgQt/GraphicsWindowQt>
#include <QVBoxLayout>

ViewerWidget::ViewerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewerWidget)
{
    ui->setupUi(this);
}

ViewerWidget::ViewerWidget(osg::ref_ptr <osgViewer::Viewer> viewer,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewerWidget)
{
    this->setMouseTracking(false);
    this->_viewer=viewer;
    camManip =new osgGA::TrackballManipulator;
    //Define de initial position of the camera. Center=1,1,1 and looking to the origin.
    camManip->setHomePosition(osg::Vec3d(1.0,0.5,1.0),osg::Vec3d(0.0,0.0,0.0),osg::Vec3d(0.0,1.0,0.0),false);
    //Attach the camera manipulator to the viewer
    viewer->setCameraManipulator( camManip.get());
    ui->setupUi(this);
    osg::ref_ptr<osg::GraphicsContext::Traits> traits ;
    traits =  new osg::GraphicsContext::Traits;
    //traits->windowDecoration = false;
    traits->x = 0;
    traits->y = 0;
    traits->width = 800;
    traits->height = 600;
    //traits->doubleBuffer = true;
    viewer->getCamera()->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height) );
    viewer->getCamera()->setGraphicsContext(new osgQt::GraphicsWindowQt(traits.get()));
    viewer->getCamera()->setProjectionMatrixAsPerspective( 30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
    //Define a trackball manipulator, the way we handle the movement of the camera with the mouse
    osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( viewer->getCamera()->getGraphicsContext() );
    if ( gw )
    {
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget( gw->getGLWidget() );
        this->setLayout( layout );
        this->_graphics=gw->getGLWidget();
    }
    connect( &this->_timer, SIGNAL(timeout()), this, SLOT(update()) );
    this->_timer.start( 80 );
}

ViewerWidget::~ViewerWidget()
{
    this->_viewer=NULL;
    this->camManip=NULL;
    delete this->_graphics;
    delete ui;
}

QPixmap ViewerWidget::Screenshot()
{
    return this->_graphics->renderPixmap();
}

void ViewerWidget::flush()
{

}
