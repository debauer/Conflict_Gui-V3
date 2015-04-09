#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>
#include "QDebug"
#include "data/lcd.h"

struct DataBase{
    QString user;
    QString db;
    QString pw;
    QString host;
    int port;
};

struct ComPort{
    QString port;
    int baud;
};

struct DashBoard{
    QString type;
    QString value;
    QString colorOk;
    QString colorWarn;
    QString colorError;
    QString rangeStart;
    QString rangeEnd;
    QString limitOk;
    QString limitWarn;
    QString limitAlarm;
};

struct DisplayReplace{
    QString before;
    QByteArray after;
    int lenght;
};

class Config : public QObject{
    Q_OBJECT
public:

    int com_baud;
    QString com_port;
    DataBase *db;
    Lcd *lcd;
    DashBoard defaultDash;
    QString display;
    QByteArray displayArray;
    QStringList displayList;
    QList<DisplayReplace> displayReplace;
    QList<DisplayReplace> displayReplaceTemperature;

    Config();
    QSettings *settings;
    QString displayString(QString str);
    QString readQString(QString def, QString str);
    int readInt(int def, QString str);

private:

};

#endif // CONFIG_H
