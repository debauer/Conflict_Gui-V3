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
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(-1, 1);
    ui->customPlot->yAxis->setRange(0, 1);
    ui->customPlot->replot();

    j = 0;
    k = 0;
    for(i = 0; i<28; i++){
        dashBox[i].setLayout(new QGridLayout());
        dashBox[i].layout()->addWidget(&dashProgressBar[i]);
        dashLayout.addWidget(&dashBox[i],k,j);
        if(j == dashPerRow){
            j = 0;
            k++;
        }else{
            j++;
        }
        dashProgressBar[i].setValue(33);
        dashProgressBar[i].setStyleSheet(QString("QProgressBar{border: 2px solid grey;border-radius: 5px;text-align: center;} QProgressBar::chunk{background-color: #FB4400;width: 2.15px;margin: 0.5px;}"));
        dashProgressBar[i].setFormat(QString("%v C°"));
        dashBox[i].setTitle(QString("Temperatur ") + QString::number(i+1-4));
    }

    dashProgressBar[0].setMaximum(9999);
    dashProgressBar[1].setMaximum(9999);
    dashProgressBar[2].setMaximum(9999);
    dashProgressBar[3].setMaximum(999);
    dashProgressBar[0].setValue(1000);
    dashProgressBar[0].setFormat(QString("%v rpm"));
    dashProgressBar[1].setFormat(QString("%v rpm"));
    dashProgressBar[2].setFormat(QString("%v rpm"));
    dashProgressBar[3].setFormat(QString("%v l/h"));
    dashBox[0].setTitle(QString("Kanal 1"));
    dashBox[1].setTitle(QString("Kanal 2"));
    dashBox[2].setTitle(QString("Kanal 3"));
    dashBox[3].setTitle(QString("DFM"));
    ui->scrollArea->setLayout(&dashLayout);

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
    QObject::connect(&core->alarm,&Alarm::HardwareChanged,[=](int type){
        // Immer TYPE_ALARM
        ui->optionenAlarmDurchfluss->setChecked((bool)core->alarm.getLowWaterflow());
        ui->optionenAlarmLuefter->setChecked((bool)core->alarm.getFanBlocked());
        ui->optionenAlarmTemperatur->setChecked((bool)core->alarm.getOverTemperatur());
    });
    QObject::connect(&core->dfm,&Dfm::HardwareChanged,[=](int type){
        if(type == TYPE_VALUE){
            dashProgressBar[3].setValue(core->dfm.getWaterFlow());
        }else if(type == TYPE_SETUP){
            ui->optionenDfmImpulse->setValue(core->dfm.getPulsePerLiter());
            ui->optionenAlarmMinDurchfluss->setValue(core->dfm.getMinFlow());
        }
    });
    for(i=0;i<3;i++){
        QObject::connect(&core->kanal[i],&Kanal::HardwareChanged,[=](int type){
            if(type == TYPE_VALUE){
                dashProgressBar[i].setValue(core->kanal[i].getRpm());
            }else if(type == TYPE_SETUP){
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
    QObject::connect(ui->optionenAlarmDurchfluss, SIGNAL(clicked(bool)),&core->alarm, SLOT(setLowWaterflow(bool)));

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

