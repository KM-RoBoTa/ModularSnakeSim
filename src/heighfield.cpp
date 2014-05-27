#include "heighfield.h"
#include "ui_heighfield.h"

Heighfield::Heighfield(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Heighfield)
{
    ui->setupUi(this);
}

Heighfield::~Heighfield()
{
    delete ui;
}

QString Heighfield::getRutaHeighfield()
{
    return this->rutaHeighfield;
}

QString Heighfield::getRutaTextura()
{
    return this->rutaTextura;
}

QVector <double>Heighfield::getOrigen()
{
    QVector <double> rta;
    rta.push_back(this->ui->OrigenX->value());
    rta.push_back(this->ui->OrigenY->value());
    rta.push_back(this->ui->OrigenZ->value());
    return rta;
}

QVector <double> Heighfield::getTamanno()
{
    QVector <double> rta;
    rta.push_back(this->ui->Ancho->value());
    rta.push_back(this->ui->Alto->value());
    return rta;
}

double Heighfield::getHeighLimit()
{
    return this->ui->LimiteAltura->value();
}

void Heighfield::setValues(QString rutaHF, QString rutaTextura, QVector<double> origen, QVector<double> tamanno, double escala)
{
    this->ui->OrigenX->setValue(origen[0]);
    this->ui->OrigenY->setValue(origen[1]);
    this->ui->OrigenZ->setValue(origen[2]);
    this->ui->Alto->setValue(tamanno[0]);
    this->ui->Ancho->setValue(tamanno[1]);
    this->rutaHeighfield=rutaHF;
    this->rutaTextura=rutaTextura;
    this->ui->LimiteAltura->setValue(escala);
    if (this->rutaHeighfield!="")
    {
        QPixmap * hf = new QPixmap(this->rutaHeighfield);
        if (!hf->isNull())
        {
            this->ui->HeighfieldImage->setText("");
            this->ui->HeighfieldImage->setPixmap(*hf);
        }
        else
            this->rutaHeighfield="";
    }
    if (this->rutaTextura!="")
    {
        QPixmap * hf = new QPixmap(this->rutaTextura);
        if (!hf->isNull())
        {
            this->ui->TexturaImage->setText("");
            this->ui->TexturaImage->setPixmap(*hf);
        }
        else
            this->rutaTextura="";
    }
}

void Heighfield::on_HeighfieldButton_clicked()
{
   this->rutaHeighfield = QFileDialog::getOpenFileName(this, tr("Select Heighfield File"),"~",tr("Image Files (*.png *.jpg *.bmp)"));
    QPixmap * hf = new QPixmap(this->rutaHeighfield);

    if (!hf->isNull())
    {
        this->ui->HeighfieldImage->setText("");
        this->ui->HeighfieldImage->setPixmap(*hf);
    }
    else //SI no se puede abrir el archivo se genera un mensaje de error
    {
        QErrorMessage errorMess;
        errorMess.showMessage("Unable to read the file");
        errorMess.setModal(true);
        errorMess.exec();
    }
}

void Heighfield::on_TexturaButton_clicked()
{
     this->rutaTextura= QFileDialog::getOpenFileName(this, tr("Select Heighfield File"),"~",tr("Image Files (*.png *.jpg *.bmp)"));
     QPixmap * textura = new QPixmap(this->rutaTextura);

     if (!textura->isNull())
     {
         this->ui->TexturaImage->setText("");
         this->ui->TexturaImage->setPixmap(*textura);
     }
     else //SI no se puede abrir el archivo se genera un mensaje de error
     {
         QErrorMessage errorMess;
         errorMess.showMessage("Unable to read the file");
         errorMess.setModal(true);
         errorMess.exec();
     }
}
