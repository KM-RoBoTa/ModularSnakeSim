#ifndef SNAKEPARAMETERS_H
#define SNAKEPARAMETERS_H

#include <QDialog>
#include <QVector>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include "Serpiente/modelos/modelo1.h"
#include "Serpiente/modelos/modelo2.h"
#include <QPixmap>

namespace Ui {
class SnakeParameters;
}

class SnakeParameters : public QDialog
{
    Q_OBJECT
    
public:
    explicit SnakeParameters(QWidget *parent = 0);
    ~SnakeParameters();
    QMap <QString,double> getParameters();
    void setParameters(QMap <QString,double> values);
    void setParametrosMotor(QMap <QString,double> values);
    QMap <QString,double> getParametersMotor();
    
private slots:
    void on_NextButton_clicked();

    void on_PrevButton_clicked();

private:
    Ui::SnakeParameters *ui;
    int actual;
    modelo1 * Mod1;
    modelo2 * Mod2;
    QVBoxLayout * cont;

};

#endif // SNAKEPARAMETERS_H
