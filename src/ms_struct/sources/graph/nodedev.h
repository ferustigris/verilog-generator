#ifndef NODEDEV_H
#define NODEDEV_H
#include "node.h"
//! node for devices

class NodeDev : public Node
{
public:
    NodeDev(GraphBody *graphWidget, INode* _parent, qint16 _id);
    void Show();
private:
    QString ms_data;
    QString ms_data_file;
    GraphBody *graph;
    qint16 id;
    virtual TopTypes getType() const;
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // NODEDEV_H
