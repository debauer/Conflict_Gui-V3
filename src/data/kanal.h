#ifndef KANAL_H
#define KANAL_H

#include <QObject>
#include "data.h"

/*
 *
 *  child of Data
 *  Objekt für einen DFM (Durchflussmesser)
 *
 */

class Kanal : public Data{
    Q_OBJECT

    private:

        intValue minTemp[24];   // im Konstruktor Initialiert
        intValue maxTemp[24];   // im Konstruktor Initialiert
        intValue rpm            = {0,0,9999,Carriage(0,0,0,0)};
        intValue power          = {0,0,100,Carriage(0,0,0,0)};
        intValue manualPower    = {0,0,100,Carriage(0,0,0,0)};
        intValue startupTime    = {0,1,255,Carriage(0,0,0,0)};
        intValue minPower       = {0,0,100,Carriage(0,0,0,0)};
        intValue autoMode       = {0,0,1,Carriage(0,0,0,0)};
        intValue stopEnabled    = {0,0,1,Carriage(0,0,0,0)};
        intValue threshold      = {0,0,50,Carriage(0,0,0,0)};
    public:
        Kanal();
        void RequestData();
        int getManualPower();
        void setManualPower(int value);
        int getStartupTime();
        void setStartupTime(int value);
        int getMinPower() ;
        void setMinPower( int value);
        int getAutoMode() ;
        void setAutoMode( int value);
        int getStopEnabled() ;
        void setStopEnabled( int value);
        int getThreshold() ;
        void setThreshold( int value);
        int getPower    () ;
        void setPower   ( int value);
        int getRpm      () ;
        void setRpm     ( int value);
        int getMaxTemp  (int tId) ;
        void setMaxTemp (int tId , int value);
        void setAllMaxTemp (int value);
        int getMinTemp  (int tId) ;
        void setMinTemp (int tId , int value);
        void setAllMinTemp (int value);
signals:
    // Changed() von parant!
public slots:
        void ProcessData(Carriage *car); // Parant Member überladen
};

#endif // KANAL_H
