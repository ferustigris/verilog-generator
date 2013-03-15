#include "input.h"

/*!\func
 * relation between states in FSM
 * \param
 - data_meneger - meneger of data
 - name - input name
 - size - input size
 * \return no
 */
Input::Input(Data* data_meneger, const qint32 id, const QString&name, const qint32 size)
{
    this->data = data_meneger;
    this->name = name;
    if(size < 1)this->size = 1;
    else
        this->size = size;
    this->id = id;
    type = "wire";
}
/*!\func
 * get input name
 * \param no
 * \return no
 */
QString Input::getName() const
{
    return name;
}
/*!\func
 * get input type
 * \param no
 * \return no
 */
QString Input::getType() const
{
    return type;
}
/*!\func
 * get input size
 * \param no
 * \return no
 */
qint32 Input::getSize() const
{
    return size;
}
/*!\func
 * get id of input
 * \param no
 * \return no
 */
qint32 Input::getId() const
{
    return id;
}
/*!\func
 * set type of output
 * \param
 * - newType - new type of output
 * \return no
 */
void Input::setType(const QString&newType)
{
    type = newType;
}
/*!\func
 * set new name of output
 * \param
 * - newName - new type of output
 * \return no
 */
void Input::setName(const QString&newName)
{
    name = newName;
}
/*!\func
 * set new name of output
 * \param
 * - newName - new type of output
 * \return no
 */
void Input::setSize(const qint32 newValue)
{
    size = newValue;
}


