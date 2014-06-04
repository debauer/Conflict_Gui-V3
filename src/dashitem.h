#ifndef DASHITEM_H
#define DASHITEM_H

#include <QString>
#include <QIcon>
#include <QtDesigner/QtDesigner>
#include <QWidget>
#include <QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT dashItem : public QWidget
{
    Q_OBJECT
    //Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "analogclock.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)

    Q_PROPERTY(int value READ value WRITE setValue)

private:
    bool initialized;
    int value;

protected:
    void paintEvent(QPaintEvent* event);

public slots:
    void setValue(int value);

public:
    int getValue();

    int heightForWidth(int width) const;
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    dashItem(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

};

#endif // DASHITEM_H
