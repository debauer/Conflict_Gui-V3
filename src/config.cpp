#include "config.h"

Config::Config(){
    int size;
    settings = new QSettings("config.ini", QSettings::IniFormat);
    qDebug() << "lade ini";

    /* ======= COM PORT ========= */

    settings->beginGroup("com");
    com.port = readQString("com1",settings->value("port").toString());
    com.baud = readInt(19600,settings->value("baud").toString());
    settings->endGroup();

    /* ======= DASH ========= */

    /* ======= DISPLAY ========= */

    size = settings->beginReadArray("display");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        display.append(displayString("row1"));
        display.append(displayString("row2"));
        display.append(displayString("row3"));
    }
    settings->endArray();

    displayReplace << (DisplayReplace){"%h",new unsigned char[2] {193,1},2};
    displayReplace << (DisplayReplace){"%m",new unsigned char[2] {193,2},2};
    displayReplace << (DisplayReplace){"%s",new unsigned char[2] {193,3},2};
    displayReplace << (DisplayReplace){"%s",new unsigned char[2] {193,3},2};

    QByteArray a;


//    s.replace("%cpu", QString(0xC1)+0x01+0x00+0x00);
//    s.replace("%f1", QString((char)0xC4)+QString((char)0x01)+QString((char)0x00));
//    s.replace("%cp", QString(195)+QString(1)+QString(0));
//    s.replace("%gp", QString(195)+QString(2)+QString(0));
//    s.replace("%ra", QString(195)+QString(3)+QString(0));
//    s.replace("%rp1", QString(197)+QString(1)+QString(0)+QString(0));
//    s.replace("%rp2", QString(197)+QString(2)+QString(0)+QString(0));
//    s.replace("%rp3", QString(197)+QString(3)+QString(0)+QString(0));
//    s.replace("%rp4", QString(197)+QString(4)+QString(0)+QString(0));
//    s.replace("%p1", QString(198)+QString(1)+QString(0));
//    s.replace("%p2", QString(198)+QString(2)+QString(0));
//    s.replace("%p3", QString(198)+QString(3)+QString(0));
//    s.replace("%p4", QString(198)+QString(4)+QString(0));

}

QString Config::displayString(QString str){
    QString s;
    int i;
    if(settings->contains(str)){
        s = settings->value(str).toString();
//        for(i=0;i<24;i++){
//            if(i<10)
//                s.replace("%t" + QString::number(i+1), QString(192)+QString(i+1)+QString(0)+QString(0));
//            else
//                s.replace("%t0" + QString::number(i+1), QString(192)+QString(i+1)+QString(0)+QString(0));
//        }
//        QByteArray(193).append(1);
//        for(i=0;i<displayReplace.size();i++){
//            s.replace(displayReplace.at(i).before,QString((char)displayReplace.at(i).after));
//        }
    }
    else
        s = QString("                ");
}

QString Config::readQString(QString def, QString str){
    if(str == "")
        return def;
    else
        return str;
}

int Config::readInt(int def, QString str){
    if(str == "")
        return def;
    else
        return str.toInt();
}
