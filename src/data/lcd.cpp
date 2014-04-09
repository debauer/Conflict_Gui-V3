#include "lcd.h"

Lcd::Lcd() : Data(){
    int i;
    for(i = 0;i<256;i++){
        content[i].max = 255;
        content[i].min = 0;
        content[i].value = 0;
    }
    for(i = 0;i<32;i++){
        screens[i].max = 255;
        screens[i].min = 0;
        screens[i].value = 0;
    }
}

void Lcd::ProcessData(Carriage *car){
    switch(car->getId()){
        case 77:
            if(car->getIndex() == 1){
                this->SetValue(&backlight,car->getData().toInt());
            }else if(car->getIndex() == 2){
                this->SetValue(&contrast,car->getData().toInt());
            }
            break;
        case 80:
            this->SetValue(&content[car->getIndex()],car->getData().toInt());
            break;
        case 81:
            this->SetValue(&screens[car->getIndex()],car->getData().toInt());
            break;
    }
}

void Lcd::RequestData(){
   // emit PushToHw(new Carriage(1, 78, 1, 0));
}

int Lcd::getBacklight(){
    return backlight.value;
}

void Lcd::setBacklight(int value){
    this->SetValue(&backlight, value);
}

int Lcd::getContrast(){
    return contrast.value;
}

void Lcd::setContrast(int value){
    this->SetValue(&contrast, value);
}

int Lcd::getScreens(int id){
    return screens[id].value;
}

void Lcd::setScreens(int id, int value){
    this->SetValue(&screens[id], value);
}

int Lcd::getContent(int id){
    return content[id].value;
}

void Lcd::setContent(int id,int value){
    this->SetValue(&content[id], value);
}


