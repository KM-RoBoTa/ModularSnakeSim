#ifndef GRAPHICPARAMETERS_H
#define GRAPHICPARAMETERS_H

#include <QDialog>
#include <QMap>
#include <QString>

namespace Ui {
class GraphicParameters;
}

class GraphicParameters : public QDialog
{
    Q_OBJECT
    
public:
    explicit GraphicParameters(QWidget *parent = 0);
    ~GraphicParameters();
    void setComboBoxList(int numModulos);
    void setButtonsStates(int Modulo, QMap<QString, bool> states);

public slots:
    void onGenEjesButton_press();
    void onGenFOGButton_press();
    void onModEjesButton_press();
    void onmodCADButton_press();
    void onModContactPoints_press();
    void onModSelector_Change(int Modulo);
    void onModTrayectoria_press();

signals:
    void Gen_EjesButton_press(bool state);
    void GenFOGButton_press(bool state);
    void ModEjesButton_press(int Modulo,bool state);
    void modCADButton_press(int Modulo,bool state);
    void ModContactPoints_press(int Modulo,bool state);
    void ModSelector_Change(int Modulo);
    void ModTrayectoriaButton_press(int Modulo,bool state);


private:
    Ui::GraphicParameters *ui;

};

#endif // GRAPHICPARAMETERS_H
