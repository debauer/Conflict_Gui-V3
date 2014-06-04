#ifndef LED_H
#define LED_H

#include "data.h"

class Led : public Data{
    Q_OBJECT

    QHash<QString, intValue*> map;
    intValue red        = {0,0,100,TYPE_SETUP,Carriage(1,79,1,0)};
    intValue green      = {0,0,100,TYPE_SETUP,Carriage(1,79,2,0)};
    intValue blue       = {0,0,100,TYPE_SETUP,Carriage(1,79,3,0)};
    intValue mode       = {0,0,255,TYPE_SETUP,Carriage(1,78,1,0)};

public:
    Led();
    void addMap(intValue* v);
    int getRed();
    int getBlue();
    int getGreen();
    int getMode();

signals:
    // Changed() von parant!
public slots:
    void setMode(int value);
    void setGreen(int value);
    void setBlue(int value);
    void setRed(int value);
    void ProcessData(Carriage *car); // Parant Member überladen

};

#endif // LED_H
