#ifndef ALARM_H
#define ALARM_H

#include <QObject>
#include "data.h"

class Alarm: public Data{
        Q_OBJECT
public:
        Alarm();
        int getLowWaterflow();
        int getFanBlocked();
        int getOverTemperatur();
private:
        intValue overTemperatur         = {0,0,1,TYPE_ALARM,Carriage(0,50,1,0)};
        intValue fanBlocked             = {0,0,1,TYPE_ALARM,Carriage(0,50,2,0)};
        intValue lowWaterflow           = {0,0,1,TYPE_ALARM,Carriage(0,50,3,0)};
        intValue enableOverTemperatur   = {0,0,1,TYPE_SETUP,Carriage(0,51,1,0)};
        intValue enableFanBlocked       = {0,0,1,TYPE_SETUP,Carriage(0,51,2,0)};
public slots:
        void setLowWaterflow(int value);
        void setFanBlocked(int value);
        void setOverTemperatur(int value);
        void setLowWaterflow(bool value);
        void setFanBlocked(bool value);
        void setOverTemperatur(bool value);
        void ProcessData(Carriage *car); // Parant Member überladen
};

#endif // ALARM_H
