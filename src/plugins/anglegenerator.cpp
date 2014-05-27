#include "anglegenerator.h"
#include "ui_anglegenerator.h"
#include <QFile>
#include <QFileDialog>
#include <cmath>
#include <QTextStream>

angleGenerator::angleGenerator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::angleGenerator)
{
    ui->setupUi(this);
}

angleGenerator::~angleGenerator()
{
    delete ui;
}

void angleGenerator::on_buttonBox_accepted()
{
    QString NombreArchivo;
    NombreArchivo = QFileDialog::getSaveFileName(this, tr("Save as..."),"../");
    QFile file(NombreArchivo);

    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream escritor(&file);

        QString linea="16\t"+QString::number(this->ui->step->value())+"\tr";
        escritor<<linea<<endl;

        for (double i=0;i<this->ui->tiempo->value();i=i+this->ui->step->value())
        {
            linea = "";
            for (int j=1;j<17;j++)
            {
                if (j%2==0)
                {
                    linea+=QString::number(this->ui->Oe->value()+this->ui->Ae->value()*(sin(((double)j*this->ui->Le->value())+(this->ui->We->value()*i))));
                } else {
                    linea+=QString::number(this->ui->Oo->value()+this->ui->Ao->value()*(sin(((double)j*this->ui->Lo->value())+(this->ui->Wo->value()*i)+this->ui->d->value())));
                }
                if (j!=16)
                    linea += "\t";
            }
            escritor<<linea<<endl;
        }
        escritor<<endl;
        file.flush();
        file.close();
        emit generationSucced(NombreArchivo);
    }
}

