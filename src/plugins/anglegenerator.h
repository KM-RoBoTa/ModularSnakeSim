#ifndef ANGLEGENERATOR_H
#define ANGLEGENERATOR_H

#include <QDialog>

namespace Ui {
class angleGenerator;
}

class angleGenerator : public QDialog
{
    Q_OBJECT
    
public:
    explicit angleGenerator(QWidget *parent = 0);
    ~angleGenerator();

signals:
    void generationSucced( QString rutaArchivo);

private slots:
    void on_buttonBox_accepted();
private:
    Ui::angleGenerator *ui;
};

#endif // ANGLEGENERATOR_H
