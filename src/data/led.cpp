#include "led.h"

void Led::addMap(intValue* v){
    map[v->car.toString()] = v;
}

Led::Led() : Data(){
    addMap(&blue);
    addMap(&red);
    addMap(&green);
    addMap(&mode);
    //list.append(QList<intvalue>({&red, &green,&blue,&mode}));
}

void Led::ProcessData(Carriage *car){
//    if(map.contains(car->toString())){
//        intValue *iv = map[car->toString()];
//        iv->car.set(car->toString());
//        map[car->toString()] = iv;
//        qDebug() << "map funzt";
//    }

    switch(car->getId()){
        case 78:
            this->SetValue(&mode,car->getData().toInt());
            break;
        case 79:
            if(car->getIndex() == 1){
                this->SetValue(&red,car->getData().toInt());
            }else if(car->getIndex() == 2){
                this->SetValue(&green,car->getData().toInt());
            }else if(car->getIndex() == 3){
                this->SetValue(&blue,car->getData().toInt());
            }
            break;
        default:
            break;
    }
}

int Led::getRed(){
    return red.value;
}

void Led::setRed(int value){
    this->SetValue(&red, value);
}

int Led::getBlue(){
    return blue.value;
}

void Led::setBlue(int value){
    this->SetValue(&blue, value);
}

int Led::getGreen(){
    return green.value;
}

void Led::setGreen(int value){
    this->SetValue(&green, value);
}

int Led::getMode(){
    return mode.value;
}

void Led::setMode(int value){
    this->SetValue(&mode, value);
}

