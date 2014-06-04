#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QDebug>
#include "../structs.h"
#include "../carriage.h"

/*
 *
 *  Von dieser Klasse wird jede Datenklasse abgeleitet werden.
 *  Parant Class of all data Classes
 *
 *  ProcessData(Carriage *car) muss in jedem Child überladen werden
 *  SetValue ist generisch, sollte nicht überladen werden und aufjedenfall statt "var.value" genutzt werden.
 *  SetValue prüft auf die min/max grenzen und setzt bei überschreitung den wert auf den min bzw max
 *
 *  Info Changed: 4.4.2014 David Bauer
 *
 */

/*
 * 04.06.2014
 * Einführung von valueChanged Typen. Einfluss auf Signals und Datenfluss.
 *
 * 0 - gangbang
 * 1 - setup Daten
 * 2 - Messwerte
 * 3 -
 *
 */

#define TYPE_VOID   0
#define TYPE_SETUP  1
#define TYPE_VALUE  2
#define TYPE_INFO   3
#define TYPE_SYSTEM 4
#define TYPE_ALARM 4


#define DATA_FROM_HW QString("fromHW")
#define DATA_FROM_GUI QString("fromGui")
#define DATA_DEFAULT QString("default")

class Data : public QObject{
    Q_OBJECT
protected:
    int wasChanged = 0; // wollte kam wärend der pausierung ein Signal
    int changedAktiv = 1;
    int id;     // Laufende nummer für Instanzen. Benötigt um signals auf ProcessData einer Instanz zuzuweisen.
public:
    void SetValue (intValue (*ptr), int value); // möglichst nicht überschreiben!
    void SetValue (intValue (*ptr), int value, QString option);
    void disableChanged();
    void enableChanged();
    void emitChanged();             // Wrapper um changed zu deaktivieren.
    void setId(int id);
    Data();
signals:
    void Changed();                 // Signal für GUI das sich das Modell geändert hat und die View geupdatet werden soll
    void HardwareChanged(int type);
    void GuiChanged(int type);
    void PushToHw(Carriage *car);   // Schieb den Carriage zur HW.
public slots:
    void RequestData();
    void ProcessData(Carriage *car);            // Überschreiben!
};

#endif // DATA_H
