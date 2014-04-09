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
        intValue waterFlow;     // Aktueller Durchfluss
        intValue pulsePerLiter; // Impulswertigkeit
        intValue minFlow;       // Alarmgrenze
    public:
        void RequestData();
        Dfm();
    signals:
        // Changed() von parant!
    public slots:
        void ProcessData(Carriage *car); // Parant Member überladen

};

#endif // DFM_H
