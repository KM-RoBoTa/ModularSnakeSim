#ifndef HEIGHFIELD_H
#define HEIGHFIELD_H

#include <QDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QErrorMessage>
#include <QVector>

namespace Ui {
class Heighfield;
}

class Heighfield : public QDialog
{
    Q_OBJECT
    
public:
    explicit Heighfield(QWidget *parent = 0);
    ~Heighfield();
    QString getRutaHeighfield();
    QString getRutaTextura();
    QVector <double> getOrigen();
    QVector <double> getTamanno();
    double getHeighLimit();
    void setValues(QString rutaHF, QString rutaTextura, QVector <double> origen, QVector <double> tamanno, double escala);
    
private slots:
    void on_HeighfieldButton_clicked();

    void on_TexturaButton_clicked();

private:
    Ui::Heighfield *ui;
    QString rutaHeighfield;
    QString rutaTextura;

};

#endif // HEIGHFIELD_H
