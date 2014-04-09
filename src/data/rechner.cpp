#include "rechner.h"

Rechner::Rechner() : Data(){

}

void Rechner::ProcessData(Carriage *car){
    switch(car->getId()){
        case 40:
            if(car->getIndex() == 1){
                this->SetValue(&hour,car->getData().toInt());
            }else if(car->getIndex() == 2){
                this->SetValue(&minute,car->getData().toInt());
            }else if(car->getIndex() == 3){
                this->SetValue(&secund,car->getData().toInt());
            }
            break;
        case 41:
            if(car->getIndex() == 1){
                this->SetValue(&cpuFreq,car->getData().toInt());
            }else if(car->getIndex() == 2){
                this->SetValue(&cpuUsage,car->getData().toInt());
            }else if(car->getIndex() == 3){
                this->SetValue(&gpuUsage,car->getData().toInt());
            }else if(car->getIndex() == 4){
                this->SetValue(&ramUsage,car->getData().toInt());
            }
            break;
    }
}

void Rechner::RequestData(){

}

void Rechner::SetCpuFreq(int d){
    this->SetValue(&cpuFreq,d);
}

void Rechner::SetCpuUsage(int d){
    this->SetValue(&cpuUsage,d);
}

void Rechner::SetGpuUsage(int d){
    this->SetValue(&gpuUsage,d);
}

void Rechner::SetRamUsage(int d){
    this->SetValue(&ramUsage,d);
}
