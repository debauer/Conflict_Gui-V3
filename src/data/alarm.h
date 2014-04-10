#ifndef ALARM_H
#define ALARM_H

#include <QObject>
#include "data.h"

class alarm: public Data{
        Q_OBJECT
public:
        alarm();
        int getLowWaterflow();
        int getFanBlocked();
        int getOverTemperatur();
private:
        intValue overTemperatur;
        intValue fanBlocked;
        intValue lowWaterflow;
public slots:
        void setLowWaterflow(int value);
        void setFanBlocked(int value);
        void setOverTemperatur(int value);
        void ProcessData(Carriage *car); // Parant Member überladen
};

#endif // ALARM_H
