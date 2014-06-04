#include "dashitem.h"

dashItem::dashItem(QObject* parent) : QObject(parent),initialized(false){

}

dashItem::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter p(this);

    QRect geo = geometry();
    int width = geo.width();
    int height = geo.height();

    int x=0, y=0;
    if ( alignment_ & Qt::AlignLeft )
       x = 0;
    else if ( alignment_ & Qt::AlignRight )
        x = width-diamX_;
    else if ( alignment_ & Qt::AlignHCenter )
        x = (width-diamX_)/2;
    else if ( alignment_ & Qt::AlignJustify )
        x = 0;

    if ( alignment_ & Qt::AlignTop )
       y = 0;
    else if ( alignment_ & Qt::AlignBottom )
        y = height-diamY_;
    else if ( alignment_ & Qt::AlignVCenter )
        y = (height-diamY_)/2;

      QRadialGradient g(x+diamX_/2, y+diamY_/2, diamX_*0.4,
           diamX_*0.4, diamY_*0.4);

      g.setColorAt(0, Qt::white);
       if ( state_ )
          g.setColorAt(1, color_);
       else
          g.setColorAt(1, Qt::black);
       QBrush brush(g);

       p.setPen(color_);
       p.setRenderHint(QPainter::Antialiasing, true);
       p.setBrush(brush);
       p.drawEllipse(x, y, diamX_-1, diamY_-1);

      if ( flashRate_ > 0 && flashing_ )
          timer_->start(flashRate_);
      else
           timer_->stop();
}

int dashItem::value(){
    return value;
}

void dashItem::setValue(int value){
    this->value = value;
}

QString dashItem::toolTip() const{
   return tr("An LED");
}

QString dashItem::whatsThis() const{
   return tr("An LED");
}

QString dashItem::group() const{
   return tr("ICS Custom Widgets");
}

QString dashItem::name() const{
   return "jow";
}

QIcon dashItem::icon() const{
   return QIcon();
}

bool dashItem::isContainer() const{
   return false;
}

QWidget * dashItem::createWidget(QWidget *parent){
    return new dashItem(parent);
}


#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(ledplugin, LEDPlugin)
#endif

