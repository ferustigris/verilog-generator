#include "rootnode.h"
#include "graphbody.h"

RootNode::RootNode(GraphBody *graphWidget) : Node(graphWidget,0,0)
{
    gb  = graphWidget;
    Node::Hide();
}
/*!\func
 * перерисовка
 * \param
 * - устройство отрисовки
 * - стиль
 * - при кэшировании не используется
 * \return нет
 */
void RootNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}
/*!\func
 * return type of the node
 * \params no
 * \return type
 */
TopTypes RootNode::getType() const
{
    return TOP_ROOT;
}
/*!\func
 * return name of the node
 * \params no
 * \return name
 */
QString RootNode::getName() const
{
    return "root";
}
/*!\func
 * no used
 * \params no
 * \return no
 */
void RootNode::setName(const QString&)
{
}
/*!\func
 * no used
 * \params no
 * \return no
 */
QString RootNode::getHelp() const
{
    return "";
}
/*!\func
 * no used
 * \params no
 * \return no
 */
void RootNode::setHelp(const QString&)
{
}
/*!\func
 * no used
 * \params no
 * \return no
 */
QString RootNode::getShortDesc() const
{
    return "";
}
/*!\func
 * no used
 * \params no
 * \return no
 */
void RootNode::setShortDesc(const QString&)
{
}
/*!\func
 * no used
 * \params no
 * \return no
 */
qint16 RootNode::getId()
{
    return 0;
}
/*!\func
 * no used
 * \params no
 * \return no
 */
INode*RootNode::getParent()
{
    return 0;
}
/*!\func
 * no used
 * \params no
 * \return no
 */
void RootNode::addEdge(Edge *)
{
}
/*!\func
 * no used
 * \params no
 * \return no
 */
void RootNode::delEdge(Edge *)
{
}
/*!\func
 * no used
 * \params no
 * \return no
 */
QList<Edge *> RootNode::edgesIn() const
{
    QList<Edge *> in;
    foreach(INode*n, gb->getDevices())
        in.append(n->edgesOut());
    return in;
}
/*!\func
 * no used
 * \params no
 * \return no
 */
QList<Edge *> RootNode::edgesOut() const
{
    QList<Edge *> out;
    foreach(INode*n, gb->getDevices())
        out.append(n->edgesIn());
    return out;
}
/*!\func
 * no used
 * \params no
 * \return no
 */
void RootNode::Show()
{
}
/*!\func
 * no used
 * \params no
 * \return no
 */
void RootNode::Hide()
{
}
/*!\func
 * no used
 * \params no
 * \return no
 */
void RootNode::remove()
{
}
/*!\func
 * какую площадь займете?
 * \param нет
 * \return занимемая пл-дь
 */
QRectF RootNode::boundingRect() const
{
    return QRectF(0,0,0,0);
}
