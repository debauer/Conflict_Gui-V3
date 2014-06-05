#ifndef STRUCTS_H
#define STRUCTS_H

#include "carriage.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QProgressBar>

#define DASHWIDGET_NONE 0
#define DASHWIDGET_BAR 1

struct intValue{
    int value;
    int min;
    int max;
    int type;
    Carriage car;

};

struct dashWidget{
    QGroupBox box;
    QProgressBar progressBar;
    int aktiv = 1;
    int value = 33;
    int type = DASHWIDGET_BAR;
};

#endif // STRUCTS_H
