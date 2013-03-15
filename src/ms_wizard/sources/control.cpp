#include "control.h"
#include "data.h"

/*!\func
 * constructor. class Control provided interface for controls by outputs
 * \param no
 * \return no
 */
Control::Control(Data* data, const qint32 id, const qint32 state, const qint32 output, const QString&condition, const QString&value)
{
	this->output_id = output;
	this->state_id = state;
	this->id = id;
	this->data = data;
	this->value = value;
	this->condition = condition;
}
/*!\func
 * get state name
 * \param no
 * \return no
 */
State Control::getState() const
{
	if(data)
		return data->getState(state_id);
	return State();
}
/*!\func
 * get output name
 * \param no
 * \return no
 */
Output Control::getOutput() const
{
	if(data)
		return data->getOutput(output_id);
	return Output();
}
/*!\func
 * get new value of output
 * \param no
 * \return no
 */
QString Control::getValue() const
{
	return value;
}
/*!\func
 * get output's size
 * \param no
 * \return no
 */
qint32 Control::getSize() const
{
	return size;
}
/*!\func
 * get id
 * \param no
 * \return no
 */
qint32 Control::getId() const
{
	return id;
}
/*!\func
 * get condition change output's value
 * \param no
 * \return no
 */
QString Control::getCondition() const
{
    return condition;
}
/*!\func
 * set condition change output's value
 * \param
 * - newValue - new condition
 * \return no
 */
void Control::setCondition(const QString&newValue)
{
    condition = newValue;
}
/*!\func
 * set new value
 * \param
 * - newValue - new value
 * \return no
 */
void Control::setValue(const QString&newValue)
{
    value = newValue;
}
