#ifndef CONTROL_H
#define CONTROL_H
#include "output.h"
#include "state.h"
#include <QString>

class Data;

class Control
{
public:
    Control(Data* data_meneger = 0, const qint32 id = -1, const qint32 state = -1, const qint32 output = -1, const QString&condition = "0", const QString&value = "0");
    State getState() const;
    Output getOutput() const;
    QString getValue() const;
    qint32 getSize() const;
    qint32 getId() const;
    QString getCondition() const;
    void setCondition( const QString &newValue);
    void setValue( const QString &newValue);
private:
    qint32 output_id;
    qint32 state_id;
    qint32 id;
    qint32 size;
    Data* data;
    QString value;
    QString condition;

};

#endif // CONTROL_H
