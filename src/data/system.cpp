#include "system.h"

System::System(){

}

void System::SetCpuFreq(int d){
    this->SetValue(&cpuFreq,d);
}

void System::SetCpuUsage(int d){
    this->SetValue(&cpuUsage,d);
}

void System::SetGpuUsage(int d){
    this->SetValue(&gpuUsage,d);
}

void System::SetRamUsage(int d){
    this->SetValue(&ramUsage,d);
}

void System::ProcessData(Carriage *car){
}
