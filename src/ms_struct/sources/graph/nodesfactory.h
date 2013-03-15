#ifndef NODESFACTORY_H
#define NODESFACTORY_H
#include "inode.h"
#include "graphbody.h"
#include <QPointF>

//! class, wich created nodes and link with graphbody
class NodesFactory
{
public:
    NodesFactory(GraphBody*graphbody);
    INode*newNode(const qint32 id, INode *parent, const QString&name, const QString&desc, const QString&help, const QPointF&pos);
    INode*newFSM(const qint32 id, INode *parent, const QString&name, const QString&desc, const QString&help, const QPointF&pos, const QString&ms_file, const QString&ms_data);
    INode*newDev(const qint32 id, const QString&name, const QString&desc, const QString&help, const QPointF&pos);
    INode*newRoot();
private:
    GraphBody*gb;
};

#endif // NODESFACTORY_H
