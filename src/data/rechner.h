#ifndef RECHNER_H
#define RECHNER_H

#include <QObject>
#include "data.h"

class Rechner : public Data{
    Q_OBJECT

private:
    intValue hour       = {0,0,23,  Carriage(0,40,1,0)};
    intValue minute     = {0,0,59,  Carriage(0,40,2,0)};
    intValue secund     = {0,0,59,  Carriage(0,40,3,0)};
    intValue cpuFreq    = {0,0,9999,Carriage(0,41,1,0)};
    intValue cpuUsage   = {0,0,100, Carriage(0,41,2,0)};
    intValue gpuUsage   = {0,0,100, Carriage(0,41,3,0)};
    intValue ramUsage   = {0,0,100, Carriage(0,41,4,0)};

public:
    Rechner();
    void RequestData();
    void UpdateClock();         // Uhrzeit
    void UpdateSysData();       // CPU Freq, CPU/RAM/GPU usage

signals:
    // Changed() von parant!
public slots:
    void SetCpuFreq(int d);
    void SetCpuUsage(int d);
    void SetGpuUsage(int d);
    void SetRamUsage(int d);
    void ProcessData(Carriage *car); // Parant Member überladen

};

#endif // RECHNER_H
