#ifndef STRUCTS_H
#define STRUCTS_H

#include "carriage.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QProgressBar>
#include <QLCDNumber>

#define DASHWIDGET_NONE 0
#define DASHWIDGET_BAR 1

struct intValue{
    int value;
    int min;
    int max;
    int type;
    Carriage car;
};

struct doubleValue{
    double value;
    double min;
    double max;
    double type;
    Carriage car;
};

struct dashWidget{
    QGroupBox *box;
    QProgressBar *progressBar;
    QLCDNumber *lcdnumber;
    int aktiv = 1;
    int value = 33;
    int ok = 33;
    int warn = 66;
    int error = 100;
    QString name;
    int type = DASHWIDGET_BAR;
};

#endif // STRUCTS_H
