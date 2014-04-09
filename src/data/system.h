#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include "data.h"


class System : public Data{
    Q_OBJECT

private:
//    intValue cpuFreq    = {0,0,9999};
//    intValue cpuUsage   = {0,0,100};
//    intValue gpuUsage   = {0,0,100};
//    intValue ramUsage   = {0,0,100};
public:
    System();

//    void UpdateClock();         // Uhrzeit
//    void UpdateSysData();       // CPU Freq, CPU/RAM/GPU usage
//    void SetCpuFreq(int d);
//    void SetCpuUsage(int d);
//    void SetGpuUsage(int d);
//    void SetRamUsage(int d);

public slots:
    void ProcessData(Carriage *car);

};

#endif // SYSTEM_H
