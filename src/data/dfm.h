#ifndef DFM_H
#define DFM_H

#include <QObject>
#include "data.h"

/*
 *
 *  child of Data
 *  Objekt für einen DFM (Durchflussmesser)
 *
 */

class Dfm : public Data{
    Q_OBJECT

    private:
        intValue waterFlow      = {0,0,999, Carriage(1,60,1,0)};   // Aktueller Durchfluss
        intValue pulsePerLiter  = {0,0,9999,Carriage(1,70,1,0)};   // Impulswertigkeit
        intValue minFlow        = {0,0,999, Carriage(1,70,2,0)};   // Alarmgrenze
    public:
        void RequestData();
        Dfm();
        int getWaterFlow();
        int getPulsePerLiter();
        int getMinFlow();

signals:
        // Changed() von parant!
public slots:
        void setWaterFlow(int value);
        void setPulsePerLiter(int value);
        void setMinFlow(int value);
        void ProcessData(Carriage *car); // Parant Member überladen

};

#endif // DFM_H
