#include "config.h"

Config::Config(){
    int size;
    settings = new QSettings("config.ini", QSettings::IniFormat);
    qDebug() << "lade ini";

    /* ======= COM PORT ========= */

    settings->beginGroup("com");
    com_port = readQString("com1",settings->value("port").toString());
    com_baud = readInt(57600,settings->value("baud").toString());
    settings->endGroup();
    qDebug() <<  com_baud;
    qDebug() << com_port ;

    /* ======= DASH ========= */

    /* ======= DISPLAY ========= */

    // %m		  : Minute 2 Char
    // %h		  : Hour 2 Char
    // %s		  : Seconds 2 Char
    displayReplace.append({"%h","\193\001",2});
    displayReplace.append({"%m","\193\2",2});
    displayReplace.append({"%s","\193\3",2});

    // %t01 - %t24 : Teperatur 4 Char
    displayReplace.append({"%t01","\192\1\0\0",4});
    displayReplace.append({"%t02","\192\2\0\0",4});
    displayReplace.append({"%t03","\192\3\0\0",4});
    displayReplace.append({"%t04","\192\4\0\0",4});
    displayReplace.append({"%t05","\192\5\0\0",4});
    displayReplace.append({"%t06","\192\6\0\0",4});
    displayReplace.append({"%t07","\192\7\0\0",4});
    displayReplace.append({"%t08","\192\8\0\0",4});
    displayReplace.append({"%t09","\192\9\0\0",4});
    displayReplace.append({"%t10","\192\10\0\0",4});
    displayReplace.append({"%t11","\192\11\0\0",4});
    displayReplace.append({"%t12","\192\12\0\0",4});
    displayReplace.append({"%t13","\192\13\0\0",4});
    displayReplace.append({"%t14","\192\14\0\0",4});
    displayReplace.append({"%t15","\192\15\0\0",4});
    displayReplace.append({"%t16","\192\16\0\0",4});
    displayReplace.append({"%t17","\192\17\0\0",4});
    displayReplace.append({"%t18","\192\18\0\0",4});
    displayReplace.append({"%t19","\192\19\0\0",4});
    displayReplace.append({"%t20","\192\20\0\0",4});
    displayReplace.append({"%t21","\192\21\0\0",4});
    displayReplace.append({"%t22","\192\22\0\0",4});
    displayReplace.append({"%t23","\192\23\0\0",4});
    displayReplace.append({"%t24","\192\24\0\0",4});

    // %cpu       : CPU Clock 4 char
    displayReplace.append({"%cpu","\194\1\0\0",4});

    // %cp        : CPU usage 3 char
    // %gp        : GPU usage 3 char
    // %ra        : RAM usage 3 char
    displayReplace.append({"%cp","\195\1\0",3});
    displayReplace.append({"%gp","\195\2\0",3});
    displayReplace.append({"%ra","\195\3\0",3});

    // %wf        : water flow 3 char
    displayReplace.append({"%wf","\196\1\0\0",4});

    // %rp1 - %rp4 : FAN RPM 4 char
    displayReplace.append({"%rp1","\197\1\0\0",4});
    displayReplace.append({"%rp2","\197\2\0\0",4});
    displayReplace.append({"%rp3","\197\3\0\0",4});
    displayReplace.append({"%rp4","\197\4\0\0",4});

    // %p1  - %p4  : FAN Power 3 char
    displayReplace.append({"%p1","\198\1\0",3});
    displayReplace.append({"%p2","\198\2\0",3});
    displayReplace.append({"%p3","\198\3\0",3});

    size = settings->beginReadArray("display");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        display.append(displayString("row1"));
        display.append(displayString("row2"));
        display.append(displayString("row3"));
    }
    settings->endArray();

    settings->value("port").toString();

    qDebug() << display;

    foreach( DisplayReplace rep, displayReplace ){
        display.replace(rep.before, rep.after);
    }

    qDebug() << display;

//    display.replace("%cpu", QString(0xC1)+0x01+0x00+0x00);
//    display.replace("%f1", QString((char)0xC4)+QString((char)0x01)+QString((char)0x00));
//    display.replace("%cp", QString(195)+QString(1)+QString(0));
//    display.replace("%gp", QString(195)+QString(2)+QString(0));
//    display.replace("%ra", QString(195)+QString(3)+QString(0));
//    display.replace("%rp1", QString(197)+QString(1)+QString(0)+QString(0));
//    display.replace("%rp2", QString(197)+QString(2)+QString(0)+QString(0));
//    display.replace("%rp3", QString(197)+QString(3)+QString(0)+QString(0));
//    display.replace("%rp4", QString(197)+QString(4)+QString(0)+QString(0));
//    display.replace("%p1", QString(198)+QString(1)+QString(0));
//    display.replace("%p2", QString(198)+QString(2)+QString(0));
//    display.replace("%p3", QString(198)+QString(3)+QString(0));
//    display.replace("%p4", QString(198)+QString(4)+QString(0));

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
