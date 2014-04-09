#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtUiTools/QUiLoader>
#include <QDebug>
#include <QDir>

#include "ui_conflictwidget.h"
#include "conflictwidget.h"

//QList<ConflictCore> *core;
//ConflictCore *core;
PortSettings portSettings;
QList<QextPortInfo> serialPorts;
QextSerialPort *SerialPort;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    int i;

    ui->setupUi(this);

    /* Serial Menü*/
    serialPorts = QextSerialEnumerator::getPorts();
    signalMapper = new QSignalMapper (this);
    for(i = 0;i < serialPorts.size();i++){
        if((QString)serialPorts.at(i).portName != (QString)""){
            QAction *menueSerialPorts = new QAction((QString)serialPorts.at(i).portName.toLocal8Bit().constData() + QString(),this);
            ui->menuVerbinden->addAction(menueSerialPorts);
            QObject::connect (menueSerialPorts, SIGNAL(triggered()), signalMapper, SLOT(map())) ;
            signalMapper -> setMapping (menueSerialPorts, i);
        }
    }

    QObject::connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(newCoreSerial(int)));


    /* ETH Menü */

    /* sonstiges Connects */
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::connectCore(ConflictCore* core){
    QObject::connect (core, SIGNAL(debugOutput(QString)), this, SLOT(debugConsole(QString)));
    QObject::connect (core, SIGNAL(Changed(ConflictCore*,QString)), this, SLOT(updateGui(ConflictCore*,QString)));
    QObject::connect(ui->ledColorGreen, SIGNAL(sliderMoved(int)),&core->led, SLOT(setGreen(int)));
    QObject::connect(ui->ledColorRed, SIGNAL(valueChanged(int)),&core->led, SLOT(setRed(int)));
    QObject::connect(ui->ledColorBlue, SIGNAL(valueChanged(int)),&core->led, SLOT(setBlue(int)));
    QObject::connect(ui->ledModus, SIGNAL(currentIndexChanged(int)),&core->led, SLOT(setMode(int)));
    QObject::connect(&core->led,&Led::Changed,[=]() { ui->ledColorRed->setValue(core->led.getRed());
                                                      ui->ledColorGreen->setValue(core->led.getGreen());
                                                      ui->ledColorBlue->setValue(core->led.getBlue());
                                                      ui->ledModus->setCurrentIndex(core->led.getMode());
                                                    });
//    QObject::connect(&core->dfm,&Dfm::Changed,[=]() { //ui->optionenDfmImpulse->setValue(core->dfm.get);
//                                                    });
    QObject::connect(&core->kanal[0],&Kanal::Changed,[=]() {
                                                           });

}

void MainWindow::disconnectCore(ConflictCore* core){

    QObject::disconnect(core,0,this,0); // alle signals des Cores an mainwindow disconnecten

    QObject::disconnect(&core->led,0);
}

ConflictCore* MainWindow::newCore(){
     ConflictCore *core = new ConflictCore();
     this->connectCore(core);
     return core;
}\

void MainWindow::newCoreSerial(int serId){
     ConflictCore *core = this->newCore();
     core->connectSerial(serId);
     ConflictWidget *conflictWidget = new ConflictWidget();
     Ui::ConflictWidget *conflictUi = conflictWidget->getObject();
     //ui->Tabs->addTab(conflictWidget,QString(""));
}

void MainWindow::newCoreETH(QString ip){
     ConflictCore *core = this->newCore();
     core->connectETH(ip);
}

/* Haupt Update Worker */
void MainWindow::updateGui(ConflictCore* core,QString dataClass){
     if(dataClass == QString("led")){
        ui->console->append(QString("updateGui LED"));
     }else if (dataClass == QString("kanal1")){
        //ui->infoKanal1RPM->setText(QString::number(core->kanal[0].getRpm()));
     }else if (dataClass == QString("kanal2")){
        //ui->infoKanal2RPM->setText(QString::number(core->kanal[1].getRpm()));
     }else if (dataClass == QString("kanal3")){

        //ui->infoKanal3RPM->setText(QString::number(core->kanal[2].getRpm()));
     }
}

void MainWindow::updateTabKanaele(int kanal){

}

void  MainWindow::speicherKanal(){
    int index = ui->kanalAktiv->currentIndex();
    Kanal* k = &aktivCore->kanal[index];
    k->setAutoMode((int) ui->kanalAutoOn->isChecked());
    k->setManualPower(ui->KanalManuellPower->value());
    if(ui->KanalGrenzwertMaxAktiv){
        if(ui->kanalGrenzwert->currentIndex() == 0)
            k->setMaxTemp(ui->kanalGrenzwert->currentIndex()-1,ui->KanalGrenzwertMax->value());
        else
            k->setAllMaxTemp(ui->KanalGrenzwertMax->value());
    }
    if(ui->KanalGrenzwertMinAktiv){
        if(ui->kanalGrenzwert->currentIndex() == 0)
            k->setMinTemp(ui->kanalGrenzwert->currentIndex()-1,ui->KanalGrenzwertMin->value());
        else
            k->setAllMinTemp(ui->KanalGrenzwertMin->value());
    }
    k->setMinPower(ui->kanalAutoMinimal->value());
    k->setStartupTime(ui->KanalStartupTime->value());
    k->setStopEnabled((int) ui->kanalAutoCanStop->isChecked());
    k->setThreshold(ui->kanalAutoStart->value());
}


void MainWindow::debugConsole(int iihh){
    ui->debug->append(QString::number(iihh));
}

void MainWindow::debugConsole(Carriage *car){
     ui->console->append(car->toString());
}

void MainWindow::debugConsole(QString str){
     ui->console->append(str);
}

