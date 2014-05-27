#ifndef MODELO2_H
#define MODELO2_H

#include <QWidget>
#include <QMap>
#include <QString>
namespace Ui {
class modelo2;
}

class modelo2 : public QWidget
{
    Q_OBJECT
    
public:
    explicit modelo2(QWidget *parent = 0);
    explicit modelo2(QMap <QString,double> values,QWidget *parent = 0);
    ~modelo2();
    void setValues(QMap <QString,double>);
    QMap <QString,double> getValues();
    QString getRutaImagen();
    
private:
    Ui::modelo2 *ui;
    QString rutaImagen;
};

#endif // MODELO2_H
