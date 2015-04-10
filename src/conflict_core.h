#ifndef CONFLICT_CORE_H
#define CONFLICT_CORE_H
#include <qextserialport.h>
#include <qextserialenumerator.h>
#include <QSignalMapper>
#include <mainwindow.h>
#include <QSharedMemory>
#include <QBuffer>
#include <QDebug>
#include <QObject>
#include <QString>

#include "driver/absInterface.h"
#include "driver/uart.h"
#include "carriage.h"
#include "data/kanal.h"
#include "data/dfm.h"
#include "data/rechner.h"
#include "data/led.h"
#include "data/lcd.h"
#include "data/temperatur.h"
#include "data/alarm.h"
#include "config.h"

/*
 *
 * Notiz 4.4.14: Datenklassen find ich grade total bescheuert... eine generische Klasse hätte es vllt auch getan.
 * Wäre etwas komplexer, aber besser zu warten. Jetzt muss ich da durch!
 *
 * -----------------------
 *
 * updateXYZ()
 *  daten an HW schicken
 *
 * setXYZ()
 *  setze Object Daten von HW oder GUI, emitted: Changed();
 *  Idee ist das man an einer stelle den Wert ändert und er automatisch überall deployed wird.
 *  Ändert sich in einem Datenobjekt ein Value
 *  Bei falschem Casten kann das böse loops provozieren.
 *
 */


class ConflictCore : public QObject{
        Q_OBJECT
    private:
        AbsInterface *interface;
        bool interfaceOpen;
        void connectInterface(QString str);

        QSignalMapper *signalMapper;
        void makeMapping(Data* obj, QString str);

    public:
        ConflictCore();
        Rechner system;
        Led led;
        Lcd lcd;
        Dfm dfm;
        Config config;
        Alarm alarm;
        Kanal kanal[4];
        Temperatur temperatur[24];

        dashWidget dfmWidget;
        dashWidget cpuWidget;
        dashWidget ramWidget;
        dashWidget gpuWidget;
        dashWidget kanalWidget[4];
        dashWidget temperaturWidget[24];

        void connectSerial(int port);
        void connectETH(QString ip);
        void disconnect();
        void printDebug(QString);
        void rcvCarriage(Carriage *car); //
    public slots:
        void restartHW(); //
        void ChangedData(QString str); // Akkregator der Changed Signale von Daten Klassen. Emittet ebenfalls Changed()
        void sendCarriage(Carriage *car); //
        void initHW();
        void initDisplay();
    signals:
        void Changed(ConflictCore* core, QString str);
        void initComplete();
        void newCarriage(Carriage *car);
        void debugOutput(QString  str);
        void syncData();
        void updateDash();
};

#endif // CONFLICT_CORE_H
