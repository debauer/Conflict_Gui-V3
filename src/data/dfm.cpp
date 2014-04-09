#include "dfm.h"


Dfm::Dfm()  : Data(){

}

void Dfm::ProcessData(Carriage *car){
    // Prüfen ob es ein Carriage für diese Klasse ist
    if(car->getId() == 70 || car->getId() == 60){
        int id = car->getId();
        int dir = car->getDirection();
        int index = car->getIndex();
        int data = car->getData().toInt();
        switch(id){
            case 60: this->SetValue(&this->waterFlow,data); break;
            case 70: if(id == 70 && index == 1){
                        this->SetValue(&this->pulsePerLiter,data);
                     }else if(id == 70 && index == 1){
                        this->SetValue(&this->minFlow,data);
                     }
            break;
        }
    }
}

void Dfm::RequestData(){
    // emit PushToHw(new Carriage(1, 78, 1, 0));
}
