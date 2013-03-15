#include "relation.h"
#include "data.h"

/*!\func
 * relation between states in FSM
 * \param
 - data_meneger - meneger of data
 - id_source - source state
 - id_dest - finish state
 * \return no
 */
Relation::Relation(Data* data_meneger, const qint32 id, const qint32 id_source, const qint32 id_dest, const QString&condition)
{
    data = data_meneger;
    src = id_source;
    dst = id_dest;
    this->id = id;
    this->condition = condition;
}
Relation::Relation(const Relation&clone)
{
    data = clone.data;
    src = clone.src;
    dst = clone.dst;
    condition = clone.condition;
    id = clone.id;
}
/*!\func
 * get source state
 * \param no
 * \return no
 */
State Relation::getSource() const
{
    if(!data)
        return State();
    else
        return data->getState(src);
}
/*!\func
 * get dest state
 * \param no
 * \return no
 */
State Relation::getDest() const
{
    if(!data)
        return State();
    else
        return data->getState(dst);
}
/*!\func
 * get dest state
 * \param no
 * \return no
 */
QString Relation::getCondition() const
{
    return condition;
}
/*!\func
 * get id of Relation
 * \param no
 * \return no
 */
qint32 Relation::getId() const
{
    return id;
}
/*!\func
 * change condition
 * \param no
 * \return no
 */
void Relation::setCondition(const QString&newValue)
{
    condition = newValue;
}
/*!\func
 * change destination state
 * \param no
 * \return no
 */
void Relation::setDest(const qint32 newState)
{
    dst  = newState;
}
