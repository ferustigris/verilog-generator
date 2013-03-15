#include "output.h"

Output::Output(Data* data, const qint32 id, const QString&name, const qint32 size, const QString&type, const QString&value)
    : Input(data, id, name, size)
{
    this->value = value;
    this->setType(type);
	//if(getType() != "reg")setType("reg");
}
/*!\func
 * get output value
 * \param no
 * \return no
 */
QString Output::getValue() const
{
    return value;
}
/*!\func
 * set output value
 * \param no
 * \return no
 */
void Output::setValue(const QString&newValue)
{
    value = newValue;
}
