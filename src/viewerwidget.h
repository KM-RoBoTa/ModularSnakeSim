#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <osg/Camera>
#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <QGLWidget>
#include <QPixmap>

namespace Ui {
class ViewerWidget;
}

class ViewerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ViewerWidget(QWidget *parent = 0);
    ViewerWidget(osg::ref_ptr <osgViewer::Viewer> viewer, QWidget *parent = 0);
    ~ViewerWidget();
    QPixmap Screenshot();
    void flush();
    
protected:
    virtual void paintEvent( QPaintEvent* event )
    { _viewer->frame(); }

private:
    Ui::ViewerWidget *ui;
    osg::ref_ptr <osgViewer::Viewer> _viewer;
    QTimer _timer;
    QGLWidget *_graphics;
    osg::ref_ptr<osgGA::TrackballManipulator> camManip;
};

#endif // VIEWERWIDGET_H
