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

    QSignalMapper* signalMapper;


public slots:
    void newCoreSerial(int ser);
    void newCoreETH(QString ip);
    void debugConsole(Carriage *car);
    void debugConsole(QString str);
    void debugConsole(int iihh);
    void updateGui(ConflictCore* core,QString dataClass);
    void updateTabKanaele(int kanal);
    void updateTabKanaeleGrenzwert(int kanal);
    void speicherKanal();
//    void speicherTemperatur();
};

#endif // MAINWINDOW_H
