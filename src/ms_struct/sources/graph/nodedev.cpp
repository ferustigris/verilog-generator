#include "nodedev.h"
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
NodeDev::NodeDev(GraphBody *graphWidget, INode*, qint16 _id) :
        Node(graphWidget, 0, _id)
{
    graph = graphWidget;
    id = _id;
}
/*!\func
 * перерисовка
 * \params
 * - устройство отрисовки
 * - стиль
 * - при кэшировании не используется
 * \return нет
 */
void NodeDev::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::lightGray);
    if (option->state & QStyle::State_Sunken) {
        painter->drawEllipse(-8, -8, 20, 20);
    } else {
        painter->drawEllipse(-7, -7, 20, 20);
    }
    QRadialGradient gradient(-3, -3, 10);
    gradient.setColorAt(0, Qt::blue);
    gradient.setColorAt(1, Qt::darkBlue);
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));

    if (option->state & QStyle::State_Sunken) {
        painter->drawEllipse(-9, -9, 20, 20);
        painter->drawImage(QRect(-8, -8, 18, 18), QImage(":/icons/dev"));
    } else {
        painter->drawEllipse(-10, -10, 20, 20);
        painter->drawImage(QRect(-9, -9, 18, 18), QImage(":/icons/dev"));
    }
}
/*!\func
 * type of node
 * \params no
 * \return type of node
 */
TopTypes NodeDev::getType() const
{
    return TOP_DEVICE;
}
/*!\func
 * show on scene
 * \params no
 * \return type of node
 */
void NodeDev::Show()
{
    show();
    name->setPos(pos());
    name->moveBy(x() - name->x() - name->boundingRect().width()/2, y() - name->y() + 10);
    name->show();
}
