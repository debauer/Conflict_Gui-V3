#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "QDebug"
#include "QObject"

#include "carriage.h"

#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "QSignalMapper"
#include "conflict_core.h"

#define STATUS_NO_CONNECTION QString("no connection")
#define STATUS_INIT QString("init")
#define STATUS_OK QString("ok")
#define STATUS_ERROR QString("error")

namespace Ui {
class MainWindow;
}

class ConflictCore;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ConflictCore* newCore();
    ConflictCore* aktivCore; // der aktuelle Core auf dem alle connects und pointer zeigen
    //QWidget* loadUiFile();
    void connectCore(ConflictCore* core);
    void disconnectCore(ConflictCore* core);
    QString guiStatus;
    QSignalMapper* signalMapper;


public slots:
    void newCoreSerial(int ser);
    void newCoreETH(QString ip);
    void dropCoreSerial();
    void conflictRestart();
    void conflictInit();
    void debugConsole(Carriage *car);
    void debugConsole(QString str);
    void debugConsole(int iihh);
    void updateGui(ConflictCore* core,QString dataClass);
    void updateTabKanaele(int kanal);
    void updateTabKanaele(); // alle
    void initComplete();
    void updateTabKanaeleGrenzwert(int kanal);
    void speicherKanal();
//    void speicherTemperatur();
};

#endif // MAINWINDOW_H
