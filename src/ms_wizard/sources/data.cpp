#include "data.h"

Data::Data()
{
}
Data::~Data()
{
}
/*!\func
 * get state by id
 * \param no
 * \return no
 */
State Data::getState(const qint32 id) const
{
	if(states.contains(id))
		return states[id];
	else return State();
}
/*!\func
 * get relation by id
 * \param no
 * \return no
 */
Relation Data::getRelation(const qint32 id) const
{
	if(relations.contains(id))
		return relations[id];
	else return Relation(0,-1, -1);
}
/*!\func
 * add state with name
 * \param
 - name - name of new state
 * \return id of new state (or -1)
 */
qint32 Data::addState(const QString&name)
{
	static qint32 newId = 0;
        states.insert(newId, State(this, newId, getUniqueStateName(newId, name)));
	emit sAddState(newId);
	return newId++;
}
/*!\func
 * add state with name
 * \param
 - name - name of new state
 * \return id of new state (or -1)
 */
bool Data::removeState(const qint32 id)
{
    if(states.contains(id))
    {
	QList<qint32> list;
	foreach(Relation relation, relations)
	    if((relation.getDest().getId() == id)||(relation.getSource().getId() == id))
		list<<relation.getId();
	foreach(qint32 id, list)
	    removeRelation(id);
	list.clear();
	foreach(Control control, controls)
	    if(control.getState().getId() == id)
		list<<control.getId();
	foreach(qint32 id, list)
	    removeControl(id);
	states.remove(id);
	emit sRemoveState(id);
	return true;
    }
    return false;
}
/*!\func
 * add relation between source & dest
 * \param no
 * \return id of new state (or -1)
 */
qint32 Data::addRelation(const qint32 source_id, const qint32 dest_id, const QString&condition)
{
    static qint32 newId = 0;
    relations.insert(newId, Relation(this, newId, source_id, dest_id, condition));
    emit sAddRelation(newId);
    return newId++;
}
/*!\func
 * get all states
 * \param no
 * \return list of states
 */
QList<State> Data::getStates() const
{
    return states.values();
}
/*!\func
 * get all Relation for state
 * \param no
 * \return list of states
 */
QList<Relation> Data::getRelations(const qint32 ids) const
{
    QList<Relation> result;
    if(ids < 0)result = relations.values();
    foreach(Relation relation, relations.values())
	if(relation.getSource().getId() == ids)
	    result<<relation;
    return result;
}
/*!\func
 * get all inputs
 * \param no
 * \return list of inputs
 */
QList<Input> Data::getInputs() const
{
    return inputs.values();
}
/*!\func
 * get one inputs by id
 * \param
 * - id - id of the input
 * \return input
 */
Input Data::getInput(const qint32 id) const
{
    if(inputs.contains(id))
	return inputs[id];
    return Input();
}
/*!\func
 * add input
 * \param
 * - name - name of new input
 * - size - size of new input
 * \return id of new input
 */
qint32 Data::addInput(const QString&name, const qint32 size)
{
    static qint32 id = 0;
    emit sAddInput(id);
    emit dataChange();
    inputs.insert(id, Input(this,id,getUniqueInputName(id, name), size));
    return id++;
}
/*!\func
 * get all outputs
 * \param no
 * \return list of inputs
 */
QList<Output> Data::getOutputs() const
{
    return outputs.values();
}
/*!\func
 * get one Output by id
 * \param
 * - id - id of the input
 * \return input
 */
Output Data::getOutput(const qint32 id) const
{
    if(outputs.contains(id))
	return outputs[id];
    return Output();
}
/*!\func
 * add Output
 * \param
 * - name - name of new input
 * - size - size of new input
 * \return id of new input
 */
qint32 Data::addOutput(const QString&name, const qint32 size, const QString&type, const QString&value)
{
    static qint32 id = 0;
    emit sAddOutput(id);
    emit dataChange();
    outputs.insert(id, Output(this,id,getUniqueOutputName(id, name), size, getValidType(type), value));
    return id++;
}
/*!\func
 * get all Controls
 * \param no
 * \return list of inputs
 */
QList<Control> Data::getControls(const qint32 state_id) const
{
	if(state_id < 0)
		return controls.values();
	else
	{
		QList<Control> list;
		foreach(Control control, controls)
			if(control.getState().getId() == state_id)
				list<<control;
		return list;
	}
}
/*!\func
 * get one Control by id
 * \param
 * - id - id of the Control
 * \return input
 */
Control Data::getControl(const qint32 id) const
{
    if(controls.contains(id))
	return controls[id];
    return Control();
}
/*! add Control to controls list
 * \params
 * - state - state's id
 * - output - output's id
 * - condition - condition control change
 * - value - control's value
 * \return 
 * id of new Control
 */
qint32 Data::addControl(const qint32 state, const qint32 output, const QString&condition, const QString&value)
{
    static qint32 id = 0;
    controls.insert(id, Control(this,id,state, output, condition, value));
    emit sAddControl(state, id);
    emit dataChange();
    return id++;
}
/*!\func
 * setStateName
 * \param
 * - id - state's id
 * - newValue - new state name
 * \return bool result operation
 */
bool Data::setStateName(const qint32 id, const QString&newValue)
{
    if(!states.contains(id))return false;
    states[id].setName(getUniqueStateName(id, newValue));
    emit sEditState(id);
    foreach(Relation relation, relations)
	if((relation.getDest().getId() == id)||(relation.getSource().getId() == id))
	    emit sEditRelation(relation.getId());
    return true;
}
/*!\func
 * setStateName
 * \param
 * - id - state's id
 * - newValue - new state name
 * \return bool result operation
 */
bool Data::setStatePosition(const qint32 id, const QPoint&value)
{
    if(!states.contains(id))return false;
    states[id].setPosition(value);
    return true;
}
/*!\func
 * set description
 * \param
 * - id - state's id
 * - newValue - new state name
 * \return bool result operation
 */
bool Data::setStateDescription(const qint32 id, const QString&newValue)
{
    if(!states.contains(id))return false;
    states[id].setDescription(newValue);
    emit sEditState(id);
    return true;
}
/*!\func
 * setRelationCondition
 * \param
 * - id - id
 * - newValue - new condition
 * \return bool result operation
 */
bool Data::setRelationCondition(const qint32 id, const QString&newValue)
{
    if(relations.contains(id))
    {
	relations[id].setCondition(newValue);
	emit sEditRelation(id);
	return true;
    }
    return false;
}
/*!\func
 * set destination state
 * \param
 * - id - id
 * - newState - new state destination
 * \return bool result operation
 */
bool Data::setRelationDest(const qint32 id, const qint32 newState)
{
    if(relations.contains(id))
    {
	relations[id].setDest(newState);
	emit sEditRelation(id);
	return true;
    }
    return false;
}
/*!\func
 * add state with name
 * \param
 - name - name of new state
 * \return id of new state (or -1)
 */
bool Data::removeRelation(const qint32 id)
{
    if(relations.contains(id))
    {
	relations.remove(id);
	emit sRemoveRelation(id);
	return true;
    }
    return false;
}
/*!\func
 * set new name of input
 * \param
 * - id - item id
 * - newValue - new name
 * \return bool result operation
 */
bool Data::setInputName(const qint32 id, const QString&newValue)
{
    if(!inputs.contains(id))return false;
    inputs[id].setName(getUniqueInputName(id, newValue));
    emit sEditInput(id);
    return true;
}
/*!\func
 * set new size of input
 * \param
 * - id - item id
 * - newValue - new size
 * \return bool result operation
 */
bool Data::setInputSize(const qint32 id, const qint32 newValue)
{
    if(!inputs.contains(id))return false;
    inputs[id].setSize(newValue);
    emit sEditInput(id);
    return true;
}
/*!\func
 * delete input
 * \param
 * \return id of new state (or -1)
 */
bool Data::removeInput(const qint32 id)
{
    if(inputs.contains(id))
    {
	inputs.remove(id);
	emit sRemoveInput(id);
	return true;
    }
    return false;
}
/*!\func
 * set new name of output
 * \param
 * - id - item id
 * - newValue - new name
 * \return bool result operation
 */
bool Data::setOutputName(const qint32 id, const QString&newValue)
{
    if(!outputs.contains(id))return false;
    outputs[id].setName(getUniqueOutputName(id, newValue));
    emit sEditOutput(id);
    return true;
}
/*!\func
 * set new type of output
 * \param
 * - id - item id
 * - newValue - new type
 * \return bool result operation
 */
bool Data::setOutputType(const qint32 id, const QString&newValue)
{
    if(!outputs.contains(id))return false;
    QString name = getValidType(newValue);
    outputs[id].setType(name);
    emit sEditOutput(id);
    return true;
}
/*!\func
 * set new Value of output
 * \param
 * - id - item id
 * - newValue - new Value
 * \return bool result operation
 */
bool Data::setOutputValue(const qint32 id, const QString&newValue)
{
    if(!outputs.contains(id))return false;
    outputs[id].setValue(newValue);
    emit sEditOutput(id);
    return true;
}
/*!\func
 * set new size of output
 * \param
 * - id - item id
 * - newValue - new size
 * \return bool result operation
 */
bool Data::setOutputSize(const qint32 id, const qint32 newValue)
{
    if(!outputs.contains(id))return false;
    outputs[id].setSize(newValue);
    emit sEditOutput(id);
    return true;
}
/*!\func
 * remove output
 * \param
 - id - output id
 * \return true/false
 */
bool Data::removeOutput(const qint32 id)
{
    if(outputs.contains(id))
    {
	QList<qint32> list;
	foreach(Control control, controls)
	    if(control.getOutput().getId() == id)
		list<<control.getId();
	foreach(qint32 id, list)
	    removeControl(id);
	outputs.remove(id);
	emit sRemoveOutput(id);
	return true;
    }
    return false;
}
/*!\func
 * set new Value of controls
 * \param
 * - id - item id
 * - newValue - new Value
 * \return bool result operation
 */
bool Data::setControlValue(const qint32 id, const QString&newValue)
{
    if(!controls.contains(id))return false;
    controls[id].setValue(newValue);
    emit sEditControl(id);
    return true;
}
/*!\func
 * set new condition of controls
 * \param
 * - id - item id
 * - newValue - new Value
 * \return bool result operation
 */
bool Data::setControlCondition(const qint32 id, const QString&newValue)
{
    if(!controls.contains(id))return false;
    controls[id].setCondition(newValue);
    emit sEditControl(id);
    return true;
}
/*!\func
 * remove output
 * \param
 - id - output id
 * \return true/false
 */
bool Data::removeControl(const qint32 id)
{
    if(controls.contains(id))
    {
	emit sRemoveControl(id);
	controls.remove(id);
	return true;
    }
    return false;
}
/*!\func
 * set common name
 * \param
 * - value - new Value
 * \return bool result operation
 */
bool Data::setModuleName(const QString&value)
{
        name = getValidIdentifier(value);
	emit sEditCommonData();
	return true;
}
/*!\func
 * set common description
 * \param
 * - value - new Value
 * \return bool result operation
 */
bool Data::setModuleDescription(const QString&value)
{
	description = value;
	emit sEditCommonData();
	return true;
}
/*!\func
 * get common description
 * \param
 * - value - new Value
 * \return bool result operation
 */
QString Data::getModuleDescription() const
{
	return description;
}
/*!\func
 * get common name
 * \param
 * - value - new Value
 * \return bool result operation
 */
QString Data::getModuleName() const
{
	return name;
}
/*!\func
 * get unique value
 * \param
 * - value - value
 * \return unique state name
 */
QString Data::getUniqueStateName(const qint32 id, const QString&name) const
{
    bool present = false;
    qsrand(1);
    QString value(getValidIdentifier(name));
    QString unique_name(value);
    do {
        present = false;
        foreach(State state, states)
            if((state.getName() == unique_name)&&(state.getId() != id))
            {
                present = true;
                unique_name = value + "_" + QString::number(qrand());
                break;
            }
    } while(present);
    return unique_name;
}
/*!\func
 * get unique value
 * \param
 * - value - value
 * \return unique in name
 */
QString Data::getUniqueInputName(const qint32 id, const QString&name) const
{
    bool present = false;
    QString value(getValidIdentifier(name));
    qsrand(1);
    QString unique_name(value);
    do {
        present = false;
        foreach(Input state, inputs)
            if((state.getName() == unique_name)&&(state.getId() != id))
            {
                present = true;
                unique_name = value + "_" + QString::number(qrand());
                break;
            }
    } while(present);
    return unique_name;
}
/*!\func
 * get unique value
 * \param
 * - value - value
 * \return unique out name
 */
QString Data::getUniqueOutputName(const qint32 id, const QString&name) const
{
    bool present = false;
    qsrand(1);
    QString value(getValidIdentifier(name));
    if(value=="wire")value = "reg";
    QString unique_name(value);
    do {
        present = false;
        foreach(Output state, outputs)
            if((state.getName() == unique_name)&&(state.getId() != id))
            {
                present = true;
                unique_name = value + "_" + QString::number(qrand());
                break;
            }
    } while(present);
    return unique_name;
}
/*!\func
 * get valid identifier by invalid value
 * \param
 * - value - invalid value
 * \return nvalid name
 */
QString Data::getValidIdentifier(const QString&invalid) const
{
    QString valid("");
    qsrand(1);
    QString digits("0123456789"), letters("qwertyuiopasdfghjklzxcvbnm_QWERTYUIOPASDFGHJKLZXCVBNM");
    if(invalid.isEmpty())
    {
        valid = "_" + QString::number(qrand());
        return valid;
    }
    if(!letters.contains(invalid[0]))
        valid = "_";
    foreach(QChar c, invalid)
        if((!letters.contains(c))&&(!digits.contains(c)))
            valid += "_";
        else
            valid += c;
    return valid;
}
/*!\func
 * get valid identifier by invalid value
 * \param
 * - value - invalid value
 * \return nvalid name
 */
QString Data::getValidType(const QString&invalid) const
{
    if((invalid != "wire")&&(invalid != "reg"))
        return "wire";
    else
        return invalid;
}

