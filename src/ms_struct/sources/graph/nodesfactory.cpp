#include "nodesfactory.h"
#include "inode.h"
#include "node.h"
#include "rootnode.h"
#include "nodedev.h"
#include "nodefsm.h"

/*!\func
 * set ms for top
 * \params
 * - ms_data - data
 * \return no
 */
NodesFactory::NodesFactory(GraphBody*graphbody)
{
    gb = graphbody;
}
/*!\func
 * create new node
 * \params
 * - parent - parent of new node
 * - name - name of node
 * - desc - short description
 * - help - help for node
 * - pos - node position
 * \return pointer to new node
 */
INode* NodesFactory::newNode(const qint32 id, INode *parent, const QString&name, const QString&desc, const QString&help, const QPointF&pos)
{
    Node*n = new Node(gb, parent, id);
    n->setPos(pos);
	n->setShortDesc(desc);
    n->setName(name);
	n->setHelpFile(help);
    parent->addNode(n);
    return n;
}
/*!\func
 * create new fsm
 * \params
 * - parent - parent of new node
 * - name - name of node
 * - desc - short description
 * - help - help for node
 * - pos - node position
 * - ms_file - file name with fsm
 * - ms_data - body of file ms_file
 * \return pointer to new node
 */
INode* NodesFactory::newFSM(const qint32 id, INode *parent, const QString&name, const QString&desc, const QString&help, const QPointF&pos, const QString&ms_file, const QString&ms_data)
{
    NodeFSM*n = new NodeFSM(gb, parent, id, ms_data);
    n->setPos(pos);
	n->setShortDesc(desc);
    n->setName(name);
	n->setHelpFile(help);
	n->setMSfile(ms_file);
    parent->addNode(n);
    return n;
}
/*!\func
 * create new device
 * \params
 * - name - name of node
 * - desc - short description
 * - help - help for node
 * - pos - node position
 * \return pointer to new node
 */
INode* NodesFactory::newDev(const qint32 id, const QString&name, const QString&desc, const QString&help, const QPointF&pos)
{
    NodeDev*n = new NodeDev(gb, 0, id);
    n->setPos(pos);
	n->setShortDesc(desc);
    n->setName(name);
	n->setHelpFile(help);
	gb->addDevice(n);
    n->Show();
    return n;
}
/*!\func
 * create new root node
 * \params no
 * \return pointer to new node
 */
INode* NodesFactory::newRoot()
{
    RootNode*n = new RootNode(gb);
    return n;
}
