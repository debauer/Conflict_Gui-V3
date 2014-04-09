#ifndef ABSINTERFACE_H
#define ABSINTERFACE_H

#include <QString>
#include "../conflict_core.h"

// Virtuelle klasse die alle Interfaces beschreibt.
// UART, ETH, CAN...

class AbsInterface{
public:
    virtual bool Open(QString adresse) = 0;
    virtual bool Close() = 0;
    virtual bool IsOpen() = 0;
    virtual void SendString(QString str) = 0; // Sendet einfach den String mit <CR> (0x0D, 0d13) terminierung.
    virtual ~AbsInterface(){};
};

#endif // ABSINTERFACE_H
