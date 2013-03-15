#include "nodefsm.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include "errors.h"
#include "graphbody.h"


/*!\func
 * constructor
 * \params no
 * \return no
 */
NodeFSM::NodeFSM(GraphBody *graphWidget, INode* _parent, qint16 _id, const QString &fsm) : Node(graphWidget, _parent, _id)
{
    ms_data = fsm;
    graph = graphWidget;
    id = _id;
}
/*!\func
 * set ms for top
 * \params
 * - ms_data - data
 * \return no
 */
void NodeFSM::setMS(const QString& ms_data)
{
    this->ms_data = ms_data;
    if (!ms_data.isEmpty())
            update();
}
/*!\func
 * get ms for top
 * \params no
 * \return no
 */
QString NodeFSM::getMS() const
{
    return ms_data;
}
/*!\func
 * set file.xml ms for top
 * \params
 * - fn - file name
 * \return no
 */
void NodeFSM::setMSfile(const QString& fn)
{
    this->ms_data_file = fn;
}
/*!\func
 * get file.xml ms for top
 * \params no
 * \return no
 */
QString NodeFSM::getMSfile() const
{
    return ms_data_file;
}
/*!\func
 * перерисовка
 * \param
 * - устройство отрисовки
 * - стиль
 * - при кэшировании не используется
 * \return нет
 */
void NodeFSM::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if(graph->getCurIndex() == id)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0xd0, 0xd0, 0xd0, 100));
        painter->drawRoundedRect(-40,-30, 80, 60, 15, 15);
    }
    if(!nodes().isEmpty())painter->drawImage(QRect(15, -9, 18, 18), QImage(":/icons/down"));
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::lightGray);
    if (option->state & QStyle::State_Sunken) {
            painter->drawEllipse(-8, -8, 20, 20);
    } else {
    painter->drawEllipse(-7, -7, 20, 20);
    }
    QRadialGradient gradient(-3, -3, 10);
    gradient.setColorAt(0, Qt::yellow);
    gradient.setColorAt(1, Qt::darkYellow);
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));

    if (option->state & QStyle::State_Sunken) {
        painter->drawEllipse(-9, -9, 20, 20);
        painter->drawImage(QRect(-8, -8, 18, 18), QImage(":/icons/fsm"));
    } else {
        painter->drawEllipse(-10, -10, 20, 20);
        painter->drawImage(QRect(-9, -9, 18, 18), QImage(":/icons/fsm"));
    }
}
/*!\func
 * type of node
 * \params no
 * \return type of node
 */
TopTypes NodeFSM::getType() const
{
    return TOP_FSM;
}
