#include "data.h"

Data::Data(){
}

void Data::ProcessData(Carriage *car){

    // ===========================
    // Wird in Childs implementiert
    // ===========================

}

void Data::RequestData(){

    // ===========================
    // Wird in Childs implementiert
    // ===========================

}

void Data::setId(int id){

    // ===========================
    // id setzen falls es mehrere Objekte gibt.
    // ===========================

    this->id = id;
}

void Data::SetValue (intValue *ptr, int value){

    // ===========================
    // Min/Max Grenzen überprüfen.
    // ===========================

    int newValue;
    if(value <= ptr->max && value >= ptr->min){
        // Value im erlaubten Bereich
        newValue = value;
    }else if(value > ptr->max){
        // value zu groß, setze auf max
        newValue = ptr->max;
    }else if(value < ptr->min){
        // value zu klein, setze auf min
        newValue = ptr->min;
    }else{
        // else halt... wer weis. Setze auf max.
        newValue = ptr->max;
    }

    if(newValue != ptr->value){
        ptr->car.setData(newValue);
        ptr->value = newValue;

        emit this->Changed(); // Schreien wir mal alles an!
        emit PushToHw(new Carriage(0,ptr->car.getId(), ptr->car.getIndex(), ptr->car.getData())); // eigentlich sollte emit PushToHw(ptr->car) ja reichen... aber funzt ned.
    }
}
