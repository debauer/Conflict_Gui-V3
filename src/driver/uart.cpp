#include "uart.h"

Uart::Uart(ConflictCore *core){
    this->core = core;
}

Uart::~Uart(){

}

bool Uart::Open(QString adresse){

    // Default Settings für Conflict
    serial.settings.BaudRate = BAUD57600;
    serial.settings.DataBits = DATA_8;
    serial.settings.Parity = PAR_NONE;
    serial.settings.StopBits = STOP_1;
    serial.settings.FlowControl = FLOW_OFF;
    serial.settings.Timeout_Millisec = 0;

    // Portliste abrufen
    serial.portlist = QextSerialEnumerator::getPorts();

    // Port öffnem
    serial.port = new QextSerialPort(serial.portlist.at(adresse.toInt()).portName,serial.settings);

    // Prüfen ob geöffnet
    if(serial.port->open(QextSerialPort::ReadWrite)){
        timer = new QTimer();
        timer->start(timerInterval);
        QObject::connect(timer, SIGNAL(timeout()),this,SLOT(sendData()));
        QObject::connect(serial.port, SIGNAL(readyRead()),this,SLOT(RcvData()));
        serial.portNr = adresse.toInt();
        serial.open = true;
        return true;
    }else{
        return false;
    }
}

bool Uart::Close(){
    if(serial.open == true){
        QObject::disconnect(timer, SIGNAL(timeout()),this,SLOT(sendData()));
        QObject::disconnect(serial.port, SIGNAL(readyRead()),this,SLOT(RcvData()));
        serial.port->close();
        serial.portNr = 0;
        serial.open = false;
        return true;
    }else{
        return false;
    }
}

void Uart::RcvData(){
        static QString telegram;
        char data;
        char *dataPtr;
        dataPtr = &data;
        int read;  // Anzahl gelesener Bytes
        while(serial.port->bytesAvailable()){
            read = serial.port->read(dataPtr,1);
            if(read > 0){
                telegram.append(QString(*dataPtr));
            }
            if(*dataPtr == 0x0D){ // <CR> empfangen
                Carriage *car = new Carriage(telegram);
                core->rcvCarriage(car);
                telegram.clear();
            }
        }
    }

bool Uart::IsOpen(){
    return serial.open;
}

void Uart::SendString(QString str){
    if(this->IsOpen()){

        core->printDebug(QString("String") + str.toLatin1() + QString(" Länge ") + QString::number(str.length()));
        //serial.port->write(str.toLatin1());
        txStack.push(str);
        //qDebug() << gesendet << "  " << (str.remove(0x0D).toLatin1() + QString(" Länge ") + QString::number(str.length()));
    }
}

void Uart::sendData(){
    if(!txStack.isEmpty()){
        gesendet++;
        QString str = txStack.pop();
        serial.port->write(str.toLatin1());
        qDebug() << gesendet << (str.remove(0x0D).toLatin1());
    }
    //qDebug() << "timer goil!";
    //timer->start(timerInterval);
}
