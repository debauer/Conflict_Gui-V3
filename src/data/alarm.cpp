#include "alarm.h"

alarm::alarm() : Data(){

}


int alarm::getFanBlocked(){
    return fanBlocked.value;
}

void alarm::setFanBlocked(int value){
    SetValue(&fanBlocked ,value);
}

int alarm::getLowWaterflow(){
    return lowWaterflow.value;
}

void alarm::setLowWaterflow(int value){
    SetValue(&lowWaterflow ,value);
}

int alarm::getOverTemperatur(){
    return overTemperatur.value;
}

void alarm::setOverTemperatur(int value){
    SetValue(&overTemperatur ,value);
}

void alarm::ProcessData(Carriage *car){
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
