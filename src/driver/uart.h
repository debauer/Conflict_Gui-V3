#ifndef UART_H
#define UART_H

#include <QTimer>
#include <QStack>

#include "conflict_core.h"

#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "absInterface.h"

class ConflictCore;

struct serialStruct{
    QextSerialPort *port;
    QList<QextPortInfo> portlist;
    PortSettings settings;
    int portNr;
    bool open;
};

class Uart : public QObject, public AbsInterface{
        Q_OBJECT
    private:
        ConflictCore *core;
        serialStruct serial;
        QTimer *timer;
        QStack<QString> txStack;
        int timerInterval = 10;
        int gesendet = 0;
    public slots:
        void RcvData();
        void sendData(); // timer event
    public:
        Uart (ConflictCore *core);
        ~Uart();

        virtual bool Open(QString adresse);
        virtual bool Close();
        virtual bool IsOpen();
        virtual void SendString(QString str); //

        //void setPortSettings(PortSettings settings);
        //PortSettings getPortSettings();
};

#endif // UART_H
