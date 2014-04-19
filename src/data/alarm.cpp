#include "alarm.h"

Alarm::Alarm() : Data(){

}


int Alarm::getFanBlocked(){
    return fanBlocked.value;
}

void Alarm::setFanBlocked(int value){
    SetValue(&fanBlocked ,value);
}

void Alarm::setFanBlocked(bool value){
    this->setFanBlocked((int)value);
}

int Alarm::getLowWaterflow(){
    return lowWaterflow.value;
}

void Alarm::setLowWaterflow(int value){
    SetValue(&lowWaterflow ,value);
}

void Alarm::setLowWaterflow(bool value){
    this->setLowWaterflow((int)value);
}

int Alarm::getOverTemperatur(){
    return overTemperatur.value;
}

void Alarm::setOverTemperatur(int value){
    SetValue(&overTemperatur ,value);
}

void Alarm::setOverTemperatur(bool value){
    this->setOverTemperatur((int)value);
}

void Alarm::ProcessData(Carriage *car){
    if(car->getId() == 50){
        switch(car->getIndex()){
            case 1:
                this->SetValue(&overTemperatur,car->getData().toInt(),DATA_FROM_HW);
                break;
            case 2:
                this->SetValue(&fanBlocked,car->getData().toInt(),DATA_FROM_HW);
                break;
            case 3:
                this->SetValue(&lowWaterflow,car->getData().toInt(),DATA_FROM_HW);
                break;
        }
    }
}
