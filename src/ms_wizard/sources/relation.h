#ifndef RELATION_H
#define RELATION_H
#include "state.h"
#include <QSharedPointer>

class Data;

class Relation
{
public:
    Relation(Data* data_meneger = 0, const qint32 id = -1, const qint32 id_source = -1, const qint32 id_dest = -1, const QString&condition="");
    Relation(const Relation&clone);
    State getSource() const ;
    State getDest() const;
    QString getCondition() const ;
    qint32 getId() const;
    void setCondition(const QString&newValue);
    void setDest(const qint32 newState);
private:
    qint32 src, dst;
    qint32 id;
    Data* data;
    QString condition;
};

#endif // RELATION_H
