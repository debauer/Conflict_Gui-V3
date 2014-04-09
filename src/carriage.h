#ifndef CARRIAGE_H
#define CARRIAGE_H

#include <QString>
#include <QList>

class Carriage{
    private:
        int id;
        int index;
        QString data;
        int direction;
    public:
        Carriage(int direction,int id, int index, QString data);
        Carriage(int direction,int id, int index, int data);
        Carriage(QString str);
        Carriage();
        void set(int direction,int id, int index, QString data);
        void set(int direction,int id, int index, int data);
        void set(QString str);

        /* getter */
        int getDirection();
        int getId();
        int getIndex();
        QString getData();
        QString getString();
        // toString == getString
        QString toString();

        /* setter */
        void setDirection(int direction);
        void setId(int id);
        void setIndex(int index);
        void setData(QString data);
        void setData(int data);
};

#endif // CARRIAGE_H
