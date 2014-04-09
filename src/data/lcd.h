#ifndef LCD_H
#define LCD_H

#include <QObject>
#include "data.h"

class Lcd : public Data{
    Q_OBJECT

    intValue backlight      = {0,0,100,Carriage(0,0,0,0)}; // Backlight helligkeit
    intValue contrast       = {0,0,100,Carriage(0,0,0,0)}; // Kontrast
    intValue content[256];  // im Konstruktor Initialiert // welcher Inhalt
    intValue screens[32];   // im Konstruktor Initialiert // Wie lange in 0,5s Steps

public:
    Lcd();
    void RequestData();
    void updateLCD(); // Contrast + Backlight
    void updateScreens();
    void updateContent();

    int  getBacklight();
    int  getContrast();
    int  getScreens(int id);
    int  getContent(int id);
signals:
    // Changed() von parant!
public slots:
    void setBacklight(int value);
    void setContrast(int value);
    void setScreens(int id, int value);
    void setContent(int id, int value);
    void ProcessData(Carriage *car); // Parant Member überladen
};

#endif // LCD_H
