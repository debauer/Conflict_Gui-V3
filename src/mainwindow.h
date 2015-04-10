#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "QDebug"
#include "QObject"

#include "carriage.h"
#include "data/data.h"

#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "QSignalMapper"
#include "QGridLayout"
#include "QProgressBar"
#include "QGroupBox"
#include "conflict_core.h"

#define STATUS_NO_CONNECTION QString("no connection")
#define STATUS_INIT QString("init")
#define STATUS_OK QString("ok")
#define STATUS_ERROR QString("error")

#define STYLE_PB_OK QString("QProgressBar{border: 2px solid grey;border-radius: 5px;text-align: center;} QProgressBar::chunk{background-color: #339900;width: 2.15px;margin: 0.5px;}")
#define STYLE_PB_WARN QString("QProgressBar{border: 2px solid grey;border-radius: 5px;text-align: center;} QProgressBar::chunk{background-color: #CC9900;width: 2.15px;margin: 0.5px;}")
#define STYLE_PB_BAD QString("QProgressBar{border: 2px solid grey;border-radius: 5px;text-align: center;} QProgressBar::chunk{background-color: #FB4400;width: 2.15px;margin: 0.5px;}")

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

    /* DASHBOARD */

    QGridLayout dashLayout;
    int dashPerRow = 3;

    void drawDashboard();
    struct dashWidget* getWidgetPtr(QString asd);
    //void DashChangeValue(int nr,int value);


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
    void updateDash();
//    void speicherTemperatur();
};

#endif // MAINWINDOW_H
