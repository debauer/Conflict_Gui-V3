#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtUiTools/QUiLoader>
#include <QDebug>
#include <QDir>

//QList<ConflictCore> *core;
//ConflictCore *core;
PortSettings portSettings;
QList<QextPortInfo> serialPorts;
QextSerialPort *SerialPort;
Config config;
int init = false;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    int i,j,k;
    guiStatus = STATUS_NO_CONNECTION;
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

    QObject::connect (ui->actionTrennen, SIGNAL(triggered()), this, SLOT(dropCoreSerial()));
    QObject::connect (ui->actionBeenden, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect (ui->actionNeustart, SIGNAL(triggered()), this, SLOT(conflictRestart()));
    QObject::connect (ui->actionInitialisierung, SIGNAL(triggered()), this, SLOT(conflictInit()));

    /* ETH Menü */

    /* sonstiges Connects */

    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine );
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Minuten");
    ui->customPlot->yAxis->setLabel("C°");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(1, 0);
    ui->customPlot->yAxis->setRange(0, 1);
    ui->customPlot->replot();
    this->drawDashboard();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::connectCore(ConflictCore* core){
    int i;
    // CORE -> GUI Connects

    QObject::connect (core, SIGNAL(debugOutput(QString)), this, SLOT(debugConsole(QString)));

    //QObject::connect (core, SIGNAL(Changed(ConflictCore*,QString)), this, SLOT(updateGui(ConflictCore*,QString)));

    QObject::connect (core, SIGNAL(initComplete()), this, SLOT(initComplete()));

    QObject::connect(&core->led,&Led::HardwareChanged,[=](int type){
        // Immer TYPE_SETUP
        ui->ledColorRed->setValue(core->led.getRed());
        ui->ledColorGreen->setValue(core->led.getGreen());
        ui->ledColorBlue->setValue(core->led.getBlue());
        ui->ledModus->setCurrentIndex(core->led.getMode());
    });

    QObject::connect(&core->lcd,&Lcd::HardwareChanged,[=](int type){
        // Immer TYPE_SETUP
        ui->optionenAnzeigeBeleuchtung->setValue(core->lcd.getBacklight());
        ui->optionenAnzeigeKontrast->setValue(core->lcd.getContrast());
        //ui->optionenAnzeige->setValue(core->lcd.getContrast());
    });

    QObject::connect(&core->alarm,&Alarm::HardwareChanged,[=](int type){
        // Immer TYPE_ALARM
        //ui->optionenAlarmDurchfluss->setChecked((bool)core->alarm.getLowWaterflow());
        ui->optionenAlarmLuefter->setChecked((bool)core->alarm.getFanBlocked());
        ui->optionenAlarmTemperatur->setChecked((bool)core->alarm.getOverTemperatur());
    });
    QObject::connect(&core->dfm,&Dfm::HardwareChanged,[=](int type){
        if(type == TYPE_VALUE){
            dashWidgets[3].progressBar.setValue(core->dfm.getWaterFlow());
        }else if(type == TYPE_SETUP){
            ui->optionenDfmImpulse->setValue(core->dfm.getPulsePerLiter());
            ui->optionenAlarmMinDurchfluss->setValue(core->dfm.getMinFlow());
        }
    });
    for(i=0;i<3;i++){
        QObject::connect(&core->kanal[i],&Kanal::HardwareChanged,[=](int type){
            if(type == TYPE_VALUE){
                dashWidgets[i].progressBar.setValue(core->kanal[i].getRpm());
            }else if(type == TYPE_SETUP){
            }
        });
    }

    for(i=0;i<24;i++){
        QObject::connect(&core->temperatur[i],&Temperatur::HardwareChanged,[=](int type){
            if(type == TYPE_VALUE){
                dashWidgets[i+4].progressBar.setValue(core->temperatur[i].getWert());
            }else if(type == TYPE_SETUP){
                ui->oneWireId1->setText(core->temperatur[0].getOneWireId());
                ui->oneWireId2->setText(core->temperatur[1].getOneWireId());
                ui->oneWireId3->setText(core->temperatur[2].getOneWireId());
                ui->oneWireId4->setText(core->temperatur[3].getOneWireId());
                ui->oneWireId5->setText(core->temperatur[4].getOneWireId());
                ui->oneWireId6->setText(core->temperatur[5].getOneWireId());
                ui->oneWireId7->setText(core->temperatur[6].getOneWireId());
                ui->oneWireId8->setText(core->temperatur[7].getOneWireId());
            }
        });
    }


    // GUI -> Core Connects

    QObject::connect(ui->ledColorGreen, SIGNAL(sliderMoved(int)),&core->led, SLOT(setGreen(int)));
    QObject::connect(ui->ledColorRed, SIGNAL(valueChanged(int)),&core->led, SLOT(setRed(int)));
    QObject::connect(ui->ledColorBlue, SIGNAL(valueChanged(int)),&core->led, SLOT(setBlue(int)));
    QObject::connect(ui->ledModus, SIGNAL(currentIndexChanged(int)),&core->led, SLOT(setMode(int)));

    QObject::connect(ui->optionenAnzeigeBeleuchtung, SIGNAL(valueChanged(int)),&core->lcd, SLOT(setBacklight(int)));
    QObject::connect(ui->optionenAnzeigeKontrast, SIGNAL(valueChanged(int)),&core->lcd, SLOT(setContrast(int)));

    QObject::connect(ui->optionenDfmImpulse, SIGNAL(valueChanged(int)),&core->dfm, SLOT(setPulsePerLiter(int)));

    QObject::connect(ui->optionenAlarmMinDurchfluss, SIGNAL(valueChanged(int)),&core->dfm, SLOT(setMinFlow(int)));
    QObject::connect(ui->optionenAlarmLuefter, SIGNAL(clicked(bool)),&core->alarm, SLOT(setFanBlocked(bool)));
    QObject::connect(ui->optionenAlarmTemperatur, SIGNAL(clicked(bool)),&core->alarm, SLOT(setOverTemperatur(bool)));
    //QObject::connect(ui->optionenAlarmDurchfluss, SIGNAL(clicked(bool)),&core->alarm, SLOT(setLowWaterflow(bool)));

    QObject::connect(ui->kanalSpeichern,SIGNAL(clicked()),this, SLOT(speicherKanal()));
    QObject::connect(ui->kanalAktiv,SIGNAL(currentIndexChanged(int)),this, SLOT(updateTabKanaele(int)));
    QObject::connect(ui->kanalGrenzwert,SIGNAL(currentIndexChanged(int)),this, SLOT(updateTabKanaeleGrenzwert(int)));

    ui->menuVerbinden->setEnabled(false);
    ui->actionNeustart->setEnabled(true);
    ui->actionTrennen->setEnabled(false);
    //ui->actionSpeichern->setEnabled(true);
    ui->actionInitialisierung->setEnabled(true);

}

void MainWindow::dropCoreSerial(){
    disconnectCore(aktivCore);
    delete aktivCore;
    this->aktivCore = NULL;
    ui->menuVerbinden->setEnabled(true);
    ui->actionNeustart->setEnabled(false);
    ui->actionTrennen->setEnabled(false);
    //ui->actionSpeichern->setEnabled(false);
    ui->actionInitialisierung->setEnabled(false);
}

void MainWindow::conflictInit(){
    aktivCore->initHW();
}

void MainWindow::conflictRestart(){
    aktivCore->restartHW();
}


void MainWindow::disconnectCore(ConflictCore* core){
    guiStatus = STATUS_NO_CONNECTION;
    QObject::disconnect(core,0,this,0); // alle signals des Cores an mainwindow disconnecten
    QObject::disconnect(&core->led,0);
}

ConflictCore* MainWindow::newCore(){
     ConflictCore *core = new ConflictCore();
     this->connectCore(core);
     return core;
}\

void MainWindow::newCoreSerial(int serId){
     guiStatus = STATUS_INIT;
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

void MainWindow::initComplete(){
     guiStatus = STATUS_OK;
     qDebug() << "INIT COMPLETE";
     this->updateTabKanaele();
}

/* Haupt Update Worker */
void MainWindow::updateGui(ConflictCore* core,QString dataClass){
    if(core == aktivCore){
        int kanalId;
         if(dataClass == QString("led")){
            ui->console->append(QString("updateGui LED"));
         }else if (dataClass == QString("kanal1")){
             //qDebug() << "kanal1";
             //this->updateTabKanaele(0);
            //ui->infoKanal1RPM->setText(QString::number(core->kanal[0].getRpm()));
         }else if (dataClass == QString("kanal2")){
             //qDebug() << "kanal2";
             //this->updateTabKanaele(1);
            //ui->infoKanal2RPM->setText(QString::number(core->kanal[1].getRpm()));
         }else if (dataClass == QString("kanal3")){
             //qDebug() << "kanal3";
             //this->updateTabKanaele(2);
            //ui->infoKanal3RPM->setText(QString::number(core->kanal[2].getRpm()));
         }else if (dataClass == QString("kanal4")){
             //qDebug() << "kanal4";
             //this->updateTabKanaele(3);
            //ui->infoKanal3RPM->setText(QString::number(core->kanal[2].getRpm()));
         }
    }
}

void MainWindow::updateTabKanaele(){
    this->updateTabKanaele(0);
    this->updateTabKanaele(1);
    this->updateTabKanaele(2);
    this->updateTabKanaele(3);
}

void MainWindow::updateTabKanaele(int kanal){
    if(kanal == (ui->kanalAktiv->currentIndex())){
        qDebug() << "kanalTabUpdate" << ui->kanalAktiv->currentIndex() << kanal;
        Kanal* k = &aktivCore->kanal[ui->kanalAktiv->currentIndex()];
        ui->kanalAutoOn->setChecked((bool)k->getAutoMode());
        ui->kanalAutoCanStop->setChecked((bool)k->getStopEnabled());
        ui->kanalAutoMinimal->setValue(k->getMinPower());
        ui->kanalAutoStart->setValue(k->getStartupTime());
        ui->KanalManuellPower->setValue(k->getManualPower());
        ui->KanalStartupTime->setValue(k->getStartupTime());
        this->updateTabKanaeleGrenzwert(ui->kanalGrenzwert->currentIndex());
    }else{
        qDebug() << "woot";
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
    k->disableChanged();
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
    updateTabKanaeleGrenzwert(index);
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



void MainWindow::drawDashboard(){
    int i,j,k;
    // Bitch! init init init!
    i = 0;
    j = 0;
    k = 0;
    for(i = 0; i<28; i++){
        dashWidgets[i].box.setLayout(new QGridLayout());
        dashWidgets[i].box.layout()->addWidget(&dashWidgets[i].progressBar);
        dashLayout.addWidget(&dashWidgets[i].box,k,j);
        if(j == dashPerRow){
            j = 0;
            k++;
        }else{
            j++;
        }
        dashWidgets[i].progressBar.setMaximum(199);
        dashWidgets[i].progressBar.setValue(dashWidgets[i].value);
        dashWidgets[i].progressBar.setStyleSheet(STYLE_PB_OK);
        dashWidgets[i].progressBar.setFormat(QString("%v C°"));
        dashWidgets[i].box.setTitle(QString("Temperatur ") + QString::number(i+1-4));
    }

    dashWidgets[0].progressBar.setMaximum(9999);
    dashWidgets[1].progressBar.setMaximum(9999);
    dashWidgets[2].progressBar.setMaximum(9999);
    dashWidgets[3].progressBar.setMaximum(999);
    dashWidgets[0].progressBar.setFormat(QString("%v rpm"));
    dashWidgets[1].progressBar.setFormat(QString("%v rpm"));
    dashWidgets[2].progressBar.setFormat(QString("%v rpm"));
    dashWidgets[3].progressBar.setFormat(QString("%v l/h"));
    dashWidgets[0].box.setTitle(QString("Kanal 1"));
    dashWidgets[1].box.setTitle(QString("Kanal 2"));
    dashWidgets[2].box.setTitle(QString("Kanal 3"));
    dashWidgets[3].box.setTitle(QString("Kanal 4 - DFM"));
    ui->tabDash2->setLayout(&dashLayout);

    //dashWidgets[13].progressBar.setStyleSheet(STYLE_PB_WARN);
    //dashWidgets[14].progressBar.setStyleSheet(STYLE_PB_BAD);

}
