#include "conflictwidget.h"
#include "ui_conflictwidget.h"

ConflictWidget::ConflictWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConflictWidget){
    ui->setupUi(this);
}

Ui::ConflictWidget *ConflictWidget::getObject(){
    return ui;
}

ConflictWidget::~ConflictWidget()
{
    delete ui;
}


