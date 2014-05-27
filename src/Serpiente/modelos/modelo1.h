#ifndef MODELO1_H
#define MODELO1_H

#include <QWidget>
#include <QMap>
#include <QString>

namespace Ui {
class modelo1;
}

class modelo1 : public QWidget
{
    Q_OBJECT
    
public:
    explicit modelo1(QWidget *parent = 0);
    explicit modelo1(QMap <QString,double> values,QWidget *parent = 0);
    ~modelo1();
    void setValues(QMap <QString,double> values);
    QMap <QString,double> getValues();
    QString getRutaImagen();

private:
    Ui::modelo1 *ui;
    QString rutaImagen;
};

#endif // MODELO1_H
