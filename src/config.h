#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>

class config : public QObject{
    Q_OBJECT
public:
    config();
    QSettings settings;
};

#endif // CONFIG_H
