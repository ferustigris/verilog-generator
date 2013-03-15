#ifndef NODEFSM_H
#define NODEFSM_H
#include "node.h"
//! node for FSM
class NodeFSM : public Node
{
public:
    NodeFSM(GraphBody *graphWidget, INode* _parent, qint16 _id, const QString &fsm);
    void setMS(const QString& ms_data);
    QString getMS() const;
    void setMSfile(const QString& fn);
    QString getMSfile() const;
    virtual TopTypes getType() const;
private:
    QString ms_data;
    QString ms_data_file;
    GraphBody *graph;
    qint16 id;
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // NODEFSM_H
