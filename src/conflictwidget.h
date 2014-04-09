#ifndef CONFLICTWIDGET_H
#define CONFLICTWIDGET_H

#include <QDialog>

namespace Ui {
class ConflictWidget;
}

class ConflictWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ConflictWidget(QWidget *parent = 0);
    ~ConflictWidget();
    Ui::ConflictWidget *getObject();

private:
    Ui::ConflictWidget *ui;
};

#endif // CONFLICTWIDGET_H
