#ifndef TEMPERATUR_H
#define TEMPERATUR_H

#include <QObject>
#include "data.h"

class Temperatur : public Data{
    Q_OBJECT

private:
    intValue wert    = {0,0,255, TYPE_VALUE, Carriage(0,30,0,0)};
    intValue spare   = {0,0,255, TYPE_SETUP, Carriage(0,31,0,0)};
    bool onewire = false;
    QByteArray onewireId;
public:
    int  getWert();
    int  getSpare();
    void setId(int id);
    QString getOneWireId();
    Temperatur();

public slots:
    void setWert(int value);
    void setSpare(int value);
    void ProcessData(Carriage *car); // Parant Member überladen
};

#endif // TEMPERATUR_H
