#include "temperatur.h"

Temperatur::Temperatur() : Data(){
    static int maxId = 0;
    this->setId(maxId + 1);
    maxId = this->id;
    if(this->id >= 8 && this->id <= 16){  // One Wire!
        this->onewire = true;
    }
    onewireId = QString("00000000").toLocal8Bit();
}

void Temperatur::ProcessData(Carriage *car){
    if(car->getIndex() == this->id){
        switch(car->getId()){
            case 30:
                this->SetValue(&wert,car->getData().toDouble()/2,DATA_FROM_HW);
                break;
            case 31:
                this->SetValue(&spare,car->getData().toInt(),DATA_FROM_HW);
                break;
        }
    }
}

void Temperatur::setId(int id){
    if(id >= 1 && id <= 24){
        this->id = id;
        wert.car.setIndex(id);
        spare.car.setIndex(id);
    }else{
        qDebug() << "falsche Temperatur ID" << id;
    }
}

void Temperatur::setSpare(int value){
    this->SetValue(&spare, value);
}

int Temperatur::getSpare(){
    return spare.value;
}

void Temperatur::setWert(double value){
    this->SetValue(&wert, value);
}

void Temperatur::setWert(int value){
    this->SetValue(&wert, (double)value);
}

int Temperatur::getWert(){
    return wert.value;
}

QString Temperatur::getOneWireId(){
    return QString(onewireId);
}
