#ifndef EDITODE_H
#define EDITODE_H

#include <QDialog>
#include <QMap>
#include <QString>


namespace Ui {
class EditODE;
}

class EditODE : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditODE(QWidget *parent = 0);
    ~EditODE();
    QMap <QString, double> getParameters();
    void setParameters(QMap <QString, double>);

private:
    Ui::EditODE *ui;
};

#endif // EDITODE_H
