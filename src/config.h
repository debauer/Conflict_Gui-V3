#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>
#include "QDebug"

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
    unsigned char* after;
    int lenght;
};

class Config : public QObject{
    Q_OBJECT
public:

    DataBase db;
    ComPort com;
    DashBoard defaultDash;
    QByteArray display;
    QStringList displayList;
    QList<DisplayReplace> displayReplace;
    QList<DisplayReplace> displayReplaceTemperature;

    Config();
    QSettings *settings;
    QString displayString(QString str);
    QString readQString(QString def, QString str);
    int readInt(int def, QString str);
};

#endif // CONFIG_H
