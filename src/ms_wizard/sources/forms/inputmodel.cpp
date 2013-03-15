#include "inputmodel.h"
#include "data.h"
#include "QStringList"
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QApplication>
#include <errors.h>

/*!\func
 *
 * \param no
 * \return no
 */
InputModel::InputModel(Data*data, const QStringList&headers)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    this->headers = headers;
    this->shared_data = data;
    if(!shared_data)return;
    connect(shared_data, SIGNAL(sAddInput(const qint32)), this, SLOT(onAddInput(const qint32)));
    connect(shared_data, SIGNAL(sRemoveInput(const qint32)), this, SLOT(onRemoveInput(const qint32)));
    foreach(Input input, shared_data->getInputs())
	ids.append(input.getId());

}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void InputModel::onAddInput (const qint32 id )
{
    qint32 row = ids.count()?ids.count()-1:0;
    beginInsertRows(QModelIndex(),row, row);
    ids.append(id);
    endInsertRows();
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void InputModel::onRemoveInput (const qint32 id )
{
	qint32 row = 0;
	for(int i=0;i < ids.count(); i++)
		if(ids[i] == id)
		{
			row = i;
			break;
		}
	beginRemoveRows(QModelIndex(), row, row);
	ids.removeAt(row);
	endRemoveRows();
}
/*!\func
 *
 * \param no
 * \return no
 */
int InputModel::rowCount ( const QModelIndex & ) const
{
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	return ids.count();
}
/*!\func
 *
 * \param no
 * \return no
 */
int InputModel::columnCount ( const QModelIndex &) const
{
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	return headers.count();
}
/*!\func
 *
 * \param no
 * \return no
 */
QVariant InputModel::data ( const QModelIndex &index, int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (!index.isValid())
	 return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole)
	 return QVariant();
    switch(index.column())
    {
	case 0:
	    if(index.row() < ids.count())
		return  shared_data->getInput(ids[index.row()]).getName();
	    break;
	case 1:
	    if(index.row() < ids.count())
		return  shared_data->getInput(ids[index.row()]).getSize();
	    break;
	case 2:
	    if(index.row() < ids.count())
		return  shared_data->getInput(ids[index.row()]).getType();
	    break;
    }
    return QVariant();
}
/*!\func
 *
 * \param no
 * \return no
 */
bool InputModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (role != Qt::EditRole)
	 return false;
    bool result = false;
    switch(index.column())
    {
	case 0:
	    if(shared_data)
		    if(ids.count() > index.row())
		    {
			shared_data->setInputName(ids[index.row()], value.toString());
			result = true;
		    }
	    break;
	case 1:
	    if(shared_data)
		    if(ids.count() > index.row())
		    {
			shared_data->setInputSize(ids[index.row()], value.toInt());
			result = true;
		    }
	    break;
	case 2:
	    break;
    }
    if (result)
    {
	    emit dataChanged(index, index);
    }
    return result;
}
/*!\func
 *
 * \param no
 * \return no
 */
Qt::ItemFlags InputModel::flags(const QModelIndex &index) const
{
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	if (!index.isValid())
		return 0;
	return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
/*!\func
 *
 * \param no
 * \return no
 */
QVariant InputModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (role != Qt::DisplayRole)
	 return QVariant();
    if (orientation == Qt::Vertical)
	 return QVariant();
    if(section < headers.count())
	return headers[section];
    return QVariant();
}
/*!\func
 * for remove data by model
 * \param
 * - row - row of item
 * - parent - not used
 * \return no
 */
bool InputModel::removeRow ( int row, const QModelIndex & parent)
{
	Q_UNUSED(parent);
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	bool result = false;
	if(shared_data)
		if(ids.count() > row)
		{
		shared_data->removeInput(ids[row]);
		result = true;
		}
	return result;
}
