#include "state.h"
#include "data.h"
#include "relation.h"
#include <QPoint>

/*!\func
 * class of  state in FSM
 * \param no
 * \return no
 */
State::State(Data*data, const qint32 id, const QString&nm) :
     name(nm), pos(0,0)
{
    this->id = id;
    this->data = data;
}
/*!\func
 * destructor
 * \param no
 * \return no
 */
State::~State()
{
}
/*!\func
 * get name of state
 * \param no
 * \return no
 */
QString State::getName() const
{
    return name;
}
/*!\func
 * get id of state
 * \param no
 * \return no
 */
qint32 State::getId() const
{
    return id;
}
/*!\func
 * get id of state
 * \param no
 * \return no
 */
QPoint State::getPosition() const
{
    return pos;
}
/*!\func
 * get name of state
 * \param no
 * \return no
 */
QString State::getDescription() const
{
    return description;
}
/*!\func
 * set new name
 * \param no
 * \return no
 */
void State::setDescription(const QString& newValue)
{
    description = newValue;
}
/*!\func
 * set new pos
 * \param no
 * \return no
 */
void State::setPosition(const QPoint& value)
{
	pos = value;
}
/*!\func
 * compare 2 states
 * \param no
 * \return no
 */
bool State::operator ==(const State&op2) const
{
    return name == op2.name;
}
/*!\func
 * get all relation
 * \param no
 * \return no
 */
QList<Relation> State::getRelations() const
{
    if(!data)
	return QList<Relation>();
    return data->getRelations(id);
}
/*!\func
 * set new name
 * \param no
 * \return no
 */
void State::setName(const QString& newValue)
{
    name = newValue;
}
