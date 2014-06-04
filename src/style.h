#ifndef STYLE_H
#define STYLE_H

#include "mainwindow.h"


namespace Ui {
    class MainWindow;
}

class Style{
    private:
        MainWindow *mw;
    public:
        style(MainWindow *mwmw);
        void setDarkOrange();
};

#endif // STYLE_H
