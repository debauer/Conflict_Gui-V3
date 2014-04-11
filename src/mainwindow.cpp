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

    QObject::connect(ui->optionenAnzeigeBeleuchtung, SIGNAL(valueChanged(int)),&core->lcd, SLOT(setBacklight(int)));
    QObject::connect(ui->optionenAnzeigeKontrast, SIGNAL(valueChanged(int)),&core->lcd, SLOT(setContrast(int)));

    QObject::connect(ui->optionenDfmImpulse, SIGNAL(valueChanged(int)),&core->dfm, SLOT(setPulsePerLiter(int)));
    QObject::connect(ui->optionenAlarmMinDurchfluss, SIGNAL(valueChanged(int)),&core->dfm, SLOT(setMinFlow(int)));

    QObject::connect(&core->dfm,&Dfm::Changed,[=]() { ui->optionenDfmImpulse->setValue(core->dfm.getPulsePerLiter());
                                                      ui->optionenAlarmMinDurchfluss->setValue(core->dfm.getMinFlow());
                                                      // show
                                                    });
    QObject::connect(ui->optionenAlarmMinDurchfluss, SIGNAL(valueChanged(int)),&core->dfm, SLOT(setMinFlow(int)));
    QObject::connect(ui->kanalSpeichern,SIGNAL(clicked()),this, SLOT(speicherKanal()));
    QObject::connect(ui->kanalAktiv,SIGNAL(currentIndexChanged(int)),this, SLOT(updateTabKanaele(int)));
    QObject::connect(ui->kanalGrenzwert,SIGNAL(currentIndexChanged(int)),this, SLOT(updateTabKanaeleGrenzwert(int)));
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
     this->aktivCore = core;
     //ConflictWidget *conflictWidget = new ConflictWidget();
     //Ui::ConflictWidget *conflictUi = conflictWidget->getObject();
     //ui->Tabs->addTab(conflictWidget,QString(""));
}

void MainWindow::newCoreETH(QString ip){
     ConflictCore *core = this->newCore();
     core->connectETH(ip);
}

/* Haupt Update Worker */
void MainWindow::updateGui(ConflictCore* core,QString dataClass){
    if(core == aktivCore){
        int kanalId;
         if(dataClass == QString("led")){
            ui->console->append(QString("updateGui LED"));
         }else if (dataClass == QString("kanal1")){
             //qDebug() << "kanal1";
             this->updateTabKanaele(1);
            //ui->infoKanal1RPM->setText(QString::number(core->kanal[0].getRpm()));
         }else if (dataClass == QString("kanal2")){
             //qDebug() << "kanal2";
             this->updateTabKanaele(2);
            //ui->infoKanal2RPM->setText(QString::number(core->kanal[1].getRpm()));
         }else if (dataClass == QString("kanal3")){
             //qDebug() << "kanal3";
             this->updateTabKanaele(3);
            //ui->infoKanal3RPM->setText(QString::number(core->kanal[2].getRpm()));
         }else if (dataClass == QString("kanal4")){
             //qDebug() << "kanal4";
             this->updateTabKanaele(4);
            //ui->infoKanal3RPM->setText(QString::number(core->kanal[2].getRpm()));
         }
    }
}

void MainWindow::updateTabKanaele(int kanal){
    if(kanal == (ui->kanalAktiv->currentIndex() + 1)){
        qDebug() << "kanalTabUpdate" << ui->kanalAktiv->currentIndex() << kanal;
        Kanal* k = &aktivCore->kanal[ui->kanalAktiv->currentIndex()];
        ui->kanalAutoOn->setChecked((bool)k->getAutoMode());
        ui->kanalAutoCanStop->setChecked((bool)k->getStopEnabled());
        ui->kanalAutoMinimal->setValue(k->getMinPower());
        ui->kanalAutoStart->setValue(k->getStartupTime());
        ui->KanalManuellPower->setValue(k->getManualPower());
        ui->KanalStartupTime->setValue(k->getStartupTime());
        this->updateTabKanaeleGrenzwert(ui->kanalGrenzwert->currentIndex());
    }
}

void MainWindow::updateTabKanaeleGrenzwert(int kanal){
    Kanal* k = &aktivCore->kanal[ui->kanalAktiv->currentIndex()];
    if(ui->kanalGrenzwert->currentIndex() == 0){ // all
        ui->KanalGrenzwertMax->setValue(k->getMaxTemp(0));
        ui->KanalGrenzwertMin->setValue(k->getMinTemp(0));
    }else if(ui->kanalGrenzwert->currentIndex() <= 25 && ui->kanalGrenzwert->currentIndex() > 0){
        ui->KanalGrenzwertMax->setValue(k->getMaxTemp(ui->kanalGrenzwert->currentIndex() - 1));
        ui->KanalGrenzwertMin->setValue(k->getMinTemp(ui->kanalGrenzwert->currentIndex() - 1));
    }
}

void  MainWindow::speicherKanal(){
    int index = ui->kanalAktiv->currentIndex();
    Kanal* k = &aktivCore->kanal[index];
    k->disbaleChanged();
    k->setAutoMode(ui->kanalAutoOn->isChecked()?1:0);
    k->setManualPower(ui->KanalManuellPower->value());
    if(ui->KanalGrenzwertMaxAktiv->isChecked()){
        if(ui->KanalGrenzwertMax->value() > 240)
            ui->KanalGrenzwertMax->setValue(240);
        if(ui->kanalGrenzwert->currentIndex() != 0)
            k->setMaxTemp(ui->kanalGrenzwert->currentIndex()-1,ui->KanalGrenzwertMax->value());
        else
            k->setAllMaxTemp(ui->KanalGrenzwertMax->value());
    }else{
        ui->KanalGrenzwertMax->setValue(0);
        if(ui->kanalGrenzwert->currentIndex() != 0)
            k->setMaxTemp(ui->kanalGrenzwert->currentIndex()-1,255);
        else
            k->setAllMaxTemp(255);
    }
    if(ui->KanalGrenzwertMinAktiv->isChecked()){
        if(ui->KanalGrenzwertMin->value() > 240)
            ui->KanalGrenzwertMin->setValue(240);
        if(ui->kanalGrenzwert->currentIndex() != 0)
            k->setMinTemp(ui->kanalGrenzwert->currentIndex()-1,ui->KanalGrenzwertMin->value());
        else
            k->setAllMinTemp(ui->KanalGrenzwertMin->value());
    }else{
        ui->KanalGrenzwertMin->setValue(0);
        if(ui->kanalGrenzwert->currentIndex() != 0)
            k->setMinTemp(ui->kanalGrenzwert->currentIndex()-1,250);
        else
            k->setAllMinTemp(250);
    }
    k->setMinPower(ui->kanalAutoMinimal->value());
    k->setStartupTime(ui->KanalStartupTime->value());
    k->setStopEnabled((int) ui->kanalAutoCanStop->isChecked());
    k->setThreshold(ui->kanalAutoStart->value());
    k->enableChanged();
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

