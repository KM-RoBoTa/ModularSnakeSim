#ifndef MELOSIMULA_H
#define MELOSIMULA_H

#include <QMainWindow>
#include "simulador.h"
#include "viewerwidget.h"
#include "graphicparameters.h"
#include "plugins/anglegenerator.h"

namespace Ui {
class Melosimula;
}

class Melosimula : public QMainWindow
{
    Q_OBJECT
    
public:
//    simulador * getSimulator();
    explicit Melosimula(QWidget *parent = 0);
    ~Melosimula();
    
private slots:
    void on_actionSalir_triggered();

    void on_actionAbout_triggered();

    void on_SimularButton_clicked();

    void on_LoadAnglesButton_clicked();

    void on_pausaButton_clicked();

    void on_SnakeValues_clicked();

    void on_reverseSpeedButton_clicked();

    void on_stepFWButton_pressed();

    void on_stepFWButton_released();

    void on_StepBWButton_pressed();

    void on_StepBWButton_released();

    void on_stopButton_clicked();

    void on_refreshButton_clicked();

    void on_ODEValues_clicked();

    void on_SpeedButton_clicked();

    void on_Heighfield_clicked();

    void on_GraphicValues_clicked();

    //SLOTS DE ENTORNO GRAFICO
    void onGraph_GenEjesButton_press(bool State);
    void onGraph_GenFOGButton_press(bool State);
    void onGraph_ModEjesButton_press(int Modulos,bool State);
    void onGraph_modCADButton_press(int Modulos,bool State);
    void onGraph_ModContactPoints_press(int Modulos,bool State);
    void onGraph_ModSelector_Change(int Modulo);
    void onGraph_ModTrayectoria_Change(int Modulo,bool State);

    void on_actionLogFile_triggered(bool checked);

    void on_actionGenerador_de_ngulos_triggered();

    //SLOTS PLUGGINS

    void onAngleGenerationSucced(QString ruta);
private:
    Ui::Melosimula *ui;
    bool saved;
    simulador * simulator;
    ViewerWidget* visualizacion;
    GraphicParameters * gp;
    angleGenerator * ag_plugin;
};

#endif // MELOSIMULA_H
