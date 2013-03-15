#ifndef ROOTNODE_H
#define ROOTNODE_H
#include "node.h"

class RootNode : public Node
{
public:
    RootNode(GraphBody *graphWidget);
    virtual TopTypes getType() const ;
    virtual QString getName() const ;
    virtual void setName(const QString&);
    virtual QString getHelp() const;
    virtual void setHelp(const QString&);
    virtual QString getShortDesc() const;
    virtual void setShortDesc(const QString&);
    virtual qint16 getId();
    virtual INode*getParent();
    virtual void addEdge(Edge *edge);
    virtual void delEdge(Edge *edge);
    virtual QList<Edge *> edgesIn() const;
    virtual QList<Edge *> edgesOut() const;
    virtual void Show();
    virtual void Hide();
    virtual void remove();
private:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
private:
    GraphBody *gb;
};

#endif // ROOTNODE_H
